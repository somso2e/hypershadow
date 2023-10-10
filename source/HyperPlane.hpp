#ifndef vtkHyperPlane_h
#define vtkHyperPlane_h

#include "vtkObject.h"
#include "vtkObjectFactory.h"

class VTK_EXPORT vtkHyperPlane : public vtkObject {
    vtkTypeMacro(vtkHyperPlane, vtkObject);

public:
    static vtkHyperPlane *New();
    void PrintSelf(ostream &os, vtkIndent indent) override;

    vtkSetVector4Macro(Origin, double);
    vtkGetVector4Macro(Origin, double);

    vtkSetVector4Macro(Normal, double);
    vtkGetVector4Macro(Normal, double);

protected:
    vtkHyperPlane();
    ~vtkHyperPlane() override;

    double Origin[4];
    double Normal[4];

private:
    vtkHyperPlane(const vtkHyperPlane &) = delete;
    void operator=(const vtkHyperPlane &) = delete;
};
#endif
