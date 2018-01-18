#ifndef MENU_H
#define MENU_H

#include <QWidget>

class Menu : public QWidget{
    Q_OBJECT
    public:
        explicit Menu(QWidget *parent = nullptr);

    protected:
        void mousePressEvent(QMouseEvent *e);
        void mouseMoveEvent(QMouseEvent *e);
        void mouseReleaseEvent(QMouseEvent *e);

    private:
        QWidget *parent;
        bool dragging = false;
        QPoint previous_pos;
};

#endif // MENU_H
