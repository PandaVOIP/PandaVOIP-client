#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H
#include <QTextEdit>

class MessageBox : public QTextEdit{
    Q_OBJECT

    private:
        QWidget *parent;

    public:
        MessageBox(QWidget *parent);
        void keyPressEvent(QKeyEvent *e);

    signals:
        void onMessageBoxReturned();
};

#endif // MESSAGEBOX_H
