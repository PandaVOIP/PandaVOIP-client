#include "voipudpsocket.h"

#include <iostream>
#include <QHostAddress>
#include <QNetworkDatagram>

using namespace std;

VoipUDPSocket::VoipUDPSocket(QString ip, quint16 port){
    // create UDP socket
    this->udp_socket = new QUdpSocket;
    this->ip = ip;
    this->port = port;
    connect(this->udp_socket, SIGNAL(readyRead()), this, SLOT(read_data()));
}

void VoipUDPSocket::read_data(){
    // length of received data
    int opusLength;
    unsigned char * opusBuffer = (unsigned char *)malloc(32000 * 4);
    QHostAddress dummy_addr;
    quint16 dummy_port;
    // read data
    opusLength = this->udp_socket->readDatagram((char *)opusBuffer, 32000*4, &dummy_addr, &dummy_port);

    // seperate the ID from the data
    int senderID;
    memcpy(&senderID, opusBuffer, sizeof(senderID));
    opusLength -= sizeof(senderID);

    // read ready
    emit voice_ready(senderID, opusBuffer + sizeof(senderID), opusLength);
}

int VoipUDPSocket::send_voice(int clientID, unsigned char* opusData, int opusLength){
    QHostAddress addr(this->ip);

    // copy into a payload
    int payloadLength = sizeof(clientID) + opusLength;
    char payload[payloadLength];
    memcpy(payload, &clientID, sizeof(clientID));
    memcpy(payload + sizeof(clientID), opusData, opusLength);

    // send data
    this->udp_socket->writeDatagram((const char *)payload, payloadLength, addr, this->port);

    free(opusData);

    return 0;
}
