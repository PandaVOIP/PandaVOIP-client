#include "pandavoip.h"
#include "ui_pandavoip.h"
#include "settings.h"
#include "servernavigation.h"
#include <iostream>
#include <QScrollBar>
#include <QTextTable>
#include <QPushButton>

using namespace std;
PandaVOIP::PandaVOIP(QWidget *parent) : QMainWindow(parent), ui(new Ui::PandaVOIP){
    ui->setupUi(this);
    setupPandaVOIP();
}

PandaVOIP::~PandaVOIP(){
    delete ui;
}

void PandaVOIP::setupPandaVOIP(){
    // Voip
    this->voipController = new VoipController(this);
    //this->voipController->controlConnect();

    // Should we move this to a function?
    /*
    QPushButton* general_label = new QPushButton(this->ui->voice_channels);
    general_label->setText("General");
    this->ui->verticalLayout->addWidget(general_label);

    general_list = new QListWidget(this->ui->voice_channels);
    general_list->setSelectionMode(QAbstractItemView::NoSelection);

    this->ui->verticalLayout->addWidget(general_list);
    */

    // Login popup... duh. Should add checks if this is necessary in the future
    //loginPopup();

    // Fix vertical alignment
    this->ui->server_navigation_layout->setAlignment(Qt::AlignTop);

    ServerNavigation *server_navigation = new ServerNavigation(this);
    server_navigation->newServerNode("General", this->ui->server_navigation_layout, this->ui->server_navigation);
    server_navigation->addUser("Slygga");

    // Connect all signals at the end
    connect(ui->message_box, &MessageBox::onMessageBoxReturned, this, &PandaVOIP::onMessageBoxReturned);
    connect(ui->close, &QPushButton::released, this, &PandaVOIP::onCloseClicked);
    connect(ui->maximize, &QPushButton::released, this, &PandaVOIP::onMaximizeClicked);
    connect(ui->minimize, &QPushButton::released, this, &PandaVOIP::onMinimizeClicked);
    connect(ui->settings, &QPushButton::released, this, &PandaVOIP::onSettingsClicked);
    //connect(general_label, SIGNAL(clicked()), this->voipController, SLOT(connectVoice()));
}

void PandaVOIP::loginPopup(){
    account = new Account(this);
    account->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Dialog);
    account->show();
}

void PandaVOIP::onMessageBoxReturned(){
    // Do nothing if there is no message to send
    if (this->ui->message_box->toPlainText().length() == 0)
        return;

    QString message = this->ui->message_box->toHtml();
    // Clear message box
    this->ui->message_box->clear();

    this->voipController->send_text_message(message);
}

void PandaVOIP::onSettingsClicked(){
    settings = new Settings(NULL);
    settings->setWindowFlags(Qt::WindowStaysOnTopHint);
    settings->setAttribute(Qt::WA_DeleteOnClose);
    settings->show();
}

void PandaVOIP::onCloseClicked(){
    QApplication::quit();
}

void PandaVOIP::onMaximizeClicked(){
    if (QMainWindow::windowState() == Qt::WindowMaximized)
        QMainWindow::showNormal();
    else
        QMainWindow::showMaximized();
}

void PandaVOIP::onMinimizeClicked(){
    QMainWindow::showMinimized();
}

void PandaVOIP::newMessage(QString username, QString message){
    // Get current position of the messages container
    QTextCursor cursor = this->ui->messages->textCursor();
    cursor.movePosition(QTextCursor::End);

    QTextTableFormat table_format;
    table_format.setBorder(0);
    table_format.setBottomMargin(10);

    QTextCharFormat username_format;
    username_format.setForeground(QBrush(QColor(Qt::white)));

    QTextCharFormat message_format;
    message_format.setForeground(QBrush(QColor(200, 200, 200)));

    // Insert a new table to the end of the messages container
    QTextTable *new_message = cursor.insertTable(2, 1, table_format);
    new_message->cellAt(0, 0).firstCursorPosition().insertText(username + ":", username_format);
    new_message->cellAt(1, 0).firstCursorPosition().insertHtml(message);

    // Set scroll bar to bottom
    this->ui->messages->verticalScrollBar()->setValue(this->ui->messages->verticalScrollBar()->maximum());
}

void PandaVOIP::updateChatUsers(vector<QString> clients){
    int i;
    for (i = general_list->count(); i > 0; i--)
        general_list->takeItem(0);
    for (i = 0; (unsigned int)i < clients.size(); i++)
        general_list->addItem(clients[i]);
}

void PandaVOIP::updateVoiceUsers(vector<QString> clients){
    int i;
    for (i = general_list->count(); i > 0; i--)
        general_list->takeItem(0);
    for (i = 0; (unsigned int)i < clients.size(); i++)
        general_list->addItem(clients[i]);
}
