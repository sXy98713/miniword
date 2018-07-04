#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QLineEdit>
#include<QPushButton>
#include<mylabel.h>
#include<string>
#include<QVBoxLayout>
#include<QMenuBar>
#include<QFontMetrics>
#include<QScrollBar>
/*光标规定：(x,y)表示第（x+1)行，第y列的字符。
 * 每一行的末尾为可输出字符，但在内存中仍有‘'\n';*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //窗口初始化
    ui->setupUi(this);

    isuntitled=true;
    shouldsaved=false;
    searchout=false;
    c.x=0;
    c.y=0;//设置当前光标坐标
    prec.x=0;
    prec.y=0;//上一个光标坐标
    QVBoxLayout *m=new QVBoxLayout;//整个窗口的布局管理器
    curfile=tr("untitle.txt");
    setWindowTitle(curfile+"- Miniword");//初始化窗口名
    //菜单栏
    openAction = new QAction(tr("&Open"), this);
    saveAction=new QAction(tr("&save"),this);
    saveasAction=new QAction(tr("&saveAs"),this);
    quitAction=new QAction(tr("&quit"),this);
    newAction=new QAction(tr("new"),this);
    connect(saveAction,&QAction::triggered,this,&MainWindow::on_actionSave_triggered);
    connect(openAction, &QAction::triggered, this, &MainWindow::on_actionOpen_triggered);
    connect(saveasAction,&QAction::triggered,this,&MainWindow::on_actionSaveAs_triggered);
    connect(quitAction,&QAction::triggered,this,&MainWindow::on_actionQuit_triggered);
    connect(newAction,&QAction::triggered,this,&MainWindow::on_actionNew_triggered);
    QMenuBar *menubar=new QMenuBar;
    QMenu *file = new QMenu(tr("&File"),this);
    menubar->addMenu(file);
    file->addAction(openAction);
    file->addAction(newAction);
    file->addAction(saveAction);
    file->addAction(saveasAction);
    file->addAction(quitAction);
    m->setAlignment(Qt::AlignTop);
    //m->addWidget(menubar);
    m->setMenuBar(menubar);
    QMenu *edit=new QMenu(tr("&edit"),this);
    menubar->addMenu(edit);
    QAction *replaceAction=new QAction(tr("&replace"),this);
    QAction *searchAction=new QAction(tr("&search"),this);
    connect(searchAction,&QAction::triggered,this,&MainWindow::creatsearchdlg);
    connect(replaceAction,&QAction::triggered,this,&MainWindow::creatrepdlg);
    edit->addAction(replaceAction);
    edit->addAction(searchAction);
    /*编辑区*/
    s=new QScrollArea;//滚动栏
    l=new mylayout;//滚动栏的垂直布局管理器
    initlabel=new mylabel;
    initlabel->sety(0);
    initlabel->setAlignment(Qt::AlignTop);
    initlabel->adjustSize();
    font=QFont("楷体",10,QFont::Bold,false);
    l->setMargin(0);
    //l->setSpacing(0);
    l->setSizeConstraint(QLayout::SetFixedSize);
    initlabel->setFont(font);
    l->add(initlabel);//初始化放置一个label,使每次文件操作时编辑区域都有第一行label
    //l->addStretch();
    s->setWidgetResizable(true);
    s->setGeometry(0,0,this->width(),this->height());
    s->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    s->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    s->setWidgetResizable(true);
    s->setFocusPolicy(Qt::NoFocus);
    QWidget *sw=new QWidget;
    QPalette pal(sw->palette());
    //设置背景色
    pal.setColor(QPalette::Background, Qt::white);
    ui->centralWidget->setAutoFillBackground(true);
    ui->centralWidget->setPalette(pal);
    sw->setAutoFillBackground(true);
    sw->setPalette(pal);
    sw->setLayout(l);
    s->setWidget(sw);
    m->addWidget(s);
    ui->centralWidget->setLayout(m);
    this->showMaximized();
}
MainWindow::~MainWindow()
{
    delete ui;
}
/*创建替换窗口*/
void MainWindow::creatrepdlg(){
    QDialog* replaceDlg = new QDialog(this);
    replaceDlg->setWindowTitle(tr("替换"));
    findline = new QLineEdit(replaceDlg);
    findline->setPlaceholderText("查找");
    replaceLineEdit = new QLineEdit(replaceDlg);
    replaceLineEdit->setPlaceholderText("替换为");
    QPushButton *searchbtn= new QPushButton(tr("查找"), replaceDlg);
    QPushButton *nextbtn= new QPushButton(tr("下一个"), replaceDlg);
    QPushButton *replacebtn=new QPushButton(tr("替换"), replaceDlg);
    QVBoxLayout *layout= new QVBoxLayout(replaceDlg);
    layout->addWidget(findline);
    layout->addWidget(replaceLineEdit);
    layout->addWidget(searchbtn);
    layout->addWidget(nextbtn);
    layout->addWidget(replacebtn);
    replaceDlg->show();
    connect(nextbtn, SIGNAL(clicked()), this, SLOT(searchnext()));
    connect(replacebtn, SIGNAL(clicked()), this, SLOT(replace()));
    connect(searchbtn,SIGNAL(clicked()),this,SLOT(search()));
}
/*查找窗口*/
void MainWindow::creatsearchdlg(){
    //查找窗口
    QDialog*find=new QDialog(this);
    find->setWindowTitle(tr("查找"));
    findline=new QLineEdit(find);
    QPushButton *thisbtn=new QPushButton(tr("查找"),find);
    QPushButton *nextbtn=new QPushButton(tr("查找下一个"),find);
    QVBoxLayout *layout= new QVBoxLayout(find);
    layout->addWidget(findline);
    //layout->addwidget(thisbtn);
    layout->addWidget(thisbtn);
    layout->addWidget(nextbtn);
    find->show();
    connect(thisbtn, SIGNAL(clicked()), this, SLOT(search()));
    connect(nextbtn, SIGNAL(clicked()), this, SLOT(searchnext()));
}
void MainWindow::locatePos(cpos cp){//光标屏幕定位函数
    //c.x=cp.x;
    //c.y=cp.y;
    int distance=0;
    if(cp.x<Data.size()){//
        if(prec.x<Data.size()){
            if((cp.x!=prec.x)||(cp.y!=prec.y)){
            l->item(prec.x)->sety(-1);
            }
        }//删除光标
        if(cp.y<=Data.getlinelength(cp.x)){
            /*获取光标在label上的横坐标*/
            QString s=Data.getqstring(cp.x);
            QFontMetrics fm(font);
            if(cp.y==0){
                distance=0;
            }
            else{
                QRect rc=fm.boundingRect(s.mid(0,cp.y));
                distance=rc.width();
            }
            l->item(c.x)->sety(distance);
        }
    }
   //s->verticalScrollBar()->setValue(c.x+800);
   //s->horizontalScrollBar()->setValue(c.y);
}
/*查找下一个*/
void MainWindow::searchnext(){
    prec.x=c.x;
    prec.y=c.y;
    if(!Data.s.empty()){
        c.x=Data.s.top().x;
        c.y=Data.s.top().y;
        locatePos(c);
        searchout=true;
    }
    else{
        QMessageBox::warning(this, tr("Miniword！"),tr("The last one has been found!"));
    }
    Data.s.pop();

}
/*查找*/
void MainWindow::search(){
    prec.x=c.x;
    prec.y=c.y;
    QString findtext;
    findtext=findline->text();
    Data.search(c,findtext);
    if(!Data.s.empty()){
        c.x=Data.s.top().x;
        c.y=Data.s.top().y;
        locatePos(c);
        searchout=true;
    }
    else{
        QMessageBox::warning(this,tr("Miniword"),tr("未查找到"));
    }
    Data.s.pop();
}
/*替换*/
void MainWindow::replace(){
    prec.x=c.x;
    prec.y=c.y;
    if(searchout){
        Data.replace(c,findline->text().size() ,replaceLineEdit->text());
        l->item(c.x)->setText(Data.getqstring(c.x));
    }
    searchout=false;
}
/*保存*/
bool MainWindow::save(){
    if (curfile=="untitle.txt") return saveas();
	else {
		return savefile(curfile);
	}
}
bool MainWindow::savefile(const QString &path)
{
	QFile file(path);
	if (!file.open(QFile::WriteOnly | QFile::Text))
	{
        QMessageBox::warning(this, tr("MiniWord"), tr("openfailed"));
		return false;
	}
	QApplication::setOverrideCursor(Qt::WaitCursor);
	Data.save(path);
	QApplication::restoreOverrideCursor();
	isuntitled = false;
    shouldsaved = false;
	curfile = QFileInfo(path).canonicalFilePath();
	setWindowTitle(curfile + tr(" - MiniWord"));
	return true;
}
/*新建文件*/
void MainWindow::newFile() {
    if (maybesave()) {
		Data.clear();
        l->clear();
    }
	isuntitled = true;
    shouldsaved = false;
    curfile = tr("未命名.txt");
    //mylabel *nl=new mylabel;
}
/*另存为*/
bool MainWindow::saveas() {
    QString path = QFileDialog::getSaveFileName(this, tr("另存为:"), curfile);
	if (path.isEmpty()) return false;
	return savefile(path);
}
/*保存条件判断*/
bool MainWindow::maybesave() {
    if (Data.isempty()) return true;
    else {
		const QMessageBox::StandardButton ret
			= QMessageBox::warning(this, tr("Miniword"),
				tr("The document has been modified.\n"
					"Do you want to save your changes?"),
				QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		switch (ret) {
		case QMessageBox::Save:
			return save();
		case QMessageBox::Cancel:
			return false;
		default:
			break;
		}
		return true;
	}
    
}
/*打开文件*/
bool MainWindow::openfile(const QString & path) {
	QFile file(path);
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
        QMessageBox::warning(this, tr("MiniWord"), tr("openfailed"));
		return false;
	}
	QApplication::setOverrideCursor(Qt::WaitCursor);
	Data.clear();
    l->clear();
	Data.read(path);
    print();
	curfile = QFileInfo(path).canonicalFilePath();
	setWindowTitle(curfile + tr(" - MiniWord"));
	isuntitled = false;
    shouldsaved = false;
	return true;
}
/*退出函数槽*/
void MainWindow::on_actionQuit_triggered()
{
    maybesave();
    this->close();
}
/*打开函数槽*/
void MainWindow::on_actionOpen_triggered()
{
    if (maybesave())
	{
		QString path = QFileDialog::getOpenFileName(this);
		if (!path.isEmpty())
		{
			openfile(path);
		}
	}
}
/*保存函数槽*/
void MainWindow::on_actionSave_triggered()
{
    save();
}
/*新建函数槽*/
void MainWindow::on_actionNew_triggered()
{

        newFile();
}
/*关闭事件*/
void MainWindow::closeEvent(QCloseEvent *event) {
    if (maybesave()){
		event->accept();//close window
	}
	else{
		event->ignore();//not close window
	}
} 
/*另存函数槽*/
void MainWindow::on_actionSaveAs_triggered(){
    saveas();
}

void MainWindow::print(){
    l->item(0)->setText(Data.getqstring(0));
    for(int i=1;i<Data.size();i++){
        plabel=new mylabel;
        plabel->setFont(font);
        plabel->setAlignment(Qt::AlignTop);
        plabel->adjustSize();
        plabel->setText(Data.getqstring(i));
        l->add(plabel);
    }
    //l->addStretch();
    prec.x=0;
    c.x=Data.size()-1;
    prec.y=0;
    c.y=Data.getlinelength(c.x);
    locatePos(c);
    //s->verticalScrollBar()->setSliderPosition(c.y);
    //s->horizontalScrollBar()->setSliderPosition(c.x);
}
/*键盘响应*/
void MainWindow::keyPressEvent(QKeyEvent* event) {
    mylabel *nexlb;
    switch(event->key()){
    case Qt::Key_Left://move left one step
        prec.x=c.x;
        prec.y=c.y;
        if(c.y>0) c.y--;//if at  the left boundary ,just go to the last line's end;
        else if(c.y==0){
            if(c.x>0){
                c.x--;
                c.y=Data.getlinelength(c.x);
            }
        }
        locatePos(c);
        break;
    case Qt::Key_Right://move right one step
        prec.x=c.x;
        prec.y=c.y;
        if(c.y<Data.getlinelength(c.x)) c.y++;
        else if(c.y==Data.getlinelength(c.x)){//if at the right end,just go to next line 's begin
            if(c.x<Data.size()-1){
                c.y=0;c.x++;
            }
        }
        locatePos(c);
        break;
    case Qt::Key_Down://move down one step
        prec.x=c.x;
        prec.y=c.y;
        if(c.x<Data.size()-1){
        c.x++;
        c.y=c.y<Data.getlinelength(c.x)?c.y:Data.getlinelength(c.x);//it depends on the next line's width,the cursorchoose which is wider
        }
        locatePos(c);
        break;
    case Qt::Key_Up://move up one step
        prec.x=c.x;
        prec.y=c.y;
        if(c.x>0){
            c.x--;
             c.y=c.y<Data.getlinelength(c.x)?c.y:Data.getlinelength(c.x);//just like the move down
        }
        locatePos(c);
        break;
    case Qt::Key_Backspace://delete the one ch before the cursor
        prec.x=c.x;
        prec.y=c.y;
        if(c.x>0){
            if(c.y>0){
                Data.backspace(c);
                l->item(c.x)->setText(Data.getqstring(c.x));//refresh the line
            }
            else if(c.y==0){//line delete
               Data.backspace(c);
               if(prec.x>0){
                delete l->take(prec.x);
               }
               l->item(c.x)->setText(Data.getqstring(c.x));
            }
        }
        else if(c.x==0){
            if(c.y>0){
                Data.backspace(c);
                l->item(c.x)->setText(Data.getqstring(c.x));
            }
        }
        locatePos(c);
        break;
    case Qt::Key_PageDown:
        prec.x=c.x;
        prec.y=c.y;
        if(c.x+22<Data.size()){
            c.x+=22;
            locatePos(c);
        }
        break;
    case Qt::Key_PageUp:
        prec.x=c.x;
        prec.y=c.y;
        if(c.x-22>=0){
            c.x-=22;
            locatePos(c);
        }
        break;
    case Qt::Key_Home:
        prec.x=c.x;
        prec.y=c.y;
        if(c.y>0){
            c.y=0;
            locatePos(c);
        }
        break;
    case Qt::Key_End:
        prec.x=c.x;
        prec.y=c.y;
        if(c.y<Data.getlinelength(c.x))
        c.y=Data.getlinelength(c.x);
        locatePos(c);
        break;
    case Qt::Key_Delete://delete the one ch back the cursor
        prec.x=c.x;
        prec.y=c.y;
        if(c.y<Data.getlinelength(c.x)){
            Data.del(c);
            l->item(c.x)->setText(Data.getqstring(c.x));//refresh
        }
        else if(c.y==Data.getlinelength(c.x)){//delete the line and connect the line and next line
            if(c.x<Data.size()-1){
            Data.del(c);
            l->item(c.x)->setText(Data.getqstring(c.x));
            delete l->take(c.x+1);
            }
        }
        break;
    case Qt::Key_Tab://change to 4 ' ’
        prec.x=c.x;
        prec.y=c.y;
        for(int i=0;i<=3;i++)
        {
            Data.insert(c,' ');
        }
        l->item(c.x)->setText(Data.getqstring(c.x));
        locatePos(c);
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        prec.x=c.x;
        prec.y=c.y;
        Data.insert(c,'\n');
        l->item(prec.x)->setText(Data.getqstring(prec.x));
        nexlb=new mylabel;
        nexlb->setFont(font);
        //nexlb->setAlignment(Qt::AlignTop);
        //nexlb->adjustSize();
        nexlb->setText(Data.getqstring(c.x));
        l->insert(nexlb,c.x);
        locatePos(c);
        break;
    default:
        prec.x=c.x;
        prec.y=c.y;
        QChar temp=event->text()[0];
        Data.insert(c,temp);
        l->item(c.x)->setText(Data.getqstring(c.x));
        locatePos(c);
        break;
        }
    }
