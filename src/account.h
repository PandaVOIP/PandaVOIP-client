#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QWidget>

namespace Ui {
class Account;
}

class Account : public QWidget
{
    Q_OBJECT

public:
    explicit Account(QWidget *parent = 0);
    ~Account();

private:
    Ui::Account *ui;
};

#endif // ACCOUNT_H
