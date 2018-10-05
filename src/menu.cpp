#include "menu.h"
#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOption>

Menu::Menu(QWidget *parent) : QWidget(parent) {
    this->parent = parent;
}

void Menu::paintEvent(QPaintEvent *) {
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void Menu::mousePressEvent(QMouseEvent *e) {
    dragging = true;
    previous_pos = e->globalPos();
}

void Menu::mouseMoveEvent(QMouseEvent *e) {
    if (dragging) {
        // Calculate the difference between the window's last position and where the mouse is now
        QPoint delta = e->globalPos() - previous_pos;

        // Move the window to that position
        this->window()->move(this->window()->x() + delta.x(), this->window()->y() + delta.y());

        // Set the window's new position
        previous_pos = e->globalPos();
    }
}

void Menu::mouseReleaseEvent(QMouseEvent *e) {
    Q_UNUSED(e);
    dragging = false;
}
