#include "HyperCubeProjectionActor.hpp"
#include "vtkAnimationCue.h"
#include "vtkAnimationScene.h"
#include "vtkEventQtSlotConnect.h"
#include "vtkGenericOpenGLRenderWindow.h"
#include "vtkOrientationMarkerWidget.h"
#include "vtkRenderer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartPointer.h"

#include <QCheckBox>
#include <QCloseEvent>
#include <QCoreApplication>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPointer>
#include <QPushButton>
#include <QSlider>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkAxesActor.h>

#include <vector>

class DoubleSpinBox : public QDoubleSpinBox {
    Q_OBJECT
public:
    DoubleSpinBox() {
        this->setSingleStep(0.05);
        this->setRange(-100.0, 100.0);
    }
};

class AngleSliderWidget : public QWidget {
    Q_OBJECT

public:
    AngleSliderWidget(const QString name, QWidget *parent = nullptr)
        : QWidget(parent) {
        this->Name = name;
        this->Label = new QLabel(this->Name + ": 0", this);

        this->Slider = new QSlider(Qt::Horizontal, this);
        this->Slider->setSingleStep(1);
        this->Slider->setRange(-180, 180);
        this->Slider->setValue(0);

        this->CheckBox = new QCheckBox();

        QHBoxLayout *layout = new QHBoxLayout(this);
        layout->addWidget(this->Label);
        layout->addWidget(this->Slider);
        layout->addWidget(this->CheckBox);
        setLayout(layout);
        connect(this->Slider, SIGNAL(valueChanged(int)), this,
                SLOT(sliderValueChanged(int)));
    }
    QPointer<QLabel> Label;
    QPointer<QSlider> Slider;
    QPointer<QCheckBox> CheckBox;
    QString Name;
    int PreviousValue;

public slots:
    void sliderValueChanged(int value) {
        this->Label->setText(this->Name + ": " + std::to_string(value).c_str());
    }
};

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(int argc, char *argv[]);
    ~MainWindow();

    void setupUI();

protected slots:

    void closeEvent(QCloseEvent *event) override;
    void onLightOriginChangedX(double a);
    void onLightOriginChangedY(double a);
    void onLightOriginChangedZ(double a);
    void onLightOriginChangedW(double a);

    void onPlaneOriginChangedX(double a);
    void onPlaneOriginChangedY(double a);
    void onPlaneOriginChangedZ(double a);
    void onPlaneOriginChangedW(double a);

    void onPlaneNormalChangedX(double a);
    void onPlaneNormalChangedY(double a);
    void onPlaneNormalChangedZ(double a);
    void onPlaneNormalChangedW(double a);

    void onAngleChangedXY(int a);
    void onAngleChangedXZ(int a);
    void onAngleChangedXW(int a);
    void onAngleChangedYZ(int a);
    void onAngleChangedYW(int a);
    void onAngleChangedZW(int a);

    void onAnimationEvents(vtkObject *caller, unsigned long event,
                           void *calldata, void *);
    void onAnimationStart(bool);

    void onResetPressed();

protected:
    void updateCube();
    void updateTables();
    void updateTable3D();
    void updateTable4D();

    vtkSmartPointer<vtkRenderWindowInteractor> RWI;
    vtkSmartPointer<vtkRenderer> Renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> RenderWindow;
    vtkSmartPointer<vtkHyperCubeProjectionActor> HyperCubeActor;

    vtkSmartPointer<vtkAnimationScene> AnimationScene;
    vtkSmartPointer<vtkAnimationCue> AnimationCue;

    vtkSmartPointer<vtkEventQtSlotConnect> Connector;

    QPointer<QVTKOpenGLNativeWidget> RenderWidget;

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

    QPointer<AngleSliderWidget> AngleSliderXY;
    QPointer<AngleSliderWidget> AngleSliderXZ;
    QPointer<AngleSliderWidget> AngleSliderXW;
    QPointer<AngleSliderWidget> AngleSliderYZ;
    QPointer<AngleSliderWidget> AngleSliderYW;
    QPointer<AngleSliderWidget> AngleSliderZW;

    std::vector<AngleSliderWidget *> AngleWidgets;

    QPointer<QPushButton> AnimationControlButton;
    QPointer<QPushButton> ResetButton;
    QPointer<QLabel> NoticeLabel;

private:
    void connectSignals();
    void setDefaultValues();

    bool SuppressUpdate = false;
};
