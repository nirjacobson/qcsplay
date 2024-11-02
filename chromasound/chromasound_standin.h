#ifndef CHROMASOUND_STANDIN_H
#define CHROMASOUND_STANDIN_H

#include <QTimer>
#include <QElapsedTimer>

#include "chromasound.h"

class Chromasound_Standin : public Chromasound
{
    public:
        Chromasound_Standin();

        void play(const QByteArray&vgm, const int, const int, const bool = false);
        void play();
        void pause();
        void stop();
        quint32 position();
        void setPosition(const float pos);
        bool isPlaying() const;
        bool isPaused() const;

    private:
        QElapsedTimer _timer;
        qint64 _ref;
        bool _playing;
        int _loopOffsetSamples;
        bool _paused;
};

#endif // CHROMASOUND_STANDIN_H
