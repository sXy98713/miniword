#ifndef MYLABEL_H
#define MYLABEL_H
#include<QLabel>
#include<QWidget>
#include<QString>
#include<QTimer>
#include<Qpen>
#include<QLabel>
class mylabel:public QLabel
{
private:
   // int x;
    int y;
   int t;
public:
    explicit mylabel(QWidget*parent=0);
    ~mylabel();
    void sety(int yy);
//protected slots:

private:
    //QString s;
    void blink();
    QPen pen;
    QTimer *timer;
    //void mousePressEvent(QMouseEvent*ev);
    void paintEvent(QPaintEvent *event);

};
#endif // MYLABEL_H
