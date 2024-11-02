#include "profilesettingswidget.h"
#include "ui_profilesettingswidget.h"

ProfileSettingsWidget::ProfileSettingsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ProfileSettingsWidget)
{
    ui->setupUi(this);

#ifdef Q_OS_WIN
    QSettings settings(qau::SettingsFile, QSettings::IniFormat);
#else
    QSettings settings(qau::Organization, qau::Application);
#endif
    QList<QString> devices = {
        qau::ChromasoundNova,
        qau::ChromasoundNovaDirect,
        qau::ChromasoundPro,
        qau::ChromasoundProDirect,
        qau::Genesis,
        qau::MSX2
    };

    QString device = settings.value(qau::DeviceKey, qau::ChromasoundNova).toString();
    bool isChromasound = settings.value(qau::IsChromasoundKey, true).toBool();
    bool discretePCM = settings.value(qau::DiscretePCMKey, false).toBool();
    bool usePCMSRAM = settings.value(qau::UsePCMSRAMKey, false).toBool();
    qau::PCMStrategy pcmStrategy = qau::pcmStrategyFromString(settings.value(qau::PCMStrategyKey, qau::Random).toString().toLower());
    qau::Profile profile(pcmStrategy, isChromasound, discretePCM, usePCMSRAM);

    deviceChanged(devices.indexOf(device));

    connect(ui->deviceComboBox, &QComboBox::currentIndexChanged, this, &ProfileSettingsWidget::deviceChanged);
    connect(ui->pcmStrategyComboBox, &QComboBox::currentTextChanged, this, &ProfileSettingsWidget::pcmStrategyChanged);

    QStringList strategies = {
        "None",
        "Inline",
        "Sequential",
        "Random"
    };

    ui->deviceComboBox->setCurrentIndex(devices.indexOf(device));
    ui->chromasoundCheckBox->setChecked(profile.isChromasound());
    ui->pcmStrategyComboBox->setCurrentText(strategies[profile.pcmStrategy()]);
    ui->discretePCMCheckBox->setChecked(profile.discretePCM());
    ui->pcmSRAMCheckBox->setChecked(profile.usePCMSRAM());
}

ProfileSettingsWidget::~ProfileSettingsWidget()
{
    delete ui;
}

QString ProfileSettingsWidget::device() const
{
    switch (ui->deviceComboBox->currentIndex()) {
    case 0:
        return qau::ChromasoundNova;
    case 1:
        return qau::ChromasoundNovaDirect;
    case 2:
        return qau::ChromasoundPro;
    case 3:
        return qau::ChromasoundProDirect;
    case 4:
        return qau::Genesis;
    case 5:
        return qau::MSX2;
    }

    return qau::ChromasoundNova;
}

qau::Profile ProfileSettingsWidget::profile() const
{
    return qau::Profile(
        qau::pcmStrategyFromString(ui->pcmStrategyComboBox->currentText().toLower()),
        ui->chromasoundCheckBox->isChecked(),
        ui->discretePCMCheckBox->isChecked(),
        ui->pcmSRAMCheckBox->isChecked());
}

void ProfileSettingsWidget::deviceChanged(const int index)
{
#ifdef Q_OS_WIN
    QSettings settings(qau::SettingsFile, QSettings::IniFormat);
#else
    QSettings settings(qau::Organization, qau::Application);
#endif

    QString device = settings.value(qau::DeviceKey, qau::ChromasoundNova).toString();
    bool isChromasound = settings.value(qau::IsChromasoundKey, true).toBool();
    bool discretePCM = settings.value(qau::DiscretePCMKey, false).toBool();
    bool usePCMSRAM = settings.value(qau::UsePCMSRAMKey, false).toBool();
    qau::PCMStrategy pcmStrategy = qau::pcmStrategyFromString(settings.value(qau::PCMStrategyKey, qau::Random).toString());
    qau::Profile currentProfile(pcmStrategy, isChromasound, discretePCM, usePCMSRAM);

    QStringList strategies = {
        "None",
        "Inline",
        "Sequential",
        "Random"
    };

    const qau::Profile* profile;
    switch (index) {
    case 0:
        profile = &qau::ChromasoundNovaPreset;
        strategies.removeOne("None");
        strategies.removeOne("Random");
        ui->chromasoundCheckBox->setVisible(true);
        ui->discretePCMCheckBox->setVisible(false);
        ui->pcmSRAMCheckBox->setVisible(false);
        break;
    case 1:
        profile = &qau::ChromasoundNovaDirectPreset;
        strategies.removeOne("None");
        strategies.removeOne("Random");
        strategies.removeOne("Sequential");
        ui->chromasoundCheckBox->setVisible(true);
        ui->discretePCMCheckBox->setVisible(false);
        ui->pcmSRAMCheckBox->setVisible(false);
        break;
    case 2:
        profile = &qau::ChromasoundProPreset;
        strategies.removeOne("None");
        ui->chromasoundCheckBox->setVisible(true);
        ui->discretePCMCheckBox->setVisible(true);
        ui->pcmSRAMCheckBox->setVisible(true);
        break;
    case 3:
        profile = &qau::ChromasoundProDirectPreset;
        strategies.removeOne("None");
        ui->chromasoundCheckBox->setVisible(true);
        ui->discretePCMCheckBox->setVisible(true);
        ui->pcmSRAMCheckBox->setVisible(false);
        break;
    case 4:
        profile = &qau::GenesisPreset;
        strategies = { "Sequential" };
        ui->chromasoundCheckBox->setVisible(false);
        ui->discretePCMCheckBox->setVisible(false);
        ui->pcmSRAMCheckBox->setVisible(false);
        break;
    case 5:
        profile = &qau::MSX2Preset;
        strategies = { "None" };
        ui->chromasoundCheckBox->setVisible(false);
        ui->discretePCMCheckBox->setVisible(false);
        ui->pcmSRAMCheckBox->setVisible(false);
        break;
    default:
        profile = &currentProfile;
        break;
    }

    ui->pcmStrategyComboBox->clear();
    ui->pcmStrategyComboBox->addItems(strategies);

    QStringList strategiesLower = strategies;

    for (QString& str : strategiesLower) {
        str = str.toLower();
    }

    ui->pcmStrategyComboBox->setCurrentIndex(strategiesLower.indexOf(qau::pcmStrategyToString(profile->pcmStrategy())));

    ui->chromasoundCheckBox->blockSignals(true);
    ui->chromasoundCheckBox->setChecked(profile->isChromasound());
    ui->chromasoundCheckBox->blockSignals(false);

    ui->discretePCMCheckBox->blockSignals(true);
    ui->discretePCMCheckBox->setChecked(profile->discretePCM());
    ui->discretePCMCheckBox->blockSignals(false);

    ui->pcmSRAMCheckBox->blockSignals(true);
    ui->pcmSRAMCheckBox->setChecked(profile->usePCMSRAM());
    ui->pcmSRAMCheckBox->blockSignals(false);
}

void ProfileSettingsWidget::pcmStrategyChanged(const QString& value)
{
    if (ui->deviceComboBox->currentIndex() == 2 || ui->deviceComboBox->currentIndex() == 3) {
        if (value == "Inline") {
            ui->pcmSRAMCheckBox->setChecked(false);
        } else if (value == "Random") {
            ui->pcmSRAMCheckBox->setChecked(true);
        }
        ui->pcmSRAMCheckBox->setVisible(value == "Sequential" && ui->deviceComboBox->currentIndex() != 3);
    }
}
