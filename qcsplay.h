#ifndef QCSPLAY_H
#define QCSPLAY_H

#include <QString>

class QCSPlay {
    public:
        enum PCMStrategy {
            NONE,
            INLINE,
            SEQUENTIAL,
            RANDOM
        };

        static const QString Organization;
        static const QString Application;

        static const QString SettingsFile;

        static const QString EmulatorOutputDeviceKey;
        static const QString OutputDeviceKey;
        static const QString EqualizerBassKey;
        static const QString EqualizerTrebleKey;
        static const QString AudioBufferSizeKey;
        static const QString ReadBufferSizeKey;
        static const QString ChromasoundTypeKey;

        static const QString Emulator;
        static const QString ChromasoundDirect;
};

#endif // QCSPLAY_H
