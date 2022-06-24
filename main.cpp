#include "mainwindow.h"
#include "controller.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("Fusion");
    MainWindow *w = new MainWindow();
    Controller *c = new Controller(w);

    c->connectFunctions();
    w->show();

    return a.exec();
}
