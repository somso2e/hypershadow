#include "Application.hpp"
#include "QVTKRenderWidget.h"

#include <QApplication>
#include <QMainWindow>
#include <QSurfaceFormat>

int main(int argc, char *argv[]) {
    QSurfaceFormat::setDefaultFormat(QVTKRenderWidget::defaultFormat());
    QApplication app(argc, argv);

    MainWindow mainWindow(argc, argv);
    mainWindow.setWindowTitle("4D Shadows");
    // mainWindow.show();

    return app.exec();
}
