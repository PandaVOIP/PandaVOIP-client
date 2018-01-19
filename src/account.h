#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QWidget>

namespace Ui {
    class Account;
}

class Account : public QWidget{
    Q_OBJECT

    public slots:
        void on_cancel_clicked();

    public:
        explicit Account(QWidget *parent = 0);
        ~Account();

    protected:
        void closeEvent(QCloseEvent *e);

    private:
        Ui::Account *ui;

        void setup_AccountScreen();
};

#endif // ACCOUNT_H
