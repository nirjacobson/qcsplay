#ifndef OUTPUTDEVICESETTINGSWIDGET_H
#define OUTPUTDEVICESETTINGSWIDGET_H

#include <QWidget>
#include <QSettings>

#include "application.h"
#include "chromasound/emu/audio_output.h"
#include "qcsplay.h"

namespace Ui {
class OutputDeviceSettingsWidget;
}

class OutputDeviceSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OutputDeviceSettingsWidget(QWidget *parent = nullptr);
    ~OutputDeviceSettingsWidget();

    int outputDeviceIndex() const;

signals:
    void changed();

private:
    Ui::OutputDeviceSettingsWidget *ui;
};

#endif // OUTPUTDEVICESETTINGSWIDGET_H
