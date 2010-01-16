#include "flickrPhotos.h"
#include "ui_flickrPhotos.h"
#include "qtflickr.h"
#include <QSettings>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDebug>
#include <QDesktopServices>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFileDialog>
#include "collectioninfo.h"
#include "syncwaitdialog.h"
#include "downloaddialog.h"
#include "settingsdialog.h"

QString UserName = "";
QString Token = "";

int reqToken;


FlickrPhotos::FlickrPhotos(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FlickrPhotos),
    m_quality(Best)
{
  ui->setupUi(this);
  ui->downloadWidget->setVisible(false);
  syncWaitDialog = new SyncWaitDialog(this);
  syncWaitDialog->setModal(true);
  connect(syncWaitDialog, SIGNAL(rejected()), this, SLOT(close()));

  m_settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "steelant.be", "FlickrPhotos");

  QVariant downloadLoc = m_settings->value("downloadLocation");
  if (downloadLoc.isNull())
  {
    SettingsDialog dialog(this);

    if (dialog.exec())
    {
      //m_downloadLoc = QDir(dialog.getDownloadLoc());
      m_downloadLoc.mkpath(dialog.getDownloadLoc());
      m_downloadLoc.setPath(dialog.getDownloadLoc());
      m_quality = dialog.getQuality();
      m_settings->setValue("downloadLocation", m_downloadLoc.absolutePath());
      m_settings->setValue("quality", m_quality);
    }
  }
  else
  {
    m_downloadLoc = QDir(downloadLoc.toString());
    QVariant v = m_settings->value("quality");
    if (!v.isNull())
      m_quality = (Quality)v.toInt();
  }
  qDebug() << "downloadloc: " << m_downloadLoc;

  m_qtFlickr = new QtFlickr("cf401e8951798a56aa7c68e317657d42", "7eab72b4d6c5e574", this);
  connect(m_qtFlickr,SIGNAL(requestFinished ( int, Response, ErrorData, void* )),
          this,SLOT(requestFinished ( int, Response, ErrorData, void* )));
  m_qtFlickr->setToken(Token);

  // temp filling of collectioninfo

  UserData* user = new UserData("89763256@N00", this);
  CollectionData* collection = new CollectionData("1390527-72157622791354067", user);  // Leander
  // CollectionData* collection = new CollectionData("1390527-72157622738851793", user);  // Natuur
  connect(collection, SIGNAL(collectionReady()), this, SLOT(collectionReady()));
  user->AddCollection(collection);
  m_userDataList.append(user);
  UpdateFlickrData();
}

FlickrPhotos::~FlickrPhotos()
{
  delete ui;
}

void FlickrPhotos::changeEvent(QEvent *e)
{
  QMainWindow::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    ui->retranslateUi(this);
    break;
  default:
    break;
  }
}

void FlickrPhotos::UpdateFlickrData()
{
  qDebug() << "downloadLoc " << m_downloadLoc.absolutePath();
  syncWaitDialog->show();
  for (QList<UserData*>::const_iterator it = m_userDataList.begin(); it != m_userDataList.end(); ++it)
  {
    UserData* user = *it;
    QList<CollectionData*> collections = user->GetCollections();
    for (QList<CollectionData*>::const_iterator itCollection = collections.begin(); itCollection != collections.end(); ++itCollection)
    {
      CollectionData* collection = *itCollection;
      collection->clear();
      Method method("flickr.collections.getTree");
      method.addArgument("user_id", user->GetUserId());
      method.addArgument("collection_id", collection->GetCollectionId());
      Request request("collection");
      request.addTag("set","id,title");
      request.addTag("collection", "title,iconsmall");
      int reqId = m_qtFlickr->get(method, request, collection);
      m_methodHash[reqId] = CollectionTree;
    }
  }
}

void FlickrPhotos::requestFinished( int reqId, Response data, ErrorData err, void* userData )
{
  QHash<int, FlickrMethods>::iterator it =  m_methodHash.find(reqId);
  if (it == m_methodHash.end())
  {
    qErrnoWarning("No method info for req id");
    return;
  }
  FlickrMethods method = *it;
  m_methodHash.erase(it);

  if(!err.code){

    if(method == CollectionTree)
    {
      QString collectionTitle = data.attrValue("collection", "title");
      QString iconSmall = data.attrValue("collection", "iconsmall");
      QList <QString> ids = data.attrValues("set", "id");
      QList <QString> titles = data.attrValues("set", "title");

      if (!userData)
      {
        QMessageBox msgBox(this);
        msgBox.setText("No collection provided");
        msgBox.exec();
        return;
      }
      CollectionData* collection = (CollectionData*)userData;
      collection->SetCollectionName(collectionTitle);
      QDir downloadLocation = cdDir(m_downloadLoc, collectionTitle);
      collection->SetDownloadLocation(downloadLocation);

      // get the image
      //          QNetworkAccessManager *manager = new QNetworkAccessManager(this);
      //          connect(manager, SIGNAL (finished(QNetworkReply*)),
      //                  collection, SLOT (thumbReceived(QNetworkReply*)));
      //          manager->get(QNetworkRequest(QUrl(iconSmall)));

      for (int i = 0; i < ids.size(); ++i)
      {
        QDir setDownloadLocation = cdDir(downloadLocation, titles[i]);

        SetData* setData = new SetData(ids[i], titles[i], setDownloadLocation, collection);
        collection->AddSet(setData);
        fillSetData(setData);
      }
    }
    if(method == SetGetPhotos)
    {
      QString primPhoto = data.attrValue("photoset", "primary");
      QList<QString> ids = data.attrValues("photo", "id");
      QList<QString> names = data.attrValues("photo", "title");
      QList<QString> urlSq = data.attrValues("photo", "url_sq");
      QList<QString> urlL = data.attrValues("photo", "url_l");
      QList<QString> urlO = data.attrValues("photo", "url_o");

      if (!userData)
      {
        QMessageBox msgBox(this);
        msgBox.setText("No set provided");
        msgBox.exec();
        return;
      }
      SetData* set = (SetData*)userData;
      set->SetPrimPhotoId(primPhoto);

      for (int i = 0; i < ids.size(); ++i)
      {
        QString  photoFile = set->GetDownloadLocation().absolutePath() + "/" + names[i] + ".jpg";
        qDebug() << "File: " << photoFile;
        PhotoData* photo = new PhotoData(ids[i], names[i], photoFile, urlSq[i], urlL[i], urlO[i], set);
        set->AddPhoto(photo);
        // get the image
        //            QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        //            connect(manager, SIGNAL (finished(QNetworkReply*)),
        //                    photo, SLOT (thumbReceived(QNetworkReply*)));
        //            manager->get(QNetworkRequest(QUrl(urlSq[i])));
      }
      set->photoReady();
    }
  }
}

void FlickrPhotos::fillSetData(SetData* set)
{
  Method method("flickr.photosets.getPhotos");
  method.addArgument("photoset_id", set->GetSetId());
  method.addArgument("extras", "url_sq,url_o, url_l");
  method.addArgument("media", "photos");
  Request request("set");
  request.addTag("photoset","primary");
  request.addTag("photo","id,title,url_sq,url_l,url_o");
  int reqId = m_qtFlickr->get(method, request, set);
  m_methodHash[reqId] = SetGetPhotos;

}

void FlickrPhotos::collectionReady()
{
  syncWaitDialog->hide();
  updateNewPhotos();
}

QDir FlickrPhotos::cdDir(QDir dir, const QString& subDir)
{
  if (!dir.cd(subDir))
  {
    dir.mkdir(subDir);
    dir.cd(subDir);
  }
  return dir;
}

void FlickrPhotos::updateNewPhotos()
{
  m_newPhotos.clear();
  // make a list of all new photos
  foreach(UserData* user, m_userDataList)
  {
    foreach(CollectionData* collection, user->GetCollections())
    {
      foreach(SetData* set, collection->GetSets())
      {
        foreach(PhotoData* photo, set->GetPhotos())
        {
          if (!QFile(photo->GetDestfilePath()).exists())
          {
            m_newPhotos.append(photo);
          }
        }
      }
    }
    foreach(SetData* set, user->GetSets())
    {
      foreach(PhotoData* photo, set->GetPhotos())
      {
        if (!QFile(photo->GetDestfilePath()).exists())
        {
          m_newPhotos.append(photo);
        }
      }
    }
  }

  ui->foundLabel->setText(QString(tr("%1 new photos found").arg(m_newPhotos.size())));
  ui->downloadButton->setEnabled(m_newPhotos.size() > 0);
  ui->downloadWidget->setVisible(true);
}
void FlickrPhotos::downloadNew()
{
  DownloadDialog downDialog;
  downDialog.setPhotos(m_newPhotos);
  downDialog.setQuality(m_quality);
  downDialog.exec();
  updateNewPhotos();
}

void FlickrPhotos::showSettings()
{
  SettingsDialog dialog(this);
  dialog.setDownloadLoc(m_downloadLoc.absolutePath());
  dialog.setQuality(m_quality);

  if (dialog.exec())
  {
    m_downloadLoc.mkpath(dialog.getDownloadLoc());
    m_downloadLoc.setPath(dialog.getDownloadLoc());
    m_quality = dialog.getQuality();
    m_settings->setValue("downloadLocation", m_downloadLoc.absolutePath());
    m_settings->setValue("quality", m_quality);
    UpdateFlickrData();
  }

}

void FlickrPhotos::openPhotos()
{
  QDesktopServices::openUrl(QUrl(m_downloadLoc.absolutePath()));
}

