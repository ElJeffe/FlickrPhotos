#include "collectioninfo.h"
#include <QNetworkReply>
#include <QImage>
#include <QDebug>
#include <QEventLoop>

void CollectionData::thumbReceived(QNetworkReply* reply)
{
  m_collectionImage = new QImage();
  m_collectionImage->loadFromData(reply->readAll());
  CheckReady();
}

//void PhotoData::thumbReceived(QNetworkReply* reply)
//{
//  m_image = new QImage();
//  m_image->loadFromData(reply->readAll());
//  photoReady();
//}

void SetData::AddPhoto(PhotoData* photo)
{
  connect(photo, SIGNAL(photoReady()), this, SLOT(photoReady()));
  m_photos.append(photo);
}

void SetData::photoReady()
{
  // check if all photos are received
  bool allReceived = true;
//  foreach(PhotoData* photo, m_photos)
//  {
//    if (!photo->GetThumbImage())
//    {
//      allReceived = false;
//      break;
//    }
//  }
  if (allReceived)
  {
    qDebug() << "set ready: " << m_setName;
    m_setReady = true;
    setReady();
  }
}

void CollectionData::AddSet(SetData* set)
{
  connect(set, SIGNAL(setReady()), this, SLOT(setReady()));
  m_sets.append(set);
}

void CollectionData::setReady()
{
  qDebug() << "set ready";

  CheckReady();
}

void CollectionData::CheckReady()
{
//  if (m_collectionImage == 0)
//    return;
  bool setsReady = true;
  foreach(SetData* set, m_sets)
  {
    if (!set->isReady())
    {
      setsReady = false;
      break;
    }
  }
  if (setsReady)
  {
    qDebug() << "collection ready: " << m_collectionName;

    collectionReady();
  }
}

QImage* PhotoData::GetThumbImage()
{
  if (m_image)
    return m_image;
  QNetworkAccessManager manager;
  QEventLoop loop;
  QNetworkReply *reply = manager.get(QNetworkRequest(m_urlSq));
  QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));

  loop.exec();

  m_image = new QImage();
  m_image->loadFromData(reply->readAll());

  reply->deleteLater();
  return m_image;
}

void PhotoData::downloadPhoto(const Quality& quality)
{
  qDebug() << "downloading " << m_destFilePath;
    QNetworkAccessManager manager;
    QEventLoop loop;
    QUrl url;
    if (quality == Best)
      url = m_urlOrig;
    else url = m_urlLarge;
    QNetworkReply *reply = manager.get(QNetworkRequest(url));
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SIGNAL(downloadProgress(qint64,qint64)));
    QObject::connect(&manager, SIGNAL (finished(QNetworkReply*)),
            this, SLOT (photoReceived(QNetworkReply*)));
    loop.exec();
}


void PhotoData::photoReceived(QNetworkReply* reply)
{
  QFile file(m_destFilePath);
  file.open(QIODevice::WriteOnly);
  file.write(reply->readAll());
  reply->deleteLater();
}
