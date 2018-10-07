#include "pandavoip.h"
#include "voipcontroller.h"
#include <QApplication>
#include <QSettings>
#include <QString>
#include <iostream>

using namespace std;

PandaVOIP::PandaVOIP() {
    // Defining the below so that future QSettings don't need to be reinitialized over and over
    QApplication::setOrganizationName("PandaVOIP");
    QApplication::setApplicationName("PandaVOIP");
}

void PandaVOIP::init() {
    QSettings settings;
    settings.remove("user_name");
    QString user_name = settings.value("user_name").toString();

    if (user_name.isNull()) {
        emit openLoginPopup();
    }

    // Voip
    this->voip_controller = new VoipController();
    //this->voipController->controlConnect();
}

void PandaVOIP::setUsername(QString user_name) {
    QSettings settings;
    settings.setValue("user_name", user_name);
}
