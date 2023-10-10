#include "HyperCubeProjectionActor.hpp"
#include "vtkGenericOpenGLRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartPointer.h"

#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <qpointer.h>
#include <QPointer>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QVTKOpenGLNativeWidget.h>

class DoubleSpinBox : public QDoubleSpinBox {
    Q_OBJECT
public:
    DoubleSpinBox() {
        this->setSingleStep(0.05);
        this->setRange(-100.0, 100.0);
    }
};

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(int argc, char *argv[]);
    ~MainWindow();

    void setupUI();

protected slots:

    void OnLightOriginChangedX(double a);
    void OnLightOriginChangedY(double a);
    void OnLightOriginChangedZ(double a);
    void OnLightOriginChangedW(double a);

    void OnPlaneOriginChangedX(double a);
    void OnPlaneOriginChangedY(double a);
    void OnPlaneOriginChangedZ(double a);
    void OnPlaneOriginChangedW(double a);

    void OnPlaneNormalChangedX(double a);
    void OnPlaneNormalChangedY(double a);
    void OnPlaneNormalChangedZ(double a);
    void OnPlaneNormalChangedW(double a);

protected:
    void UpdateCube();
    void UpdateTables();
    void UpdateTable3D();
    void UpdateTable4D();

    vtkSmartPointer<vtkRenderWindowInteractor> RWI;
    vtkSmartPointer<vtkRenderer> Renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> RenderWindow;
    QPointer<QVTKOpenGLNativeWidget> RenderWidget;

    vtkSmartPointer<vtkHyperCubeProjectionActor> HyperCubeActor;

    QPointer<DoubleSpinBox> LightOriginSpinBoxX;
    QPointer<DoubleSpinBox> LightOriginSpinBoxY;
    QPointer<DoubleSpinBox> LightOriginSpinBoxZ;
    QPointer<DoubleSpinBox> LightOriginSpinBoxW;

    QPointer<DoubleSpinBox> PlaneNormalSpinBoxX;
    QPointer<DoubleSpinBox> PlaneNormalSpinBoxY;
    QPointer<DoubleSpinBox> PlaneNormalSpinBoxZ;
    QPointer<DoubleSpinBox> PlaneNormalSpinBoxW;

    QPointer<DoubleSpinBox> PlaneOriginSpinBoxX;
    QPointer<DoubleSpinBox> PlaneOriginSpinBoxY;
    QPointer<DoubleSpinBox> PlaneOriginSpinBoxZ;
    QPointer<DoubleSpinBox> PlaneOriginSpinBoxW;

    QPointer<QTableWidget> TableWidget4D;
    QPointer<QTableWidget> TableWidget3D;
};
