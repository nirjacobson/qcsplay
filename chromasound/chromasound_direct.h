#ifndef CHROMASOUND_DIRECT_H
#define CHROMASOUND_DIRECT_H

#include <QMap>

#include <QTimer>
#include <QMutex>

#include "chromasound.h"
#include "direct/vgmplayer.h"

class Chromasound_Direct : public Chromasound
{
    public:
        Chromasound_Direct();
        ~Chromasound_Direct();

        quint32 position();
        void setPosition(const float pos);
        void play(const QByteArray& vgm, const int currentOffsetSamples, const int currentOffsetData, const bool isSelection = false);
        void play();
        void pause();
        void stop();
        bool isPlaying() const;
        bool isPaused() const;

    private:
        int _gpioFd;

        QMutex _mutex;

        VGMPlayer* _vgmPlayer;

        long _timeOffset;

        void reset();
};

#endif // CHROMASOUND_DIRECT_H
