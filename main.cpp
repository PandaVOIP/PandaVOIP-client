#include "pandavoip.h"
#include <QApplication>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    PandaVOIP w;
    w.show();

    return a.exec();
}
