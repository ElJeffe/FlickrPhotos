#ifndef COLLECTIONINFO_H
#define COLLECTIONINFO_H

#include <QString>
#include <QList>
#include <QObject>
#include <QUrl>
#include <QDir>
#include <QFile>
#include "quality.h"

class QImage;
class QNetworkReply;

class PhotoData: public QObject
{
  Q_OBJECT
public:
  PhotoData(const QString& photoId, const QString& name, const QString& destFilePath, const QUrl& urlSq, const QUrl& urlLarge, const QUrl& urlOrig, QObject* parent):
      QObject(parent), m_photoId(photoId), m_name(name), m_destFilePath(destFilePath), m_urlSq(urlSq), m_urlLarge(urlLarge), m_urlOrig(urlOrig), m_image(0) {}
  QImage* GetThumbImage();
  const QString& GetDestfilePath() const { return m_destFilePath; }
  const QString& GetName() const { return m_name; }
  const QUrl& GetUrlLarge() const { return m_urlLarge; }
  const QUrl& GetUrlOrig() const { return m_urlOrig; }
  void downloadPhoto(const Quality& quality);

private slots:
  void photoReceived(QNetworkReply* reply);
signals:
  void photoReady();
  void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
  QString m_photoId;
  QString m_name;
  QString m_destFilePath;
  QUrl m_urlSq;
  QUrl m_urlLarge;
  QUrl m_urlOrig;
  QImage* m_image;
};

class SetData: public QObject
{
  Q_OBJECT
public:
  SetData(QString setId, QString setName, QDir downloadLocation, QObject* parent = 0) :
      QObject(parent), m_setId(setId), m_setName(setName), m_setReady(false), m_downloadLocation(downloadLocation) {}
  QString GetSetId() { return m_setId; }
  QImage* GetImage() { return 0; }
  void SetPrimPhotoId(QString primPhotoId) { m_primPhotoId = primPhotoId; }
  void AddPhoto(PhotoData* photo);
  QList<PhotoData*> const GetPhotos() const {return m_photos; }
  bool isReady() const { return m_setReady; }
  QDir GetDownloadLocation(){ return m_downloadLocation; }
public slots:
  void photoReady();
signals:
  void setReady();

private:
  QString m_setId;
  QString m_setName;
  QString m_primPhotoId;
  QList<PhotoData*> m_photos;
  bool m_setReady;
  QDir m_downloadLocation;
};

class CollectionData: public QObject
{
  Q_OBJECT
public:
  CollectionData(QString collectionId, QObject* parent = 0) :
    QObject(parent), m_collectionId(collectionId), m_collectionImage(0), m_collectionReady(false) {}
  virtual ~CollectionData() {}
  QString GetCollectionId() const { return m_collectionId; }
  QString GetCollectionName() const { return m_collectionName; }
  void SetCollectionName(QString collectionName) { m_collectionName = collectionName; }
  void SetDownloadLocation(QDir downloadLocation) { m_downloadLocation = downloadLocation; }
  QImage* GetImage() { return m_collectionImage; }
  void SetImage(QImage* image) { m_collectionImage = image; }
  void AddSet(SetData* set);
  QList<SetData*> GetSets() const { return m_sets; }
  void clear() { m_sets.clear(); }
  bool IsCollectionReady() const { return m_collectionReady; }
  QDir GetDownloadLocation(){ return m_downloadLocation; }

public slots:
  void thumbReceived(QNetworkReply* reply);
  void setReady();

signals:
  void collectionReady();
private:
  void CheckReady();

private:
  QString m_collectionId;
  QString m_collectionName;
  QImage* m_collectionImage;
  QList<SetData*> m_sets;
  bool m_collectionReady;
  QDir m_downloadLocation;
};


class UserData: public QObject
{
public:
  UserData(QString userId, QObject* parent = 0) :
      QObject(parent), m_userId(userId) {}
  UserData(QString userId, QString userName) :
      m_userId(userId), m_userName(userName) {}
  QString GetUserName() const { return m_userName; }
  void SetUserName(QString userName) { m_userName = userName; }
  QString GetUserId() const { return m_userId; }
  void AddCollection(CollectionData* collection) { m_collections.append(collection); }
  QList<CollectionData*> GetCollections() const { return m_collections; }
  QList<SetData*> GetSets() const { return m_sets; }
private:
  QString m_userId;
  QString m_userName;
  QList<CollectionData*> m_collections;
  QList<SetData*> m_sets;
};




#endif // COLLECTIONINFO_H
