#include "Application.hpp"

#include "HyperPoints.hpp"

#include <qtablewidget.h>

#define connectSpinbox(dim, name)                                         \
    connect(this->name##SpinBox##dim, SIGNAL(valueChanged(double)), this, \
            SLOT(On##name##Changed##dim(double)))

#define connectSpinBoxes(name) \
    connectSpinbox(X, name);   \
    connectSpinbox(Y, name);   \
    connectSpinbox(Z, name);   \
    connectSpinbox(W, name);

void printarray(double arr[3]) {
    std::cout << "(";
    for (int i = 0; i < 3; i++) {
        std::cout << arr[i] << ",";
    }
    std::cout << ")" << std::endl;
}
void printarray4(double arr[4]) {
    std::cout << "(";
    for (int i = 0; i < 4; i++) {
        std::cout << arr[i] << ",";
    }
    std::cout << ")" << std::endl;
}

MainWindow::MainWindow(int argc, char *argv[]) {
    this->setupUI();

    this->RenderWindow = vtkGenericOpenGLRenderWindow::New();
    this->RenderWidget->setRenderWindow(this->RenderWindow.Get());
    this->Renderer = vtkRenderer::New();
    this->RenderWindow->AddRenderer(this->Renderer);

    this->HyperCubeActor = vtkHyperCubeProjectionActor::New();
    double l[4] = {5.0, 5.0, 5.0, 5.0};
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
    double n[4] = {0.0, 0.0, 1.0, 1.0};
    this->HyperCubeActor->GetHyperPlane()->SetNormal(n);
    this->PlaneNormalSpinBoxX->setValue(n[0]);
    this->PlaneNormalSpinBoxY->setValue(n[1]);
    this->PlaneNormalSpinBoxZ->setValue(n[2]);
    this->PlaneNormalSpinBoxW->setValue(n[3]);

    this->HyperCubeActor->UpdatePositions();
    this->UpdateTables();

    connectSpinBoxes(LightOrigin);
    connectSpinBoxes(PlaneNormal);
    connectSpinBoxes(PlaneOrigin);

    this->Renderer->AddActor(this->HyperCubeActor);

    this->RenderWidget->show();
    this->show();
}

MainWindow::~MainWindow() {
    // this->RWI->Delete();
    this->HyperCubeActor->Delete();
    this->Renderer->Delete();
}

void MainWindow::setupUI() {
    auto sidebarLayout = new QGridLayout;
    sidebarLayout->setAlignment(Qt::AlignTop);

    auto locLabel = new QLabel("Light Source Location");
    auto locLabel2 = new QLabel("Hyperplane Origin");
    auto locLabel3 = new QLabel("Hyperplane Normal");

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
    sidebarLayout->addWidget(locLabel, i, 0, 1, 2, Qt::AlignHCenter);
    i++;
    sidebarLayout->addWidget(this->LightOriginSpinBoxX, i, 1);
    sidebarLayout->addWidget(labelX, i, 0);
    i++;
    sidebarLayout->addWidget(this->LightOriginSpinBoxY, i, 1);
    sidebarLayout->addWidget(labelY, i, 0);
    i++;
    sidebarLayout->addWidget(this->LightOriginSpinBoxZ, i, 1);
    sidebarLayout->addWidget(labelZ, i, 0);
    i++;
    sidebarLayout->addWidget(this->LightOriginSpinBoxW, i, 1);
    sidebarLayout->addWidget(labelW, i, 0);
    i++;

    labelX = new QLabel("X:");
    labelY = new QLabel("Y:");
    labelZ = new QLabel("Z:");
    labelW = new QLabel("W:");
    sidebarLayout->addWidget(locLabel2, i, 0, 1, 2, Qt::AlignHCenter);
    i++;
    sidebarLayout->addWidget(this->PlaneOriginSpinBoxX, i, 1);
    sidebarLayout->addWidget(labelX, i, 0);
    i++;
    sidebarLayout->addWidget(this->PlaneOriginSpinBoxY, i, 1);
    sidebarLayout->addWidget(labelY, i, 0);
    i++;
    sidebarLayout->addWidget(this->PlaneOriginSpinBoxZ, i, 1);
    sidebarLayout->addWidget(labelZ, i, 0);
    i++;
    sidebarLayout->addWidget(this->PlaneOriginSpinBoxW, i, 1);
    sidebarLayout->addWidget(labelW, i, 0);
    i++;

    labelX = new QLabel("X:");
    labelY = new QLabel("Y:");
    labelZ = new QLabel("Z:");
    labelW = new QLabel("W:");
    sidebarLayout->addWidget(locLabel3, i, 0, 1, 2, Qt::AlignHCenter);
    i++;
    sidebarLayout->addWidget(this->PlaneNormalSpinBoxX, i, 1);
    sidebarLayout->addWidget(labelX, i, 0);
    i++;
    sidebarLayout->addWidget(this->PlaneNormalSpinBoxY, i, 1);
    sidebarLayout->addWidget(labelY, i, 0);
    i++;
    sidebarLayout->addWidget(this->PlaneNormalSpinBoxZ, i, 1);
    sidebarLayout->addWidget(labelZ, i, 0);
    i++;
    sidebarLayout->addWidget(this->PlaneNormalSpinBoxW, i, 1);
    sidebarLayout->addWidget(labelW, i, 0);
    i++;

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

void MainWindow::UpdateCube() {
    this->HyperCubeActor->UpdatePositions();
    this->UpdateTables();
    this->Renderer->Render();
    this->RenderWindow->Render();
}

void MainWindow::UpdateTable4D() {
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

void MainWindow::UpdateTable3D() {
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

void MainWindow::UpdateTables() {
    this->UpdateTable3D();
    this->UpdateTable4D();
}
#define LightOriginSlot(X, i)                            \
    void MainWindow::OnLightOriginChanged##X(double a) { \
        auto l = this->HyperCubeActor->GetLightOrigin(); \
        l[i] = a;                                        \
        this->HyperCubeActor->SetLightOrigin(l);         \
        this->UpdateCube();                              \
    }

#define PlaneSlot(name, X, i)                                        \
    void MainWindow::OnPlane##name##Changed##X(double a) {           \
        auto d = this->HyperCubeActor->GetHyperPlane()->Get##name(); \
        d[i] = a;                                                    \
        this->HyperCubeActor->GetHyperPlane()->Set##name(d);         \
        this->UpdateCube();                                          \
    }

LightOriginSlot(X, 0);
LightOriginSlot(Y, 1);
LightOriginSlot(Z, 2);
LightOriginSlot(W, 3);

PlaneSlot(Origin, X, 0);
PlaneSlot(Origin, Y, 1);
PlaneSlot(Origin, Z, 2);
PlaneSlot(Origin, W, 3);

PlaneSlot(Normal, X, 0);
PlaneSlot(Normal, Y, 1);
PlaneSlot(Normal, Z, 2);
PlaneSlot(Normal, W, 3);
