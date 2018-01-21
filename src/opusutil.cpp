/*
 * Reference: https://chromium.googlesource.com/chromium/deps/opus/+/1.1.1/doc/trivial_example.c
 */
#include "opusutil.h"
#include <string.h>
#include <opus/opus.h>
#include <stdlib.h>
#include <iostream>

// Opus related values
// These are constants that cannot be changed for the application
#define FRAME_SIZE 960
#define SAMPLE_RATE 48000
#define CHANNELS 2
#define BITRATE 64000
#define APPLICATION OPUS_APPLICATION_AUDIO

#define MAX_FRAME_SIZE 6*960
#define MAX_PACKET_SIZE (3*1276)

OpusVoiceEncoder::OpusVoiceEncoder(){
    // create an encoder for use
    encoder = (OpusEncoder *)malloc(opus_encoder_get_size(CHANNELS));
    opus_encoder_init(encoder, SAMPLE_RATE, CHANNELS, APPLICATION);
    // specific bitrate for voice
    opus_encoder_ctl(encoder, OPUS_SET_BITRATE(BITRATE));
}

OpusVoiceEncoder::encode(unsigned char* wavData, int wavLength, unsigned char** opusAudio, int* opusLength){
    // temporary buffer to use for encoding
    unsigned char inputBuffer[MAX_FRAME_SIZE*CHANNELS*2];
    // used to store the PCM samples as shorts instead of bytes
    opus_int16 in[FRAME_SIZE*CHANNELS];
    // output from the encoding
    unsigned char opusBuffer[wavLength];
    // output for encoding a frame
    unsigned char cbits[MAX_PACKET_SIZE];
    // size of a frame after encoding
    int nbBytes;

    int i;

    // size in bytes of the raw wav audio data
    int rawAudioSize = wavLength;
    // amount of bytes read from the wav buffer so far
    int wavRead = 0;
    int bytesWritten = 0;

    // copy the raw audio size so the decoder knows the final length of the PCM data
    memcpy(opusBuffer + bytesWritten, &rawAudioSize, sizeof(int));
    bytesWritten += sizeof(int);

    // read all PCM samples
    while (wavRead < rawAudioSize){
        // copy into temporary buffer
        memcpy(inputBuffer, wavData, CHANNELS*FRAME_SIZE*2);
        wavRead += CHANNELS*FRAME_SIZE*2;
        wavData += CHANNELS*FRAME_SIZE*2;

        // copy all raw samples into the short buffer
        for (i = 0; i < CHANNELS*FRAME_SIZE; i++){
            in[i]=inputBuffer[2*i+1]<<8|inputBuffer[2*i];
        }

        // encode a frame
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

// Decoder for voice data
OpusVoiceDecoder::OpusVoiceDecoder(int id){
    // there are as many decoders as there are voice clients,
    // so we must keep track of which client this decoder is for
    this->id = id;

    // create decoder
    decoder = (OpusDecoder *)malloc(opus_decoder_get_size(CHANNELS));
    opus_decoder_init(decoder, SAMPLE_RATE, CHANNELS);

    // create output device
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

// decodes audio and then immediately plays it to the speaker
OpusVoiceDecoder::decode_and_play(unsigned char* opusData, int opusLength){
    // initial output of decoding process
    opus_int16 out[MAX_FRAME_SIZE*CHANNELS];
    // intermediate PCM output
    unsigned char output[MAX_FRAME_SIZE*CHANNELS*2];
    // input into the decoder
    unsigned char cbits[MAX_PACKET_SIZE];
    // number of bytes decoded
    int nbBytes;
    // size of the decoded frame
    int frame_size;
    int i;

    // variables to track the process
    // how many opus bytes have been read
    int bytesRead = 0;
    // how many PCM bytes have been written
    int bytesWritten = 0;
    int wavAudioSize;


    // get the output size
    memcpy(&wavAudioSize, opusData + bytesRead, sizeof(int));
    bytesRead += sizeof(int);

    unsigned char wavAudio[wavAudioSize];

    while (bytesRead < opusLength){
        // read the opus frame size first
        memcpy(&nbBytes, opusData + bytesRead, sizeof(int));
        bytesRead += sizeof(int);
        // read the opus data
        memcpy(cbits, opusData + bytesRead, nbBytes);
        bytesRead += nbBytes;

        frame_size = opus_decode(decoder, cbits, nbBytes, out, MAX_FRAME_SIZE, 0);

        // conver shorts to bytes
        for(i = 0; i < frame_size*CHANNELS; i++){
            output[2*i]=out[i]&0xFF;
            output[2*i+1]=(out[i]>>8)&0xFF;
        }

        // copy into output buffer
        memcpy(wavAudio + bytesWritten, output, sizeof(short) * frame_size * CHANNELS);
        bytesWritten += sizeof(short) * frame_size * CHANNELS;
    }

    outputBuffer->write((char *)wavAudio, bytesWritten);

    return 0;
}












