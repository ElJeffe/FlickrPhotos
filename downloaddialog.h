#ifndef DOWNLOADDIALOG_H
#define DOWNLOADDIALOG_H

#include <QDialog>
#include <QList>
#include "quality.h"

class PhotoData;

namespace Ui {
  class DownloadDialog;
}

class DownloadDialog : public QDialog {
  Q_OBJECT
public:
  DownloadDialog(QWidget *parent = 0);
  ~DownloadDialog();
  void setPhotos(QList<PhotoData*> photos);
  void setQuality(Quality quality) { m_quality = quality; }
public slots:
  virtual int exec();
signals:
  void currentPhotoChanged(int pos);

protected:
  void changeEvent(QEvent *e);
private slots:
  void startDownload();
  void changeCurrentPhoto(int pos);
  void progress(qint64 one, qint64 two);

private:
  Ui::DownloadDialog *ui;
  QList<PhotoData*> m_photos;
  Quality m_quality;
};

#endif // DOWNLOADDIALOG_H
