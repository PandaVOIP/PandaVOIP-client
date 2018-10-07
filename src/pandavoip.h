#ifndef PANDAVOIP_H
#define PANDAVOIP_H
#include "voipcontroller.h"
#include <QObject>

using namespace std;

class PandaVOIP : public QObject {
    Q_OBJECT

    public:
        VoipController *voip_controller;

        PandaVOIP();
        void init();
        void setUsername(QString user_name);

    signals:
        void openLoginPopup();
};

#endif // PANDAVOIP_H
