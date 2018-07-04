#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QScrollArea>
#include<QGridLayout>
#include<QDialog>
#include<QMessageBox>
#include<QFileDialog>
#include <QMainWindow>
#include<data.h>
#include<QLabel>
#include<QFont>
#include<QPainter>
#include<QPixmap>
#include<mylayout.h>
#include<QLineEdit>
#include<mylabel.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    cpos c;
    cpos prec;
protected:
    bool save();
    bool savefile(const QString &path);
    void newFile();
    bool saveas();
    bool maybesave();
    bool openfile(const QString &path);
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    //void mousePressEvent(QMouseEvent *event);
protected slots:
    void search();
    void replace();
    void searchnext();

private slots:
    void on_actionSaveAs_triggered();
    void on_actionQuit_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionNew_triggered();

private:
    void creatsearchdlg();
    void creatrepdlg();
    void locatePos(cpos cp);
    void print();
    bool searchout;
    bool isuntitled;
    bool shouldsaved;
    Ui::MainWindow *ui;
    mylayout *l;
    QWidget *central;
    QScrollArea *s;
    mylabel *plabel;
    QString curfile;
    QString str;
    datacontrol Data;
    mylabel* initlabel;

    QFont font;
    QAction* openAction;
    QAction* saveAction;
    QAction* saveasAction;
    QAction* newAction;
    QAction* quitAction;
    QAction* replaceAction;
    QAction* searchAction;
    QLineEdit *findline;
    QLineEdit* replaceLineEdit;

};

#endif // MAINWINDOW_H
