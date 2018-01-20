#include "voipaudioiodevice.h"
#include "opusutil.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

using namespace std;

VoipAudioIODevice::VoipAudioIODevice(QObject *parent): QIODevice(parent){
    encoder = new OpusVoiceEncoder;
    this->voice_conn = NULL;
    this->clientID = 0;

    return;
}

void VoipAudioIODevice::setClientId(int clientID){
    this->clientID = clientID;
}

void VoipAudioIODevice::setVoiceConn(VoipUDPSocket* voice_conn){
    this->voice_conn = voice_conn;
    connect(this->voice_conn, SIGNAL(voice_ready(int, unsigned char *, int)), this, SLOT(read_socket(int, unsigned char*, int)));
}

// Implementation required for this pure virtual function
qint64 VoipAudioIODevice::readData(char *data, qint64 max_size){
    Q_UNUSED(data);
    Q_UNUSED(max_size);
    return 0;
}

qint64 VoipAudioIODevice::writeData(const char *data, qint64 max_size){
    if (voice_conn == NULL || this->clientID == 0){
        return max_size;
    }

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
    voice_conn->send_voice(clientID, spxOut, spxOutSize);
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
