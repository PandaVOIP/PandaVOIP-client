#ifndef PANDAVOIP_H
#define PANDAVOIP_H
#include "voipcontroller.h"


class PandaVOIP {
    public:
        PandaVOIP();

    private:
        VoipController* voip_controller;
};

#endif // PANDAVOIP_H
