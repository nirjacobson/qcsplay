#ifndef PROFILESETTINGSWIDGET_H
#define PROFILESETTINGSWIDGET_H

#include <QWidget>
#include <QSettings>

#include "qau.h"

namespace Ui {
class ProfileSettingsWidget;
}

class ProfileSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProfileSettingsWidget(QWidget *parent = nullptr);
    ~ProfileSettingsWidget();

    QString device() const;
    qau::Profile profile() const;

private:
    Ui::ProfileSettingsWidget *ui;

private slots:
    void deviceChanged(const int index);
    void pcmStrategyChanged(const QString& value);
};

#endif // PROFILESETTINGSWIDGET_H
