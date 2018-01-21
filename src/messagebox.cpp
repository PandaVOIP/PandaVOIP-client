#include "messagebox.h"
#include <QTextEdit>
#include <QKeyEvent>

MessageBox::MessageBox(QWidget *parent = 0) : QTextEdit(parent){
    this->parent = parent;
}

// hook for QT to tell us when the return key is pressed
void MessageBox::keyPressEvent(QKeyEvent *e){
    if(e->key() == Qt::Key_Return){
        emit on_message_box_returned();
    }
    else{
        QTextEdit::keyPressEvent(e);
    }
}
