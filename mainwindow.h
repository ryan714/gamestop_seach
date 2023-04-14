#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QObject>
#include <QApplication>
#include <QLineEdit>
#include <QList>
#include <libxml++/libxml++.h>
#include "downloader.h"

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent=0);
    int parseHtml(std::string);
    void goThroughNode(xmlpp::Node*);
    QString getTheNetworkData();
    QString networkAnswer;
private:
    QLabel *label;
    QPushButton *button1;
    QPushButton *button2;
    QVBoxLayout* layout;
    QLineEdit *textBox;
    QList<QString> urlList;
    QList<QString> titleList;
    QMap<QString, QString> linkMap;


private slots:
    void setText();
    void openLink();
    void getUrlData();
};

#endif // MAINWINDOW_H
