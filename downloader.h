#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QApplication>

class Downloader : public QObject {
Q_OBJECT
public:
    explicit Downloader(QApplication *application);
    void TestConnection(QString) const;
    void ReplyFinished(QNetworkReply *reply);
    QString getNetworkData();
    QNetworkAccessManager *manager;
private:
    QString theNetworkData;

};

#endif // DOWNLOADER_H
