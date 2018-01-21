#include "account.h"
#include "ui_account.h"
#include "pandavoip.h"
#include <iostream>

Account::Account(QWidget *parent) : QWidget(parent), ui(new Ui::Account){
    ui->setupUi(this);

    setupAccountScreen();
}

void Account::setupAccountScreen(){
    connect(this->ui->cancel, &QPushButton::released, this, &Account::onCancelClicked);
    connect(this->ui->login, &QPushButton::released, this, &Account::onLoginClicked);
}

Account::~Account(){
    delete ui;
}

void Account::closeEvent(QCloseEvent *e){
<<<<<<< HEAD
    Q_UNUSED(e);
=======
>>>>>>> 116638c8298b6259da36c2a9b2dc1c17dd6e427c
    QApplication::quit();
}

void Account::onCancelClicked(){
    QApplication::quit();
}

void Account::onLoginClicked(){
    std::cout << "moron" << std::endl;
}
