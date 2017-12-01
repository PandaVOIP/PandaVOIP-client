#include "voiptcpsocket.h"

#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonArray>
#include <iostream>

using namespace std;

#define BUFFER_SIZE 8192

VoipTCPSocket::VoipTCPSocket(VoipController* controller, QString ip, quint16 port, int client_id)
{
    this->controller = controller;
    this->tcp_socket = new QTcpSocket;
    this->ip = ip;
    this->port = port;
    this->client_id = client_id;

    QHostAddress addr(this->ip);

    connect(this->tcp_socket, SIGNAL(connected()), this, SLOT(tcp_connected()));
    connect(this->tcp_socket, SIGNAL(disconnected()), this, SLOT(tcp_disconnected()));
    connect(this->tcp_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(tcp_error(QAbstractSocket::SocketError)));

    this->tcp_socket->connectToHost(addr, this->port);
    connect(this->tcp_socket, SIGNAL(readyRead()), this, SLOT(read_data()));
    QJsonObject json_message{
        {"command", "establish"}
    };
    this->send_data(json_message);
    //this->tcp_socket->write((char*)&client_id, sizeof(client_id));

}

void VoipTCPSocket::tcp_connected(){

}

void VoipTCPSocket::tcp_disconnected(){

}

void VoipTCPSocket::tcp_error(QAbstractSocket::SocketError error){
    Q_UNUSED(error);
}

void VoipTCPSocket::send_data(QJsonObject json_data){
    json_data["client_id"] = client_id;

    QJsonDocument doc(json_data);

    QByteArray json = doc.toJson(QJsonDocument::Compact);

    char data[BUFFER_SIZE];
    memset(data, 0, BUFFER_SIZE);

    string json_string = json.toStdString();
    char raw[json_string.size()];
    memcpy(raw, json_string.c_str(), json_string.size());

    memcpy(data, raw, json_string.size());

    this->tcp_socket->write(data, BUFFER_SIZE);
}

void VoipTCPSocket::read_data(){
    char read_size[11];
    char buffer[BUFFER_SIZE];
    if (tcp_socket->bytesAvailable() <= 10){
        return;
    }
    int bytes_read = tcp_socket->read(read_size, 10);
    read_size[10] = '\0';
    bytes_read = tcp_socket->read(buffer, atoi(read_size));
    buffer[bytes_read] = '\0';

    QJsonDocument doc = QJsonDocument::fromJson(QString(buffer).toUtf8());
    QJsonObject response = doc.object();

    QString command = response["command"].toString();

    cout << command.toStdString() << endl;

    if (QString::compare(command, "update_voice_users") == 0){
        controller->updateVoiceUsers(response);
    } else if (QString::compare(command, "new_message") == 0){
        controller->receive_text_message(response);
    }
    if (tcp_socket->bytesAvailable()){
        this->read_data();
    }
}

void VoipTCPSocket::connect_to_voice(){    
    QJsonObject json_message{
        {"command", "voice connect"}
    };

    this->send_data(json_message);
}

void VoipTCPSocket::send_command(QJsonObject message){
    this->send_data(message);
}

void VoipTCPSocket::disconnect_from_voice(){
    QJsonObject json_message{
        {"command", "voice disconnect"}
    };

    this->send_data(json_message);
}
