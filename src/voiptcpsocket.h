#ifndef VOIPTCPSOCKET_H
#define VOIPTCPSOCKET_H

#include <QTcpSocket>
#include <QString>
#include <QObject>
#include <QAbstractSocket>

#include "voipcontroller.h"

class VoipTCPSocket : public QObject
{
    Q_OBJECT
public:
    VoipTCPSocket(VoipController*, QString, quint16, int);
    void connect_to_voice();
    void send_text_message(char *);
    void disconnect_from_voice();
public slots:
    void read_data();
    void tcp_connected();
    void tcp_disconnected();
    void tcp_error(QAbstractSocket::SocketError);
private:
    void send_data(char *, int);

    VoipController* controller;
    QTcpSocket* tcp_socket;
    QString ip;
    quint16 port;
    int client_id;
};

#endif // VOIPTCPSOCKET_H
