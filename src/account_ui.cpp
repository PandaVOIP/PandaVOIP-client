#include "account_ui.h"
#include "ui_account.h"
#include "pandavoip_ui.h"
#include <iostream>

Account::Account(QWidget *parent) : QWidget(parent), ui(new Ui::Account) {
    ui->setupUi(this);
    setupAccountScreen();
}

void Account::setupAccountScreen() {
    connect(this->ui->cancel, &QPushButton::released, this, &Account::onCancelClicked);
    connect(this->ui->login, &QPushButton::released, this, &Account::onLoginClicked);
}

Account::~Account() {
    delete ui;
}

void Account::onCancelClicked() {
    QApplication::quit();
}

void Account::onLoginClicked() {
    if (!this->ui->username->text().isNull()) {
        reinterpret_cast<PandaVOIPUI *>(parent())->panda_voip->setUsername(this->ui->username->text());

        close();
    }
}
