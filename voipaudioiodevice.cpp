#include "voipaudioiodevice.h"
#include "opusutil.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

using namespace std;

VoipAudioIODevice::VoipAudioIODevice(QObject *parent): QIODevice(parent){
    this->loadOrCreateClientId();
    voice_conn = new VoipUDPSocket("73.161.137.88", 50038);
    command_conn = new VoipTCPSocket("73.161.137.88", 50039, client_id);
    encoder = new OpusVoiceEncoder;

    command_conn->connect_to_voice();
    connect(this->voice_conn, SIGNAL(voice_ready(int, unsigned char *, int)), this, SLOT(read_socket(int, unsigned char*, int)));

    return;
}

void VoipAudioIODevice::loadOrCreateClientId(){
    FILE * client_file = fopen("client.txt", "w+");
    if (ftell(client_file) == 0){
        srand(time(NULL));
        client_id = (rand() * RAND_MAX) % 100000000;
        fprintf(client_file, "%d", client_id);
    } else {
        fscanf(client_file, "%1d", &client_id);
    }
    cout << client_id << endl;
    fclose(client_file);
}

void VoipAudioIODevice::setOutput(QIODevice* outputIODevice){
    outputBuffer = outputIODevice;
}

void VoipAudioIODevice::setDevice(QAudioOutput * outputDevice){
    this->outputDevice = outputDevice;
}

// Implementation required for this pure virtual function
qint64 VoipAudioIODevice::readData(char *data, qint64 max_size){
    Q_UNUSED(data);
    Q_UNUSED(max_size);
    return 0;
}

qint64 VoipAudioIODevice::writeData(const char *data, qint64 max_size){
    short *buf = (short *)data;

    int i;
    double sum = 0;
    for (i = 0; i < max_size/2; i++){
        sum += ((int)buf[i] * (int)buf[i]);
    }
    double mean = (double)sum / (double)max_size / 2.0;
    double vol = sqrt(mean);

    if (vol < 200){
        if (timer <= 0){
            return max_size;
        }
        timer--;
    } else {
        timer = 10;
    }

    unsigned char* spxOut;
    int spxOutSize;
    encoder->encode((unsigned char *)data, max_size, &spxOut, &spxOutSize);
    voice_conn->send_voice(client_id, spxOut, spxOutSize);
    return max_size;
}

OpusVoiceDecoder* VoipAudioIODevice::get_or_create_decoder(int sender_id){
    unsigned int i;
    OpusVoiceDecoder* decoder;

    for(i = 0; i < decoders.size(); i++){
        if (decoders[i]->get_id() == sender_id){
            return decoders[i];
        }
    }

    decoder = new OpusVoiceDecoder(sender_id);
    this->decoders.push_back(decoder);
    return decoder;
}

void VoipAudioIODevice::read_socket(int sender_id, unsigned char* spx_data, int spx_length){
    OpusVoiceDecoder* decoder = this->get_or_create_decoder(sender_id);
    decoder->decode_and_play(spx_data, spx_length);

    free(spx_data);

    return;
}
