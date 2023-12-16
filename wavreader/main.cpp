#include <QCoreApplication>
#include <QFile>

#include <iostream>

// Структура, описывающая заголовок WAV файла.
struct WAVHEADER
{
    // WAV-формат начинается с RIFF-заголовка:

    // Содержит символы "RIFF" в ASCII кодировке
    // (0x52494646 в big-endian представлении)
    char chunkId[4];

    // 36 + subchunk2Size, или более точно:
    // 4 + (8 + subchunk1Size) + (8 + subchunk2Size)
    // Это оставшийся размер цепочки, начиная с этой позиции.
    // Иначе говоря, это размер файла - 8, то есть,
    // исключены поля chunkId и chunkSize.
    uint32_t chunkSize;

    // Содержит символы "WAVE"
    // (0x57415645 в big-endian представлении)
    char format[4];

    // Формат "WAVE" состоит из двух подцепочек: "fmt " и "data":
    // Подцепочка "fmt " описывает формат звуковых данных:

    // Содержит символы "fmt "
    // (0x666d7420 в big-endian представлении)
    char subchunk1Id[4];

    // 16 для формата PCM.
    // Это оставшийся размер подцепочки, начиная с этой позиции.
    uint32_t subchunk1Size;

    // Аудио формат, полный список можно получить здесь http://audiocoding.ru/wav_formats.txt
    // Для PCM = 1 (то есть, Линейное квантование).
    // Значения, отличающиеся от 1, обозначают некоторый формат сжатия.
    uint16_t  audioFormat;

    // Количество каналов. Моно = 1, Стерео = 2 и т.д.
    uint16_t numChannels;

    // Частота дискретизации. 8000 Гц, 44100 Гц и т.д.
    uint32_t sampleRate;

    // sampleRate * numChannels * bitsPerSample/8
    uint32_t  byteRate;

    // numChannels * bitsPerSample/8
    // Количество байт для одного сэмпла, включая все каналы.
    uint16_t blockAlign;

    // Так называемая "глубиная" или точность звучания. 8 бит, 16 бит и т.д.
    uint16_t bitsPerSample;

    // Подцепочка "data" содержит аудио-данные и их размер.

    // Содержит символы "data"
    // (0x64617461 в big-endian представлении)
    char subchunk2Id[4];

    // numSamples * numChannels * bitsPerSample/8
    // Количество байт в области данных.
    uint32_t subchunk2Size;

    // Далее следуют непосредственно Wav данные.
};

struct S_FRAME_16S
{
    int16_t sample1;
    int16_t sample2;
};



bool read_print_wav(const QString filename)
{

    WAVHEADER  header;
    S_FRAME_16S frame;
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly))
    {
        //qDebug()<<file.errorString();
        return false;
    }


    if (file.read(reinterpret_cast<char *>(&header),sizeof(WAVHEADER))==-1)
        return false;


    if (header.bitsPerSample !=16 || header.numChannels!=2 ||
            header.sampleRate!= 44100)
        return false;   // TODO добавить поддержку других форматов.



    quint64 byte_read;

    while(
          (byte_read = file.read(reinterpret_cast<char *>(&frame),sizeof (S_FRAME_16S)))>1)


        std::cout << /*std::hex <<*/ frame.sample1 << " " <<  frame.sample2 << std::endl;



    file.close();

    return true;
}





int main(int argc, char *argv[])
{

    // QCoreApplication a(argc, argv);

    //char * progname = nullptr;
    char * filename = nullptr;

    if (argc>1)
        filename = argv[1];
    else
        return -1;


    if(!read_print_wav(filename))
        return -1;

    //return a.exec();

    return 0;
}
