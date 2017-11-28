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
    connect(ui->message_box, &MessageBox::on_message_box_returned, this, &PandaVOIP::on_message_box_returned);
    QPushButton* general_label = new QPushButton(this->ui->voice_channels);
    general_label->setText("General");
    this->ui->verticalLayout->addWidget(general_label);


    QListWidget* general_list = new QListWidget(this->ui->voice_channels);
    general_list->setSelectionMode(QAbstractItemView::NoSelection);

    general_list->addItem("ritlew");
    general_list->addItem("Slygga");
    general_list->addItem("Aoushtan");

    this->ui->verticalLayout->addWidget(general_list);
}

void PandaVOIP::on_message_box_returned(){
    QString message = this->ui->message_box->toHtml();

    // Do nothing if there is no message to send
    if(this->ui->message_box->toPlainText().length() == 0)
        return;

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
    new_message->cellAt(0, 0).firstCursorPosition().insertText("Slygga:", username_format);
    new_message->cellAt(1, 0).firstCursorPosition().insertHtml(message);

    // Set scroll bar to bottom
    this->ui->messages->verticalScrollBar()->setValue(this->ui->messages->verticalScrollBar()->maximum());

    // Clear message box
    this->ui->message_box->clear();
}
