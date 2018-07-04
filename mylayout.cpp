#include"mylayout.h"
mylayout::mylayout(QWidget *parent)
    : QVBoxLayout(parent)
{

}
mylayout::~mylayout(){
    mylabel *item;
    for(int i=0;i<cout();i++){
        item=take(0);
        delete item;
    }
}

void mylayout::add(mylabel *item)
{
    list.append(item);
    this->addWidget(item);
}

int mylayout::cout() const
{
    return list.size();
}

void mylayout::clear()
{
    mylabel *item;
    for(int i=list.size()-1;i>0;i--){
        item=list.takeAt(i);
        this->removeWidget(item);
        item->setParent(NULL);
        delete item;
    }
    QString ns;
    ns.clear();
    list.value(0)->setText(ns);
    list.value(0)->sety(0);
}
void mylayout::insert(mylabel* l,int i){
    if(i<list.size()){//在链表内部插入
        //mylabel* m=new mylabel;
        //m=this->item(i);
        list.insert(i,l);
        this->insertWidget(i,l);
    }
    else if(i==list.size()){//在链表末尾插入
        this->add(l);
    }
}
mylabel *mylayout::item(int idx) const
{
       return list.value(idx);

}

mylabel *mylayout::take(int idx)
{
    removeWidget(list.value(idx));
    return list.takeAt(idx);

}
