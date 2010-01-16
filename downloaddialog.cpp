#include "downloaddialog.h"
#include "ui_downloaddialog.h"
#include "collectioninfo.h"
#include <QTimer>
#include <QDebug>

DownloadDialog::DownloadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DownloadDialog)
{
    ui->setupUi(this);
    connect(this, SIGNAL(currentPhotoChanged(int)), this, SLOT(changeCurrentPhoto(int)));
}

DownloadDialog::~DownloadDialog()
{
    delete ui;
}

void DownloadDialog::changeEvent(QEvent *e)
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

void DownloadDialog::setPhotos(QList<PhotoData*> photos)
{
  m_photos = photos;
  ui->totalProgress->setMaximum(photos.size());
}


int DownloadDialog::exec()
{
  QTimer::singleShot(0, this, SLOT(startDownload()));
  return QDialog::exec();
}

void DownloadDialog::changeCurrentPhoto(int pos)
{
  PhotoData* photo = m_photos[pos];
      QImage* thumbImage = photo->GetThumbImage();
      ui->thumbImage->setMinimumSize(thumbImage->size());
      ui->thumbImage->setMaximumSize(thumbImage->size());
      ui->thumbImage->setPixmap(QPixmap::fromImage(*thumbImage,  Qt::AutoColor));
      ui->statusLabel->setText(QString(tr("Downloading image %1")).arg(photo->GetName()));
      ui->totalProgress->setValue(pos);
      //connect(photo, SIGNAL(downloadProgress(qint64,qint64)), ui->photoProgress, SLOT(setValue(int)));
      connect(photo, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(progress(qint64,qint64)));
}

void DownloadDialog::progress(qint64 one, qint64 two)
{
  ui->photoProgress->setMaximum(two);
  ui->photoProgress->setValue(one);
}

void DownloadDialog::startDownload()
{
  for (int i = 0; i < m_photos.size() && this->isVisible(); ++i)
  {
    currentPhotoChanged(i);
    m_photos[i]->downloadPhoto(m_quality);
  }
  close();
}
