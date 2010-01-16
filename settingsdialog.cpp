#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QFileDialog>
#include <QSettings>
#include <QDebug>
#include <QTranslator>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
                       QSettings::NativeFormat);
    QVariant v = settings.value("My Pictures");
    if (!v.isNull())
    {
      ui->downloadLoc->setText(v.toString() + "\\flickrFotos");
    }
    else
    {
      ui->downloadLoc->setText(QDir::homePath() + "/flickrFotos");
    }
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::setDownloadLoc(const QString& downloadLoc)
{
  ui->downloadLoc->setText(downloadLoc);
}

void SettingsDialog::setQuality(const Quality& quality)
{
  if (quality == Best)
    ui->rbBest->setChecked(true);
  else
    ui->rbGood->setChecked(true);
}


void SettingsDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

QString SettingsDialog::getDownloadLoc() const
{
  return ui->downloadLoc->text();
}
Quality SettingsDialog::getQuality() const
{
  if (ui->rbBest->isChecked())
    return Best;
  return Good;
}

void SettingsDialog::dirDialog()
{
  QFileDialog dialog(this, tr("Download to"));
  dialog.setFileMode(QFileDialog::DirectoryOnly);
  dialog.setDirectory(getDownloadLoc());
  if (dialog.exec())
  {
    ui->downloadLoc->setText(dialog.directory().absolutePath());
  }
}


//QFileDialog dialog(this, tr("Download photos to"));
//dialog.setFileMode(QFileDialog::DirectoryOnly);
//dialog.setDirectory(QDir::home());
//if (dialog.exec())
//{
//    m_downloadLoc = dialog.directory();
//    m_downloadLoc = cdDir(m_downloadLoc, "flickrFotos");
//
//    m_settings->setValue("downloadLocation", m_downloadLoc.absolutePath());
//  }

QString SettingsDialog::getLanguageFromText(QString lang)
{
  if (lang == "Nederlands")
    return "nl";
  if (lang == "Deutch")
    return "de";
  return "en";
}

void SettingsDialog::setLanguage(QString lang)
{
  if (lang == "")
    return;
  qDebug() << "setting lang to " << getLanguageFromText(lang) << " lang " << lang;
  QTranslator translator;
  translator.load(QString("flickrphotos_") + getLanguageFromText(lang));
  QApplication::installTranslator(&translator);
}
