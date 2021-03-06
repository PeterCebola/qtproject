#ifndef MOUSE_QLABEL_H
#define MOUSE_QLABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QEvent>
#include <QDebug>

class mouse_qlabel : public QLabel
{
    Q_OBJECT
public:
    explicit mouse_qlabel(QWidget *parent = 0);

    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void leaveEvent(QEvent *);

    int x,y;

signals:
    void Mouse_Pressed();
    void Mouse_Pos();
    void Mouse_Left();


public slots:

};

#endif // MOUSE_QLABEL_H
