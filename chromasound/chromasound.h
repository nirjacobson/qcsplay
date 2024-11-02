#ifndef CHROMASOUND_H
#define CHROMASOUND_H

#include <stdio.h>
#include <cstdlib>

#include <QObject>

#include "qcsplay.h"

class Chromasound : public QObject
{
        Q_OBJECT

    public:
        virtual ~Chromasound() {};

        virtual quint32 position() = 0;
        virtual void setPosition(const float pos) = 0;

        virtual void play(const QByteArray& vgm, const int currentOffsetSamples, const int currentOffsetData, const bool isSelection = false) = 0;
        virtual void play() = 0;
        virtual void pause() = 0;
        virtual void stop() = 0;

        virtual bool isPlaying() const = 0;
        virtual bool isPaused() const = 0;

    signals:
        void pcmUploadStarted();
        void pcmUploadFinished();
        void stopped();
};

#endif // CHROMASOUND_H
