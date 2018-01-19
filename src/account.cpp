#include "account.h"
#include "ui_account.h"
#include "pandavoip.h"
#include <iostream>

Account::Account(QWidget *parent) : QWidget(parent), ui(new Ui::Account){
    ui->setupUi(this);

    setup_AccountScreen();
}

void Account::setup_AccountScreen(){
    connect(this->ui->cancel, &QPushButton::toggled, this, &Account::on_cancel_clicked);
}

Account::~Account(){
    delete ui;
}

void Account::closeEvent(QCloseEvent *e){
    QApplication::quit();
}

void Account::on_cancel_clicked(){
    QApplication::quit();
}
