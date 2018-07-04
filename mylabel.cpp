#include<QTimer>
#include<QPaintEvent>
#include<QPainter>
#include<QPen>
#include"mylabel.h"
#include<QTime>
#include<QFont>
mylabel::mylabel(QWidget*parent):QLabel(parent){
    pen.setWidth(2);
    t=0;
    y=-1;
    QPalette palette =this->palette();
    palette.setColor(this->backgroundRole(), Qt::white);
    this->setPalette(palette);
    //QFont font("楷体",15,QFont::Bold,false);
    //this->setStyleSheet("background-clor:white,color:black");
    //this->setFont(font);
    pen.setStyle(Qt::SolidLine);
    pen.setColor(Qt::blue);
    //this->setMargin(0);
    this->setAlignment(Qt::AlignTop);
    this->adjustSize();
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &mylabel::blink);
    timer->start(700);
}
mylabel::~mylabel()
{
    //timer->stop();
}
/*void mylabel::focusInEvent(QFocusEvent *ev){
    timer->start(500);
}
void mylabel::focusOutEvent(QFocusEvent *ev){
    timer->stop();
}*/
void mylabel::blink(){
    t++;
    if(t%2==0){
        pen.setColor(Qt::blue);
    }
    else{
        pen.setColor(Qt::white);
    }
    repaint();
}
void mylabel::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    QPainter painter(this);
    painter.setPen(pen);
    QFontMetrics metrics = painter.fontMetrics();
    int len;
    len=metrics.ascent();
    if(y>=0){
        painter.drawLine(y,0,y,len+len/2);
    }
}
void mylabel::sety(int yy)
{
    this->y=yy;
    repaint();
}
