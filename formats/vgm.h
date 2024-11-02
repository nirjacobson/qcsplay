#ifndef VGMSTREAM_H
#define VGMSTREAM_H

#include <QByteArray>

class VGM
{
    public:
        static QByteArray generateHeader(const QByteArray& data, const int totalSamples, const int loopOffsetData, const int gd3size, const bool selectionLoop);
};

#endif // VGMSTREAM_H
