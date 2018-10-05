#ifndef MENU_H
#define MENU_H

#include <QWidget>

class Menu : public QWidget {
    Q_OBJECT

    private:
        QWidget *parent;
        bool dragging = false;
        QPoint previous_pos;

    protected:
        void paintEvent(QPaintEvent *);
        void mousePressEvent(QMouseEvent *e);
        void mouseMoveEvent(QMouseEvent *e);
        void mouseReleaseEvent(QMouseEvent *e);

    public:
        explicit Menu(QWidget *parent = nullptr);
};

#endif // MENU_H
