#ifndef ACCOUNTUI_H
#define ACCOUNTUI_H
#include <QWidget>

namespace Ui {
    class Account;
}

class Account : public QWidget {
    Q_OBJECT

    private:
        Ui::Account *ui;

        void setupAccountScreen();

    public:
        explicit Account(QWidget *parent);
        ~Account();

    private slots:
        void onCancelClicked();
        void onLoginClicked();
};

#endif // ACCOUNTUI_H
