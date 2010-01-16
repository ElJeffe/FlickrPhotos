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


#ifndef QTFLICKR_H
#define QTFLICKR_H

#include <QtCore>
#include <QObject>
#include <QXmlStreamReader>

class QNetworkAccessManager;
class QNetworkReply;

/**
 * Flickr errors. Find more about flickr errors on http://www.flickr.com/services/api/
 * page
 */
struct ErrorData
{
	/**
	 * error codes:<br>
	 * <ul><li>0 - No errors</li>
	 * </ul>
	 */
	int code;
	/**
	 * Error string
	 */
	QString message;
};


/**
 * tag struct
 */
struct TagData
{
	/** Tag value */
	QString value;
	/** Tag attributes */
	QMap<QString,QString> attributes;
};


/**
 * @class Method
 * Use this class to create method for accessing Flickr API<br>
 * Example:
 * @code
 * Method method("flickr.photos.comments.addComment");
 * method.addArgument("photo_id","87243209893");
 * method.addArgument("comment_text","This comment was added through QtFlickr library :) ");
 *
 * QtFlickr qtFlickr("apiKey","apiSecret");
 * qtFlickr.get(method);
 * @endcode
 */
class Method
{
	public:
		/**
		 * @param method Flickr API valid method, for example "flickr.photos.setPerms"
		 */
		Method ( const QString &method )
		{
			map["method"] = method;
		}

		/**
		 * <b><i>Note: You don't have to provide api_key as argument.</i></b>
		 * @param argument name, for example "photo_id"
		 * @param value Value of the argument, for example "435834534"
		 */
		void addArgument ( const QString &argument, const QString &value )
		{
			map[argument] = value;
		}
	private:
		friend class QtFlickr;
		QMap<QString,QString> map;
};

/**
 * @class Request
 * Use this class to provide information for the XML parser, parser will
 * search only those tags that you have been provided.<br>
 * Example:
 * @code
 * <?xml version="1.0" encoding="utf-8" ?>
 * <rsp stat="ok">
 * 	<photos page="2" pages="89" perpage="10" total="881">
 * 		<photo id="2636" owner="47058503995@N01"/>
 * 		<photo id="2635" owner="47058503995@N01"/>
 * 	</photos>
 * </rsp>
 * @endcode
 * Let assume server response is a XML above and you want to get all id's and owner information for the
 * photo list. The code will look like this:
 * @code
 * Request request("photo","id,owner");
 *
 * QtFlickr qtFlickr("apiKey","apiSecret");
 * qtFlickr.get(Method("flickr.photos.getWithGeoData"),request);
 * @endcode
 */
class Request
{
	public:
		/**
		 * Constructs empty request, this mean that you don't want any information to
		 * be parsed
		 */
		Request() {}

		/**
		 * Construct request and adds one tag name to be parsed
		 * @param name tag name to be parsed
		 * @param attrs A comma-delimited list of attributes to be parsed within this tag name
		 */
		Request ( const QString &name, const QString &attrs = QString() )
		{
			addTag ( name,attrs );
		}
		/**
		 * Adds tag name to be parsed into request list
		 * @param name tag name to be parsed
		 * @param attrs A comma-delimited list of attributes to be parsed within this tag name
		 */
		void addTag ( const QString &name, const QString &attrs = QString() )
		{
			TagData tagData;
			if ( !attrs.isEmpty() )
			{
				QStringList attrList = attrs.split ( "," );
				for ( int i = 0; i < attrList.size(); ++i )
				{
					tagData.attributes.insert ( attrList.at ( i ),QString() );
				}
			}
			map.insert ( name,tagData );
		}
	private:
		friend class QtFlickr;
		QMap<QString,TagData> map;
};

/**
 * @class Photo
 * This class is used when uploading or replacing photos.
 * Similar to Method class
 */
class Photo
{
	public:
		/**
		 * Constructor
		 * @param file File to be uploaded
		 * @param photoId The ID of the photo to replace, do not set this
		 * value if you want just to upload photo
		 */
		Photo ( const QString &file,
		        const QString &photoId = QString() )
				:file ( file ),
				photoId ( photoId ) {}

		/**
		 * Adds optional arguments
		 * @param argument The name of the argument, for example "title"
		 * @param value Value of the argument, for example "My photo"
		 */
		void addArgument ( const QString &argument, const QString &value )
		{
			map[argument] = value;
		}
	private:
		friend class QtFlickr;
		QString file;
		QString photoId;
		QMap<QString,QString> map;
};

/**
 * @class Response
 * Class is used to collect data from the parser,
 * it will return only those values that has been setted with the Request object<br>
 * Example:
 * @code
 * <?xml version="1.0" encoding="utf-8" ?>
 * <rsp stat="ok">
 * 	<photos page="2" pages="89" perpage="10" total="881">
 * 		<photo id="2636" owner="47058503995@N01"/>
 * 		<photo id="2635" owner="47058503995@N01"/>
 * 	</photos>
 * </rsp>
 * @endcode
 * If you want to get page number from XML above:
 * @code
 * QString page = response.attrValue("photos","page");
 * qDebug()<<page //output: 2
 * @endcode
 */
class Response
{
	public:
		Response() {}
		/**
		 * @param tagName
		 * @return value of the tag
		 */
		QString tagValue ( const QString &tagName )
		{
			return map.value ( tagName ).value;
		}

		/**
		 * @param tagName
		 * @param attrName
		 * @return attribute value
		 */
		QString attrValue ( const QString &tagName, const QString &attrName )
		{
			return map.value ( tagName ).attributes.value ( attrName );
		}

		/**
		 * @param tagName
		 * @return list of values with this tag name
		 */
		QList<QString> tagValues ( const QString &tagName )
		{
			QList<TagData> tmp = map.values ( tagName );
			QList<QString> res;
			for ( int i = 0; i < tmp.size(); ++i )
			{
				res << tmp.at ( i ).value;
			}
			return res;
		}

		/**
		 * @param tagName
		 * @param attrName
		 * @return list of attribute values with this tag name
		 */
		QList<QString> attrValues ( const QString &tagName, const QString &attrName )
		{
			QList<TagData> tmp = map.values ( tagName );
			QList<QString> res;
			for ( int i = 0; i < tmp.size(); ++i )
			{
				res << tmp.at ( i ).attributes.value ( attrName );
			}
			return res;
		}
	private:
		friend class QtFlickr;
		QMap<QString,TagData> map;
};


/**
 * @class QtFlickr
 * Main class of QtFlickr APi<br>
 * Authorization example:
 * @code
 * UIClass::UIClass
 * {
 * 	qtFlickr = new QtFlickr("xxxxxxxxx", "xxxxxxxxxxxxx", this);
 * 	connect(qtFlickr,SIGNAL(requestFinished ( int, Response, ErrorData, void* )),
 * 		this,SLOT(requestFinished ( int, Response, ErrorData, void* )));
 *
 * 	Method method("flickr.auth.getFrob");
 * 	Request request("frob");
 * 	frobRequest = qtFlickr->get(method,request);
 * }
 *
 * void UIClass::requestFinished ( int reqId, Response data, ErrorData err, void* userData )
 * {
 * 	if(err.code != 0){
 * 		if(reqId == frobRequest){
 * 			QString frob = data.tagValue("frob");
 * 			QUrl authUrl = qtFlickr->authorizationUrl(frob);
 * 			QDesktopServices::openUrl ( authUrl );
 *
 * 			QMessageBox msgBox;
 * 			msgBox.setText("Press Ok button when you have completed authorization through web browser")
 * 			int result = msgBox.exec();
 * 			if( result == QDialog::Accepted){
 * 				Method method("flickr.auth.getToken");
 * 				method.addArgument( "frob", frob );
 * 				Request request("token");
 * 				request.addTag("user","username,fullname");
 * 				tokenRequest = qtFlickr->get(method, request);
 * 			}
 * 		}else if(reqId == tokenRequest){
 * 			QString token = data.tagValue("token");
 * 			QString username = data.attrValue("user", "username");
 * 			QString fullname = data.attrValue("user", "fullname");
 *
 * 			qDebug()<<"Your username: "<<username;
 * 			qDebug()<<"Your fullname: "<<fullname;
 *
 * 			qtFlickr->setToken(token);
 *			//Now you can call authorized calls with "write" permission
 * 		}
 * 	}else{
 * 		qDebug()<<"Error: "<<err.message;
 * 	}
 * }
 * @endcode
 */
class QtFlickr : public QObject
{
		Q_OBJECT
	public:

		/**
		 * Constructor
		 * @param apiKey Flickr api key
		 * @param apiSecret Flickr api secret
		 * @param parent object
		 */
		QtFlickr ( const QString &apiKey,
		           const QString &apiSecret,
		           QObject *parent = 0 );
		~QtFlickr();

		/**
		 * Sets token for the QtFlickr API
		 * @param token
		 */
		void setToken ( const QString &token );

		/**
		 * Returns authorization url for give frob
		 * @param frob
		 * @param perms "write", "read" or "delete"
		 */
		QUrl authorizationUrl ( const QString &frob, const QString &perms = "write" );

		/**
		 * Fetches data from Flickr API with GET method
		 * @param method Flickr API method
		 * @param request XML request data
		 * @param userData user data to be transfered through signal/slots mechanism,
		 * this parametr has no effect on this function
		 * @return request id
		 */
		int get ( const Method &method,
		          const Request &request = Request(),
		          void* userData = 0 );

		/**
		 * Fetches data from Flickr API with POST method
		 * @param method Flickr API method
		 * @param request XML request data
		 * @param userData user data to be transfered through signal/slots mechanism,
		 * this parametr has no effect on this function
		 * @return request id
		 */
		int post ( const Method &method,
		           const Request &request = Request(),
		           void* userData = 0 );

		/**
		 * Uploads or replaces photo
		 * @param photo to be uploaded
		 * @param request XML request data
		 * @param userData user data to be transfered through signal/slots mechanism,
		 * this parametr has no effect on this function
		 * @return request id
		 */
		int upload ( const Photo &photo,
		             const Request &request = Request(),
		             void* userData = 0 );

	signals:
		/**
		 * Emitted after get(), post() and upload() functions
		 * @param reqId The request id
		 * @param data Response XML data
		 * @param err possible error
		 * @param userData user data transfered through signal/slots mechanism
		 */
		void requestFinished ( int reqId, Response data, ErrorData err, void* userData );

		/**
		 * Emitted while photo uploading is in progress
		 * @param percent 0-100, returns -1 when cannot determine uploading
		 * progress.
		 */
		void uploadProgress ( int percent );
	private slots:
		void replyFinished ( QNetworkReply *reply );
		void uploadProgress ( qint64 bytesSent, qint64 bytesTotal );
	private:
		QByteArray generateBoundary();
		QByteArray constructField ( QString name,
		                            QString content,
		                            QByteArray boundary,
		                            QString filename = QString() );
		int request ( const Method &method, const Request &request, bool get, void* userData );
		void parse ( const QByteArray &data, const QString &startTag, const QStringList &tags );
		void readUnknownElement();
		void readElements ( const QStringList &tags );
		void readErrorElement( );
		void readError();
		void readData();

		struct RequestData
		{
			void* userData;
			QMap<QString,TagData> tagList;
		};

		QString md5 ( const QString &data );

		QString apiKey;
		QString apiSecret;
		QString token;

		QNetworkAccessManager *manager;
		QXmlStreamReader xml;

		QMap<QString,TagData> tagList;
		QMap<QString,TagData> replyTagList;
		Response req;
		QMap<int,RequestData> requestDataMap;
		ErrorData err;
};

#endif
