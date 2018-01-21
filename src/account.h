#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QWidget>

namespace Ui {
    class Account;
}

class Account : public QWidget{
    Q_OBJECT

    private:
        Ui::Account *ui;

        void setupAccountScreen();

    protected:
        void closeEvent(QCloseEvent *e);

    public:
        explicit Account(QWidget *parent = 0);
        ~Account();

    private slots:
        void onCancelClicked();
        void onLoginClicked();
};

#endif // ACCOUNT_H
