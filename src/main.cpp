#include "pandavoip_ui.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    PandaVOIPUI w;

    w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    w.show();

    return a.exec();
}
