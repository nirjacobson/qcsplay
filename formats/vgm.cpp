#include "vgm.h"

QByteArray VGM::generateHeader(const QByteArray& data, const int totalSamples, const int loopOffsetData, const int gd3size, const bool selectionLoop)
{

    QByteArray headerData(128, 0);

    // VGM header
    headerData[0] = 'V';
    headerData[1] = 'g';
    headerData[2] = 'm';
    headerData[3] = ' ';

    // EOF
    *(uint32_t*)&headerData[0x4] = data.size() + gd3size + 128 - 0x4;
    // Version
    headerData[0x8] = 0x50;
    headerData[0x9] = 0x01;
    // SN76489 clock
    *(uint32_t*)&headerData[0xC] = 3579545;
    // YM2413 clock
    *(uint32_t*)&headerData[0x10] = 3579545;
    // GD3 offset
    *(uint32_t*)&headerData[0x14] = data.size() + 128 - 0x14;
    // Total samples
    *(uint32_t*)&headerData[0x18] = totalSamples;
    // Loop offset
    *(uint32_t*)&headerData[0x1C] = (loopOffsetData == 0) ? 0 : (loopOffsetData - 0x1C);
    // Loop # samples
    *(uint32_t*)&headerData[0x20] = 0;
    // SN76489AN flags
    *(uint16_t*)&headerData[0x28] = 0x0003;
    headerData[0x2A] = 15;
    // YM2612 clock
    *(uint32_t*)&headerData[0x2C] = 7680000;
    // Data offset
    *(uint32_t*)&headerData[0x34] = headerData.size() - 0x34;
    // AY8910 clock
    *(uint32_t*)&headerData[0x74] = 1789773;

    return headerData;
}
