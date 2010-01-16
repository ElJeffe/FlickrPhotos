#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "quality.h"

namespace Ui {
    class SettingsDialog;
}


class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();
    QString getDownloadLoc() const;
    Quality getQuality() const;
    void setDownloadLoc(const QString& downloadLoc);
    void setQuality(const Quality& quality);


protected:
    void changeEvent(QEvent *e);
    QString getLanguageFromText(QString lang);
  private slots:
    void dirDialog();
    void setLanguage(QString lang);

private:
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
