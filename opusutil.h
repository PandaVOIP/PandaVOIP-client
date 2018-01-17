#ifndef OPUSUTIL_H
#define OPUSUTIL_H

#include <opus.h>
#include <QAudioOutput>
#include <QIODevice>

class OpusUtil
{
public:
    OpusUtil();
    static int encode(unsigned char *, int, unsigned char **, int *);
    static int decode(unsigned char *, int, unsigned char **, int *);
};

class OpusVoiceEncoder {
public:
    OpusVoiceEncoder();
    int encode(unsigned char *, int, unsigned char **, int *);
private:
    OpusEncoder* encoder;
};

class OpusVoiceDecoder {
public:
    OpusVoiceDecoder(int);
    int get_id();
    int decode_and_play(unsigned char *, int);
private:
    int id;
    OpusDecoder* decoder;
    QAudioOutput* outputDevice;
    QIODevice* outputBuffer;
};

#endif // OPUSUTIL_H
