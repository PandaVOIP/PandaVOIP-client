#include "pandavoip_ui.h"
#include "ui_pandavoip.h"
#include "settings_ui.h"
#include <iostream>
#include <QScrollBar>
#include <QTextTable>
#include <QPushButton>
#include <QTimer>

using namespace std;
PandaVOIPUI::PandaVOIPUI(QWidget *parent) : QMainWindow(parent), ui(new Ui::PandaVOIPUI) {
    ui->setupUi(this);
    setupPandaVOIP();
}

PandaVOIPUI::~PandaVOIPUI() {
    delete ui;
}

void PandaVOIPUI::setupPandaVOIP() {
    this->panda_voip = new PandaVOIP();

    // Fix vertical alignment for dynamic server navigation
    this->ui->channel_layout->setAlignment(Qt::AlignTop);

    createChannelWidget("General");

    // Connect all signals at the end
    connect(ui->message_box, &MessageBox::onMessageBoxReturned, this, &PandaVOIPUI::onMessageBoxReturned);
    connect(ui->close, &QPushButton::released, this, &PandaVOIPUI::onCloseClicked);
    connect(ui->maximize, &QPushButton::released, this, &PandaVOIPUI::onMaximizeClicked);
    connect(ui->minimize, &QPushButton::released, this, &PandaVOIPUI::onMinimizeClicked);
    connect(ui->settings, &QPushButton::released, this, &PandaVOIPUI::onSettingsClicked);
    connect(this->panda_voip, &PandaVOIP::openLoginPopup, this, &PandaVOIPUI::openLoginPopup);
    //connect(general_label, SIGNAL(clicked()), this->voipController, SLOT(connectVoice()));

    // Run core logic
    this->panda_voip->init();
}

void PandaVOIPUI::onMessageBoxReturned() {
    QString message;

    // Do nothing if there is no message to send
    if (this->ui->message_box->toPlainText().length() == 0) {
        return;
    }

    message = this->ui->message_box->toHtml();

    if (this->panda_voip->voip_controller->send_text_message(message)) {
        // Clear message box
        this->ui->message_box->clear();
    }
}

void PandaVOIPUI::onSettingsClicked() {
    this->settings = new Settings();
    this->settings->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->settings->setAttribute(Qt::WA_DeleteOnClose);
    this->settings->show();
}

void PandaVOIPUI::openLoginPopup() {
    this->account = new Account(this);
    this->account->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Dialog);
    this->account->show();
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

void PandaVOIPUI::createChannelWidget(QString channel_name) {
    //parent_layout->setAlignment(Qt::AlignTop);

    QWidget *widget = new QWidget(this->ui->channel_navigation);
    widget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    QVBoxLayout *layout = new QVBoxLayout(widget);

    QPushButton *join_channel = new QPushButton(channel_name, widget);
    layout->addWidget(join_channel);

    this->ui->channel_layout->addWidget(widget);

    this->channels[channel_name].widget = widget;
    this->channels[channel_name].layout = layout;
}

void PandaVOIPUI::createUserList(QString channel_name) {
    QListWidget *user_list = new QListWidget(this->channels[channel_name].widget);
    user_list->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);

    this->channels[channel_name].layout->addWidget(user_list);
    this->channels[channel_name].user_list = user_list;
}

void PandaVOIPUI::addUser(QString channel_name, QString user_name) {
    if (!this->channels[channel_name].user_list || this->channels[channel_name].user_list->count() == 0) {
        createUserList(channel_name);
    }

    this->channels[channel_name].user_list->addItem(user_name);
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
