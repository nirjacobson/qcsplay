#include "application.h"
#include "tools/player/player.h"

Application::Application(int &argc, char **argv, int flags)
    : QApplication(argc, argv, flags)
    , _paused(false)
    , _ignoreCSTime(false)
    , _output(nullptr)
    , _chromasound(nullptr)
    , _player(nullptr)
{
    setupChromasound();
}

Application::~Application()
{
    if (_output) {
        _output->stop();
        _output->destroy();
    }

    delete _chromasound;
}


void Application::pause()
{
    _chromasound->pause();
    _paused = true;
}

bool Application::paused() const
{
    return _paused;
}

void Application::stop()
{
    _chromasound->stop();
    _paused = false;
}

bool Application::isPlaying() const
{
    return _chromasound->isPlaying();
}

Chromasound& Application::chromasound()
{
    return *_chromasound;
}

void Application::setupChromasound()
{
#ifdef Q_OS_WIN
    QSettings settings(QCSPlay::SettingsFile, QSettings::IniFormat);
#else
    QSettings settings(QCSPlay::Organization, QCSPlay::Application);
#endif

    int audioBufferSize = settings.value(QCSPlay::AudioBufferSizeKey, 256).toInt();
    int readBufferSize = settings.value(QCSPlay::ReadBufferSizeKey, 1).toInt();

    bool emu = settings.value(QCSPlay::OutputDeviceKey, QCSPlay::Emulator).toString() == QCSPlay::Emulator;

    if (_output) {
        _output->stop();
        _output->destroy();
    }

    if (_chromasound) {
        delete _chromasound;
    }

    if (emu) {
        _output = AudioOutput<int16_t>::instance();
        _output->init();

        int outputDeviceIndex = settings.value(QCSPlay::EmulatorOutputDeviceKey, AudioOutput<int16_t>::instance()->defaultDeviceIndex()).toInt();
        _output->open(outputDeviceIndex, audioBufferSize);
        _chromasound = new Chromasound_Emu();
        _output->producer(dynamic_cast<Chromasound_Emu*>(_chromasound));
        _output->start();
    } else {
#ifdef Q_OS_LINUX
        _chromasound = new Chromasound_Direct();
#endif
    }

    connect(_chromasound, &Chromasound::pcmUploadStarted, this, &Application::pcmUploadStarted);
    connect(_chromasound, &Chromasound::pcmUploadFinished, this, &Application::pcmUploadFinished);
}

void Application::setWindow(Player* player)
{
    _player = player;
}

void Application::showWindow()
{
    _player->show();
}

