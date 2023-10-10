#include "HyperCubeProjectionActor.hpp"

#include <vtkProp3D.h>

VTK_ABI_NAMESPACE_BEGIN
vtkStandardNewMacro(vtkHyperCubeProjectionActor);

vtkHyperCubeProjectionActor::vtkHyperCubeProjectionActor() {
    this->HyperCube = vtkHyperCube::New();
    this->HyperCube->SetLength(1.0);
    double o[4] = {0.0, 0.0, 0.0, 0.0};
    this->HyperCube->SetOrigin(o);

    this->HyperPlane = vtkHyperPlane::New();
    double norm[4] = {0.5, 0.5, 1.0, 1.0};
    this->HyperPlane->SetNormal(norm);
    this->HyperPlane->SetOrigin(o);

    this->ProjectedPoints = vtkPoints::New();

    this->Transform4D = vtkTransform::New();

    double light[4] = {5.0, 5.0, 5.0, 5.0};
    this->SetLightOrigin(light);

    for (int i = 0; i < 16; i++) {
        this->SphereProperty[i] = vtkProperty::New();

        this->SphereSource[i] = vtkSphereSource::New();
        this->SphereSource[i]->SetRadius(0.05);
        this->SphereSource[i]->SetPhiResolution(16);
        this->SphereSource[i]->SetThetaResolution(16);

        this->SphereMapper[i] = vtkPolyDataMapper::New();
        this->SphereMapper[i]->SetInputConnection(
            this->SphereSource[i]->GetOutputPort());

        this->SphereActor[i] = vtkActor::New();
        this->SphereActor[i]->SetMapper(this->SphereMapper[i]);
        this->SphereActor[i]->SetProperty(this->SphereProperty[i]);

        this->AddPart(this->SphereActor[i]);
    }

    vtkNew<vtkCellArray> lines;
    for (int i = 0; i < 32; i++) {
        vtkNew<vtkLine> line;
        line->GetPointIds()->SetId(0, this->EDGES[i][0]);
        line->GetPointIds()->SetId(1, this->EDGES[i][1]);
        lines->InsertNextCell(line);
    }

    this->LinePolyData = vtkPolyData::New();
    this->LinePolyData->SetLines(lines);

    this->LineMapper = vtkPolyDataMapper::New();
    this->LineMapper->SetInputData(this->LinePolyData);

    this->LineActor = vtkActor::New();
    this->LineActor->SetMapper(this->LineMapper);

    this->LineProperty = vtkProperty::New();
    this->LineActor->SetProperty(this->LineProperty);

    this->AddPart(this->LineActor);
    this->UpdatePositions();
    this->LinePolyData->SetPoints(this->ProjectedPoints);
}

vtkHyperCubeProjectionActor::~vtkHyperCubeProjectionActor() {
    for (int i = 0; i < 16; i++) {
        this->SphereActor[i]->Delete();
        this->SphereMapper[i]->Delete();
        this->SphereSource[i]->Delete();
        this->SphereProperty[i]->Delete();
    }
    this->LineActor->Delete();
    this->LineMapper->Delete();
    this->LineProperty->Delete();
    this->LinePolyData->Delete();
}

void vtkHyperCubeProjectionActor::RotateAlongPlane(double theta, Plane plane) {
    theta = vtkMath::RadiansFromDegrees(theta);
    const double s = sin(theta);
    const double c = cos(theta);
    // matrices taken from https://math.stackexchange.com/a/3311905
    switch (plane) {
        case XY: {
            // clang-format off
            double mat[16] = {
                1, 0, 0, 0,  
                0, 1, 0, 0 ,
                0, 0,c, -s,
                0, 0,s, c, 
            };
            // clang-format on
            this->Transform4D->Concatenate(mat);
            break;
        }
        case XZ: {
            // clang-format off
            double mat[16] = {
                1, 0, 0, 0,  
                0, c, 0, -s ,
                0, 0, 1, 0 ,
                0, s, 0, c ,
            };
            // clang-format on
            this->Transform4D->Concatenate(mat);
            break;
        }
        case XW: {
            // clang-format off
            double mat[16] = {
                1, 0, 0, 0,  
                0, c, -s, 0 ,
                0, s, c,  0 ,
                0, 0, 0, 1 ,
            };
            // clang-format on
            this->Transform4D->Concatenate(mat);
            break;
        }
        case YZ: {
            // clang-format off
            double mat[16] = {
                c, 0, 0, -s,  
                0, 1, 0, 0 ,
                0, 0, 1, 0 ,
                s, 0, 0, c ,
            };
            // clang-format on
            this->Transform4D->Concatenate(mat);
            break;
        }
        case YW: {
            // clang-format off
            double mat[16] = {
                c, 0, -s, 0,  
                0, 1, 0, 0 ,
                s, 0, c, 0 ,
                0, 0, 0, 1 ,
            };
            // clang-format on
            this->Transform4D->Concatenate(mat);
            break;
        }
        case ZW: {
            // clang-format off
            double mat[16] = {
                c, -s, 0, 0,  
                s, c, 0, 0 ,
                0, 0, 1, 0 ,
                0, 0, 0, 1 ,
            };
            // clang-format on
            this->Transform4D->Concatenate(mat);
            break;
        }
    }
}

bool vtkHyperCubeProjectionActor::UpdatePositions() {
    auto normal = this->HyperPlane->GetNormal();
    auto origin = this->HyperPlane->GetOrigin();
    vtkNew<vtkPoints> newPts;

    for (int i = 0; i < 16; i++) {
        auto p = this->HyperCube->GetPoints()->GetPoint(i);
        double d[4];
        d[0] = this->LightOrigin[0] - p[0];
        d[1] = this->LightOrigin[1] - p[1];
        d[2] = this->LightOrigin[2] - p[2];
        d[3] = this->LightOrigin[3] - p[3];
        auto dotProduct = vtkMath::Dot(normal, d);
        if (abs(dotProduct) < 1e-6) {
            vtkErrorMacro(
                << "Cannot cast shadow to the plane from one of the points.");
            return false;
        }
        double ol[4];
        ol[0] = origin[0] - this->LightOrigin[0];
        ol[1] = origin[1] - this->LightOrigin[1];
        ol[2] = origin[2] - this->LightOrigin[2];
        ol[3] = origin[3] - this->LightOrigin[3];

        double t = vtkMath::Dot(normal, ol) / dotProduct;
        double point4D[4];
        point4D[0] = this->LightOrigin[0] + d[0] * t;
        point4D[1] = this->LightOrigin[1] + d[1] * t;
        point4D[2] = this->LightOrigin[2] + d[2] * t;
        point4D[3] = this->LightOrigin[3] + d[3] * t;
        double final[4];
        this->Transform4D->MultiplyPoint(point4D, final);
        // drop the z axis and keep x,y,w
        final[2] = final[3];
        newPts->InsertNextPoint(final);
    }

    this->ProjectedPoints->DeepCopy(newPts);
    for (int i = 0; i < this->ProjectedPoints->GetNumberOfPoints(); i++) {
        auto N = this->ProjectedPoints->GetPoint(i);
        this->SphereSource[i]->SetCenter(N);
    }
    this->LinePolyData->Modified();
    this->Modified();
    return true;
}

void vtkHyperCubeProjectionActor::PrintSelf(ostream &os, vtkIndent indent) {
    this->Superclass::PrintSelf(os, indent);
    os << indent << "Light Origin: (" << this->LightOrigin[0] << ", "
       << this->LightOrigin[1] << ", " << this->LightOrigin[2] << ", "
       << this->LightOrigin[3] << ")\n";
}
VTK_ABI_NAMESPACE_END
