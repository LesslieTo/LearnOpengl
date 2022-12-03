#include "Widgets/glWidget.h"
#include <QApplication>
#include <QSurfaceFormat>
int main(int argc, char *argv[])
{
    auto fmt = QSurfaceFormat::defaultFormat();
    fmt.setRenderableType(QSurfaceFormat::OpenGL);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    fmt.setVersion(3, 3);
    QSurfaceFormat::setDefaultFormat(fmt);

    QApplication a(argc, argv);
    auto AppPath = qApp->applicationFilePath().split("MacOS").at(0);

    glWidget w;
    w.setFixedSize(800, 800);
    w.show();
    return a.exec();
}
