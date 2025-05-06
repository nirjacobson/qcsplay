#ifndef CHROMASOUND_EMU_H
#define CHROMASOUND_EMU_H

#include <QTimer>
#include <QSettings>
#include <QMutex>
#include <QThread>

#include "chromasound.h"
#include "emu/gme.h"
#include "emu/Music_Emu.h"
#include "emu/producer.h"
#include "emu/audio_output.h"
#include "formats/vgm.h"

class Chromasound_Emu : public Chromasound, public Producer<int16_t>
{
    public:
        Chromasound_Emu();
        ~Chromasound_Emu();

    private:
        class Player;

        Music_Emu* _emu;
        gme_type_t _type;

        Music_Emu::sample_t* _buffers[2];
        int _buffer;
        int _bufferIdx;
        int _framesPerReadBuffer;

        quint32 _position;
        quint32 _positionOffset;
        bool _stopped;
        bool _paused;

        track_info_t _info;

        QMutex _loadLock;

        Player* _player;

        bool _haveInfo;

        bool _isSelection;

        void setEqualizer();
        void setBufferSizes();

        // Chromasound interface
    public:
        quint32 position();
        void setPosition(const float pos);
        void play(const QByteArray& vgm, const int currentOffsetSamples, const int currentOffsetData, const bool isSelection = false);
        void play();
        void pause();
        void stop();
        bool isPlaying() const;
        bool isPaused() const;

        // Producer interface
    public:
        int16_t* next(int size);
};

class Chromasound_Emu::Player : public QThread {
    public:
        enum Action {
            Idle,
            Load,
            Exit
        };

        Player(Chromasound_Emu& emu);

        void action(const Action action);
        Action action();

        void buffer(const int buffer);

    private:
        Chromasound_Emu& _emu;

        Action _action;

        int _buffer;

        QMutex _actionLock;


        // QThread interface
    protected:
        void run();
};

#endif // CHROMASOUND_EMU_H
