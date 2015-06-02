#include "mouse_qlabel.h"

mouse_qlabel::mouse_qlabel(QWidget *parent) :
    QLabel(parent)
{
}

void mouse_qlabel::mouseMoveEvent(QMouseEvent *ev)
{
    this->x = ev->x();
    this->y = ev->y();
    emit Mouse_Pos();

}

void mouse_qlabel::mousePressEvent(QMouseEvent *ev)
{
    emit Mouse_Pressed();
}

void mouse_qlabel::leaveEvent(QEvent *)
{
    emit Mouse_Left();
}
