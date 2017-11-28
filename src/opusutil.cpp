/*
 * Reference: https://chromium.googlesource.com/chromium/deps/opus/+/1.1.1/doc/trivial_example.c
 */
#include "opusutil.h"
#include <string.h>
#include <opus.h>
#include <stdlib.h>
#include <iostream>

#define FRAME_SIZE 960
#define SAMPLE_RATE 48000
#define CHANNELS 2
#define BITRATE 64000
#define APPLICATION OPUS_APPLICATION_AUDIO

#define MAX_FRAME_SIZE 6*960
#define MAX_PACKET_SIZE (3*1276)

using namespace std;

OpusUtil::OpusUtil(){

}
\
OpusVoiceEncoder::OpusVoiceEncoder(){
    encoder = (OpusEncoder *)malloc(opus_encoder_get_size(CHANNELS));
    opus_encoder_init(encoder, SAMPLE_RATE, CHANNELS, APPLICATION);
    opus_encoder_ctl(encoder, OPUS_SET_BITRATE(BITRATE));
}

OpusVoiceEncoder::encode(unsigned char* wavData, int wavLength, unsigned char** opusAudio, int* opusLength){
    unsigned char inputBuffer[MAX_FRAME_SIZE*CHANNELS*2];
    opus_int16 in[FRAME_SIZE*CHANNELS];
    unsigned char opusBuffer[wavLength];
    unsigned char cbits[MAX_PACKET_SIZE];
    int nbBytes;

    int i;

    // size in bytes of the raw wav audio data
    int rawAudioSize = wavLength;
    // amount of bytes read from the wav buffer so far
    int wavRead = 0;
    int bytesWritten = 0;

    memcpy(opusBuffer + bytesWritten, &rawAudioSize, sizeof(int));
    bytesWritten += sizeof(int);

    while (wavRead < rawAudioSize){
        memcpy(inputBuffer, wavData, CHANNELS*FRAME_SIZE*2);
        wavRead += CHANNELS*FRAME_SIZE*2;
        wavData += CHANNELS*FRAME_SIZE*2;

        for (i = 0; i < CHANNELS*FRAME_SIZE; i++){
            in[i]=inputBuffer[2*i+1]<<8|inputBuffer[2*i];
        }

        nbBytes = opus_encode(encoder, in, FRAME_SIZE, cbits, MAX_PACKET_SIZE);

        // copy the encoded frame length into the buffer
        memcpy(opusBuffer + bytesWritten, &nbBytes, sizeof(int));
        bytesWritten += sizeof(int);

        // copy the actual encoded audio into the buffer
        memcpy(opusBuffer + bytesWritten, cbits, nbBytes);
        bytesWritten += nbBytes;
    }
    // output
    (*opusAudio) = (unsigned char *)malloc(bytesWritten);
    memcpy((*opusAudio), opusBuffer, bytesWritten);
    *opusLength = bytesWritten;

    return 0;
}

OpusVoiceDecoder::OpusVoiceDecoder(int id){
    this->id = id;

    decoder = (OpusDecoder *)malloc(opus_decoder_get_size(CHANNELS));
    opus_decoder_init(decoder, SAMPLE_RATE, CHANNELS);

    QAudioFormat format;
    format.setSampleRate(48000);
    format.setChannelCount(2);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    outputDevice = new QAudioOutput(format);
    outputBuffer = outputDevice->start();
}

OpusVoiceDecoder::get_id(){
    return this->id;
}

OpusVoiceDecoder::decode_and_play(unsigned char* opusData, int opusLength){
    opus_int16 out[MAX_FRAME_SIZE*CHANNELS];
    unsigned char cbits[MAX_PACKET_SIZE];
    int nbBytes;
    int frame_size;
    int i;

    // my variables
    int bytesRead = 0;
    int bytesWritten = 0;
    int wavAudioSize;
    unsigned char output[MAX_FRAME_SIZE*CHANNELS*2];

    // get the output size
    memcpy(&wavAudioSize, opusData + bytesRead, sizeof(int));
    bytesRead += sizeof(int);

    unsigned char wavAudio[wavAudioSize];

    while (bytesRead < opusLength){
        // read the speex frame size first
        memcpy(&nbBytes, opusData + bytesRead, sizeof(int));
        bytesRead += sizeof(int);
        // read the speex data
        memcpy(cbits, opusData + bytesRead, nbBytes);
        bytesRead += nbBytes;

        frame_size = opus_decode(decoder, cbits, nbBytes, out, MAX_FRAME_SIZE, 0);

        for(i = 0; i < frame_size*CHANNELS; i++){
            output[2*i]=out[i]&0xFF;
            output[2*i+1]=(out[i]>>8)&0xFF;
        }

        memcpy(wavAudio + bytesWritten, output, sizeof(short) * frame_size * CHANNELS);
        bytesWritten += sizeof(short) * frame_size * CHANNELS;
    }

    outputBuffer->write((char *)wavAudio, bytesWritten);

    return 0;
}












