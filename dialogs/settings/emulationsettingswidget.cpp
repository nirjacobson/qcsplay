#include "emulationsettingswidget.h"
#include "ui_emulationsettingswidget.h"

EmulationSettingsWidget::EmulationSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EmulationSettingsWidget)
{
    ui->setupUi(this);

#ifdef Q_OS_WIN
    QSettings settings(QCSPlay::SettingsFile, QSettings::IniFormat);
#else
    QSettings settings(QCSPlay::Organization, QCSPlay::Application);
#endif
    ui->bassSpinBox->setValue(settings.value(QCSPlay::EqualizerBassKey, 0).toInt());
    ui->trebleSpinBox->setValue(settings.value(QCSPlay::EqualizerTrebleKey, 0).toInt());
    ui->audioBufferSizeSpinBox->setValue(settings.value(QCSPlay::AudioBufferSizeKey, 256).toInt());
    ui->readBufferSizeSpinBox->setValue(settings.value(QCSPlay::ReadBufferSizeKey, 1).toInt());
}

EmulationSettingsWidget::~EmulationSettingsWidget()
{
    delete ui;
}

int EmulationSettingsWidget::bass() const
{
    return ui->bassSpinBox->value();
}

int EmulationSettingsWidget::treble() const
{
    return ui->trebleSpinBox->value();
}

int EmulationSettingsWidget::audioBufferSize() const
{
    return ui->audioBufferSizeSpinBox->value();
}

int EmulationSettingsWidget::readBufferSize() const
{
    return ui->readBufferSizeSpinBox->value();
}
