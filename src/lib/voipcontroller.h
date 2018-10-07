#ifndef VOIPCONTROLLER_H
#define VOIPCONTROLLER_H

#include <QObject>
#include <QAudioInput>
#include "voipudpsocket.h"
#include "voipaudioiodevice.h"
#include "voiptcpsocket.h"

class VoipTCPSocket;
class VoipAudioIODevice;
class PandaVOIPUI;

class VoipController : public QObject {
    Q_OBJECT

    private:
        void loadOrCreateClientId();

        int clientID;
        QAudioInput* audioInput;
        VoipAudioIODevice* voipIO;
        VoipTCPSocket* command_conn;
        VoipUDPSocket* voice_conn;

    public:
        VoipController();
        bool controlConnect();
        void receive_text_message(QJsonObject);
        bool send_text_message(QString);
        void updateChatUsers(QJsonObject);
        void updateVoiceUsers(QJsonObject);

    public slots:
        bool connectVoice();

    signals:
        void updateVoiceUsers(std::vector<QString>);
        void updateChatUsers(std::vector<QString>);
        void onNewMessage(QString, QString);
};

#endif // VOIPCONTROLLER_H
