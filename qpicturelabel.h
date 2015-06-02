#ifndef QPICTURELABEL_H
#define QPICTURELABEL_H

#include <QLabel>
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QMouseEvent>
#include <QEvent>
#include <QDebug>

class QPictureLabel : public QLabel
{
    Q_OBJECT
private:
    QPixmap _qpSource; //preserve the original, so multiple resize events won't break the quality
    QPixmap _qpCurrent;

    void displayImage();

public:
    explicit QPictureLabel(QWidget *parent = 0);

    void setPixmap(QPixmap picture);
    void paintEvent(QPaintEvent *event);

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

#endif // QPICTURELABEL_H
