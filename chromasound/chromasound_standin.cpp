#include "chromasound_standin.h"

quint32 Chromasound_Standin::position()
{
    float sampleNS = 1e9f / 44100;

    quint32 pos = (_ref + _timer.nsecsElapsed()) / sampleNS;

    if (!_playing) {
        pos = _ref / sampleNS;
    }

    return pos;
}

void Chromasound_Standin::setPosition(const float pos)
{
    _ref = pos;
}

bool Chromasound_Standin::isPlaying() const
{
    return _playing;
}

bool Chromasound_Standin::isPaused() const
{
    return _paused;
}

Chromasound_Standin::Chromasound_Standin()
    : _ref(0)
    , _playing(false)
    , _loopOffsetSamples(-1)
    , _paused(false)
{

}

void Chromasound_Standin::play(const QByteArray& vgm, const int, const int, const bool)
{
    QTimer::singleShot(3000, [&]() {
        emit pcmUploadFinished();
    });
    emit pcmUploadStarted();

    quint32 length = *(quint32*)&vgm.constData()[0x18];
    quint32 loopLength = *(quint32*)&vgm.constData()[0x20];
    quint32 introLength = length - loopLength;

    _loopOffsetSamples = introLength;
    _playing = true;
    _paused = false;
    _timer.restart();
}

void Chromasound_Standin::play()
{
    QTimer::singleShot(3000, [&]() {
        emit pcmUploadFinished();
    });
    emit pcmUploadStarted();

    _playing = true;
    _paused = false;
    _timer.restart();
}

void Chromasound_Standin::pause()
{
    _ref += _timer.nsecsElapsed();
    _playing = false;
    _paused = true;
}

void Chromasound_Standin::stop()
{
    _playing = false;
    _paused = false;
    _ref = 0;
}
