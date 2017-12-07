#include "voipaudioiodevice.h"
#include "opusutil.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

using namespace std;

VoipAudioIODevice::VoipAudioIODevice(QObject *parent): QIODevice(parent){
    // create an encoder for voice
    encoder = new OpusVoiceEncoder;
    this->voice_conn = NULL;
    this->clientID = 0;

    return;
}

void VoipAudioIODevice::setClientId(int clientID){
    this->clientID = clientID;
}

void VoipAudioIODevice::setVoiceConn(VoipUDPSocket* voice_conn){
    // voice conenction for the IO device to write and read
    this->voice_conn = voice_conn;
    connect(this->voice_conn, SIGNAL(voice_ready(int, unsigned char *, int)), this, SLOT(read_socket(int, unsigned char*, int)));
}

// Implementation required for this pure virtual function
qint64 VoipAudioIODevice::readData(char *data, qint64 max_size){
    // this function should never be called
    Q_UNUSED(data);
    Q_UNUSED(max_size);
    return 0;
}

qint64 VoipAudioIODevice::writeData(const char *data, qint64 max_size){
    // do not send voice if client is not connected
    if (voice_conn == NULL || this->clientID == 0){
        return max_size;
    }

    // examine buffer as an array of shorts
    short *buf = (short *)data;

    // this section calculates the volume of the audio
    // https://en.wikipedia.org/wiki/Root_mean_square
    int i;
    double sum = 0;
    for (i = 0; i < max_size/2; i++){
        sum += ((int)buf[i] * (int)buf[i]);
    }
    double mean = (double)sum / (double)max_size / 2.0;

    // volume of voice data
    double vol = sqrt(mean);

    // if the volume is below the threshold
    if (vol < 200){
        // if we were not just transmitting voice data
        if (timer <= 0){
            // do not transmit
            return max_size;
        }
        // reduce timer if we transmitted anyway
        timer--;
    // if the volume is loud enough to transmit
    } else {
        // transmit the next 10 times this function is called
        timer = 10;
    }

    unsigned char* opusOut;
    int opusOutSize;
    // encode the audio
    encoder->encode((unsigned char *)data, max_size, &opusOut, &opusOutSize);
    // send the audio
    voice_conn->send_voice(clientID, opusOut, opusOutSize);
    return max_size;
}

OpusVoiceDecoder* VoipAudioIODevice::get_or_create_decoder(int sender_id){
    unsigned int i;
    OpusVoiceDecoder* decoder;

    // try to get a decoder for client
    for(i = 0; i < decoders.size(); i++){
        if (decoders[i]->get_id() == sender_id){
            return decoders[i];
        }
    }

    // create a new decoder if one was not found
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
