#include "HyperPlane.hpp"
VTK_ABI_NAMESPACE_BEGIN

vtkStandardNewMacro(vtkHyperPlane);

vtkHyperPlane::vtkHyperPlane() {
    this->Origin[0] = 0.0;
    this->Origin[1] = 0.0;
    this->Origin[2] = 0.0;
    this->Origin[3] = 0.0;

    this->Normal[0] = 0.0;
    this->Normal[1] = 0.0;
    this->Normal[2] = 0.0;
    this->Normal[3] = 0.0;
}

vtkHyperPlane::~vtkHyperPlane() {
}
void vtkHyperPlane::PrintSelf(ostream &os, vtkIndent indent) {
    this->Superclass::PrintSelf(os, indent);
    os << indent << "Origin: (" << this->Origin[0] << ", " << this->Origin[1]
       << ", " << this->Origin[2] << ", " << this->Origin[3] << ")\n";
    os << indent << "Normal: (" << this->Normal[0] << ", " << this->Normal[1]
       << ", " << this->Normal[2] << ", " << this->Normal[3] << ")\n";
}
VTK_ABI_NAMESPACE_END
