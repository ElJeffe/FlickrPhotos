#ifndef FLICKRPHOTOS_H
#define FLICKRPHOTOS_H

#include <QMainWindow>
#include <QList>
#include <QDir>
#include "qtflickr.h"
#include "quality.h"

class QSettings;
class QStandardItemModel;
class QtFlickr;
class UserData;
class SetData;
class PhotoData;
class SyncWaitDialog;

enum FlickrMethods
{
  CollectionTree,
  SetGetPhotos
};

//typedef QList<CollectionInfo> CollectionInfoList;

namespace Ui {
  class FlickrPhotos;
}

class FlickrPhotos : public QMainWindow {
  Q_OBJECT
public:
  FlickrPhotos(QWidget *parent = 0);
  ~FlickrPhotos();

public slots:
  void downloadNew();
  void showSettings();
  void openPhotos();

protected:
  void changeEvent(QEvent *e);
  void UpdateFlickrData();
  void fillSetData(SetData* set);

private slots:
  void requestFinished( int reqId, Response data, ErrorData err, void* userData );
  void collectionReady();

private:
  QDir cdDir(QDir dir, const QString& subDir);
  void updateNewPhotos();


private:
  Ui::FlickrPhotos *ui;
  SyncWaitDialog *syncWaitDialog;

  QtFlickr* m_qtFlickr;
  QSettings* m_settings;
  QStandardItemModel* m_treeViewCollectionData;
  QList<UserData*> m_userDataList;
  QHash<int, FlickrMethods> m_methodHash;
  QDir m_downloadLoc;
  Quality m_quality;
  QList<PhotoData*> m_newPhotos;
};

#endif // FLICKRPHOTOS_H
