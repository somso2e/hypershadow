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
#include "vtkTransform.h"

#include "vtkMatrix4x4.h"

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
    vtkGetObjectMacro(Transform4D, vtkTransform);

    vtkGetVector4Macro(LightOrigin, double);
    vtkSetVector4Macro(LightOrigin, double);

    bool UpdatePositions();
    void RotateXY(double theta) {
        this->RotateAlongPlane(theta, XY);
    };
    void RotateXZ(double theta) {
        this->RotateAlongPlane(theta, XZ);
    };
    void RotateXW(double theta) {
        this->RotateAlongPlane(theta, XW);
    };
    void RotateYZ(double theta) {
        this->RotateAlongPlane(theta, YZ);
    };
    void RotateYW(double theta) {
        this->RotateAlongPlane(theta, YW);
    };
    void RotateZW(double theta) {
        this->RotateAlongPlane(theta, ZW);
    };

protected:
    enum Plane {
        XY,
        XZ,
        XW,
        YZ,
        YW,
        ZW,
    };

    void RotateAlongPlane(double theta, Plane plane);

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

    vtkTransform *Transform4D;

private:
    vtkHyperCubeProjectionActor(const vtkHyperCubeProjectionActor &) = delete;
    void operator=(const vtkHyperCubeProjectionActor &) = delete;

    static constexpr int EDGES[32][2] = {
        {0, 1},   {1, 2},   {2, 3},   {3, 0},   {4, 5},   {5, 6},   {6, 7},
        {7, 4},   {0, 4},   {1, 5},   {2, 6},   {3, 7},   {8, 9},   {9, 10},
        {10, 11}, {11, 8},  {12, 13}, {13, 14}, {14, 15}, {15, 12}, {8, 12},
        {9, 13},  {10, 14}, {11, 15}, {0, 8},   {1, 9},   {2, 10},  {3, 11},
        {4, 12},  {5, 13},  {6, 14},  {7, 15}};
};
#endif
