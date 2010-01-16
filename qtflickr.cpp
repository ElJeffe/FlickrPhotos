/******************************************************************************
 *   Copyright (C) 2009 by Evgeni Gordejev   *
 *   evgeni.gordejev@gmail.com   *
 *                                                                            *
 *   This program is free software; you can redistribute it and/or modify     *
 *   it under the terms of the GNU Library Lesser General Public License as   *
 *   published by the Free Software Foundation; either version 2 of the       *
 *   License, or (at your option) any later version.                          *
 *                                                                            *
 *   This program is distributed in the hope that it will be useful,          *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *   GNU Lesser General Public License for more details.                      *
 *                                                                            *
 *   You should have received a copy of the GNU Library Lesser General Public *
 *   License along with this program; if not, write to the                    *
 *   Free Software Foundation, Inc.,                                          *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                *
 ******************************************************************************/

#include "qtflickr.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QCryptographicHash>


QtFlickr::QtFlickr ( const QString &apiKey,
                     const QString &apiSecret,
                     QObject *parent )
		:QObject ( parent )	,
		apiKey ( apiKey ),
		apiSecret ( apiSecret )
{
	manager = new QNetworkAccessManager ( this );
	connect ( manager, SIGNAL ( finished ( QNetworkReply* ) ),
	          this, SLOT ( replyFinished ( QNetworkReply* ) ) );
}


QtFlickr::~QtFlickr()
{
}

void QtFlickr::setToken ( const QString &token )
{
	this->token = token;
}

QUrl QtFlickr::authorizationUrl ( const QString &frob, const QString &perms )
{
	QString authUrl;
	QString apiSig = apiSecret+"api_key"+apiKey+"frob"+frob+"perms"+perms;

	apiSig = md5 ( apiSig );
	authUrl = "http://flickr.com/services/auth/?api_key="+apiKey+
	          "&perms="+perms+"&frob="+frob+"&api_sig="+apiSig;

	return QUrl ( authUrl );
}

int QtFlickr::get ( const Method &method, const Request &req, void* userData )
{
	return request ( method, req, true, userData );
}

int QtFlickr::post ( const Method &method, const Request &req, void* userData )
{
	return request ( method, req, false, userData );
}

int QtFlickr::upload ( const Photo &photo,
                       const Request &request,
                       void* userData )
{
	QByteArray boundary = generateBoundary();
	QByteArray payload;
	QDataStream dataStream ( &payload, QIODevice::WriteOnly );

	QMap<QString,QString> map = photo.map;
	
	
	map.insert ( "api_key", apiKey );
	if ( !token.isEmpty() )
	{
		map.insert ( "auth_token", token );
	}
	
	bool uploading = photo.photoId.isEmpty();
	if(!uploading){
		map.insert ( "photo_id", photo.photoId );
	}

	QMapIterator<QString, QString> i ( map );
	QStringList keyList;
	while ( i.hasNext() )
	{
		i.next();
		keyList << i.key();
	}
	qSort ( keyList.begin(), keyList.end() );

	QString apiSig ( apiSecret );
	QString tmpUrl ( "http://www.flickr.com/services/rest/?api_key="+apiKey );
	for ( int i = 0; i < keyList.size(); ++i )
	{
		apiSig.append ( keyList.at ( i ) + map.value ( keyList.at ( i ) ) );

		QByteArray field = constructField ( keyList.at ( i ),map.value ( keyList.at ( i ) ),boundary );
		dataStream.writeRawData ( field.data(), field.length() );

	}
	apiSig = md5 ( apiSig );

	QByteArray sigField = constructField ( "api_sig", apiSig, boundary );
	dataStream.writeRawData ( sigField.data(), sigField.length() );

	QByteArray fileField = constructField ( "photo", "", boundary, photo.file );
	dataStream.writeRawData ( fileField.data(), fileField.length() );

	QFile file ( photo.file );
	file.open ( QIODevice::ReadOnly );
	while ( !file.atEnd() )
	{
		QByteArray line = file.readLine();
		dataStream.writeRawData ( line.data(),line.length() );
	}

	file.close();

	QByteArray endField;
	endField.append ( "\r\n--" );
	endField.append ( boundary );
	endField.append ( "--\r\n\r\n" );
	dataStream.writeRawData ( endField.data(), endField.length() );

	QString urlTmp("http://api.flickr.com/services/");
	urlTmp.append((uploading)?"upload/":"replace/");

	QNetworkRequest uploadRequest ( urlTmp );
	uploadRequest.setRawHeader ( "Content-Type","multipart/form-data; boundary="+boundary );
	uploadRequest.setRawHeader ( "Host","ww.api.flickr.com" );

	RequestData requestData;
	requestData.tagList = request.map;
	requestData.userData = userData;

	QNetworkReply *reply = manager->post ( uploadRequest,payload );
	connect(reply,SIGNAL(uploadProgress ( qint64, qint64 )),
			this, SLOT(uploadProgress ( qint64, qint64 )));
	requestDataMap.insert ( (int)reply,requestData );
	return (int)reply;
}


/************************* PRIVATE SLOTS **************************/
/******************************************************************/
void QtFlickr::replyFinished ( QNetworkReply *reply )
{

	QByteArray data = reply->readAll();
	qDebug()<<"*******************************RESPONSE*******************************";	
	qDebug()<<data;
	qDebug()<<"**********************************************************************\n\n";


	replyTagList.clear();
	err.code = 0;
	err.message = "No Errors";

	if ( reply->error() != QNetworkReply::NoError )
	{
		err.code = 1001;
		err.message = reply->errorString ();
	}
	else
	{
		tagList = requestDataMap.value ( ( int ) reply ).tagList;
		parse ( data, "rsp" , QStringList() );
	}

	req.map = replyTagList;

	void* userData = requestDataMap.value ( ( int ) reply ).userData;

	requestDataMap.remove ( ( int ) reply );
	emit requestFinished ( ( int ) reply,req, err, userData );
}

void QtFlickr::uploadProgress ( qint64 bytesSent, qint64 bytesTotal )
{
	int percent;
	if ( bytesTotal != -1 )
		percent = qRound ( ( qreal ) bytesSent/ ( qreal ) bytesTotal*100 );
	else
		percent = -1;

	emit uploadProgress ( percent );
}

/********************* PRIVATE FUNCTIONS **************************/
/******************************************************************/
QByteArray QtFlickr::generateBoundary()
{
	int lenght = 15;
	QChar unicode[lenght];
	for ( int i = 0; i < lenght; ++i )
	{
		int sel = qrand() % 2;
		int temp = ( sel ) ? qrand() % 9 + 49 : qrand() % 23 + 98;
		unicode[i] = QChar ( temp );
	}

	int size = sizeof ( unicode ) / sizeof ( QChar );
	QString str = QString::fromRawData ( unicode, size );

	return str.toLatin1();
}

QByteArray QtFlickr::constructField ( QString name,
                                      QString content,
                                      QByteArray boundary,
                                      QString filename )
{
	QByteArray data;
	data.append ( "--" );
	data.append ( boundary );
	data.append ( "\r\n" );
	data.append ( "Content-Disposition: form-data; name=\"" );
	data.append ( name );
	if ( filename.isEmpty() )
	{
		data.append ( "\"\r\n\r\n" );
		data.append ( content );
		data.append ( "\r\n" );
	}
	else
	{
		data.append ( "\"; filename=\"" );
		data.append ( filename );
		data.append ( "\"\r\n" );
		data.append ( "Content-Type: image/jpeg\r\n\r\n" );
	}

	return data;
}


int QtFlickr::request ( const Method &method, const Request &request, bool get, void* userData )
{
	QMap<QString,QString> map = method.map;

	map.insert ( "api_key",apiKey );
	if ( !token.isEmpty() )
	{
		map.insert ( "auth_token", token );
	}

	QMapIterator<QString, QString> i ( map );
	QStringList keyList;
	while ( i.hasNext() )
	{
		i.next();
		keyList << i.key();
	}
	qSort ( keyList.begin(), keyList.end() );

	QString apiSig ( apiSecret );
	QUrl url ( "http://www.flickr.com/services/rest/", QUrl::TolerantMode );
	for ( int i = 0; i < keyList.size(); ++i )
	{
		apiSig.append ( keyList.at ( i ) + map.value ( keyList.at ( i ) ) );
		url.addQueryItem ( keyList.at ( i ),  map.value ( keyList.at ( i ) ) );
	}

	url.addQueryItem ( "api_sig",  md5 ( apiSig ) );

	RequestData requestData;
	requestData.tagList = request.map;
	requestData.userData = userData;
	int replyId;

	if ( !get )
		replyId = ( int ) manager->post ( QNetworkRequest ( QUrl("http://www.flickr.com/services/rest/") ),
		                                  url.encodedQuery () );
	else
		replyId = ( int ) manager->get ( QNetworkRequest ( url ) );

	requestDataMap.insert ( replyId,requestData );
	return replyId;
}

void QtFlickr::parse ( const QByteArray &data, const QString &startTag, const QStringList &tags )
{
	xml.clear();
	xml.addData ( data );

	while ( !xml.atEnd() )
	{
		xml.readNext();
		if ( xml.isStartElement() )
		{
			if ( xml.name() == startTag )
			{
				if ( xml.attributes().value ( "stat" ) == "ok" )
					readElements ( tags );
				else
					readErrorElement();
			}
			else
			{
				xml.raiseError ( QObject::tr ( "The file is not an RSP file." ) );
			}
		}
	}

	if ( xml.hasError() )
	{
		err.code = 1002;
		err.message = xml.errorString();
	}
}

void QtFlickr::readUnknownElement()
{
	while ( !xml.atEnd() )
	{
		xml.readNext();

		if ( xml.isEndElement() )
			break;

		if ( xml.isStartElement() )
			readUnknownElement();
	}
}

void QtFlickr::readElements ( const QStringList &/*tags*/ )
{
	while ( !xml.atEnd() )
	{
		xml.readNext();

		if ( xml.isStartElement() )
		{
			readData();
		}
	}
}


void QtFlickr::readErrorElement( )
{
	while ( !xml.atEnd() )
	{
		xml.readNext();

		if ( xml.isEndElement() )
			break;

		if ( xml.isStartElement() )
		{
			if ( xml.name() == "err" )
				readError();
			else
				readUnknownElement();
		}
	}
}

void QtFlickr::readError()
{
	bool ok;
	err.code = xml.attributes().value ( "code" ).toString().toInt ( &ok );
	err.message = xml.attributes().value ( "msg" ).toString();
	xml.readElementText();
}


void QtFlickr::readData()
{
	QMap<QString, TagData>::const_iterator i = tagList.find ( xml.name().toString() );
	if ( i != tagList.end() )
	{
		TagData tagData;

		QList<QString> attrKeys = i.value().attributes.keys();
		for ( int j = 0; j < attrKeys.size(); ++j )
		{
			QString value = xml.attributes().value ( attrKeys.at ( j ) ).toString();
			tagData.attributes.insert ( attrKeys.at ( j ), value );
		}

		if ( xml.readNext() == QXmlStreamReader::Characters )
		{
			tagData.value =  xml.text ().toString();
		}
		replyTagList.insertMulti ( i.key(),tagData );
	}
}

QString QtFlickr::md5 ( const QString &data )
{
	QString hash ( QCryptographicHash::hash ( data.toLatin1(),QCryptographicHash::Md5 ).toHex() );
	return hash;
}
