#include "HyperPlane.hpp"
#include "HyperPoints.hpp"
#include "vtkObject.h"

class VTK_EXPORT vtkHyperCube : public vtkObject {
    vtkTypeMacro(vtkHyperCube, vtkObject);

public:
    static vtkHyperCube *New();

    void PrintSelf(ostream &os, vtkIndent indent) override;
    vtkSetMacro(Length, double);
    vtkGetMacro(Length, double);

    vtkSetVector4Macro(Origin, double);
    vtkGetVector4Macro(Origin, double);

    vtkSetObjectMacro(Points, vtkHyperPoints);
    vtkGetObjectMacro(Points, vtkHyperPoints);

    void ProjectToHyperPlane(vtkHyperPlane *plane, vtkPoints *pts);

    void UpdatePoints();

protected:
    vtkHyperCube();
    ~vtkHyperCube() override;

    vtkHyperPoints *Points;
    double Length;
    double Origin[4];

private:
    vtkHyperCube(const vtkHyperCube &) = delete;
    void operator=(const vtkHyperCube &) = delete;
};
