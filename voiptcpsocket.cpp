#include "voiptcpsocket.h"

#include <QHostAddress>
#include <iostream>

using namespace std;

#define BUFFER_SIZE 1024

VoipTCPSocket::VoipTCPSocket(QString ip, quint16 port, int client_id)
{
    this->tcp_socket = new QTcpSocket;
    this->ip = ip;
    this->port = port;
    this->client_id = client_id;

    QHostAddress addr(this->ip);

    this->tcp_socket->connectToHost(addr, this->port);
    connect(this->tcp_socket, SIGNAL(readyRead()), this, SLOT(read_data()));
    this->send_data((char *)0, 0);
    //this->tcp_socket->write((char*)&client_id, sizeof(client_id));

}

void VoipTCPSocket::send_data(char* send_data, int length){
    char buffer[BUFFER_SIZE];

    memset(buffer, 0, BUFFER_SIZE);

    memcpy(buffer, &client_id, sizeof(client_id));
    memcpy(buffer + sizeof(client_id), &length, sizeof(length));
    memcpy(buffer + sizeof(client_id) + sizeof(length), send_data, length);

    this->tcp_socket->write(buffer, BUFFER_SIZE);
}

void VoipTCPSocket::read_data(){
    char buffer[BUFFER_SIZE];

    int bytes_read = tcp_socket->read(buffer, BUFFER_SIZE);
    buffer[bytes_read] = '\0';
    cout << buffer << endl;
}

void VoipTCPSocket::connect_to_voice(){
    char * message = "voice connect";
    this->send_data(message, strlen(message));
}
