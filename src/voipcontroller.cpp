#include <QIODevice>
#include <QAudioFormat>
#include <QString>

#include <vector>
#include <stdlib.h>
#include <iostream>
#include <time.h>

#include "voipcontroller.h"

using namespace std;

#ifdef QT_DEBUG
    #define SERVER_IP "127.0.0.1"
#else
    #define SERVER_IP "73.161.137.88"
#endif

VoipController::VoipController(PandaVOIP* gui){
    this->gui = gui;
    this->loadOrCreateClientId();
    command_conn = NULL;
    voice_conn = NULL;

    voipIO = new VoipAudioIODevice();
    voipIO->open(QIODevice::ReadWrite);
    voipIO->setClientId(this->clientID);

    QAudioFormat format;
    format.setSampleRate(48000);
    format.setChannelCount(2);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    audioInput = new QAudioInput(format);

    audioInput->start(voipIO);

}

void VoipController::loadOrCreateClientId(){
    /*
    FILE * client_file = fopen("client.txt", "w+");
    if (ftell(client_file) == 0){

        client_id = (rand() * RAND_MAX) % 100000000;
        fprintf(client_file, "%d", client_id);
    } else {
        fscanf(client_file, "%1d", &client_id);
    }
    cout << client_id << endl;
    fclose(client_file);
    */

    srand(time(NULL));
    clientID = (rand() * RAND_MAX) % 100000000;
}

bool VoipController::controlConnect(){
    command_conn = new VoipTCPSocket(this, SERVER_IP, 50039, clientID);
    return true;
}

void VoipController::receive_text_message(char * message){
    char user[9];
    char text_message[strlen(message) - 8];

    strncpy(user, message, 8);
    user[8] = '\0';

    strncpy(text_message, message+8, strlen(message) - 8);
    text_message[strlen(message) - 8] = '\0';

    this->gui->new_message(QString(user), QString(text_message));
}

bool VoipController::send_text_message(QString message){
    if (command_conn == NULL){
        return false;
    }

    command_conn->send_text_message((char *)message.toStdString().c_str());
    return true;
}

bool VoipController::connectVoice(){
    if (command_conn == NULL){
        return false;
    }
    if (voice_conn == NULL){
        command_conn->connect_to_voice();
        voice_conn = new VoipUDPSocket(SERVER_IP, 50038);
        voipIO->setVoiceConn(voice_conn);
        return true;
    } else {
        command_conn->disconnect_from_voice();
        voice_conn = NULL;
        voipIO->setVoiceConn(voice_conn);
        return true;
    }
}

void VoipController::updateVoiceUsers(char * users_str){
    vector<QString> users;
    unsigned int i;
    char user[9];

    for (i = 0; i < strlen(users_str); i += 8){
        strncpy(user, users_str + i, 8);
        user[8] = '\0';
        users.push_back(QString(user));
    }
    gui->updateVoiceUsers(users);

}
