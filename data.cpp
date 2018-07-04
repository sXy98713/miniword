#include"data.h"
#include<QFile>
#include<QTextStream>
#include<QString>
datacontrol::datacontrol(){

    //Node n;
    //data.push_back(n);
}
void datacontrol::data_delete(Heap *temp, list<Node>::iterator currentnode,int position,int num,int i) {//num表示要删除的字符数，position 表示删除的起始位置,*temp表示删除的起始节点 i表示起始节点的序数；
																					  																				 
	int j = i;
    int l = 0,k=0;

    l=position;
    if (l==position&&temp->ch[l] == '\n')
    {
        currentnode++;
        int kchar=currentnode->charnum;
        currentnode--;
        currentnode->charnum=currentnode->charnum+kchar-1;
        currentnode->heapnum=(currentnode->charnum-1)/100+1;
        currentnode++;
        Heap *t;
        t = currentnode->firstheap;
        for (int j = 0; j < currentnode->charnum; j++)
        {
            if (j > 0 && j % 100 == 0)
            {
                t = t->next;
            }
            if (l == 100)
            {
                l = 0;
                Heap *n;
                n = new Heap;
                temp->next = n;
                temp = temp->next;
            }
            temp->ch[l] = t->ch[j % 100];
            l++;
        }
        currentnode = data.erase(currentnode);
    }
    else
    {
        l=0;
        while (j <= currentnode->heapnum) {
            if (j== i && j < currentnode->heapnum) {
                for (l = position; l< 100-num; l++) {
                    temp->ch[l] = temp->ch[l + num];

            }
                for(k=0;k<100&&l<100;l++,k++)
                temp->ch[l] = temp->next->ch[k];
            }
            else if (j == i && j == currentnode->heapnum) {//此时为最后一个节点
                for (l= position; l < 100-num; l++) {
                    temp->ch[l] = temp->ch[l + num];
                }
                for (; l < 100; l++) {
                    temp->ch[l] ='\0';
                }
            }
            else if (j> i && j== currentnode->charnum/100) {
                for (l = 0; l < 100-k; j++) {
                    temp->ch[l] = temp->ch[l+num];
                }
                if(l==0){
                    delete temp->next;
                }
                else{
                    for (; l < 100; l++) {
                        temp->ch[l] = '\0';
                    }
                }
            }
            else {
                for (l = 0; l < 100 - k; j++) {
                    temp->ch[l] = temp->ch[l + num];
                }
                for (k = 0; k<100 && l<100; l++, k++)
                    temp->ch[l] = temp->next->ch[k];
            }
            j++;
            temp = temp->next;
        }
    }

	currentnode->charnum -= num;
}
void data_connect(Heap *a, Heap *b,int n, int &hnum ) {//连接a,b Heap,n为a的连接起始序号，hnum为b以及b之后所有的Heap的Charnum.
	//因为是上一行最后的Heap与当前行第一个heap的连接，因此上一行的最后一个字符一定为'\n'，故应从该字符起进行修改与连接
    /*if (n == 99) {
		a->next = b;
		return;
    }*/
    //else {
		int m=0;
		a->next = b;
		while (1) {
			if (n == 100) {
				a = a->next;
				n = 0;
			}
			if (m == 100) {
				b = b->next;
				m = 0;
			}
			if (b->ch[m] == '\n') {
				a->ch[n] = '\n';
                /*if (n>m)
                {
                    hnum--;
                }
				n++;
				for (; n < 100; n++)
				{
					a->ch[n] = '\0';
                }*/
                a->next = NULL;
				break;
			}
            a->ch[n] = b->ch[m];
            n++;
            m++;
		}
    //}
}
void datacontrol:: backspace(cpos &c){
        int i,num,hnum;
        Heap *tmpa;
        curnode=data.begin();
        for(i=0;i<c.x;++i,++curnode);

        tmp=curnode->firstheap;
		hnum = curnode->heapnum;
		num = curnode->charnum;

        for(i=0;i<c.y/100;++i,tmp=tmp->next);

        if(c.y==0){//行删除
            if(c.x==0) return;//在第一行无法删除
            else{
                tmp=curnode->firstheap;
                curnode=data.erase(curnode);//删除当前行
                --curnode;//得到上一行的迭代器
                tmpa=curnode->firstheap;
                for(i=0;i<curnode->heapnum-1;++i,tmpa=tmpa->next);//得到上一行最后一个Heap指针

                if(tmpa->ch[0]=='\n'){//若上一行最后一个字符在末堆的第一位，则将当前行的第一个堆的值赋给上一行的末堆，将剩余堆连接到该末堆
                    tmpa->next=tmp->next;
                    for(int x=0;x<100;x++){
                        tmpa->ch[x]=tmp->ch[x];
                    }
                }
                else{
                    if(tmp){
                        link(tmpa,tmp,(curnode->charnum-1)%100-1,0);
                    }
                }
                    //else data_connect(tmpa,tmp,(curnode->charnum-num)%100,hnum);//连接上一行最后一个Heap指针与当前第一个Heap指针
                //上一行的最后一个字符必为‘'\n'
            }
            curnode->charnum=curnode->charnum+num-1;
            curnode->heapnum=(curnode->charnum-1)/100+1;
            c.x--;
            c.y=curnode->charnum-num;
        }
        else{//字符删除
            if((hnum==1)&&(num==1)){
                curnode->firstheap=NULL;
                curnode->heapnum=0;
                curnode->charnum=0;
                if(c.x==0){
                    c.y=0;
                    return;
                }
                c.y--;
                return;
            }
            data_delete(tmp,curnode,c.y%100-1,1,(c.y-1)/100+1);
            c.y--;
            return;
        }
    }
void datacontrol::del(cpos &c){
        curnode=data.begin();
		int i;
        for(i=0;i<c.x;++i,++curnode);
        tmp=curnode->firstheap;
         for(i=0;i<c.y/100;++i,tmp=tmp->next);
        if(c.y<=curnode->charnum-1){//若光标不在此行的最后一个字符后（不包括换行符）
            data_delete(tmp,curnode,c.y%100,1,c.y/100+1);
        }
        else return;
       
    }
void datacontrol::insert(cpos &c,QChar ch1){
    if(data.empty()){//若内部数据为空
        Node n;
        Heap *h;
        h=new Heap;
        h->ch[0]=ch1;
        n.firstheap=h;
        n.charnum=1;
        n.heapnum=1;
        data.push_back(n);
        if(ch1=='\n'){//若插入的为换行符
            Node n1;
            data.push_back(n1);
            c.x++;
            c.y=0;
        }
        else{
            c.y++;
        }
    }
    else{
        int i=0,j=0,k=0,g=0;
        QChar ch2;
        curnode=data.begin();
        for(i=0;i<c.x;++i,++curnode);
        tmp=curnode->firstheap;
        for(i=0;i<c.y/100;++i,tmp=tmp->next);
        if(ch1=='\n'){//行插入
            Node newnode;
            Heap* tmp1=tmp;
            if(c.y==getlinelength(c.x)&&c.x<data.size()-1){//光标在当前行末，但不在文末
				Heap *t = new Heap;
                if(getlinelength(c.x)==curnode->charnum-1){//当前行最后一位为‘\n'
                    t->ch[0]='\n';
                }
                else{//当前行最后一个字符不是'\n'
                    if((curnode->charnum+1)%100!=0){//在当前节点Heap内
                        tmp->ch[(curnode->charnum+1)%100]='\n';
                    }
                    else{
                        tmp1=new Heap;
                        tmp1->ch[0]='\n';
                        tmp->next=tmp1;
                    }
                }
				newnode.firstheap = t;
                newnode.charnum=1;
                newnode.heapnum=1;
            }
            /*在文末*/

            else if(c.x==data.size()-1&&c.y==getlinelength(c.x)){
                if((curnode->charnum==0)&&curnode->heapnum==0){//若当前行为空行
                    tmp=new Heap;
                    tmp->ch[0]='\n';
                    curnode->firstheap=tmp;
                }
                tmp=curnode->firstheap;
                for(i=0;i<c.y/100;++i,tmp=tmp->next);
                tmp->ch[(curnode->charnum)%100]='\n';//在当前行末添加换行符
                Node n;
                data.push_back(n);
                c.x++;
                c.y=0;
                return;
            }
            else {//不在行末,那么必不在文末
                tmp=curnode->firstheap;
                for(i=0;i<c.y/100;++i,tmp=tmp->next);
                newnode.charnum = curnode->charnum - c.y;
                if (c.y>0&&c.y % 100 == 0) {//在节点末尾换行
					newnode.firstheap = tmp->next;
					Heap *t = new Heap;
					t->ch[0] = '\n';
                    tmp->next = t;
					newnode.heapnum = curnode->heapnum - c.y / 100;
					newnode.charnum = curnode->charnum - c.y;
					curnode->charnum = c.y + 1;
                    curnode->heapnum = c.y / 100 + 1;
				}
                if(c.y==0)         //在行首换行
                {
                    newnode.firstheap = tmp;
                    Heap *t = new Heap;
                    t->ch[0] = '\n';
                    curnode->firstheap = t;
                    newnode.heapnum = curnode->heapnum - c.y / 100;
                    newnode.charnum = curnode->charnum - c.y;
                    curnode->charnum = c.y + 1;
                    curnode->heapnum = c.y / 100 + 1;
                }
				else {//不在节点末尾换行
					newnode.charnum = curnode->charnum - c.y;
					Heap *t = new Heap;
                    for (j = 0; j <= newnode.charnum / 100; j++) {//创建Heap链表
						if (j == 0) {
							newnode.firstheap = t;
						}
						else {
							t->next = new heap;
							t = t->next;
						}
                    }
                    newnode.heapnum = j;
                    curnode->charnum = c.y+1;
                    curnode->heapnum = c.y / 100+1;
                    k = c.y % 100;
					t = newnode.firstheap;
                    i=0;
                    j = 0;
					while ((i < newnode.charnum) && t) {
						t->ch[j] = tmp->ch[k];
						if (j == 99) {
                            j = -1;
							t = t->next;
						}
						if (k == 99) {
                            k = -1;
                            if(tmp->next!=NULL)
							tmp = tmp->next;
                            else break;
						}
                        j++, k++;
                        i++;
					}
					tmp1->ch[c.y % 100] = '\n';
					for (int l = c.y % 100 + 1; l < 100; l++) {
						tmp1->ch[l] = '\0';
					}
					delete tmp1->next;
				}
			}
            curnode++;
			data.insert(curnode, newnode);
            c.x++;
            c.y=0;
         }
         else{//字符插入
			 i = c.y % 100;
			 g = c.y;
             if(curnode->charnum%100==0)
             {
                 if(curnode->charnum>0){//在最后一个堆后创建新堆
                    Heap *e=new Heap;
                    e=tmp;
                    int g=c.y/100+1;
                    for(;g<curnode->heapnum;e=e->next,g++);
                    Heap *f=new Heap;
                    tmp->next=f;
                    curnode->heapnum++;
                 }
                 else{//说明当前无堆
                     Heap *e=new Heap;
                     curnode->firstheap=e;
                     curnode->heapnum++;
                 }
             }
             if(g==0&&curnode->charnum==0){
                 curnode->firstheap->ch[0]=ch1;
             }
             else{
                while(g<=curnode->charnum){
                    if(i==100){
                        i=0;
                        tmp=tmp->next;
                    }
                    ch2=tmp->ch[i];
                    tmp->ch[i]=ch1;
                    ch1=ch2;
                    i++;
                    g++;
                    //}
                   //tmp->ch[i]='\n';
                }
             }
             curnode->charnum++;
             c.y++;
         }
    }
}
bool datacontrol::replace(cpos &c,int len,QString str){ //len 为被替换字符串长度  str为输入串
        curnode=data.begin();
        int i,k;
        for(i=0;i<c.x;++i,++curnode);
        tmp=curnode->firstheap;
        for(i=0;i<c.y/100;++i,tmp=tmp->next);
        if(str.length()==len){
            for (i = c.y % 100,k=0;k<str.length(); i++,k++) {
                tmp->ch[i]=str[k];
                if (i == 99) {
                    i = -1;
                    tmp=tmp->next;
                }
            }
        }
        else if(str.length()<len){
            for (i = c.y % 100,k=0;k<str.length(); i++,k++) {
                tmp->ch[i]=str[k];
                if (i == 99) {
                    i = 0;
                    tmp=tmp->next;
                }
            }
            data_delete(tmp,curnode,i,len-k,c.y/100+1);
        }
        else{
            int u;
            cpos c1;
            c1.x=c.x;
            c1.y=c.y;
            Heap *tmp1;
            if(curnode->charnum%100+str.length()-len>100)
            {
                for(tmp1=curnode->firstheap;;tmp1=tmp1->next)
                {
                    if(tmp1->next==NULL)
                    {
                        break;
                    }
                }
                u=(curnode->charnum%100+str.length()-len-1)/100;
                for(int y=0;y<u;y++)
                {
                    Heap *g;
                    tmp1->next=g;
                }
            }
            for(i=c.y%100,k=0;k<str.length();i++,k++){
                if(k<len){
                    tmp->ch[i]=str[k];
                    c1.y++;
                }
                else{
                  insert(c1,str[k]);
                }
                if(i==99){
                    i=-1;
                    tmp=tmp->next;
                }
            }
        }
    }
void datacontrol::search(cpos &c,QString str){
    stack<cpos> s1;
    curnode=data.begin();
    int i,j,k;
    while(!s.empty()){
        s.pop();
    }
    for(i=0;i<c.x;++i,++curnode);
    tmp=curnode->firstheap;
    for(i=0;i<c.y/100;++i,tmp=tmp->next);           //tmp指向光标所在的堆
    j = 0;
    i = c.y % 100;
    int flag=c.y;
    for(;flag<curnode->charnum;flag++)              //当前单行的搜索
    {
        if(flag+str.length()<curnode->charnum)
        {
            if(tmp->ch[i]==str[j])
            {
                if(str.length()==1)
                {
                    cpos c1;
                    c1.y=flag;
                    c1.x=c.x;
                    s1.push(c1);
                    j=0;
                    i++;
                }
                else
                {
                    i++,j++;
                    while(tmp->ch[i]==str[j])
                    {
                        i++;
                        j++;
                        if(i==100)
                        {
                            if(tmp->next==NULL)
                            {
                                break;
                            }
                            else
                            {
                                i=0;
                                tmp=tmp->next;
                            }
                        }
                        if(j==str.length())
                        {
                            cpos c1;
                            c1.y=flag;
                            c1.x=c.x;
                            s1.push(c1);
                            j=0;
                        }
                    }
                    if(j!=0)      //匹配失败
                    {
                        j=0;
                    }
                    else           //匹配成功
                    {
                        flag=flag+str.length()-1;
                    }
                }
            }
            else
            {
                i++;
                j=0;
                if(i==100){
                    if(tmp->next==NULL)
                    {
                        break;
                    }
                    else
                    {
                        i=0;
                        tmp=tmp->next;
                    }
                }
            }
        }
        else
        {
            break;
        }
    }
    for(int x=c.x+1;x<data.size();x++){//之后所有行的搜索
        curnode++;
        tmp=curnode->firstheap;
        j=0,i=0;
        for(k=0;k<curnode->charnum;k++)
        {
            if(k+str.length()<curnode->charnum)
            {
                if(tmp->ch[i]==str[j])
                {
                    if(str.length()==1)
                    {
                        cpos c1;
                        c1.y=k;
                        c1.x=x;
                        s1.push(c1);
                        j=0;
                        i++;
                    }
                    else
                    {
                        i++,j++;
                        while(tmp->ch[i]==str[j])
                        {
                            i++;
                            j++;
                            if(i==100)
                            {
                                if(tmp->next==NULL)
                                {
                                    break;
                                }
                                else
                                {
                                    i=0;
                                    tmp=tmp->next;
                                }
                            }
                            if(j==str.length())
                            {
                                cpos c1;
                                c1.y=k;
                                c1.x=x;
                                s1.push(c1);
                                j=0;
                            }
                        }
                        if(j!=0)      //匹配失败
                        {
                            j=0;
                        }
                        else           //匹配成功
                        {
                            k=k+str.length()-1;
                        }
                    }
                }
                else
                {
                    i++;
                    j=0;
                }
            }
            else
            {
                break;
            }
        }
    }
   while(!s1.empty())
   {
       s.push(s1.top());
       s1.pop();
   }
}
int datacontrol::getlinelength(int j){
    curnode=data.begin();
    for(int i=0;i<j;i++){
        curnode++;
    }
    tmp=curnode->firstheap;
    for(int i=0;i<curnode->heapnum-1;++i,tmp=tmp->next);
    if(tmp==NULL) return 0;
    if((tmp->ch[(curnode->charnum)%100-1])=='\n')
    return curnode->charnum-1;//不包含换行符
    else return curnode->charnum;
}
QString datacontrol::getqstring(int j){
    curnode=data.begin();
    QString st;
    st.clear();
    //int k;
    Heap* t=new Heap;
    for(int i=1;i<=j;++i,curnode++);
    t=curnode->firstheap;
    int len;
    for(int i=1;i<=curnode->heapnum;i++){
        /*k=0;
        while((t->ch[k]!='\n')&&k<100){
           st.push_back(t->ch[k]);
           k++;
        }*/
        if(i<curnode->heapnum){
            len=100;
        }
        else len=curnode->charnum%100;
        if(len>0){
            QString buf(t->ch,len);
            st+=buf;
            buf.clear();
        }
        else {
            QString buf;
            buf.clear();
            st+=buf;
        }
        t=t->next;
    }
    return st;
}
void datacontrol::save(QString path){
    QFile file(path);
        if (!file.open(QFile::WriteOnly | QFile::Text)){
            //qDebug() << "Data::save::file open fail";
            return;
        }
        int len;
        QTextStream out(&file);
        curnode=data.begin();
        for(int i=0;i<data.size();i++){
            Heap* t=new Heap;
            t=curnode->firstheap;
            for(int j=0;j<curnode->heapnum;j++){
                if(j<curnode->heapnum-1){
                    len=100;
                }
                else len=curnode->charnum%100;
                QString buf(t->ch,len);
                out<<buf;
                buf.clear();
            }
            curnode++;
        }
        file.close();
}
void datacontrol::read(QString path){
    data.clear();
    QFile file(path);
        if (!file.open(QFile::ReadOnly | QFile::Text)){
            //qDebug() << "Data::read::file open fail";
            return;
        }
        QTextStream in(&file);
        Heap* t;
        Heap* h;
        QString buf;
        int k;
        while (!in.atEnd()){
            Node n;
            t=new Heap;
            h=new Heap;
            buf=in.readLine();
            buf+='\n';
            k=0;
            n.charnum=buf.size();
            for(int i=0;i<buf.size();i++){
               h->ch[k]=buf[i];
               k++;                //
               if(k==100){
                   k=0;
              // k++;
                   if(n.heapnum==0){
                       n.firstheap=h;
                       t=n.firstheap;
                       h=new Heap;//
                   }
                   else{
                       t->next=h;
                       t=t->next;
                       h=new Heap;//
                   }
                   n.heapnum++;
               }
            }
            if(n.heapnum==0){
                n.firstheap=h;
                n.heapnum++;
            }
            else{
                if(n.charnum%100!=0){
                    t->next=h;
                    n.heapnum++;
                }
            }
            data.push_back(n);
        }
        file.close();
    }
void datacontrol::link(Heap *a,Heap* b,int ap,int bp){//从a的第ap个元素开始，连接到b的第bp个元素（连接成功后ap与bp相邻）
    int x=ap+1,y=bp,flag=0;
    if(0<ap<99){
        for(;x<100;x++,y++){//首先补全ap之后的字符
            a->ch[x]=b->ch[y];
            if(y==99){
                if(b->next!=NULL){//若b的下一个节点存在
                    b=b->next;
                    y=0;
                }
                else{//若不存在则退出,此时a节点的100个字符可能已满，也可能不满，但都无需再添加了
                    flag=1;
                    break;
                }
            }
        }
        if(flag){
            for(;x<100;x++){
                a->ch[x]='\0';
            }//末尾补全
            return;
        }
        //退出循环时，y必不可能等于99
        Heap *c=new Heap;
        Heap *ctemp=c;
        for(int i=0;i<100;i++){
            c->ch[i]=b->ch[y];
            y++;
            if(y==99){
                if(b->next!=NULL){//若b的下一个节点存在
                    b=b->next;
                    y=0;
                }
                else{//若不存在则退出
                    break;
                }
            }
            if(i==99){
                c->next=new Heap;
                c=c->next;
            }
        }
       a->next=ctemp;
    }
    else{//ap==99，则将b的字符全部接到a的下一个节点即可
        Heap *c=new Heap;
        Heap *ctemp=c;
        for(int i=0;i<100;i++){
            
            c->ch[i]=b->ch[y];
            y++;
            if(y==100){
                if(b->next!=NULL){//若b的下一个节点存在
                    b=b->next;
                    y=0;
                }
                else{//若不存在则退出
                    break;
                }
            }
            if(i==100){
                c->next=new Heap;
                c=c->next;
            }
        }
       a->next=ctemp;
    }
}
void datacontrol::insert(cpos &c,QString qstr){
    curnode=data.begin();
    int i=0,j=0,k=0,num=0;
    for(i=0;i<c.x;++i,++curnode);
    Heap *h =new Heap;
    Heap *q=h;
    tmp=curnode->firstheap;
    for(i=0;i<c.y/100;++i,tmp=tmp->next);//光标定位至内存
    for(j=0;j<qstr.length();j++){
        h->ch[k]=qstr[j];
        if(k==99){
            k=0;
            num++;
            h->next=new Heap;
            h=h->next;
        }
    }//将qstr存入缓存的节点链表

    if((c.y+1)%100==0){
        h=tmp->next;
        link(tmp,q,c.y%100,0);//连接光标前字符与qstr
        if(h==NULL) return;
        else{
            tmp=curnode->firstheap;
            for(i=0;i<(c.y+qstr.length())/100;++i,tmp=tmp->next);
            link(tmp,h,(c.y+qstr.length())%100,0);
        }
    }
    else{
        h=tmp;
        link(tmp,q,c.y%100,0);
            tmp=curnode->firstheap;
            for(i=0;i<(c.y+qstr.length())/100;++i,tmp=tmp->next);
             link(tmp,h,(c.y+qstr.length())%100,(c.y+1)%100);
    }
    c.y+=qstr.length();
}
