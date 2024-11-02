#include "chromasound_direct.h"
#include "direct/gpio.h"

Chromasound_Direct::Chromasound_Direct()
    : _timeOffset(0)
{
#ifdef Q_OS_WIN
    QSettings settings(QCSPlay::SettingsFile, QSettings::IniFormat);
#else
    QSettings settings(QCSPlay::Organization, QCSPlay::Application);
#endif
    _gpioFd = gpio_init();

    _vgmPlayer = new VGMPlayer(this);
    reset();

    connect(_vgmPlayer, &VGMPlayer::pcmUploadStarted, this, &Chromasound::pcmUploadStarted);
    connect(_vgmPlayer, &VGMPlayer::pcmUploadFinished, this, &Chromasound::pcmUploadFinished);

    _vgmPlayer->start();
}

Chromasound_Direct::~Chromasound_Direct()
{
    _vgmPlayer->stop();
    _vgmPlayer->quit();
    _vgmPlayer->wait();

    delete _vgmPlayer;

    gpio_close(_gpioFd);
}

quint32 Chromasound_Direct::position()
{
    uint32_t time = _vgmPlayer->time();
    uint32_t introLength = _vgmPlayer->introLength();
    uint32_t loopLength = _vgmPlayer->loopLength();
    uint32_t length = _vgmPlayer->length();

    if (_vgmPlayer->loopLength() <= 0) {
        if (_vgmPlayer->isPlaying() && (time + _timeOffset) >= _vgmPlayer->length()) {
            stop();
            return 0;
        }
        return (time + _timeOffset);
    }

    if (_vgmPlayer->introLength() <= 0) {
        return (_timeOffset + (time % length));
    } else {
        uint32_t t = _timeOffset + time;
        return ((t < introLength)
                ? t
                : (((t - introLength) % loopLength) + introLength));
    }
}

void Chromasound_Direct::setPosition(const float pos)
{

}

void Chromasound_Direct::play(const QByteArray& vgm, const int currentOffsetSamples, const int currentOffsetData, const bool isSelection)
{
    if (isSelection) {
        _timeOffset = currentOffsetSamples;
    }

    _vgmPlayer->stop();
    _vgmPlayer->quit();
    _vgmPlayer->wait();

    reset();

    _vgmPlayer->setVGM(vgm, currentOffsetData);
    _vgmPlayer->start();
}

void Chromasound_Direct::play()
{
    _vgmPlayer->start();
}

void Chromasound_Direct::pause()
{
    _vgmPlayer->pause();
}

void Chromasound_Direct::stop()
{
    _vgmPlayer->stop();
    _vgmPlayer->quit();
    _vgmPlayer->wait();

    reset();

    _timeOffset = 0;
}

bool Chromasound_Direct::isPlaying() const
{
    return _vgmPlayer->isPlaying();
}

bool Chromasound_Direct::isPaused() const
{
    return _vgmPlayer->isPaused();
}

void Chromasound_Direct::reset()
{
    gpio_write(_gpioFd, 2, 0);
    QThread::usleep(10000);

    gpio_write(_gpioFd, 2, 1);
    QThread::usleep(10000);
}
