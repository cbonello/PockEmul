// FIXME: test with no config.ini. Seems to crash

#include <QApplication>
#if QT_VERSION >= 0x050000
// Qt5 code
#   include <QtWidgets>
#else
// Qt4 code
#   include <QtCore>
#   include <QFileDialog>
#   include <QGraphicsObject>
#   include <QtGui>
#endif
#include <QDebug>
#include <QFrame>
#include <QLabel>
#include <QListView>
#include <QMenu>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QUrl>
#include <QSettings>
#include <QCryptographicHash>

#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickView>
#include <QtQuickWidgets/QQuickWidget>
#include <QKeyEvent>


#include "applicationconfig.h"
#include "cloudwindow.h"
#include "cloudimageprovider.h"

#include "mainwindowpockemul.h"
#include "renderView.h"
#include "pobject.h"
#include "Keyb.h"

#include "quazip/quazip.h"
#include "quazip/quazipfile.h"
#include "quazip/JlCompress.h"

extern MainWindowPockemul *mainwindow;
extern CrenderView* view;
extern int ask(QWidget *parent, QString msg, int nbButton);
extern void m_addShortcut(QString name,QString param);

extern bool syncEnabled;
extern bool soundEnabled;
extern bool kbSoundEnabled;
extern bool hiRes;
extern bool flipOnEdge;
extern bool trackerEnabled;
extern bool magnifyTouch;

extern QString workDir;

extern QList<CPObject *> listpPObject;
extern QSettings* settings;

CloudWindow::CloudWindow(QWidget *parent,QString src)
    : QWidget(parent),cloud(this)
{

    if (cloud.getValueFor("serverURL","")=="")
        cloud.saveValueFor("serverURL","http://pockemul.ddns.net:6969");

//    if (mainwindow->openGlFlag)
    {
        cloudView = new QQuickWidget;
        cloudView->engine()->addImageProvider(QLatin1String("PockEmulCloud"),cloud.imgprov );
        cloudView->rootContext()->setContextProperty("cloud", &cloud);
        cloudView->setSource(QUrl(src));
        cloudView->setResizeMode(QQuickWidget::SizeRootObjectToView);
        cloud.object = (QObject*) cloudView->rootObject();
    }


    connect(cloud.object, SIGNAL(close()), this,SLOT(closeQuick()));
    connect(&cloud,SIGNAL(downloadEnd()),this,SLOT(closeQuick()));

    QObject::connect(cloud.object, SIGNAL(sendWarning(QString)), &cloud, SLOT(warning(QString)));


    connect(cloud.m_fileDialog, SIGNAL(fileSelected(QString)),
            this, SLOT(sendPML(QString)));

    QVBoxLayout *windowLayout = new QVBoxLayout(this);
//    QWidget *container = QWidget::createWindowContainer(view,this);
//    container->setMinimumSize(...);
//    container->setMaximumSize(...);
//    container->setFocusPolicy(Qt::TabFocus);

//    windowLayout->addWidget(container);
    windowLayout->addWidget(cloudView);
    windowLayout->setMargin(0);


}

void CloudWindow::resizeEvent(QResizeEvent *e) {
    Q_UNUSED(e)

//    view->setGeometry(this->geometry());
}

QSize CloudWindow::sizeHint() const
{
    return QSize(500, 700);
}

Cloud::Cloud(QWidget* parent)
{
    this->parent = parent;
    imgprov = new CloudImageProvider(parent);
    m_fileDialog = new QFileDialog(parent);
    m_fileDialog->setFileMode(QFileDialog::ExistingFile);
    m_fileDialog->setNameFilter("PockEmul Session files (*.pml)");
    object=0;
}

int Cloud::askDialog(QString msg, int nbButton)
{
    return ask(parent, msg, nbButton);
}

void CloudWindow::wheelEvent(QWheelEvent *event)
{
    event->accept();
}

void Cloud::refresh()
{

}


QString Cloud::save()
{
//    hide();
//    mainwindow->repaint();

//    parent->hide();
    QString s = mainwindow->saveassessionString().remove(0,1);
//    parent->show();
//    qWarning()<<"session saved";
//    qWarning()<<s.left(500);
//    qWarning()<<"session saved";
//    show();
    return s;

//    sendPML(filePath);
//    show();
//    refresh();
}






void Cloud::sendPML(const QString &filePath)
{
//    qWarning()<<"sendPML";
    if (filePath.isEmpty())
        return;

    QFile file(filePath);

    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();
    file.close();
    QNetworkAccessManager *mgr = new QNetworkAccessManager();
    // the HTTP request

    QString apikey = getValueFor("apikey","");
    QString server = getValueFor("serverURL","")+"savePML";
    // Check if apikey exists

// Qt5 code
    QUrlQuery qu;
    qu.addQueryItem("apikey",apikey);
    qu.addQueryItem("content",QString(data).replace("+","%2B"));
//    qWarning()<<qu.query(QUrl::FullyEncoded).toUtf8();
    // some parameters for the HTTP request

    QNetworkRequest req(server);
//    qWarning()<<req.url();
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(finishedSave(QNetworkReply*)));
    QNetworkReply *reply = mgr->post(req, qu.query(QUrl::FullyEncoded).toUtf8());
    Q_UNUSED(reply)

}

void Cloud::finishedSave(QNetworkReply *reply)
{

    QByteArray xmlData = reply->readAll();

//    qWarning()<<"received id:"<<xmlData;

    QString pmlid,username,ispublic,isdeleted,title,description,keywords;

    QXmlStreamReader *xml = new QXmlStreamReader(xmlData);
    if (xml->readNextStartElement()) {
        if (xml->name() == "item") {
            while (!xml->atEnd()) {
                while (xml->readNextStartElement()) {
                    QString eltname = xml->name().toString();
                    if (eltname == "pmlid") pmlid = xml->readElementText();
                    if (eltname == "username") username = xml->readElementText();
                    if (eltname == "ispublic") ispublic = xml->readElementText();
                    if (eltname == "isdeleted") isdeleted = xml->readElementText();
                    if (eltname == "title") title = xml->readElementText();
                    if (eltname == "description") description = xml->readElementText();
                    if (eltname == "keywords") keywords = xml->readElementText();
                }
            }
        }
    }
//    qWarning()<<pmlid<<username<<ispublic<<isdeleted<<title<<description<<keywords;

    QMetaObject::invokeMethod(object, "addRefPmlModel",
                              Q_ARG(QVariant, pmlid),
                              Q_ARG(QVariant, username),
                              Q_ARG(QVariant, keywords),
                              Q_ARG(QVariant, ispublic),
                              Q_ARG(QVariant, isdeleted),
                              Q_ARG(QVariant, title),
                              Q_ARG(QVariant, description)
                              );


    reply->deleteLater();
}


void Cloud::downloadFinishedPml()
{
//    qWarning()<<"CloudWindow::downloadFinished - ";
    QByteArray data = m_reply->readAll();

    QBuffer buf(&data);

    QuaZip zip(&buf);

    zip.open(QuaZip::mdUnzip);

    if (!zip.setCurrentFile("session.pml")) {
        // ERROR
        qWarning()<<"ERROR - package.json missing";
        return;
    }
    QuaZipFile file(&zip);
    file.open(QIODevice::ReadOnly);
    QByteArray xmlData =   file.readAll();


//    qWarning() << "data="<<xmlData.left(200);
    QXmlStreamReader *xml = new QXmlStreamReader(xmlData);


    mainwindow->opensession(xml);
//    emit imageChanged(m_object.value("id").toString());

    m_reply->deleteLater();
    QMetaObject::invokeMethod(object, "hideWorkingScreen");

    emit downloadEnd();
//    this->hide();

}

void Cloud::downloadFinishedPsk()
{
    qWarning()<<"CloudWindow::downloadFinishedPsk - ";
    QByteArray data = m_reply->readAll();

    QBuffer buf(&data);

    QuaZip zip(&buf);

    zip.open(QuaZip::mdUnzip);

    // check Package.json exist
    if (!zip.setCurrentFile("package.json")) {
        // ERROR
        qWarning()<<"ERROR - package.json missing";
        return;
    }

    // open Package.json
    QuaZipFile file(&zip);
    file.open(QIODevice::ReadOnly);
    QByteArray _ba =   file.readAll();
    QJsonDocument json = QJsonDocument::fromJson(_ba);

    QString model = json.object().value("model").toString();

    if (model.isEmpty()) {
        // ERROR
        qWarning()<<"ERROR model empty";
        return;
    }
    file.close();

    // Check if workDir+"/res/"+model directory exist
    QDir home(workDir);
    home.mkpath("res/"+model);
    JlCompress::extractDir(&buf,workDir+"/res/"+model);

    qWarning()<<json;

    m_reply->deleteLater();
    QMetaObject::invokeMethod(object, "hideWorkingScreen");

    emit downloadEnd();
//    this->hide();

}

void Cloud::downloadFinished2()
{
//    qWarning()<<"CloudWindow::downloadFinished - ";
    QByteArray xmlData = m_reply->readAll();
//    qWarning() << "data="<<xmlData.left(200);
    QXmlStreamReader *xml = new QXmlStreamReader(xmlData);

    if (xml->readNextStartElement() && (xml->name() == "elgg")) {
        if (xml->readNextStartElement() &&
                (xml->name() == "status") &&
                (xml->readElementText().toInt()==0)) {
            if (xml->readNextStartElement() &&
                    (xml->name() == "result")) {
                QString pmlData = xml->readElementText();
                qWarning() << "data="<<pmlData.left(200);
                QXmlStreamReader *pmlxml = new QXmlStreamReader(pmlData);
                mainwindow->opensession(pmlxml);
            }
        }
    }

    m_reply->deleteLater();
    QMetaObject::invokeMethod(object, "hideWorkingScreen");

    emit downloadEnd();
//    this->hide();

}


void Cloud::showFileDialog()
{
    m_fileDialog->show();
}

void Cloud::getPML(int id,int version,QString auth_token, QString type) {
    QNetworkAccessManager *mgr = new QNetworkAccessManager();
    QString url;

    QMetaObject::invokeMethod(object, "showWorkingScreen");

    if (version==0) {
        QString apikey = getValueFor("apikey","");
        QString server = getValueFor("serverURL","http://pockemul.dscloud.me/pocketcloud/")+"getPML";
        url = server+QString("/%1/%2").arg(getValueFor("apikey","0")).arg(id);
    }
    else if (getValueFor("api","elgg") == "elgg") {
        url = getValueFor("serverURL","http://pockemul.dscloud.me/elgg/")+
                QString("services/api/rest/xml/?method=file.get_pml")+
                QString("&file_guid=%1").arg(id)+
                "&api_key=7118206e08fed2c5ec8c0f2db61bbbdc09ab2dfa"+
                "&auth_token="+auth_token;
        qWarning()<<url;
    }
    else if (getValueFor("api","elgg") == "parse") {
        url = auth_token;
        qWarning()<<url;
    }

    QNetworkRequest req(url);

//    qWarning()<<req.url();
    m_reply = mgr->get(req);
    if (version==0)
        connect(m_reply, SIGNAL(finished()), this, SLOT(downloadFinishedPml()));
    else if (getValueFor("api","elgg") == "parse") {
        if (type == "pml") connect(m_reply, SIGNAL(finished()), this, SLOT(downloadFinishedPml()));
        if (type == "psk") connect(m_reply, SIGNAL(finished()), this, SLOT(downloadFinishedPsk()));
    }
    else if (getValueFor("api","elgg") == "elgg")
        connect(m_reply, SIGNAL(finished()), this, SLOT(downloadFinished2()));
}

extern QString workDir;

QString Cloud::getValueFor(const QString &objectName, const QString &defaultValue)
{
//    qWarning()<<"getValue("<<objectName<<","<<defaultValue<<")";
    if (settings->value(objectName).isNull()) {
        return defaultValue;
    }
    return settings->value(objectName).toString();
}

extern int vibDelay;
void Cloud::saveValueFor(const QString &objectName, const QString &inputValue)
{
    settings->setValue(objectName, QVariant(inputValue));
//    qWarning()<<"saveValue("<<objectName<<","<<inputValue<<") in "<<settings->fileName();

    if (objectName == "trackerEnabled") trackerEnabled =  (inputValue=="on") ? true : false;
    if (objectName == "syncEnabled") syncEnabled =  (inputValue=="on") ? true : false;
    if (objectName == "soundEnabled") soundEnabled =  (inputValue=="on") ? true : false;
    if (objectName == "kbSoundEnabled") kbSoundEnabled =  (inputValue=="on") ? true : false;
    if (objectName == "hiRes") hiRes =  (inputValue=="on") ? true : false;
    if (objectName == "vibDelay") {
        vibDelay = (inputValue.toInt());
    }
    if (objectName == "flipOnEdge") flipOnEdge =  (inputValue=="on") ? true : false;
    if (objectName == "magnifyTouch") magnifyTouch =  (inputValue=="on") ? true : false;
}

QByteArray Cloud::generateKey(QString username,QString password) {
    QString key = QString("PockEmul"+username+"A"+password+"TRFGHUIJ");
//    qWarning()<<"KEY:"<<key;
    return QCryptographicHash::hash ( key.toUtf8(), QCryptographicHash::Sha1).toBase64();
}

void Cloud::saveCache(QString fileName, QString xml)
{
    QFile file(workDir+fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
             return;
    file.write(xml.toUtf8());
    file.close();
}

QString Cloud::loadCache(QString fileName)
{
    QFile file(workDir+fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return"";
    QString xml(file.readAll());
    file.close();
    return xml;
}


void Cloud::warning(QString msg) {
    ask(parent, msg, 1);
}

void CloudWindow::closeQuick()
{
    this->hide();
}

void Cloud::addShortcut(QString param) {
    m_addShortcut("test",param);
}

void Cloud::clearCache(QString s)
{
    s.replace("image://pockemulcloud/","http://");
    qWarning()<<"CloudWindow::clearCache:"<<s;
    imgprov->clearCache(s);
}

bool Cloud::isPortraitOrientation() {
#if QT_VERSION >= 0x050000

    return  (QGuiApplication::primaryScreen()->orientation() == Qt::PortraitOrientation) ||
            (QGuiApplication::primaryScreen()->orientation() == Qt::InvertedPortraitOrientation) ||
             ( (QGuiApplication::primaryScreen()->orientation() == Qt::LandscapeOrientation ||
              QGuiApplication::primaryScreen()->orientation() == Qt::InvertedLandscapeOrientation) &&
             (parent->height()>parent->width())) ;

#endif
}
