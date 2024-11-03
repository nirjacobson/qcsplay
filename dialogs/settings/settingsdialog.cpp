#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

#ifndef Q_OS_LINUX
    ui->outputDeviceGroupBox->setVisible(false);
#endif

    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::close);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::accepted);
    connect(ui->outputDeviceWidget, &OutputDeviceSettingsWidget::changed, this, &SettingsDialog::chromasoundChanged);

    chromasoundChanged();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::accepted()
{
#ifdef Q_OS_WIN
    QSettings settings(QCSPlay::SettingsFile, QSettings::IniFormat);
#else
    QSettings settings(QCSPlay::Organization, QCSPlay::Application);
#endif

    settings.setValue(QCSPlay::OutputDeviceKey, ui->outputDeviceWidget->outputDeviceIndex() == 0 ? QCSPlay::Emulator : QCSPlay::ChromasoundDirect);
    settings.setValue(QCSPlay::EqualizerBassKey, ui->emulationSettingsWidget->bass());
    settings.setValue(QCSPlay::EqualizerTrebleKey, ui->emulationSettingsWidget->treble());
    settings.setValue(QCSPlay::AudioBufferSizeKey, ui->emulationSettingsWidget->audioBufferSize());
    settings.setValue(QCSPlay::ReadBufferSizeKey, ui->emulationSettingsWidget->readBufferSize());
    settings.setValue(QCSPlay::EmulatorOutputDeviceKey, ui->emulatorOutputDeviceWidget->outputDeviceIndex());

    emit done();

    close();
}

void SettingsDialog::chromasoundChanged()
{
    bool showEmuSettings = ui->outputDeviceWidget->outputDeviceIndex() == 0;
    ui->emuOutputDeviceGroupBox->setVisible(showEmuSettings);
    ui->emuAdjustmentsGroupBox->setVisible(showEmuSettings);

    if (showEmuSettings) {
        ui->emulatorOutputDeviceWidget->doUpdate();
    }
}
