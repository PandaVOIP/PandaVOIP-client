#include "voiptcpsocket.h"

#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonArray>
#include <iostream>

using namespace std;

#define BUFFER_SIZE 8192

// command server class
VoipTCPSocket::VoipTCPSocket(VoipController* controller, QString ip, quint16 port, int client_id)
{
    // reference to the parent
    this->controller = controller;
    this->tcp_socket = new QTcpSocket;
    this->ip = ip;
    this->port = port;
    this->client_id = client_id;

    QHostAddress addr(this->ip);

    // placeholders
    connect(this->tcp_socket, SIGNAL(connected()), this, SLOT(tcp_connected()));
    connect(this->tcp_socket, SIGNAL(disconnected()), this, SLOT(tcp_disconnected()));
    connect(this->tcp_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(tcp_error(QAbstractSocket::SocketError)));

    // connect to the command server
    this->tcp_socket->connectToHost(addr, this->port);
    connect(this->tcp_socket, SIGNAL(readyRead()), this, SLOT(read_data()));

    // tell the server we are establishing the command connection
    QJsonObject json_message{
        {"command", "establish"}
    };
    this->send_data(json_message);

}

void VoipTCPSocket::tcp_connected(){

}

void VoipTCPSocket::tcp_disconnected(){

}

void VoipTCPSocket::tcp_error(QAbstractSocket::SocketError error){
    Q_UNUSED(error);
}

// wrapper for sending messages
void VoipTCPSocket::send_data(QJsonObject json_data){
    // client id is included in all messages
    json_data["client_id"] = client_id;

    // byte buffer for sending
    char data[BUFFER_SIZE];
    memset(data, 0, BUFFER_SIZE);

    // output the json to a string
    QJsonDocument doc(json_data);
    QByteArray json = doc.toJson(QJsonDocument::Compact);
    string json_string = json.toStdString();
    char raw[json_string.size()];
    memcpy(raw, json_string.c_str(), json_string.size());
    memcpy(data, raw, json_string.size());

    // send json string
    this->tcp_socket->write(data, BUFFER_SIZE);
}

void VoipTCPSocket::read_data(){
    // all transmissions are preceded by the length, a 10 characater 0 padding number
    char read_size[11];
    // actual data buffer
    char buffer[BUFFER_SIZE];

    // do not read if there is less than 11 bytes
    // this means we recieved the length of the transmission
    // but not the transmission itself
    if (tcp_socket->bytesAvailable() <= 10){
        return;
    }
    // read the length of the transmission
    int bytes_read = tcp_socket->read(read_size, 10);
    read_size[10] = '\0';
    // read the actual transmission
    bytes_read = tcp_socket->read(buffer, atoi(read_size));
    buffer[bytes_read] = '\0';

    // create a json object out of the transmission
    QJsonDocument doc = QJsonDocument::fromJson(QString(buffer).toUtf8());
    QJsonObject response = doc.object();

    // get the command
    QString command = response["command"].toString();

#ifdef QT_DEBUG
    cout << command.toStdString() << endl;
#endif

    // see which command it is
    if (QString::compare(command, "update_voice_users") == 0){
        controller->updateVoiceUsers(response);
    } else if (QString::compare(command, "update_chat_users") == 0){
        controller->updateChatUsers(response);
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
