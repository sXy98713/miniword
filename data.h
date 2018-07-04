#include<QChar>
#include<stack>
#include<list>
using namespace std;
typedef struct heap {
	QChar *ch;
	heap* next;
	heap() {
        ch = new QChar[100];
		next = NULL;
	}
}Heap;
typedef struct node {
	Heap *firstheap;
	int heapnum;
	int charnum;
    node(){
        firstheap=NULL;
        charnum=0;
        heapnum=0;
    }
}Node;
typedef struct postion {
     int x;
     int y;
}cpos;
class datacontrol {
private:
	list<Node> data;
	list<Node> ::iterator curnode;
	Heap *tmp;

	friend void data_connect(Heap* a, Heap *b, int n);
    void data_delete(Heap *temp, list<Node>::iterator currentnode, int position, int num, int i);
    void link(Heap *a, Heap *b, int ap, int bp);
public:
    stack <cpos> s;
    datacontrol();
    ~datacontrol(){
        data.clear();
    }
	void backspace(cpos &c);
	void del(cpos &c);
	void insert(cpos &c, QChar ch1);
	bool replace(cpos &c, int len, QString str);
    void search(cpos &c, QString str);
    int size(){
        return data.size();
    }
    void clear(){
        data.clear();
        //Node n;
        //data.push_back(n);
    }
    bool isempty(){
        return data.empty();
    }
    int getlinelength(int j);
    QString getqstring(int j);
    void save(QString path);
    void read(QString path);
    void insert(cpos &c, QString qstr);
};
