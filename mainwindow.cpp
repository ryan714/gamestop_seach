#include "mainwindow.h"
#include "downloader.h"
#include <QApplication>
#include <QLineEdit>
#include <QList>
#include <QCompleter>
#include <QDesktopServices>
#include <QMap>
#include <libxml++/libxml++.h>
#include <iostream>
#include <cstdlib>

#include <QTimer>

MainWindow::MainWindow(QWidget *parent): QWidget(parent)
{

    label = new QLabel("<b><center>Search Gamestop</center></b>");
    button1 = new QPushButton ("Close");
    button2 = new QPushButton ("Open Link");
    textBox = new QLineEdit;
    textBox->setPlaceholderText("Enter Search Term");
    layout = new QVBoxLayout(this);
    layout->addWidget(textBox);
    layout->addWidget(button2);
    layout->addWidget(button1);
    layout->addWidget(label);

    setLayout(layout);

    // Signals and Slots

    connect( button1, SIGNAL( clicked() ),
    qApp, SLOT( quit() ) );

    connect(button2, SIGNAL(clicked() ),
    this, SLOT(openLink()));

    connect(textBox, SIGNAL(returnPressed() ),
    this, SLOT(openLink()));

    connect(textBox, SIGNAL(textEdited(QString)),
    this, SLOT(getUrlData()));
}

void MainWindow::getUrlData() {

    auto downloaderObject = new Downloader(qApp);
    downloaderObject->TestConnection("https://www.gamestop.com/on/demandware.store/Sites-gamestop-us-Site/default/SearchServices-GetSuggestions?q=" + textBox->text() + "&headerRedesign=true&hitSuggestion=true");

    // Delay 300ms for network reply
    QEventLoop loop;
    QTimer::singleShot(300, &loop, &QEventLoop::quit);
    loop.exec();

    networkAnswer = downloaderObject->getNetworkData();
    setText();
}

QString MainWindow::getTheNetworkData() {
    return networkAnswer;
}

void MainWindow::openLink() {

    if (linkMap.contains(textBox->text())) {
        QDesktopServices::openUrl(QUrl(linkMap.value(textBox->text())));
    }
}

void MainWindow::setText() {

    parseHtml(this->getTheNetworkData().toStdString());

    QList<QString>::iterator it;
    QString populateLinks;
    QString titleString;
    for (it = urlList.begin(); it != urlList.end(); it++) {

        titleString = *it;
        titleString.replace("/search/?q=", "").replace("&suggestionV=2", "").replace("%20", " ").replace("%3A", ":");
        titleList.append(titleString);
        linkMap.insert(titleString, "http://gamestop.com" + *it);
    }

    QCompleter *completer = new QCompleter(titleList, this);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    textBox->setCompleter(completer);
}

int MainWindow::parseHtml(std::string htmlPath) {

    try
    {
        xmlpp::DomParser parser;
        parser.parse_memory(htmlPath);

        if(parser)
        {
            //Walk the tree:
            xmlpp::Node* pNode = parser.get_document()->get_root_node();
            goThroughNode(pNode);
        }
    }
    catch(const std::exception& ex)
    {
        std::cerr << "Exception caught: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void MainWindow::goThroughNode(xmlpp::Node* node)
{

    std::cout << std::endl; //Separate nodes by an empty line.

    const xmlpp::ContentNode* nodeContent = dynamic_cast<const xmlpp::ContentNode*>(node);
    const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(node);

    if(nodeText && nodeText->is_white_space()) //Let's ignore the indenting
        return;

    if(const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(node))
    {
        // search for href attributes:
        const auto& attributes = nodeElement->get_attributes();
        for(auto iter  = attributes.begin(); iter != attributes.end(); ++iter)
        {
            auto attribute = *iter;

            if (attribute->get_name() == "href") {
                urlList.append(QString::fromStdString(attribute->get_value()));
            }
        }
    }
    if(!nodeContent)
    {
        //Recurse through child nodes:
        xmlpp::Node::NodeList list = node->get_children();
        for(xmlpp::Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter)
        {
            // Recursive
            goThroughNode(*iter);
        }
    }
}
