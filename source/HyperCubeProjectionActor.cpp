#include "HyperCubeProjectionActor.hpp"

VTK_ABI_NAMESPACE_BEGIN
vtkStandardNewMacro(vtkHyperCubeProjectionActor);
const static int edges[32][2] = {
    {0, 1},   {1, 2},   {2, 3},   {3, 0},   {4, 5},   {5, 6},   {6, 7},
    {7, 4},   {0, 4},   {1, 5},   {2, 6},   {3, 7},   {8, 9},   {9, 10},
    {10, 11}, {11, 8},  {12, 13}, {13, 14}, {14, 15}, {15, 12}, {8, 12},
    {9, 13},  {10, 14}, {11, 15}, {0, 8},   {1, 9},   {2, 10},  {3, 11},
    {4, 12},  {5, 13},  {6, 14},  {7, 15}};

vtkHyperCubeProjectionActor::vtkHyperCubeProjectionActor() {
    this->HyperCube = vtkHyperCube::New();
    this->HyperCube->SetLength(1.0);
    double o[4] = {0.0, 0.0, 0.0, 0.0};
    this->HyperCube->SetOrigin(o);
    this->HyperCube->UpdatePoints();

    this->HyperPlane = vtkHyperPlane::New();
    double norm[4] = {0.5, 0.5, 1.0, 1.0};
    this->HyperPlane->SetNormal(norm);
    this->HyperPlane->SetOrigin(o);

    this->ProjectedPoints = vtkPoints::New();
    double light[4] = {5.0, 5.0, 5.0, 5.0};
    this->SetLightOrigin(light);

    for (int i = 0; i < 16; i++) {
        this->SphereProperty[i] = vtkProperty::New();

        this->SphereSource[i] = vtkSphereSource::New();
        this->SphereSource[i]->SetRadius(0.05);
        this->SphereSource[i]->SetPhiResolution(16);
        this->SphereSource[i]->SetThetaResolution(16);
        // this->SphereSource[i]->SetCenter(this->ProjectedPoints->GetPoint(i));

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
        line->GetPointIds()->SetId(0, edges[i][0]);
        line->GetPointIds()->SetId(1, edges[i][1]);
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

void vtkHyperCubeProjectionActor::UpdatePositions() {
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
            return;
        }
        double ol[4];
        ol[0] = origin[0] - this->LightOrigin[0];
        ol[1] = origin[1] - this->LightOrigin[1];
        ol[2] = origin[2] - this->LightOrigin[2];
        ol[3] = origin[3] - this->LightOrigin[3];

        double t = vtkMath::Dot(normal, ol) / dotProduct;
        double point[3];
        point[0] = this->LightOrigin[0] + d[0] * t;
        point[1] = this->LightOrigin[1] + d[1] * t;
        // point[2] = this->LightOrigin[2] + d[2] * t;
        point[2] = this->LightOrigin[3] + d[3] * t;

        newPts->InsertNextPoint(point);
    }

    this->ProjectedPoints->DeepCopy(newPts);
    for (int i = 0; i < this->ProjectedPoints->GetNumberOfPoints(); i++) {
        auto N = this->ProjectedPoints->GetPoint(i);
        // std::cout << "(" << N[0] << "," << N[1] << "," << N[2] << ")"
        //           << std::endl;
        this->SphereSource[i]->SetCenter(N);
    }
    this->LinePolyData->Modified();
}

void vtkHyperCubeProjectionActor::PrintSelf(ostream &os, vtkIndent indent) {
    this->Superclass::PrintSelf(os, indent);
    os << indent << "Light Origin: (" << this->LightOrigin[0] << ", "
       << this->LightOrigin[1] << ", " << this->LightOrigin[2] << ", "
       << this->LightOrigin[3] << ")\n";
}
VTK_ABI_NAMESPACE_END
