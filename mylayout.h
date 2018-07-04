#ifndef MYLAYOUT_H
#define MYLAYOUT_H
#include<QVBoxLayout>
#include<QWidget>
#include<QList>
#include<QLabel>
#include<mylabel.h>
class mylayout:public QVBoxLayout
{
public:
    explicit mylayout(QWidget*parent=0);
    ~mylayout();
    void add(mylabel *item);
    int cout() const;
    void clear();
    mylabel *item(int) const;
    mylabel *take(int);
    void insert(mylabel *item, int i);
private:
    QList<mylabel*> list;
};
        #endif // MYLAYOUT_H
