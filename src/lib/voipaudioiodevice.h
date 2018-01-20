#ifndef VOIPAUDIOIODEVICE_H
#define VOIPAUDIOIODEVICE_H

#include <QtCore/QIODevice>
#include <QAudio>
#include <QAudioOutput>

#include <queue>
#include <vector>

#include "voipudpsocket.h"
#include "voiptcpsocket.h"
#include "opusutil.h"

class VoipAudioIODevice: public QIODevice
{
    Q_OBJECT
public:
    explicit VoipAudioIODevice(QObject *parent = 0);
    void setVoiceConn(VoipUDPSocket*);
    void setClientId(int);
public slots:
    void read_socket(int, unsigned char *, int);
protected:
    qint64 readData(char *data, qint64 maxSize);
    qint64 writeData(const char *data, qint64 maxSize);

private:
    void loadOrCreateClientId();
    OpusVoiceDecoder* get_or_create_decoder(int);

    VoipUDPSocket* voice_conn;
    VoipTCPSocket* command_conn;
    std::queue <unsigned char*> wavQueue;
    OpusVoiceEncoder* encoder;
    std::vector<OpusVoiceDecoder*> decoders;

    int timer;
    int clientID;
};

#endif // VOIPAUDIOIODEVICE_H
