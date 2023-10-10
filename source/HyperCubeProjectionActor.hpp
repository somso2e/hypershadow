#ifndef vtkHyperCubeProjectionActor_h
#define vtkHyperCubeProjectionActor_h

#include "HyperCube.hpp"
#include "vtkActor.h"
#include "vtkAssembly.h"
#include "vtkCellArray.h"
#include "vtkLine.h"
#include "vtkMath.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkSphereSource.h"

class vtkProperty;
class vtkActor;
class vtkSphereSource;
class vtkPolyDataMapper;

class VTK_EXPORT vtkHyperCubeProjectionActor : public vtkAssembly {
    vtkTypeMacro(vtkHyperCubeProjectionActor, vtkAssembly);

public:
    static vtkHyperCubeProjectionActor *New();
    void PrintSelf(ostream &os, vtkIndent indent) override;

    vtkGetObjectMacro(HyperPlane, vtkHyperPlane);
    vtkGetObjectMacro(HyperCube, vtkHyperCube);
    vtkGetObjectMacro(ProjectedPoints, vtkPoints);

    vtkGetVector4Macro(LightOrigin, double);
    vtkSetVector4Macro(LightOrigin, double);

    void UpdatePositions();

protected:
    vtkHyperCubeProjectionActor();
    ~vtkHyperCubeProjectionActor() override;
    vtkHyperCube *HyperCube;
    vtkHyperPlane *HyperPlane;

    vtkProperty *SphereProperty[16];
    vtkSphereSource *SphereSource[16];
    vtkActor *SphereActor[16];
    vtkPolyDataMapper *SphereMapper[16];

    vtkProperty *LineProperty;
    vtkPolyDataMapper *LineMapper;
    vtkActor *LineActor;
    vtkPolyData *LinePolyData;
    // vtkPolyDataMapper *HyperCubeMapper;
    // vtkActor
    vtkPoints *ProjectedPoints;
    double LightOrigin[4];

private:
    vtkHyperCubeProjectionActor(const vtkHyperCubeProjectionActor &) = delete;
    void operator=(const vtkHyperCubeProjectionActor &) = delete;
};
#endif
