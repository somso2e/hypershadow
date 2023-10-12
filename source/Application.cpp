#include "Application.hpp"

MainWindow::MainWindow(int argc, char *argv[]) {
    this->setupUI();

    this->RenderWindow = vtkGenericOpenGLRenderWindow::New();
    this->RenderWidget->setRenderWindow(this->RenderWindow.Get());
    this->Renderer = vtkRenderer::New();
    this->RenderWindow->AddRenderer(this->Renderer);

    this->HyperCubeActor = vtkHyperCubeProjectionActor::New();
    this->Renderer->AddActor(this->HyperCubeActor);
    this->setDefaultValues();
    this->HyperCubeActor->UpdatePositions();
    this->updateTables();

    this->AnimationScene = vtkAnimationScene::New();
    this->AnimationScene->SetModeToRealTime();
    this->AnimationScene->SetFrameRate(1);
    this->AnimationScene->SetLoop(true);
    this->AnimationScene->SetStartTime(0);
    this->AnimationScene->SetEndTime(1);

    this->AnimationCue = vtkAnimationCue::New();
    this->AnimationCue->SetStartTime(0);
    this->AnimationCue->SetEndTime(1);
    this->AnimationScene->AddCue(this->AnimationCue);

    // TODO: it doesnt work
    //
    // vtkNew<vtkOrientationMarkerWidget> orientationWidget;
    // vtkNew<vtkAxesActor> axesActor;
    // vtkNew<vtkTransform> t;
    // axesActor->SetUserTransform(t);
    // orientationWidget->SetOrientationMarker(axesActor);
    // orientationWidget->SetViewport(0.0, 0.0, 0.2, 0.2);
    // orientationWidget->SetInteractor(this->RenderWidget->interactor());
    // orientationWidget->SetDefaultRenderer(this->Renderer);
    // orientationWidget->SetEnabled(true);
    // orientationWidget->InteractiveOff();
    // orientationWidget->On();
    this->connectSignals();

    this->Renderer->GetActiveCamera()->Pitch(45);
    this->Renderer->GetActiveCamera()->Yaw(45);
    this->Renderer->ResetCamera();
    // this->Renderer->GetActiveCamera()->Elevation(-45);
    this->RenderWidget->show();
    this->show();
}

MainWindow::~MainWindow() {
    this->AnimationScene->Delete();
    this->AnimationCue->Delete();
    // this->Connector->Delete();
    // this->RWI->Delete(); // causes crash?
    this->HyperCubeActor->Delete();
    this->Renderer->Delete();
}

void MainWindow::setupUI() {
    auto sidebarLayout = new QGridLayout;
    sidebarLayout->setAlignment(Qt::AlignTop);

    auto titleLabel1 = new QLabel("Light Source Location");
    auto titleLabel2 = new QLabel("Hyperplane Origin");
    auto titleLabel3 = new QLabel("Hyperplane Normal");
    auto titleLabel4 = new QLabel("Rotate Align Planes");

    this->LightOriginSpinBoxX = new DoubleSpinBox();
    this->LightOriginSpinBoxY = new DoubleSpinBox();
    this->LightOriginSpinBoxZ = new DoubleSpinBox();
    this->LightOriginSpinBoxW = new DoubleSpinBox();
    this->PlaneNormalSpinBoxX = new DoubleSpinBox();
    this->PlaneNormalSpinBoxY = new DoubleSpinBox();
    this->PlaneNormalSpinBoxZ = new DoubleSpinBox();
    this->PlaneNormalSpinBoxW = new DoubleSpinBox();
    this->PlaneOriginSpinBoxX = new DoubleSpinBox();
    this->PlaneOriginSpinBoxY = new DoubleSpinBox();
    this->PlaneOriginSpinBoxZ = new DoubleSpinBox();
    this->PlaneOriginSpinBoxW = new DoubleSpinBox();

    auto labelX = new QLabel("X:");
    auto labelY = new QLabel("Y:");
    auto labelZ = new QLabel("Z:");
    auto labelW = new QLabel("W:");

    int i = 0;
    sidebarLayout->addWidget(titleLabel1, i++, 0, 1, 2, Qt::AlignHCenter);
    sidebarLayout->addWidget(this->LightOriginSpinBoxX, i, 1);
    sidebarLayout->addWidget(labelX, i++, 0);
    sidebarLayout->addWidget(this->LightOriginSpinBoxY, i, 1);
    sidebarLayout->addWidget(labelY, i++, 0);
    sidebarLayout->addWidget(this->LightOriginSpinBoxZ, i, 1);
    sidebarLayout->addWidget(labelZ, i++, 0);
    sidebarLayout->addWidget(this->LightOriginSpinBoxW, i, 1);
    sidebarLayout->addWidget(labelW, i++, 0);

    labelX = new QLabel("X:");
    labelY = new QLabel("Y:");
    labelZ = new QLabel("Z:");
    labelW = new QLabel("W:");
    sidebarLayout->addWidget(titleLabel2, i++, 0, 1, 2, Qt::AlignHCenter);
    sidebarLayout->addWidget(this->PlaneOriginSpinBoxX, i, 1);
    sidebarLayout->addWidget(labelX, i++, 0);
    sidebarLayout->addWidget(this->PlaneOriginSpinBoxY, i, 1);
    sidebarLayout->addWidget(labelY, i++, 0);
    sidebarLayout->addWidget(this->PlaneOriginSpinBoxZ, i, 1);
    sidebarLayout->addWidget(labelZ, i++, 0);
    sidebarLayout->addWidget(this->PlaneOriginSpinBoxW, i, 1);
    sidebarLayout->addWidget(labelW, i++, 0);

    labelX = new QLabel("X:");
    labelY = new QLabel("Y:");
    labelZ = new QLabel("Z:");
    labelW = new QLabel("W:");
    sidebarLayout->addWidget(titleLabel3, i++, 0, 1, 2, Qt::AlignHCenter);
    sidebarLayout->addWidget(this->PlaneNormalSpinBoxX, i, 1);
    sidebarLayout->addWidget(labelX, i++, 0);
    sidebarLayout->addWidget(this->PlaneNormalSpinBoxY, i, 1);
    sidebarLayout->addWidget(labelY, i++, 0);
    sidebarLayout->addWidget(this->PlaneNormalSpinBoxZ, i, 1);
    sidebarLayout->addWidget(labelZ, i++, 0);
    sidebarLayout->addWidget(this->PlaneNormalSpinBoxW, i, 1);
    sidebarLayout->addWidget(labelW, i++, 0);

    sidebarLayout->addWidget(titleLabel4, i++, 0, 1, 2, Qt::AlignHCenter);

    this->AngleSliderXY = new AngleSliderWidget("XY");
    this->AngleSliderXZ = new AngleSliderWidget("XZ");
    this->AngleSliderXW = new AngleSliderWidget("XW");
    this->AngleSliderYZ = new AngleSliderWidget("YZ");
    this->AngleSliderYW = new AngleSliderWidget("YW");
    this->AngleSliderZW = new AngleSliderWidget("ZW");

    sidebarLayout->addWidget(this->AngleSliderXY, i++, 0, 1, 2);
    sidebarLayout->addWidget(this->AngleSliderXZ, i++, 0, 1, 2);
    sidebarLayout->addWidget(this->AngleSliderXW, i++, 0, 1, 2);
    sidebarLayout->addWidget(this->AngleSliderYZ, i++, 0, 1, 2);
    sidebarLayout->addWidget(this->AngleSliderYW, i++, 0, 1, 2);
    sidebarLayout->addWidget(this->AngleSliderZW, i++, 0, 1, 2);

    this->AngleWidgets.push_back(this->AngleSliderXY);
    this->AngleWidgets.push_back(this->AngleSliderXZ);
    this->AngleWidgets.push_back(this->AngleSliderXW);
    this->AngleWidgets.push_back(this->AngleSliderYZ);
    this->AngleWidgets.push_back(this->AngleSliderYW);
    this->AngleWidgets.push_back(this->AngleSliderZW);

    this->AnimationControlButton = new QPushButton("Start Animation");
    this->AnimationControlButton->setCheckable(true);
    this->AnimationControlButton->setChecked(false);
    sidebarLayout->addWidget(this->AnimationControlButton, i++, 0, 1, 2);

    this->ResetButton = new QPushButton("Reset");
    sidebarLayout->addWidget(this->ResetButton, i++, 0, 1, 2);

    this->NoticeLabel = new QLabel("");
    this->NoticeLabel->setWordWrap(true);
    this->NoticeLabel->setStyleSheet("color: red;");
    sidebarLayout->addWidget(this->NoticeLabel, i++, 0, 1, 2);

    auto sidebarLayout2 = new QVBoxLayout();

    auto tableLabel3D = new QLabel("Projected 3D Points Coordinates");
    auto tableLabel4D = new QLabel("4D Points Coordinates");

    this->TableWidget3D = new QTableWidget(16, 3);
    this->TableWidget3D->setHorizontalHeaderLabels({"X", "Y", "Z"});
    this->TableWidget3D->setEditTriggers(QTableWidget::NoEditTriggers);

    this->TableWidget4D = new QTableWidget(16, 4);
    this->TableWidget4D->setHorizontalHeaderLabels({"X", "Y", "Z", "W"});
    this->TableWidget4D->setEditTriggers(QTableWidget::NoEditTriggers);

    sidebarLayout2->addWidget(tableLabel3D);
    sidebarLayout2->addWidget(this->TableWidget3D);
    sidebarLayout2->addWidget(tableLabel4D);
    sidebarLayout2->addWidget(this->TableWidget4D);

    this->RenderWidget = new QVTKOpenGLNativeWidget(this);

    auto mainLayout = new QHBoxLayout;
    mainLayout->addLayout(sidebarLayout);
    mainLayout->addWidget(this->RenderWidget);
    mainLayout->addLayout(sidebarLayout2);
    mainLayout->setStretchFactor(sidebarLayout, 20);
    mainLayout->setStretchFactor(this->RenderWidget, 60);
    mainLayout->setStretchFactor(sidebarLayout2, 25);

    auto centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    this->setCentralWidget(centralWidget);
}

void MainWindow::setDefaultValues() {
    double l[4] = {0.0, 0.0, 1.5, 1.0};
    this->HyperCubeActor->SetLightOrigin(l);
    this->LightOriginSpinBoxX->setValue(l[0]);
    this->LightOriginSpinBoxY->setValue(l[1]);
    this->LightOriginSpinBoxZ->setValue(l[2]);
    this->LightOriginSpinBoxW->setValue(l[3]);

    double o[4] = {0.0, 0.0, 0.0, 0.0};
    this->HyperCubeActor->GetHyperPlane()->SetOrigin(o);
    this->PlaneOriginSpinBoxX->setValue(o[0]);
    this->PlaneOriginSpinBoxY->setValue(o[1]);
    this->PlaneOriginSpinBoxZ->setValue(o[2]);
    this->PlaneOriginSpinBoxW->setValue(o[3]);

    double n[4] = {0.0, 0.0, 1.0, 0.0};
    this->HyperCubeActor->GetHyperPlane()->SetNormal(n);
    this->PlaneNormalSpinBoxX->setValue(n[0]);
    this->PlaneNormalSpinBoxY->setValue(n[1]);
    this->PlaneNormalSpinBoxZ->setValue(n[2]);
    this->PlaneNormalSpinBoxW->setValue(n[3]);

    for (auto widget : this->AngleWidgets) {
        widget->Slider->setValue(0);
    }
}

void MainWindow::connectSignals() {
#define connectSpinbox(dim, name)                                         \
    connect(this->name##SpinBox##dim, SIGNAL(valueChanged(double)), this, \
            SLOT(on##name##Changed##dim(double)))

    connectSpinbox(X, LightOrigin);
    connectSpinbox(Y, LightOrigin);
    connectSpinbox(Z, LightOrigin);
    connectSpinbox(W, LightOrigin);

    connectSpinbox(X, PlaneOrigin);
    connectSpinbox(Y, PlaneOrigin);
    connectSpinbox(Z, PlaneOrigin);
    connectSpinbox(W, PlaneOrigin);

    connectSpinbox(X, PlaneNormal);
    connectSpinbox(Y, PlaneNormal);
    connectSpinbox(Z, PlaneNormal);
    connectSpinbox(W, PlaneNormal);
#undef connectSpinbox

#define connectSlider(dim)                                                   \
    connect(this->AngleSlider##dim->Slider, SIGNAL(valueChanged(int)), this, \
            SLOT(onAngleChanged##dim(int)));

    connectSlider(XY);
    connectSlider(XZ);
    connectSlider(XW);
    connectSlider(YZ);
    connectSlider(YW);
    connectSlider(ZW);
#undef connectSlider

    connect(this->AnimationControlButton, SIGNAL(toggled(bool)), this,
            SLOT(onAnimationStart(bool)));
    connect(this->ResetButton, SIGNAL(clicked()), this, SLOT(onResetPressed()));

    this->Connector = vtkEventQtSlotConnect::New();
    this->Connector->Connect(
        this->AnimationCue, vtkCommand::AnimationCueTickEvent, this,
        SLOT(onAnimationEvents(vtkObject *, unsigned long, void *, void *)));
    this->Connector->Connect(
        this->AnimationCue, vtkCommand::EndAnimationCueEvent, this,
        SLOT(onAnimationEvents(vtkObject *, unsigned long, void *, void *)));
}

void MainWindow::onAnimationEvents(vtkObject *caller, unsigned long event,
                                   void *calldata, void *) {
    switch (event) {
        case vtkCommand::AnimationCueTickEvent: {
            // the slider calls the updateCube() method which is resource-heavy
            // we temporary suppress it and only call it once at the end instead of once per plane
            this->SuppressUpdate = true;
            bool needToUpdate = false;
            for (auto widget : this->AngleWidgets) {
                if (widget->CheckBox->isChecked()) {
                    auto delta = widget->PreviousValue + 1;
                    delta = delta >= 180 ? -180 : delta;
                    widget->Slider->setValue(delta);
                    needToUpdate = true;
                }
            }
            this->SuppressUpdate = false;
            if (needToUpdate) {
                this->updateCube();
            }
            QCoreApplication::processEvents();
            break;
        }
    }
};
void MainWindow::updateCube() {
    if (this->SuppressUpdate) {
        return;
    }
    bool success = this->HyperCubeActor->UpdatePositions();
    if (success) {
        this->NoticeLabel->setText("");
    } else {
        this->NoticeLabel->setText(
            "No shadow can be casted from one of the points of the Hypercube "
            "on the Hyperplane. Try changing the Light origin or the "
            "Hyperplane origin and normal.");
    }

    this->updateTables();
    this->RenderWindow->Render();
    this->Renderer->Render();
}

void MainWindow::updateTable4D() {
    vtkSmartPointer<vtkHyperPoints> points =
        this->HyperCubeActor->GetHyperCube()->GetPoints();
    for (int r = 0; r < points->GetNumberOfPoints(); r++) {
        auto p = points->GetPoint(r);
        for (int c = 0; c < 4; c++) {
            auto item = new QTableWidgetItem(std::to_string(p[c]).c_str());
            this->TableWidget4D->setItem(r, c, item);
        }
    }
}

void MainWindow::updateTable3D() {
    vtkSmartPointer<vtkPoints> points =
        this->HyperCubeActor->GetProjectedPoints();
    for (int r = 0; r < points->GetNumberOfPoints(); r++) {
        auto p = points->GetPoint(r);
        for (int c = 0; c < 3; c++) {
            auto item = new QTableWidgetItem(std::to_string(p[c]).c_str());
            this->TableWidget3D->setItem(r, c, item);
        }
    }
}

void MainWindow::updateTables() {
    this->updateTable3D();
    this->updateTable4D();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    event->accept();
    this->AnimationScene->Stop();
}

// Slots
void MainWindow::onAnimationStart(bool) {
    if (this->AnimationScene->IsInPlay()) {
        this->AnimationControlButton->setText("Start Animation");
        this->AnimationScene->Stop();
    } else {
        this->AnimationControlButton->setText("Stop Animation");
        this->AnimationScene->Play();
    }
}

void MainWindow::onResetPressed() {
    this->SuppressUpdate = true;
    this->setDefaultValues();
    this->HyperCubeActor->GetTransform4D()->Identity();
    this->SuppressUpdate = false;
    this->updateCube();
}

#define AngleSlot(dim)                                          \
    void MainWindow::onAngleChanged##dim(int a) {               \
        auto delta = a - this->AngleSlider##dim->PreviousValue; \
        this->AngleSlider##dim->PreviousValue = a;              \
        this->HyperCubeActor->Rotate##dim(delta);               \
        this->updateCube();                                     \
    }

AngleSlot(XY);
AngleSlot(XZ);
AngleSlot(XW);
AngleSlot(YZ);
AngleSlot(YW);
AngleSlot(ZW);
#undef AngleSlot

#define LightOriginSlot(X, i)                            \
    void MainWindow::onLightOriginChanged##X(double a) { \
        auto l = this->HyperCubeActor->GetLightOrigin(); \
        l[i] = a;                                        \
        this->HyperCubeActor->SetLightOrigin(l);         \
        this->updateCube();                              \
    }
LightOriginSlot(X, 0);
LightOriginSlot(Y, 1);
LightOriginSlot(Z, 2);
LightOriginSlot(W, 3);
#undef LightOriginSlot

#define PlaneSlot(name, X, i)                                        \
    void MainWindow::onPlane##name##Changed##X(double a) {           \
        auto d = this->HyperCubeActor->GetHyperPlane()->Get##name(); \
        d[i] = a;                                                    \
        this->HyperCubeActor->GetHyperPlane()->Set##name(d);         \
        this->updateCube();                                          \
    }

PlaneSlot(Origin, X, 0);
PlaneSlot(Origin, Y, 1);
PlaneSlot(Origin, Z, 2);
PlaneSlot(Origin, W, 3);

PlaneSlot(Normal, X, 0);
PlaneSlot(Normal, Y, 1);
PlaneSlot(Normal, Z, 2);
PlaneSlot(Normal, W, 3);
#undef PlaneSlot
