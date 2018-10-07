#ifndef VOIPTCPSOCKET_H
#define VOIPTCPSOCKET_H

#include <QTcpSocket>
#include <QString>
#include <QObject>
#include <QAbstractSocket>
#include <QJsonObject>

class VoipController;

class VoipTCPSocket : public QObject
{
    Q_OBJECT
public:
    VoipTCPSocket(VoipController*, QString, quint16, int);
    void connect_to_voice();
    void send_command(QJsonObject);
    void disconnect_from_voice();
public slots:
    void read_data();
    void tcp_connected();
    void tcp_disconnected();
    void tcp_error(QAbstractSocket::SocketError);
private:
    void send_data(QJsonObject);

    VoipController* controller;
    QTcpSocket* tcp_socket;
    QString ip;
    quint16 port;
    int client_id;
};

#endif // VOIPTCPSOCKET_H
