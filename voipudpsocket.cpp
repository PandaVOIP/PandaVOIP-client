#include "voipudpsocket.h"

#include <iostream>
#include <QHostAddress>
#include <QNetworkDatagram>

using namespace std;

VoipUDPSocket::VoipUDPSocket(QString ip, quint16 port){
    this->udp_socket = new QUdpSocket;
    this->ip = ip;
    this->port = port;
    connect(this->udp_socket, SIGNAL(readyRead()), this, SLOT(read_data()));
}

void VoipUDPSocket::read_data(){
    int spx_length;
    unsigned char * spx_buffer = (unsigned char *)malloc(32000 * 4);
    QHostAddress dummy_addr;
    quint16 dummy_port;
    spx_length = this->udp_socket->readDatagram((char *)spx_buffer, 32000*4, &dummy_addr, &dummy_port);

    int sender_id;
    memcpy(&sender_id, spx_buffer, sizeof(sender_id));
    spx_length -= sizeof(sender_id);

    emit voice_ready(sender_id, spx_buffer + sizeof(sender_id), spx_length);
}

int VoipUDPSocket::send_voice(int client_id, unsigned char* spx_data, int spx_length){
    QHostAddress addr(this->ip);

    int payload_length = sizeof(client_id) + spx_length;
    char payload[payload_length];
    memcpy(payload, &client_id, sizeof(client_id));
    memcpy(payload + sizeof(client_id), spx_data, spx_length);

    this->udp_socket->writeDatagram((const char *)payload, payload_length, addr, this->port);

    free(spx_data);

    return 0;
}
