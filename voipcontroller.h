#ifndef VOIPCONTROLLER_H
#define VOIPCONTROLLER_H

#include <QObject>
#include <QAudioInput>

#include "voipudpsocket.h"

class VoipTCPSocket;
class VoipAudioIODevice;
class PandaVOIP;

class VoipController : public QObject
{
    Q_OBJECT
public:
    VoipController(PandaVOIP*);
    bool controlConnect();
    void receive_text_message(char *);
    bool send_text_message(QString);
    void updateVoiceUsers(char *);

public slots:
    bool connectVoice();

private:
    void loadOrCreateClientId();

    int clientID;
    PandaVOIP* gui;
    QAudioInput* audioInput;
    VoipAudioIODevice* voipIO;
    VoipTCPSocket* command_conn;
    VoipUDPSocket* voice_conn;
};

#include "voipaudioiodevice.h"
#include "voiptcpsocket.h"
#include "pandavoip.h"

#endif // VOIPCONTROLLER_H
