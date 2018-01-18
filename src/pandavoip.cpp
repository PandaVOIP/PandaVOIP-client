#include "pandavoip.h"
#include "ui_pandavoip.h"
#include "settings.h"
#include <iostream>
#include <QScrollBar>
#include <QTextTable>
#include <QPushButton>

using namespace std;
PandaVOIP::PandaVOIP(QWidget *parent) : QMainWindow(parent), ui(new Ui::PandaVOIP){
    ui->setupUi(this);
    setup_PandaVOIP();
}

PandaVOIP::~PandaVOIP(){
    delete ui;
}

void PandaVOIP::setup_PandaVOIP(){
    // Voip
    this->voipController = new VoipController(this);
    this->voipController->controlConnect();


    // Should we move this to a function?
    QPushButton* general_label = new QPushButton(this->ui->voice_channels);
    general_label->setText("General");
    this->ui->verticalLayout->addWidget(general_label);

    general_list = new QListWidget(this->ui->voice_channels);
    general_list->setSelectionMode(QAbstractItemView::NoSelection);

    this->ui->verticalLayout->addWidget(general_list);

    // Login popup... duh. Should add checks if this is necessary in the future
    login_popup();

    // Connect all signals at the end
    connect(ui->message_box, &MessageBox::on_message_box_returned, this, &PandaVOIP::on_message_box_returned);
    connect(general_label, SIGNAL(clicked()), this->voipController, SLOT(connectVoice()));
    connect(ui->settings, &QPushButton::toggled, this, &PandaVOIP::on_settings_clicked);
    connect(ui->close, &QPushButton::toggled, this, &PandaVOIP::on_close_clicked);
}

void PandaVOIP::login_popup(){
    account = new Account(this->parentWidget());
    account->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    account->show();
}

void PandaVOIP::on_message_box_returned(){
    // Do nothing if there is no message to send
    if(this->ui->message_box->toPlainText().length() == 0){
        return;
    }
    QString message = this->ui->message_box->toHtml();
    // Clear message box
    this->ui->message_box->clear();

    this->voipController->send_text_message(message);

}

void PandaVOIP::on_settings_clicked(){
    settings = new Settings(this->parentWidget());
    settings->show();
}

void PandaVOIP::on_close_clicked(){
    if(account != NULL){
        account->close();
        delete account;
    }

    if(settings != NULL){
        settings->close();
        delete settings;
    }

    this->close();
}

void PandaVOIP::new_message(QString username, QString message){
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
    // remove all elements from list
    for (i = general_chat_list->count(); i > 0; i--){
        general_chat_list->takeItem(0);
    }
    // add all updated users to list
    for(i = 0; (unsigned int)i < clients.size(); i++){
        general_chat_list->addItem(clients[i]);
    }
}

void PandaVOIP::updateVoiceUsers(vector<QString> clients){
    int i;
    // remove all elements from list
    for (i = general_voice_list->count(); i > 0; i--){
        general_voice_list->takeItem(0);
    }
    // add all updated users to list
    for(i = 0; (unsigned int)i < clients.size(); i++){
        general_voice_list->addItem(clients[i]);
    }
}
