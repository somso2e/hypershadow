#include "HyperCube.hpp"
VTK_ABI_NAMESPACE_BEGIN

vtkStandardNewMacro(vtkHyperCube);

void HyperPointRelativeToHyperPlane(const double x[4], const double origin[4],
                                    const double normal[4], double xproj[3]) {
    double t, xo[4];

    xo[0] = x[0] - origin[0];
    xo[1] = x[1] - origin[1];
    xo[2] = x[2] - origin[2];
    xo[3] = x[3] - origin[3];

    t = vtkMath::Dot(normal, xo);

    xproj[0] = xo[0] - t * normal[0];
    xproj[1] = xo[1] - t * normal[1];
    xproj[2] = xo[2] - t * normal[2];
    // xproj[3] = xo[3] - t * normal[3]; discarded
}

static const double CORNERS[16][4] = {
    {-0.5, -0.5, -0.5, -0.5}, {0.5, -0.5, -0.5, -0.5}, {0.5, 0.5, -0.5, -0.5},
    {-0.5, 0.5, -0.5, -0.5},  {-0.5, -0.5, 0.5, -0.5}, {0.5, -0.5, 0.5, -0.5},
    {0.5, 0.5, 0.5, -0.5},    {-0.5, 0.5, 0.5, -0.5},  {-0.5, -0.5, -0.5, 0.5},
    {0.5, -0.5, -0.5, 0.5},   {0.5, 0.5, -0.5, 0.5},   {-0.5, 0.5, -0.5, 0.5},
    {-0.5, -0.5, 0.5, 0.5},   {0.5, -0.5, 0.5, 0.5},   {0.5, 0.5, 0.5, 0.5},
    {-0.5, 0.5, 0.5, 0.5}};

vtkHyperCube::vtkHyperCube() {
    this->Points = vtkHyperPoints::New();

    this->Length = 1.0;
    this->Origin[0] = 0.0;
    this->Origin[1] = 0.0;
    this->Origin[2] = 0.0;
    this->Origin[3] = 0.0;
    this->UpdatePoints();
}
vtkHyperCube::~vtkHyperCube() {
    this->Points->Delete();
}

void vtkHyperCube::UpdatePoints() {
    this->Points->SetNumberOfPoints(0);
    for (int i = 0; i < 16; i++) {
        double edge[4] = {
            CORNERS[i][0] * this->Length + this->Origin[0],
            CORNERS[i][1] * this->Length + this->Origin[1],
            CORNERS[i][2] * this->Length + this->Origin[2],
            CORNERS[i][3] * this->Length + this->Origin[3],
        };
        this->Points->InsertNextPoint(edge);
    }
}

void vtkHyperCube::ProjectToHyperPlane(vtkHyperPlane *plane, vtkPoints *pts) {
    auto origin = plane->GetOrigin();
    auto normal = plane->GetNormal();
    for (int i = 0; i < this->Points->GetNumberOfPoints(); i++) {
        auto point4D = this->Points->GetPoint(i);
        double proj3D[3];
        HyperPointRelativeToHyperPlane(point4D, origin, normal, proj3D);
        pts->InsertNextPoint(proj3D);
    }
}

void vtkHyperCube::PrintSelf(ostream &os, vtkIndent indent) {
    this->Superclass::PrintSelf(os, indent);
    os << indent << "Origin: (" << this->Origin[0] << ", " << this->Origin[1]
       << ", " << this->Origin[2] << ", " << this->Origin[3] << ")\n";
    os << indent << "Length: " << this->Length << "\n";
}

VTK_ABI_NAMESPACE_END
