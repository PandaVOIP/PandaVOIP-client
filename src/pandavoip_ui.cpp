#include "pandavoip_ui.h"
#include "ui_pandavoip.h"
#include "settings_ui.h"
#include "servernavigation.h"
#include <iostream>
#include <QScrollBar>
#include <QTextTable>
#include <QPushButton>

using namespace std;
PandaVOIPUI::PandaVOIPUI(QWidget *parent) : QMainWindow(parent), ui(new Ui::PandaVOIPUI) {
    ui->setupUi(this);
    setupPandaVOIP();
}

PandaVOIPUI::~PandaVOIPUI() {
    delete ui;
}

void PandaVOIPUI::setupPandaVOIP() {
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

    //ServerNavigation *server_navigation = new ServerNavigation(this);
    //server_navigation->newServerNode("General", this->ui->server_navigation_layout, this->ui->server_navigation);
    //server_navigation->addUser("Slygga");

    // Connect all signals at the end
    connect(ui->message_box, &MessageBox::onMessageBoxReturned, this, &PandaVOIPUI::onMessageBoxReturned);
    connect(ui->close, &QPushButton::released, this, &PandaVOIPUI::onCloseClicked);
    connect(ui->maximize, &QPushButton::released, this, &PandaVOIPUI::onMaximizeClicked);
    connect(ui->minimize, &QPushButton::released, this, &PandaVOIPUI::onMinimizeClicked);
    connect(ui->settings, &QPushButton::released, this, &PandaVOIPUI::onSettingsClicked);
    //connect(general_label, SIGNAL(clicked()), this->voipController, SLOT(connectVoice()));
}

void PandaVOIPUI::onMessageBoxReturned() {
    // Do nothing if there is no message to send
    if (this->ui->message_box->toPlainText().length() == 0) {
        return;
    }

    QString message = this->ui->message_box->toHtml();
    // Clear message box
    this->ui->message_box->clear();

    // TODO: This needs to be updated to use the new decoupled class PandaVOIP
    //this->voip_controller->send_text_message(message);
}

void PandaVOIPUI::onSettingsClicked() {
    settings = new Settings();
    settings->setWindowFlags(Qt::WindowStaysOnTopHint);
    settings->setAttribute(Qt::WA_DeleteOnClose);
    settings->show();
}

void PandaVOIPUI::onLoginPopup() {
    account = new Account(this);
    account->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Dialog);
    account->show();
}

void PandaVOIPUI::onCloseClicked() {
    QApplication::quit();
}

void PandaVOIPUI::onMaximizeClicked() {
    if (QMainWindow::windowState() == Qt::WindowMaximized) {
        QMainWindow::showNormal();
    } else {
        QMainWindow::showMaximized();
    }
}

void PandaVOIPUI::onMinimizeClicked() {
    QMainWindow::showMinimized();
}

void PandaVOIPUI::updateChatUsers(vector<QString> clients) {
    for (int i = general_list->count(); i > 0; i--) {
        general_list->takeItem(0);
    }

    for (unsigned int i = 0; i < static_cast<unsigned int>(clients.size()); i++) {
        general_list->addItem(clients[i]);
    }
}

void PandaVOIPUI::updateVoiceUsers(vector<QString> clients) {
    for (int i = general_list->count(); i > 0; i--) {
        general_list->takeItem(0);
    }

    for (unsigned int i = 0; i < clients.size(); i++) {
        general_list->addItem(clients[i]);
    }
}

void PandaVOIPUI::onNewMessage(QString username, QString message) {
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
