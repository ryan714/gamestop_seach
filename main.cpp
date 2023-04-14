#include "mainwindow.h"
#include "downloader.h"
#include <QApplication>

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    MainWindow* window = new MainWindow;
    window->setWindowTitle("Search GameStop");
    window->resize(400, window->height());
    window->show();
    return app.exec();
}
