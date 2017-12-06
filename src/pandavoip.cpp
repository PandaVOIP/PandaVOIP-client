#include "pandavoip.h"
#include "ui_pandavoip.h"
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
    this->voipController = new VoipController(this);

    this->voipController->controlConnect();

    connect(ui->message_box, &MessageBox::on_message_box_returned, this, &PandaVOIP::on_message_box_returned);

    // voice area
    QPushButton* general_voice_label = new QPushButton(this->ui->voice_channels);
    general_voice_label->setText("General");
    this->ui->voiceLayout->addWidget(general_voice_label);

    connect(general_voice_label, SIGNAL(clicked()), this->voipController, SLOT(connectVoice()));

    general_voice_list = new QListWidget(this->ui->voice_channels);
    general_voice_list->setSelectionMode(QAbstractItemView::NoSelection);

    this->ui->voiceLayout->addWidget(general_voice_list);

    // text users area
    QPushButton* general_chat_label = new QPushButton(this->ui->voice_channels);
    general_chat_label->setText("General");
    this->ui->chatLayout->addWidget(general_chat_label);

    general_chat_list = new QListWidget(this->ui->voice_channels);
    general_chat_list->setSelectionMode(QAbstractItemView::NoSelection);

    this->ui->chatLayout->addWidget(general_chat_list);
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
    for (i = general_chat_list->count(); i > 0; i--){
        general_chat_list->takeItem(0);
    }
    for(i = 0; (unsigned int)i < clients.size(); i++){
        general_chat_list->addItem(clients[i]);
    }
}

void PandaVOIP::updateVoiceUsers(vector<QString> clients){
    int i;
    for (i = general_voice_list->count(); i > 0; i--){
        general_voice_list->takeItem(0);
    }
    for(i = 0; (unsigned int)i < clients.size(); i++){
        general_voice_list->addItem(clients[i]);
    }
}
