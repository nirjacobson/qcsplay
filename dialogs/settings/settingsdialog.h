#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>

#include "qcsplay.h"

namespace Ui {
    class SettingsDialog;
}

class SettingsDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit SettingsDialog(QWidget *parent = nullptr);
        ~SettingsDialog();

    signals:
        void done();

    private:
        Ui::SettingsDialog *ui;

    private slots:
        void accepted();
        void chromasoundChanged();
};

#endif // SETTINGSDIALOG_H
