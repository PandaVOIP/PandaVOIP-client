#ifndef VOIPUDPSOCKET_H
#define VOIPUDPSOCKET_H

#include <QUdpSocket>
#include <QString>
#include <QObject>

class VoipUDPSocket : public QObject
{
    Q_OBJECT
public:
    VoipUDPSocket(QString, quint16);
    int send_voice(int, unsigned char*, int);
private:
    QUdpSocket* udp_socket;
    QString ip;
    quint16 port;
public slots:
    void read_data();
signals:
    void voice_ready(int, unsigned char *, int);
};

#endif // VOIPUDPSOCKET_H
