#ifndef MESSAGEBOXUI_H
#define MESSAGEBOXUI_H
#include <QTextEdit>

class MessageBox : public QTextEdit {
    Q_OBJECT

    private:
        QWidget *parent;

    public:
        MessageBox(QWidget *parent);
        void keyPressEvent(QKeyEvent *e);

    signals:
        void onMessageBoxReturned();
};

#endif // MESSAGEBOXUI_H
