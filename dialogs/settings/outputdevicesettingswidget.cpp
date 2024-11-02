#include "outputdevicesettingswidget.h"
#include "ui_outputdevicesettingswidget.h"

OutputDeviceSettingsWidget::OutputDeviceSettingsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::OutputDeviceSettingsWidget)
{
    ui->setupUi(this);

#ifdef Q_OS_WIN
    QSettings settings(QCSPlay::SettingsFile, QSettings::IniFormat);
#else
    QSettings settings(QCSPlay::Organization, QCSPlay::Application);
#endif

    ui->outputDeviceComboBox->setCurrentIndex(settings.value(QCSPlay::OutputDeviceKey, QCSPlay::Emulator).toString() == QCSPlay::Emulator);

    connect(ui->outputDeviceComboBox, &QComboBox::currentIndexChanged, this, &OutputDeviceSettingsWidget::changed);
}

OutputDeviceSettingsWidget::~OutputDeviceSettingsWidget()
{
    delete ui;
}

int OutputDeviceSettingsWidget::outputDeviceIndex() const
{
    return ui->outputDeviceComboBox->currentIndex();
}
