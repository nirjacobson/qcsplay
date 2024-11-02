#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QList>
#include <QElapsedTimer>
#include <QUndoStack>

#include "chromasound/chromasound.h"
#include "chromasound/chromasound_standin.h"
#include "chromasound/chromasound_emu.h"
#include "chromasound/chromasound_dual.h"
#include "qcsplay.h"

#ifdef Q_OS_LINUX
#include "chromasound/chromasound_direct.h"
#endif

class Player;

class Application : public QApplication
{
        Q_OBJECT

    public:
        Application(int &argc, char **argv, int flags = ApplicationFlags);
        ~Application();

        void pause();
        bool paused() const;
        void stop();

        bool isPlaying() const;

        Chromasound& chromasound();

        void setupChromasound();

        void setWindow(Player* player);
        void showWindow();

    signals:
        void pcmUploadStarted();
        void pcmUploadFinished();
        void compileStarted();
        void compileFinished();

    private:
        Player* _player;
        AudioOutput<int16_t>* _output;
        Chromasound* _chromasound;

        bool _paused;
        bool _ignoreCSTime;
};

#endif // APPLICATION_H
