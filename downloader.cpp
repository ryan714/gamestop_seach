#include "downloader.h"
#include "mainwindow.h"
#include <QApplication>

Downloader::Downloader(QApplication *app)  {
    manager = new QNetworkAccessManager(app);
}

void Downloader::TestConnection(QString theUrl) const {

    auto status = connect(manager, &QNetworkAccessManager::finished, this, &Downloader::ReplyFinished);
    manager->get(QNetworkRequest(QUrl(theUrl)));
}

void Downloader::ReplyFinished(QNetworkReply *reply) {

    theNetworkData = reply->readAll();
}

QString Downloader::getNetworkData() {

    return theNetworkData;
}


