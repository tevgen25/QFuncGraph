#include "mainwindow.h"
#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    AppWindow *appWindow = new AppWindow();

    appWindow->resize(800, 600);
    appWindow->show();

    return app.exec();
}
