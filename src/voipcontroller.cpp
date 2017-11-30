#include <QIODevice>
#include <QAudioFormat>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>

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

void VoipController::receive_text_message(QJsonObject data){
    this->gui->new_message(data["message"].toObject()["sender_id"].toString(), data["message"].toObject()["text"].toString());
}

bool VoipController::send_text_message(QString message){
    if (command_conn == NULL){
        return false;
    }

    QJsonObject json_message{
        {"command", "text message"},
        {"message", message}
    };

    command_conn->send_command(json_message);
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

void VoipController::updateVoiceUsers(QJsonObject data){
    vector<QString> users;
    unsigned int i;
    char user[9];

    for (i = 0; i < data["users"].toArray().size(); i += 8){
        /*
        strncpy(user, users_str + i, 8);
        user[8] = '\0';
        users.push_back(QString(user));
        */
    }
    gui->updateVoiceUsers(users);

}
