#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHeaderView>
#include <QString>
#include <QDebug>
#include <QFileDialog>
#include <QIcon>
#include <QTemporaryFile>
#include <QFileIconProvider>
#include <QFile>
#include <QDir>
#include <QTextCodec>
#include <openssl/aes.h>
#include <openssl/ssl.h>
#include <openssl/aes.h>
#include <openssl/rsa.h>
#include <openssl/rc4.h>
#include <openssl/md5.h>
#include "iostream"
#include "string"
#include "fstream"
#include "math.h"
#include <tchar.h>
#include <fstream>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QFile>
#include <QUrl>
#include <stdio.h>
#include <QDesktopServices>
#include <QMessageBox>
#include <QCryptographicHash>



using namespace std;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setWindowIcon(QIcon(":/new/prefix1/D:/AG安全团队2017年工具包/Files/Tray98.ico"));
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);
    ui->setupUi(this);
    setFixedSize(this->width(),this->height());
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
    ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection); //设置为可以选中多个目标
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//表格禁止更改
    ui->tableWidget->verticalHeader()->setVisible(false);   //隐藏列表头
    ui->tableWidget->setFrameShape(QFrame::NoFrame); //设置无边框
    ui->tableWidget->setColumnWidth(0,510); //设计行的宽
    ui->tableWidget->setColumnWidth(1,70);
    ui->tableWidget->setColumnWidth(2,100);
    ui->tableWidget->setFocusPolicy(Qt::NoFocus); //去掉点击的虚线框
    ui->tableWidget->setShowGrid(false); //去掉格子线
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色
    ui->tableWidget->setStyleSheet("selection-background-color:lightblue;"); //设置点击的背景颜色
    ui->tableWidget->horizontalHeader()->setHighlightSections(false);
    ui->tableWidget->setContextMenuPolicy (Qt::CustomContextMenu);

    connect(ui->tableWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(getItem(QTreeWidgetItem*,int)));
    QList<QTableWidgetSelectionRange>ranges=ui->tableWidget->selectedRanges();

        int count=ranges.count();

        for(int i=0;i<count;i++)

        {

           int topRow=ranges.at(i).topRow();

           int bottomRow=ranges.at(i).bottomRow();

           for(int j=topRow;j<=bottomRow;j++)

           {

              qDebug()<<"selectRow"<<j;

            }

    }
        ui->tableWidget->setContextMenuPolicy (Qt::CustomContextMenu);
        connect(ui->tableWidget,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(show_menu(QPoint)));
        //ui->textEdit->setAcceptDrops(false);
            setAcceptDrops(true);
            //setCentralWidget(ui->textEdit);

}

MainWindow::~MainWindow()
{
    delete ui;
}
void  MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())//数据是否包含URls
    {
        event->acceptProposedAction();//接收动作
        //若不添加此函数，则外部文件无法添加到窗体中
    }
    else
        event->ignore();//忽略事件

}
void MainWindow::dropEvent(QDropEvent *event) //拖放事件
{
    QList<QUrl> urls = event->mimeData()->urls();
        if(urls.isEmpty())
            return;

        //往文本框中追加文件名
        foreach(QUrl url, urls) {
            QString file_name = url.toLocalFile();
            //ui->textEdit->append(file_name);
            qDebug() << file_name;
            int row_count = ui->tableWidget->rowCount(); //获取表单行数
            ui->tableWidget->insertRow(row_count); //插入新行
            row_count = ui->tableWidget->rowCount();
            qDebug() << row_count;
            QTableWidgetItem *item = new QTableWidgetItem();
            qDebug() << file_name;
            QString aabc = file_name.replace(QString("/") , QString('\\'));
            QFileInfo file_info(file_name);
            QFileIconProvider icon_provider;
            QIcon icon = icon_provider.icon(aabc);
            item->setIcon(icon);
            //qDebug() << item;
           // qDebug() << aabc;
            item->setText(file_name);





            QFileInfo fileinfo (file_name);
            qDebug() <<fileinfo.isFile();
            ui->tableWidget->setItem(row_count-1, 0, item); //插入行

            if(!fileinfo.isFile()) //判断是否目录
             {
                quint64 size1 = dirFileSize(file_name);
                QString size2= "B";
                if (size1 > 1024)
                {
                    size1 = size1/ 1024;
                    size2= "KB";
                }
                if (size1 > 1024)
                {
                    size1 = size1/ 1024;
                    size2= "MB";
                }
                if (size1 > 1024)
                {
                    size1 = size1/ 1024;
                    size2= "GB";
                }
                 size2=QString("%1%2").arg(size1).arg(size2);
                 addItemContent(row_count-1,1,size2);
                // chakan(file_name,3);

            }else
            {
                QFile file(file_name);
                quint64 size1 = file.size();
                QString size2= "B";
                if (size1 > 1024)
                {
                    size1 = size1/ 1024;
                    size2= "KB";
                }
                if (size1 > 1024)
                {
                    size1 = size1/ 1024;
                    size2= "MB";
                }
                if (size1 > 1024)
                {
                    size1 = size1/ 1024;
                    size2= "GB";
                }

                file.close();
                 size2=QString("%1%2").arg(size1).arg(size2);
                 addItemContent(row_count-1,1,size2);

            }

            //size2=size1+size2;




            qDebug() <<"row_count:"<<row_count;

            //addItemContent(row_count-1,1,size2);
            addItemContent(row_count-1,2,"无操作");

            }


}

void MainWindow::on_tableWidget_itemClicked(QTableWidgetItem *item) //表格点击事件
{
    QList<QTableWidgetSelectionRange>ranges=ui->tableWidget->selectedRanges();

        int count=ranges.count();

        for(int i=0;i<count;i++)

        {

           int topRow=ranges.at(i).topRow();

           int bottomRow=ranges.at(i).bottomRow();

           for(int j=topRow;j<=bottomRow;j++)

           {

              qDebug()<<"行"<<j;

            }

    }
        QList<QTableWidgetItem*>items=ui->tableWidget->selectedItems();

        int count1=items.count();
        int x =0;

        for(int i=0;i<count1;i++)

            {

               int row=ui->tableWidget->row(items.at(i));//获取选中的行

               QTableWidgetItem*item=items.at(i);

               QString name=item->text();//获取内容

               if(x==0)
               {
                   x++;
                   qDebug()<<"路径:"<<name;

               }

            }
       // QList<QTableWidgetSelectionRange>ranges=ui->tableWidget->selectedRanges();

}

void MainWindow::show_menu(const QPoint pos) //表格右键菜单
{
    int x = pos.x ();
    int y = pos.y ();
    QModelIndex index = ui->tableWidget->indexAt (QPoint(x,y));
    int row = index.row ();//获得QTableWidget列表点击的行数
    if(row == -1) //  防止点击空白区域出现菜单
    {
        return ;
    }
//设置菜单选项
QMenu *menu = new QMenu(ui->tableWidget);
QAction *pnew = new QAction("移除",ui->tableWidget);
QAction *pnew1 = new QAction("加密",ui->tableWidget);
QAction *pnew2 = new QAction("解密",ui->tableWidget);

connect (pnew,SIGNAL(triggered()),this,SLOT(clickgoose()));
connect (pnew1,SIGNAL(triggered()),this,SLOT(clickmms()));
connect (pnew2,SIGNAL(triggered()),this,SLOT(clickdxx()));
menu->addAction(pnew);
menu->addAction(pnew1);
menu->addAction(pnew2);
menu->move (cursor ().pos ());
menu->show ();
//获得鼠标点击的x，y坐标点

qDebug() << row ;
}

void MainWindow::clickgoose() //表格右键菜单栏-移除
{
    qDebug() << 1 ;

    int rowIndex = ui->tableWidget->currentRow();
    if(rowIndex == -1)
    {
        QMessageBox::about(NULL, "提示", "请选择一个 <font color='red'>文件</font>");
        return ;
    }
      if (rowIndex != -1)
      {
          ui->tableWidget->removeRow(rowIndex);
      }



}

void MainWindow::clickmms() //表格右键菜单栏-加密
{
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);
    qDebug() << 2 ;
    QList<QTableWidgetItem*>items=ui->tableWidget->selectedItems();

   int count=items.count();
   int row=ui->tableWidget->row(items.at(0));//获取选中的行
   QTableWidgetItem*item=items.at(0);
   if(ui->lineEdit->text() == "")
   {
       QMessageBox::about(NULL, "提示", "请输入 <font color='red'>密钥</font>");
       return ;
   }
   if(row == -1)
   {
       QMessageBox::about(NULL, "提示", "请选择一个 <font color='red'>文件</font>");
       return ;
   }
   QString name=item->text();//获取内容
   qDebug() << name;
   QFileInfo fileinfo (name);
   if(!fileinfo.isFile()) //判断是否目录
    {
       chakan(name,0);

   }
   else
   {
   QString file_name, file_path;
   QFileInfo fi;
   fi = QFileInfo(name);
   file_name = fi.fileName();  //获取文件名
   file_path = fi.absolutePath(); //获取文件路径(不包含文件名)
   qDebug() << file_name;
   qDebug() << file_path;
   file_name=file_name+".mt";
   qDebug() << file_name;
   QString str2 = file_name.replace(QRegExp(".mt"), "");
   qDebug() << str2;
   QString temp_key = ui->lineEdit->text(); //QString转char *
   QByteArray tem_key = temp_key.toLatin1();
   char *key = tem_key.data();
   qDebug() << key;
   if(ui->comboBox->currentIndex() == 0) //普通加密
   {
       AESencryptfile(name,name+".mt",key,16,row);
   }
   if(ui->comboBox->currentIndex() == 1) //高强度加密
   {
       AESXencryptfile(name,name+".mt",key,16,row);
   }
   if(ui->comboBox->currentIndex() == 2) //企业级加密
   {
       AES_S_encryptfile(name,name+".mt",key,16,row);
   }
   /*if(ui->comboBox->currentIndex() == 3) //国家级加密
   {
       AES_SS_encryptfile(name,name+".mt",key,16,row);
   }
   */

   if(ui->checkBox->isChecked()) //判断加密删除文件复选框
    {

       string delfile = name.toStdString();
       ofstream in(delfile, ios::out | ios::binary | ios::trunc);
       in.write("/0", 5);
       in.write("abc123456", 5);
       in.write("", 5);
       DeleteFileA(delfile.c_str());
       in.close();
       remove(delfile.c_str());
       ofstream fout(delfile);
       if (fout) {
                       fout << "~!@#$%^&*(+_)(*&^%$@#!0123456789/*-+`QERTYUIOP{}ASDFGHJKL:" << endl;
                   fout.close();
                   }
       QFile file(name);
       if (file.exists())
       {
           file.remove();
          }
                   remove(delfile.c_str());

   }
   }





}

void MainWindow::clickdxx() //表格右键菜单栏-解密
{
    qDebug() << 3 ;
    QList<QTableWidgetItem*>items=ui->tableWidget->selectedItems();
   int count=items.count();
   int row=ui->tableWidget->row(items.at(0));//获取选中的行
   QTableWidgetItem*item=items.at(0);
   if(ui->lineEdit->text() == "")
   {
       QMessageBox::about(NULL, "提示", "请输入 <font color='red'>密钥</font>");
       return ;
   }
   if(row == -1)
   {
       QMessageBox::about(NULL, "提示", "请选择一个 <font color='red'>文件</font>");
       return ;
   }
   QString name=item->text();//获取内容
   qDebug() << name;
   QFileInfo fileinfo (name);

   if(!fileinfo.isFile()) //判断是否目录
    {
       chakan(name,1);

   }
   else
   {
   QString file_name, file_path;
   QFileInfo fi;
   fi = QFileInfo(name);
   file_name = fi.fileName();  //获取文件名
   file_path = fi.absolutePath(); //获取文件路径(不包含文件名)
   qDebug() << file_name;
   qDebug() << file_path;
   //file_name=file_name+".mt";
   //qDebug() << file_name;
   QString str2 = file_path+"/"+file_name.replace(QRegExp(".mt"), "");
   qDebug() << str2;
   QString temp_key = ui->lineEdit->text(); //QString转char *
   QByteArray tem_key = temp_key.toLatin1();
   char *key = tem_key.data();
   qDebug() << key;
   QFile tmp(name);
   if(!tmp.open(QIODevice::ReadOnly))
   {
       qDebug() << "打开文件失败" << endl;
       addItemContent(row,2,"打开文件失败");
       return ;
   }
   char *a_data = new char[40];
   tmp.read(a_data,33);
   char *p=strstr(a_data,"af5179903a31bb02");
   if(NULL != p)
   {
       qDebug() << "普通解密" << endl;
       AesDecryptFile(name,str2,key,16,row);
   }
   p=strstr(a_data,"a08500733dbf0052");
   if(NULL != p)
   {
       qDebug() << "高强度解密" << endl;
       AesXDecryptFile(name,str2,key,16,row);
   }
   p=strstr(a_data,"bfbdd049800da83d283824fc4a758440");
   if(NULL != p)
   {
       qDebug() << "企业级解密" << endl;
       Aes_S_DecryptFile(name,str2,key,16,row);
   }
   p=strstr(a_data,"d9268c456c109f1fe2811b8cb6c75998");
   if(NULL != p)
   {
       qDebug() << "国家级解密" << endl;
       Aes_SS_DecryptFile(name,str2,key,16,row);
   }
   tmp.close();
   tmp.flush();



    if(ui->checkBox_3->isChecked()) //判断解密删除文件复选框
     {

        string delfile = name.toStdString();
        ofstream in(delfile, ios::out | ios::binary | ios::trunc);
        in.write("/0", 5);
        in.write("abc123456", 5);
        in.write("", 5);
        DeleteFileA(delfile.c_str());
        in.close();
        remove(delfile.c_str());
        ofstream fout(delfile);
        if (fout) {
                        fout << "~!@#$%^&*(+_)(*&^%$@#!0123456789/*-+`QERTYUIOP{}ASDFGHJKL:" << endl;
                    fout.close();
                    }
        QFile file(name);
        if (file.exists())
        {
            file.remove();
           }
                    remove(delfile.c_str());

    }
   }



}

void MainWindow::on_pushButton_5_clicked() //全部移除
{
    ui->tableWidget->setRowCount(0);
}

void MainWindow::addItemContent(int row, int column, QString content) //表格插入数据
 {


      QTableWidgetItem *item = new QTableWidgetItem (content);


      ui->tableWidget->setItem(row, column, item);

 }

void MainWindow::on_pushButton_6_clicked() //清除表格一行
{
    int rowIndex = ui->tableWidget->currentRow();
      if (rowIndex != -1)
      {
          ui->tableWidget->removeRow(rowIndex);
      }

}

void MainWindow::on_pushButton_3_clicked() // 添加文件到表格
{

    //QString file_name = QFileDialog::getOpenFileName(NULL,"加密的文件",".","*.*");
    QFileDialog fileDialog(this);

    //设置窗口的标题
    fileDialog.setWindowTitle("请选择文件");
    fileDialog.setNameFilter("所有文件(*.*)"); //设置一个过滤器
     //这个标志用来设置选择的类型，比如默认是单个文件。QFileDialog::ExistingFiles 多个文件,还可以用来选择文件夹QFileDialog::Directory。QFileDialog::ExistingFile 单个文件。注意这个ExistingFile，单词后面多了一个s 表示选择多个文件。要看清楚了。
    fileDialog.setFileMode(QFileDialog::ExistingFiles);
    //如果是要保存文件，那就需要再设置一下
    //fileDialog.setAcceptMode(QFileDialog::AcceptSave);


    //弹出对话框
    QStringList file_name;
    if (fileDialog.exec() == QDialog::Accepted)
    {

    //strPathList  返回值是一个list，如果是单个文件选择的话，只要取出第一个来就行了。
    file_name = fileDialog.selectedFiles();

    }

    for(int i = 0; i < file_name.size(); i++)
     {
    if(file_name[i] == "")
    {
        return ;
    }}
    for(int i = 0; i < file_name.size(); i++)
     {


    qDebug() << file_name[i] << endl;
    qDebug() << file_name[i];
    int row_count = ui->tableWidget->rowCount(); //获取表单行数
    ui->tableWidget->insertRow(row_count); //插入新行
    row_count = ui->tableWidget->rowCount();

    qDebug() << row_count;
    QTableWidgetItem *item = new QTableWidgetItem();
    qDebug() << file_name[i];
    QString aabc = file_name[i].replace(QString("/") , QString('\\'));
    QFileInfo file_info(file_name[i]);
    QFileIconProvider icon_provider;
    QIcon icon = icon_provider.icon(aabc);
   // qDebug() << file_name[i];
    item->setIcon(icon);
    //qDebug() << item;
   // qDebug() << aabc;
    item->setText(file_name[i]);
    QFile file(aabc);
    //qDebug() << file.size();
    int size1 = file.size();
    file.close();
    QString size2= "B";
    if (size1 > 1024)
    {
        size1 = size1/ 1024;
        size2= "KB";
    }
    if (size1 > 1024)
    {
        size1 = size1/ 1024;
        size2= "MB";
    }
    if (size1 > 1024)
    {
        size1 = size1/ 1024;
        size2= "GB";
    }

    //size2=size1+size2;
    size2=QString("%1%2").arg(size1).arg(size2);




    qDebug() <<"row_count:"<<row_count;
    ui->tableWidget->setItem(row_count-1, 0, item); //插入行
    addItemContent(row_count-1,1,size2);
    addItemContent(row_count-1,2,"无操作");

    }

}

void MainWindow::on_pushButton_4_clicked() //添加目录到
{
    QFileDialog fileDialog(this);

    //设置窗口的标题
    fileDialog.setWindowTitle("请选择目录");
    fileDialog.setFileMode(QFileDialog::Directory);
    QStringList strPathList;
    //弹出对话框
    if (fileDialog.exec() == QDialog::Accepted)
    {
        strPathList = fileDialog.selectedFiles();
        qDebug() << strPathList[0];

    //strPathList  返回值是一个list，如果是单个文件选择的话，只要取出第一个来就行了。


    }
    for(int i = 0; i < strPathList.size(); i++)
     {
        qDebug() << strPathList[i];
    if(strPathList[i] == "")
    {
        return ;
    }}
    if(strPathList[0] == "")
    {
        return ;
    }



   // chakan(strPathList[0]);


  //  qDebug() << GetFileList(strPathList[0]);

    qDebug() << strPathList[0] << endl;
qDebug() << strPathList[0];
int row_count = ui->tableWidget->rowCount(); //获取表单行数
ui->tableWidget->insertRow(row_count); //插入新行
row_count = ui->tableWidget->rowCount();
qDebug() << row_count;
QTableWidgetItem *item = new QTableWidgetItem();
qDebug() << strPathList[0];
QString aabc = strPathList[0].replace(QString("/") , QString('\\'));
QFileInfo file_info(strPathList[0]);
QFileIconProvider icon_provider;
QIcon icon = icon_provider.icon(aabc);
qDebug() << strPathList[0];
item->setIcon(icon);
//qDebug() << item;
qDebug() << aabc;
item->setText(strPathList[0]);
int size1 =dirFileSize(aabc);
QString size2= "B";
if (size1 > 1024)
{
    size1 = size1/ 1024;
    size2= "KB";
}
if (size1 > 1024)
{
    size1 = size1/ 1024;
    size2= "MB";
}
if (size1 > 1024)
{
    size1 = size1/ 1024;
    size2= "GB";
}
size2=QString("%1%2").arg(size1).arg(size2);

// ui->tableWidget->setItem(row_count, 0, item);


ui->tableWidget->setItem(row_count-1, 0, item);
//addItemContent(row_count-1,0,file_name);
addItemContent(row_count-1,1,size2);
addItemContent(row_count-1,2,"目录无操作");







}

quint64 MainWindow::dirFileSize(const QString &path) // 获取目录大小
{
QDir dir(path);
quint64 size = 0;
//dir.entryInfoList(QDir::Files)返回文件信息
foreach(QFileInfo fileInfo, dir.entryInfoList(QDir::Files))
{
//计算文件大小
size += fileInfo.size();
}
//dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot)返回所有子目录，并进行过滤
foreach(QString subDir, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
{
//若存在子目录，则递归调用dirFileSize()函数
size += dirFileSize(path + QDir::separator() + subDir);
}
return size;
}

void MainWindow::chakan(QString path,int encryption) //遍历目录
{
  QDir dir(path);

  foreach(QFileInfo mfi ,dir.entryInfoList())
  {
    if(mfi.isFile()) // 0加密 1解密
    {

        QTextCodec *codec = QTextCodec::codecForName("utf-8");
        QTextCodec::setCodecForLocale(codec);
        qDebug() << 2 ;
        QList<QTableWidgetItem*>items=ui->tableWidget->selectedItems();

       int count=items.count();
       int row=ui->tableWidget->row(items.at(0));//获取选中的行
       QTableWidgetItem*item=items.at(0);
       //QString name=item->text();//获取内容
       if(encryption == 0) // 0
       {
           //qDebug() << name;
           QString file_name, file_path;
           QFileInfo fi;
           QString mulu=mfi.absoluteFilePath();
           fi = QFileInfo(mulu);
           file_name = fi.fileName();  //获取文件名
           file_path = fi.absolutePath(); //获取文件路径(不包含文件名)
           qDebug() << file_name;
           qDebug() << file_path;
           file_name=file_name+".mt";
           qDebug() << file_name;
           QString str2 = file_name.replace(QRegExp(".mt"), "");
           qDebug() << str2;
           QString temp_key = ui->lineEdit->text(); //QString转char *
           QByteArray tem_key = temp_key.toLatin1();
           char *key = tem_key.data();
           qDebug() << key;

           if(ui->comboBox->currentIndex() == 0) //普通加密
           {
               AESencryptfile(mulu,mulu+".mt",key,16,row);
           }
           if(ui->comboBox->currentIndex() == 1) //高强度加密
           {
               AESXencryptfile(mulu,mulu+".mt",key,16,row);
           }
           if(ui->comboBox->currentIndex() == 2) //企业级加密
           {
               AES_S_encryptfile(mulu,mulu+".mt",key,16,row);
           }
           if(ui->comboBox->currentIndex() == 3) //国家级加密
           {
               AES_SS_encryptfile(mulu,mulu+".mt",key,16,row);
           }

           if(ui->checkBox->isChecked()) //判断加密删除文件复选框
            {

               string delfile = mulu.toStdString();
               ofstream in(delfile, ios::out | ios::binary | ios::trunc);
               in.write("/0", 5);
               in.write("abc123456", 5);
               in.write("", 5);
               DeleteFileA(delfile.c_str());
               in.close();
               remove(delfile.c_str());
               ofstream fout(delfile);
               if (fout) {
                               fout << "~!@#$%^&*(+_)(*&^%$@#!0123456789/*-+`QERTYUIOP{}ASDFGHJKL:" << endl;
                           fout.close();
                           }
               QFile file(mulu);
               if (file.exists())
               {
                   file.remove();
                  }
                           remove(delfile.c_str());
               }


       }
       if (encryption ==1)
       {
           QList<QTableWidgetItem*>items=ui->tableWidget->selectedItems();
          int count=items.count();
          int row=ui->tableWidget->row(items.at(0));//获取选中的行
          QTableWidgetItem*item=items.at(0);
          QString file_name, file_path;
          QFileInfo fi;
          QString mulu=mfi.absoluteFilePath();
          fi = QFileInfo(mulu);
          file_name = fi.fileName();  //获取文件名
          file_path = fi.absolutePath(); //获取文件路径(不包含文件名)
          qDebug() << file_name;
          qDebug() << file_path;
          //file_name=file_name+".mt";
          //qDebug() << file_name;
          QString str2 = file_path+"/"+file_name.replace(QRegExp(".mt"), "");
          qDebug() << str2;
          QString temp_key = ui->lineEdit->text(); //QString转char *
          QByteArray tem_key = temp_key.toLatin1();
          char *key = tem_key.data();
          qDebug() << key;

          QFile tmp(mulu);
          if(!tmp.open(QIODevice::ReadOnly))
          {
              qDebug() << "打开文件失败" << endl;
              addItemContent(row,2,"打开文件失败");
              return ;
          }
          char *a_data = new char[40];
          tmp.read(a_data,33);
          char *p=strstr(a_data,"af5179903a31bb02");
          if(NULL != p)
          {
              qDebug() << "普通解密" << endl;
              AesDecryptFile(mulu,str2,key,16,row);
          }
          p=strstr(a_data,"a08500733dbf0052");
          if(NULL != p)
          {
              qDebug() << "高强度解密" << endl;
              AesXDecryptFile(mulu,str2,key,16,row);
          }
          p=strstr(a_data,"bfbdd049800da83d283824fc4a758440");
          if(NULL != p)
          {
              qDebug() << "企业级解密" << endl;
              Aes_S_DecryptFile(mulu,str2,key,16,row);
          }
          p=strstr(a_data,"d9268c456c109f1fe2811b8cb6c75998");
          if(NULL != p)
          {
              qDebug() << "国家级解密" << endl;
              Aes_SS_DecryptFile(mulu,str2,key,16,row);
          }
          tmp.close();
          tmp.flush();;
           if(ui->checkBox_3->isChecked()) //判断解密删除文件复选框
           {

              string delfile = mfi.absoluteFilePath().toStdString();
              ofstream in(delfile, ios::out | ios::binary | ios::trunc);
              in.write("/0", 5);
              in.write("abc123456", 5);
              in.write("", 5);
              DeleteFileA(delfile.c_str());
              in.close();
              remove(delfile.c_str());
              ofstream fout(delfile);
              if (fout) {
                              fout << "~!@#$%^&*(+_)(*&^%$@#!0123456789/*-+`QERTYUIOP{}ASDFGHJKL:" << endl;
                          fout.close();
                          }
              QFile file(mfi.absoluteFilePath());
              if (file.exists())
              {
                  file.remove();
                 }
                          remove(delfile.c_str());

          }






       }


       qDebug()<< "File :" <<mfi.absoluteFilePath()<< mfi.fileName();

    }

    else
    {
        if(mfi.fileName()=="." || mfi.fileName() == "..")continue;
        qDebug() << "Entry Dir" << mfi.absoluteFilePath();
        chakan(mfi.absoluteFilePath(),encryption);
    }
  }
}


#define RELESE(P) if (P)        \
{                               \
    delete P;                   \
    P = NULL;                   \
}

#define RELESE_ARRAY(P) if (P)  \
{                               \
    delete[] P;                 \
    P = NULL;                   \
}



// 测试使用aes加密算法的例子
void TestAesEncrypt()
{
    unsigned char buf[16]; //原字符串
    memset(buf,1,sizeof(buf));
    strcpy((char *)buf, "zengraoli");

    qDebug() << "current buf value is :" << buf << endl;

    unsigned char buf2[16]; //加密结果
    unsigned char buf3[16]; //解密结果
    unsigned char aes_keybuf[32];

    memset(aes_keybuf,0,sizeof(aes_keybuf));
    strcpy((char *)aes_keybuf, "zeng");
    qDebug() << "current aes_keybuf value is :" << aes_keybuf << endl;
    AES_KEY aeskey;

    
    AES_set_encrypt_key(aes_keybuf,256,&aeskey);
    AES_encrypt(buf,buf2,&aeskey);

    qDebug() << "current buf2 value is :" << buf2 << endl;

    memset(aes_keybuf,0,sizeof(aes_keybuf));
    strcpy((char *)aes_keybuf, "zeng2");
    qDebug() << "current aes_keybuf value is :" << aes_keybuf << endl;
    AES_set_decrypt_key(aes_keybuf,256,&aeskey);
    AES_decrypt(buf2,buf3,&aeskey);

    qDebug() << "current buf3 value is :" << buf3 << endl;

    if(memcmp(buf,buf3,sizeof(buf))==0)
        printf("test success\r\n");
    else
        printf("test fail\r\n");
}


// 测试使用aes加密文件算法的例子
int MainWindow::TestAesEncryptFile(std::string in_file_path, std::string out_file_path,
                       const char *rc4_encrypt_key, int encrypt_chunk_size = 16)
{
    //std::locale loc = std::locale::global(std::locale(""));
    //setlocale(LC_ALL,"Chinese-simplified");
    //char file =in_file_path.c_str();
    setlocale(LC_ALL,"Chinese-simplified");
    std::locale loc = std::locale::global(std::locale(""));
    ifstream fin(in_file_path, ios::binary);
    qDebug() << in_file_path.c_str();
    ofstream fout(out_file_path, ios::binary);
    //qDebug() << out_file_path;

    if(!fin)
    {
        qDebug() << "Can not open fin file." ;
        //return 1;
    }
    if(!fout)
    {
        qDebug() << "Can not open fout file." ;
        return 1;
    }

    //用指定密钥对一段内存进行加密，结果放在outbuffer中
    unsigned char aes_keybuf[32];
    memset(aes_keybuf,0,sizeof(aes_keybuf));
    strcpy((char *)aes_keybuf, rc4_encrypt_key);
    AES_KEY aeskey;
    AES_set_encrypt_key(aes_keybuf, 256, &aeskey);

    char *in_data  = new char[encrypt_chunk_size + 1];
    char *out_data = new char[encrypt_chunk_size + 1];
    while(!fin.eof())
    {
        fin.read(in_data, encrypt_chunk_size);
        if(fin.gcount() < encrypt_chunk_size)
        {
            qDebug() <<"gcount:" <<fin.gcount();
            qDebug() <<"encrypt_chunk_size:" <<encrypt_chunk_size;
            qDebug() <<"in_data:"<<in_data ;
            fout.write(in_data, fin.gcount());
        }
        else
        {
            qDebug() <<"gcount:" <<fin.gcount();
            qDebug() <<"encrypt_chunk_size:" <<encrypt_chunk_size;
            //qDebug() <<"in_data:"<<in_data ;
            AES_encrypt((const unsigned char *)in_data, (unsigned char *)out_data, &aeskey);
            //AES_encrypt("123", (unsigned char *)out_data, &aeskey);
            //qDebug() <<"out_data:"<<out_data ;
            fout.write(out_data, fin.gcount());
        }
    };

    fout.close();
    fin.close();

    RELESE_ARRAY(in_data);
    RELESE_ARRAY(out_data);

    return 0;
}

// 测试使用aes解密文件算法的例子
int TestAesDecryptFile(std::string in_file_path, std::string out_file_path,
                       const char *rc4_dencrypt_key, int encrypt_chunk_size = 16)
{
    ifstream fin(in_file_path.c_str(), ios::binary);
    ofstream fout(out_file_path, ios::binary);

    if(!fin)
    {
        cout << "Can not open fin file." << endl;
        return 1;
    }
    if(!fout)
    {
        cout << "Can not open fout file." << endl;
        return 1;
    }

    //用指定密钥对一段内存进行加密，结果放在outbuffer中
    unsigned char aes_keybuf[32];
    memset(aes_keybuf,0,sizeof(aes_keybuf));
    strcpy((char *)aes_keybuf, rc4_dencrypt_key);
    AES_KEY aeskey;
    AES_set_decrypt_key(aes_keybuf, 256, &aeskey);

    char *in_data  = new char[encrypt_chunk_size + 1];
    char *out_data = new char[encrypt_chunk_size + 1];
    int i = 0;
    while( ! fin.eof() )
    {
        fin.read(in_data, encrypt_chunk_size);
        if(fin.gcount() < encrypt_chunk_size)
        {
            fout.write(in_data, fin.gcount());
        }
        else
        {
            AES_decrypt((unsigned char *)in_data, (unsigned char *)out_data, &aeskey);
            fout.write(out_data, fin.gcount());
        }
    };

    fout.close();
    fin.close();

    RELESE_ARRAY(in_data);
    RELESE_ARRAY(out_data);

    return 0;
}


// 测试使用aes加密算法的例子
void TestRsaEncrypt()
{
    BIGNUM b={0};
    RSA* pRsa = RSA_generate_key(1024, RSA_F4, 0, 0);

    //pRsa中包含了N D,你这里自己修改就可以了
    char in_data[] = "zengraoli";
    cout << "current in_data value is : " << in_data << endl;

    int len = RSA_size(pRsa);
    char* out_data = new char[len];
    memset(out_data, 0, len);
    RSA_public_encrypt(sizeof(in_data), (unsigned char *)in_data,
                        (unsigned char *)out_data, pRsa, RSA_PKCS1_PADDING);
    cout << "current out_data value is : " << out_data << endl;

    char out[1024] = {0};
    RSA_private_decrypt(len, (unsigned char *)out_data,
                        (unsigned char *)out, pRsa, RSA_PKCS1_PADDING);
    RSA_free(pRsa);

    cout << "current out value is : " << out << endl;
}


// 测试使用rc4加密算法的例子
void TestRc4Encrypt()
{
    char code[64]={0};
    int codelen = sizeof(code);
    memcpy_s(code, 64, "This is secrect", 15);

    cout << "before encrypt :" << code << endl;
    unsigned char *outbuffer = new unsigned char[codelen];

    //用指定密钥对一段内存进行加密，结果放在outbuffer中
    RC4_KEY rc4_key;
    RC4_set_key(&rc4_key,7,(unsigned char *)"zenraoli");
    RC4(&rc4_key,codelen,(unsigned char *)code,outbuffer);
    cout << "after encrypt :" << outbuffer << endl;

    //用指定密钥对outbuffer中的密文进行解密，结果放回原来的内存
    memset(code,0,sizeof(code));
    RC4_set_key(&rc4_key,7,(unsigned char *)"zenraoli");//这里必须再次设置密钥
    RC4(&rc4_key,codelen,outbuffer,(unsigned char *)code);
    cout << "after decrypt :" << code << endl;

    if (outbuffer)
    {
        delete[] outbuffer;
        outbuffer = NULL;
    }
}


// 测试使用rc4加密文件算法的例子
int TestRc4EncryptFile(std::string in_file_path, std::string out_file_path,
                       const char *rc4_encrypt_key, int encrypt_chunk_size = 16)
{
    ifstream fin(in_file_path.c_str(), ios::binary);
    ofstream fout(out_file_path, ios::binary);

    if(!fin)
    {
        cout << "Can not open fin file." << endl;
        return 1;
    }
    if(!fout)
    {
        cout << "Can not open fout file." << endl;
        return 1;
    }

    //用指定密钥对一段内存进行加密，结果放在outbuffer中
    char code[64] = {0};
    int codelen = sizeof(code);
    RC4_KEY rc4_key;
    RC4_set_key(&rc4_key, strlen(rc4_encrypt_key), (unsigned char *)rc4_encrypt_key);

    char *in_data  = new char[encrypt_chunk_size + 1];
    char *out_data = new char[encrypt_chunk_size + 1];
    while(!fin.eof())
    {
        fin.read(in_data, encrypt_chunk_size);
        RC4(&rc4_key, (size_t)fin.gcount(),(unsigned char *)in_data, (unsigned char *)out_data);
        fout.write(out_data, fin.gcount());
    };

    fout.close();
    fin.close();

    RELESE_ARRAY(in_data);
    RELESE_ARRAY(out_data);

    return 0;
}

// 测试使用rc4解密文件算法的例子
int TestRc4DecryptFile(std::string in_file_path, std::string out_file_path,
                       const char *rc4_dencrypt_key, int encrypt_chunk_size = 16)
{
    ifstream fin(in_file_path.c_str(), ios::binary);
    ofstream fout(out_file_path, ios::binary);

    if(!fin)
    {
        cout << "Can not open fin file." << endl;
        return 1;
    }
    if(!fout)
    {
        cout << "Can not open fout file." << endl;
        return 1;
    }

    //用指定密钥对一段内存进行加密，结果放在outbuffer中
    char code[64] = {0};
    int codelen = sizeof(code);
    RC4_KEY rc4_key;
    RC4_set_key(&rc4_key, strlen(rc4_dencrypt_key), (unsigned char *)rc4_dencrypt_key);

    char *in_data  = new char[encrypt_chunk_size + 1];
    char *out_data = new char[encrypt_chunk_size + 1];
    while(!fin.eof())
    {
        fin.read(in_data, encrypt_chunk_size);
        RC4(&rc4_key, (size_t)fin.gcount(),(unsigned char *)in_data, (unsigned char *)out_data);
        fout.write(out_data, fin.gcount());
    };

    fout.close();
    fin.close();

    RELESE_ARRAY(in_data);
    RELESE_ARRAY(out_data);

    return 0;
}


void MainWindow::on_pushButton_7_clicked() //加密文件
{
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);
    qDebug() << 2 ;
    if(ui->lineEdit->text() == "")
    {
        QMessageBox::about(NULL, "提示", "请输入 <font color='red'>密钥</font>");
        return ;
    }
    QList<QTableWidgetItem*>items=ui->tableWidget->selectedItems();

   int count=items.count();
   int row=ui->tableWidget->row(items.at(0));//获取选中的行
   qDebug() << row;
   if(row == -1)
   {
        QMessageBox::about(NULL, "提示", "请选择一个 <font color='red'>文件</font>");
       return ;
   }
   QTableWidgetItem*item=items.at(0);
   QString name=item->text();//获取内容
   qDebug() << name;
   QFileInfo fileinfo (name);
   if(!fileinfo.isFile()) //判断是否目录
    {
       chakan(name,0);

   }
   else
   {

       QString file_name, file_path;
       QFileInfo fi;
       fi = QFileInfo(name);
       file_name = fi.fileName();  //获取文件名
       file_path = fi.absolutePath(); //获取文件路径(不包含文件名)
       qDebug() << file_name;
       qDebug() << file_path;
       file_name=file_name+".mt";
       qDebug() << file_name;
       QString str2 = file_name.replace(QRegExp(".mt"), "");
       qDebug() << str2;
       QString temp_key = ui->lineEdit->text(); //QString转char *
       QByteArray tem_key = temp_key.toLatin1();
       char *key = tem_key.data();
       qDebug() << key;
       if(ui->comboBox->currentIndex() == 0) //普通加密
       {
           AESencryptfile(name,name+".mt",key,16,row);
       }
       if(ui->comboBox->currentIndex() == 1) //高强度加密
       {
           AESXencryptfile(name,name+".mt",key,16,row);
       }
       if(ui->comboBox->currentIndex() == 2) //企业级加密
       {
           AES_S_encryptfile(name,name+".mt",key,16,row);
       }
       if(ui->comboBox->currentIndex() == 3) //国家级加密
       {
           AES_SS_encryptfile(name,name+".mt",key,16,row);
       }


       if(ui->checkBox->isChecked()) //判断加密删除文件复选框
       {

          string delfile = name.toStdString();
          ofstream in(delfile, ios::out | ios::binary | ios::trunc);
          in.write("/0", 5);
          in.write("abc123456", 5);
          in.write("", 5);
          DeleteFileA(delfile.c_str());
          in.close();
          remove(delfile.c_str());
          ofstream fout(delfile);
          if (fout) {
                          fout << "~!@#$%^&*(+_)(*&^%$@#!0123456789/*-+`QERTYUIOP{}ASDFGHJKL:" << endl;
                      fout.close();
                      }
          QFile file(name);
          if (file.exists())
          {
              file.remove();
             }
                      remove(delfile.c_str());
          }



   }




}

void MainWindow::on_pushButton_8_clicked() //解密文件
{
    qDebug() << 3 ;
    QList<QTableWidgetItem*>items=ui->tableWidget->selectedItems();
   int count=items.count();
   int row=ui->tableWidget->row(items.at(0));//获取选中的行
   if(ui->lineEdit->text() == "")
   {
       QMessageBox::about(NULL, "提示", "请输入 <font color='red'>密钥</font>");
       return ;
   }
   if(row == -1)
   {
       QMessageBox::about(NULL, "提示", "请选择一个 <font color='red'>文件</font>");
       return ;
   }

   QTableWidgetItem*item=items.at(0);
   QString name=item->text();//获取内容
   qDebug() << name;
   QFileInfo fileinfo (name);
   if(!fileinfo.isFile()) //判断是否目录
    {
       chakan(name,1);

   }
   else
   {
   QString file_name, file_path;
   QFileInfo fi;
   fi = QFileInfo(name);
   file_name = fi.fileName();  //获取文件名
   file_path = fi.absolutePath(); //获取文件路径(不包含文件名)
   qDebug() << file_name;
   qDebug() << file_path;
   //file_name=file_name+".mt";
   //qDebug() << file_name;
   QString str2 = file_path+"/"+file_name.replace(QRegExp(".mt"), "");
   qDebug() << str2;
   QString temp_key = ui->lineEdit->text(); //QString转char *
   QByteArray tem_key = temp_key.toLatin1();
   char *key = tem_key.data();
   qDebug() << key;
   QFile tmp(name);
   if(!tmp.open(QIODevice::ReadOnly))
   {
       qDebug() << "打开文件失败" << endl;
       addItemContent(row,2,"打开文件失败");
       return ;
   }
   char *a_data = new char[40];
   tmp.read(a_data,33);
   char *p=strstr(a_data,"af5179903a31bb02");
   if(NULL != p)
   {
       qDebug() << "普通解密" << endl;
       AesDecryptFile(name,str2,key,16,row);
   }
   p=strstr(a_data,"9f357e999851c9e9");
   if(NULL != p)
   {
       qDebug() << "高强度解密" << endl;
       AesXDecryptFile(name,str2,key,16,row);
   }
   p=strstr(a_data,"bfbdd049800da83d283824fc4a758440");
   if(NULL != p)
   {
       qDebug() << "企业级解密" << endl;
       Aes_S_DecryptFile(name,str2,key,16,row);
   }
   p=strstr(a_data,"d9268c456c109f1fe2811b8cb6c75998");
   if(NULL != p)
   {
       qDebug() << "国家级解密" << endl;
       Aes_SS_DecryptFile(name,str2,key,16,row);
   }
   tmp.close();
   tmp.flush();


    if(ui->checkBox_3->isChecked()) //判断解密删除文件复选框
     {

        string delfile = name.toStdString();
        ofstream in(delfile, ios::out | ios::binary | ios::trunc);
        in.write("/0", 5);
        in.write("abc123456", 5);
        in.write("", 5);
        DeleteFileA(delfile.c_str());
        in.close();
        remove(delfile.c_str());
        ofstream fout(delfile);
        if (fout) {
                        fout << "~!@#$%^&*(+_)(*&^%$@#!0123456789/*-+`QERTYUIOP{}ASDFGHJKL:" << endl;
                    fout.close();
                    }
        QFile file(name);
        if (file.exists())
        {
            file.remove();
           }
                    remove(delfile.c_str());

    }
   }




}


int MainWindow::AESencryptfile(QString in_file_path, QString out_file_path,
                   const char *rc4_encrypt_key, int encrypt_chunk_size,int hang) //普通加密
{
    QTextCodec *codec = QTextCodec::codecForName("GB2312");
    QTextCodec::setCodecForLocale(codec);
    unsigned char aes_keybuf[32];
    memset(aes_keybuf,0,sizeof(aes_keybuf));
    strcpy((char *)aes_keybuf, rc4_encrypt_key);
    AES_KEY aeskey;
    AES_set_encrypt_key(aes_keybuf, 256, &aeskey);

    char *in_data  = new char[encrypt_chunk_size + 1];
    char *out_data = new char[encrypt_chunk_size + 1];

    QFile f(in_file_path);
    QFile x(out_file_path);

        if(!f.open(QIODevice::ReadOnly))
        {
            qDebug() << "打开文件失败" << endl;
            addItemContent(hang,2,"打开文件失败");

            return 0;
        }
        if(!x.open(QIODevice::ReadWrite))
        {
            qDebug() << "打开文件失败" << endl;
            addItemContent(hang,2,"打开文件失败");

            return 0;
        }

        x.write("af5179903a31bb02",16);
        AES_encrypt((unsigned char *)"a08500733dbf0052", (unsigned char *)out_data, &aeskey);
        int len=strlen(out_data);
        char *tme0 = new char[2];
        sprintf(tme0, "%d", len);
        x.write(tme0,2);
        x.write(out_data,len);


        
        QFile bak(in_file_path+".bak");
        bak.open(QIODevice::ReadWrite);
        while(!f.atEnd())
        {
            qint64 in_1data = f.read(in_data,encrypt_chunk_size);
            if(in_1data >= 16)
            {
                AES_encrypt((unsigned char *)in_data, (unsigned char *)out_data, &aeskey);
                x.write(out_data,in_1data);

            }
            else
            {
                x.write(in_data,in_1data);

            }

            if(ui->checkBox_2->isChecked())
            {
                        bak.write(in_data,in_1data);

                    }
            else
            {
               bak.remove();


            }

            }





        f.close();
        x.close();
        addItemContent(hang,2,"加密成功");

        RELESE_ARRAY(in_data);
        RELESE_ARRAY(out_data);
        return 0;
}
int MainWindow::AesDecryptFile(QString in_file_path, QString out_file_path,
                       const char *rc4_dencrypt_key, int encrypt_chunk_size,int hang) //普通解密
{

    QTextCodec *codec = QTextCodec::codecForName("GB2312");
    QTextCodec::setCodecForLocale(codec);
    unsigned char aes_keybuf[32];
    memset(aes_keybuf,0,sizeof(aes_keybuf));
    strcpy((char *)aes_keybuf, rc4_dencrypt_key);
    AES_KEY aeskey;
    AES_set_decrypt_key(aes_keybuf, 256, &aeskey);

    char *in_data  = new char[encrypt_chunk_size + 1];
    char *out_data = new char[encrypt_chunk_size + 1];

    QFile f(in_file_path);
    QFile x(out_file_path);

        if(!f.open(QIODevice::ReadOnly))
        {
            qDebug() << "打开文件失败" << endl;
            addItemContent(hang,2,"打开文件失败");

            return 0;
        }
        if(!x.open(QIODevice::ReadWrite))
        {
            qDebug() << "打开文件失败" << endl;
            addItemContent(hang,2,"打开文件失败");

            return 0;
        }

    //用指定密钥对一段内存进行加密，结果放在outbuffer中
    f.read(16);
    int test;
    f.read(in_data,2);
    test=atoi(in_data);
    qint64 in_1data = f.read(in_data,test);
    char *a_data = new char[test+1];
    AES_decrypt((unsigned char *)in_data, (unsigned char *)a_data, &aeskey);
    char *p=strstr(a_data,"a08500733dbf0052");
    if(NULL == p)
    {
        qDebug()<<a_data;
        addItemContent(hang,2,"解密失败");
        return 0;
    }






    while(!f.atEnd())
    {
        qint64 in_1data = f.read(in_data,encrypt_chunk_size);
        if(in_1data >= 16)
        {
            AES_decrypt((unsigned char *)in_data, (unsigned char *)out_data, &aeskey);
            x.write(out_data,in_1data);

        }
        else
        {
            x.write(in_data,in_1data);


        }




    }

    f.close();
    f.flush();
    x.close();
    x.flush();
    addItemContent(hang,2,"解密成功");


    RELESE_ARRAY(in_data);
    RELESE_ARRAY(out_data);

    return 0;
}

int MainWindow::AESXencryptfile(QString in_file_path, QString out_file_path,
                   const char *rc4_encrypt_key, int encrypt_chunk_size,int hang) //高强度加密
{
    QTextCodec *codec = QTextCodec::codecForName("GB2312");
    QTextCodec::setCodecForLocale(codec);
    unsigned char aes_keybuf[32];
    memset(aes_keybuf,0,sizeof(aes_keybuf));
    strcpy((char *)aes_keybuf, rc4_encrypt_key);
    AES_KEY aeskey;
    AES_set_encrypt_key(aes_keybuf, 256, &aeskey);
    char code[64] = {0};
    int codelen = sizeof(code);
    RC4_KEY rc4_key;
    RC4_set_key(&rc4_key, strlen(rc4_encrypt_key), (unsigned char *)rc4_encrypt_key);

    char *in_data  = new char[encrypt_chunk_size + 1];
    char *out_data = new char[encrypt_chunk_size + 1];
    char *RC4_data = new char[encrypt_chunk_size + 1];

    QFile f(in_file_path);
    QFile x(out_file_path);

        if(!f.open(QIODevice::ReadOnly))
        {
            qDebug() << "打开文件失败" << endl;
            addItemContent(hang,2,"打开文件失败");

            return 0;
        }
        if(!x.open(QIODevice::ReadWrite))
        {
            qDebug() << "打开文件失败" << endl;
            addItemContent(hang,2,"打开文件失败");

            return 0;
        }
        x.write("9f357e999851c9e9",16);
        AES_encrypt((unsigned char *)"a08500733dbf0052", (unsigned char *)out_data, &aeskey);
        int len=strlen(out_data);
        char *tme0 = new char[2];
        sprintf(tme0, "%d", len);
        x.write(tme0,2);
        x.write(out_data,len);

        QFile bak(in_file_path+".bak");
        bak.open(QIODevice::ReadWrite);
        while(!f.atEnd())
        {
            qint64 in_1data = f.read(in_data,encrypt_chunk_size);
            if(in_1data >= 16)
            {
                RC4(&rc4_key, (size_t)in_1data,(unsigned char *)in_data, (unsigned char *)RC4_data);
                AES_encrypt((unsigned char *)RC4_data, (unsigned char *)out_data, &aeskey);

                x.write(out_data,in_1data);

            }
            else
            {
                x.write(in_data,in_1data);

            }

            if(ui->checkBox_2->isChecked())
            {


                        bak.write(in_data,in_1data);

                    }
            else
            {
               bak.remove();


            }

            }





        f.close();
        x.close();
        addItemContent(hang,2,"加密成功");

        RELESE_ARRAY(in_data);
        RELESE_ARRAY(out_data);
        RELESE_ARRAY(RC4_data);
        return 0;
}
int MainWindow::AesXDecryptFile(QString in_file_path, QString out_file_path,
                       const char *rc4_dencrypt_key, int encrypt_chunk_size,int hang) //高强度解密
{

    QTextCodec *codec = QTextCodec::codecForName("GB2312");
    QTextCodec::setCodecForLocale(codec);
    unsigned char aes_keybuf[32];
    memset(aes_keybuf,0,sizeof(aes_keybuf));
    strcpy((char *)aes_keybuf, rc4_dencrypt_key);
    AES_KEY aeskey;
    AES_set_decrypt_key(aes_keybuf, 256, &aeskey);
    char code[64] = {0};
    int codelen = sizeof(code);
    RC4_KEY rc4_key;
    RC4_set_key(&rc4_key, strlen(rc4_dencrypt_key), (unsigned char *)rc4_dencrypt_key);

    char *in_data  = new char[encrypt_chunk_size + 1];
    char *out_data = new char[encrypt_chunk_size + 1];
    char *RC4_data = new char[encrypt_chunk_size + 1];

    QFile f(in_file_path);
    QFile x(out_file_path);

        if(!f.open(QIODevice::ReadOnly))
        {
            qDebug() << "打开文件失败" << endl;
            addItemContent(hang,2,"打开文件失败");

            return 0;
        }
        if(!x.open(QIODevice::ReadWrite))
        {
            qDebug() << "打开文件失败" << endl;
            addItemContent(hang,2,"打开文件失败");

            return 0;
        }

    //用指定密钥对一段内存进行加密，结果放在outbuffer中


    f.read(16);
    int test;
    f.read(in_data,2);
    test=atoi(in_data);
    qint64 in_1data = f.read(in_data,test);
    char *a_data = new char[test+1];
    AES_decrypt((unsigned char *)in_data, (unsigned char *)a_data, &aeskey);
    char *p=strstr(a_data,"a08500733dbf0052");
    if(NULL == p)
    {
        qDebug()<<a_data;
        addItemContent(hang,2,"解密失败");
        return 0;
    }

    while(!f.atEnd())
    {
        qint64 in_1data = f.read(in_data,encrypt_chunk_size);
        if(in_1data >= 16)
        {

            AES_decrypt((unsigned char *)in_data, (unsigned char *)RC4_data, &aeskey);
            RC4(&rc4_key, (size_t)in_1data,(unsigned char *)RC4_data, (unsigned char *)out_data);
            x.write(out_data,in_1data);

        }
        else
        {
            x.write(in_data,in_1data);


        }




    }

    f.close();
    f.flush();
    x.close();
    x.flush();
    addItemContent(hang,2,"解密成功");


    RELESE_ARRAY(in_data);
    RELESE_ARRAY(out_data);

    return 0;
}

int MainWindow::AES_S_encryptfile(QString in_file_path, QString out_file_path,
                   const char *rc4_encrypt_key, int encrypt_chunk_size,int hang) //企业级加密
{
    QTextCodec *codec = QTextCodec::codecForName("GB2312");
    QTextCodec::setCodecForLocale(codec);
    unsigned char aes_keybuf[32];
    memset(aes_keybuf,0,sizeof(aes_keybuf));
    strcpy((char *)aes_keybuf, rc4_encrypt_key);
    AES_KEY aeskey;
    AES_set_encrypt_key(aes_keybuf, 256, &aeskey);
    char code[64] = {0};
    int codelen = sizeof(code);
    RC4_KEY rc4_key;
    QString md5;
    QString KEY=rc4_encrypt_key;
    QByteArray bb;
    bb = QCryptographicHash::hash (KEY.toUtf8(), QCryptographicHash::Md5 );
    md5.append(bb.toHex());
    QByteArray ba = md5.toLatin1();
    char *mm = ba.data();
    RC4_set_key(&rc4_key, strlen(mm), (unsigned char *)mm);

    char *in_data  = new char[encrypt_chunk_size + 1];
    char *out_data = new char[encrypt_chunk_size + 1];
    char *RC4_data = new char[encrypt_chunk_size + 1];

    QFile f(in_file_path);
    QFile x(out_file_path);

        if(!f.open(QIODevice::ReadOnly))
        {
            qDebug() << "打开文件失败" << endl;
            addItemContent(hang,2,"打开文件失败");

            return 0;
        }
        if(!x.open(QIODevice::ReadWrite))
        {
            qDebug() << "打开文件失败" << endl;
            addItemContent(hang,2,"打开文件失败");

            return 0;
        }
        x.write("bfbdd049800da83d283824fc4a758440",32);
        AES_encrypt((unsigned char *)"a08500733dbf0052", (unsigned char *)out_data, &aeskey);
        int len=strlen(out_data);
        char *tme0 = new char[2];
        sprintf(tme0, "%d", len);
        x.write(tme0,2);
        x.write(out_data,len);

        QFile bak(in_file_path+".bak");
        bak.open(QIODevice::ReadWrite);
        while(!f.atEnd())
        {
            qint64 in_1data = f.read(in_data,encrypt_chunk_size);
            if(in_1data >= 16)
            {

                AES_encrypt((unsigned char *)in_data, (unsigned char *)RC4_data, &aeskey);
                RC4(&rc4_key, (size_t)in_1data,(unsigned char *)RC4_data, (unsigned char *)out_data);


                x.write(out_data,in_1data);

            }
            else
            {
                x.write(in_data,in_1data);

            }

            if(ui->checkBox_2->isChecked())
            {


                        bak.write(in_data,in_1data);

                    }
            else
            {
               bak.remove();


            }

            }





        f.close();
        x.close();
        addItemContent(hang,2,"加密成功");

        RELESE_ARRAY(in_data);
        RELESE_ARRAY(out_data);
        RELESE_ARRAY(RC4_data);
        return 0;
}
int MainWindow::Aes_S_DecryptFile(QString in_file_path, QString out_file_path,
                       const char *rc4_dencrypt_key, int encrypt_chunk_size,int hang) //企业级解密
{

    QTextCodec *codec = QTextCodec::codecForName("GB2312");
    QTextCodec::setCodecForLocale(codec);
    unsigned char aes_keybuf[32];
    memset(aes_keybuf,0,sizeof(aes_keybuf));
    strcpy((char *)aes_keybuf, rc4_dencrypt_key);
    AES_KEY aeskey;
    AES_set_decrypt_key(aes_keybuf, 256, &aeskey);
    char code[64] = {0};
    int codelen = sizeof(code);
    RC4_KEY rc4_key;
    QString md5;
    QString KEY=rc4_dencrypt_key;
    QByteArray bb;
    bb = QCryptographicHash::hash (KEY.toUtf8(), QCryptographicHash::Md5 );
    md5.append(bb.toHex());
    QByteArray ba = md5.toLatin1();
    char *mm = ba.data();
    RC4_set_key(&rc4_key, strlen(mm), (unsigned char *)mm);

    char *in_data  = new char[encrypt_chunk_size + 1];
    char *out_data = new char[encrypt_chunk_size + 1];
    char *RC4_data = new char[encrypt_chunk_size + 1];

    QFile f(in_file_path);
    QFile x(out_file_path);

        if(!f.open(QIODevice::ReadOnly))
        {
            qDebug() << "打开文件失败" << endl;
            addItemContent(hang,2,"打开文件失败");

            return 0;
        }
        if(!x.open(QIODevice::ReadWrite))
        {
            qDebug() << "打开文件失败" << endl;
            addItemContent(hang,2,"打开文件失败");

            return 0;
        }

    //用指定密钥对一段内存进行加密，结果放在outbuffer中


    f.read(32);
    int test;
    f.read(in_data,2);
    test=atoi(in_data);
    qint64 in_1data = f.read(in_data,test);
    char *a_data = new char[test+1];
    AES_decrypt((unsigned char *)in_data, (unsigned char *)a_data, &aeskey);
    char *p=strstr(a_data,"a08500733dbf0052");
    if(NULL == p)
    {
        qDebug()<<a_data;
        addItemContent(hang,2,"解密失败");
        return 0;
    }

    while(!f.atEnd())
    {
        qint64 in_1data = f.read(in_data,encrypt_chunk_size);
        if(in_1data >= 16)
        {
            RC4(&rc4_key, (size_t)in_1data,(unsigned char *)in_data, (unsigned char *)RC4_data);
            AES_decrypt((unsigned char *)RC4_data, (unsigned char *)out_data, &aeskey);

            x.write(out_data,in_1data);

        }
        else
        {
            x.write(in_data,in_1data);


        }




    }

    f.close();
    f.flush();
    x.close();
    x.flush();
    addItemContent(hang,2,"解密成功");


    RELESE_ARRAY(in_data);
    RELESE_ARRAY(out_data);

    return 0;
}

int MainWindow::AES_SS_encryptfile(QString in_file_path, QString out_file_path,
                   const char *rc4_encrypt_key, int encrypt_chunk_size,int hang) //国家级加密
{
    QTextCodec *codec = QTextCodec::codecForName("GB2312");
    QTextCodec::setCodecForLocale(codec);
    unsigned char aes_keybuf[32];

    memset(aes_keybuf,0,sizeof(aes_keybuf));
    strcpy((char *)aes_keybuf, rc4_encrypt_key);
    AES_KEY aeskey;
    AES_set_encrypt_key(aes_keybuf, 256, &aeskey);
    RC4_KEY rc4_key;
    QString md5;
    QString KEY=rc4_encrypt_key;
    QByteArray bb;
    bb = QCryptographicHash::hash (KEY.toUtf8(), QCryptographicHash::Md5 );
    md5.append(bb.toHex());
    QByteArray ba = md5.toLatin1();
    char *mm = ba.data();
    RC4_set_key(&rc4_key, strlen(mm), (unsigned char *)mm);

    unsigned char aes_keybuf1[32];
    AES_KEY aeskey1;
    memset(aes_keybuf,0,sizeof(aes_keybuf1));
    strcpy((char *)aes_keybuf1, mm);
    AES_set_encrypt_key(aes_keybuf1, 256, &aeskey1);

    char *in_data  = new char[encrypt_chunk_size + 1];
    char *out_data = new char[encrypt_chunk_size + 1];
    char *RC4_data = new char[encrypt_chunk_size + 1];
    char *aes_data = new char[encrypt_chunk_size + 1];

    QFile f(in_file_path);
    QFile x(out_file_path);

        if(!f.open(QIODevice::ReadOnly))
        {
            qDebug() << "打开文件失败" << endl;
            addItemContent(hang,2,"打开文件失败");

            return 0;
        }
        if(!x.open(QIODevice::ReadWrite))
        {
            qDebug() << "打开文件失败" << endl;
            addItemContent(hang,2,"打开文件失败");

            return 0;
        }
        x.write("d9268c456c109f1fe2811b8cb6c75998",32);
        AES_encrypt((unsigned char *)"a08500733dbf0052", (unsigned char *)out_data, &aeskey);
        int len=strlen(out_data);
        char *tme0 = new char[2];
        sprintf(tme0, "%d", len);
        x.write(tme0,2);
        x.write(out_data,len);

        QFile bak(in_file_path+".bak");
        bak.open(QIODevice::ReadWrite);
        while(!f.atEnd())
        {
            qint64 in_1data = f.read(in_data,encrypt_chunk_size);
            if(in_1data >= 16)
            {

                AES_encrypt((unsigned char *)in_data, (unsigned char *)RC4_data, &aeskey);
                RC4(&rc4_key, (size_t)in_1data,(unsigned char *)RC4_data, (unsigned char *)aes_data);
                AES_encrypt((unsigned char *)aes_data, (unsigned char *)out_data, &aeskey1);

                x.write(out_data,in_1data);

            }
            else
            {
                x.write(in_data,in_1data);

            }

            if(ui->checkBox_2->isChecked())
            {


                        bak.write(in_data,in_1data);

                    }
            else
            {
               bak.remove();


            }

            }





        f.close();
        x.close();
        addItemContent(hang,2,"加密成功");

        RELESE_ARRAY(in_data);
        RELESE_ARRAY(out_data);
        RELESE_ARRAY(RC4_data);
        return 0;
}

int MainWindow::Aes_SS_DecryptFile(QString in_file_path, QString out_file_path,
                       const char *rc4_dencrypt_key, int encrypt_chunk_size,int hang)//国家级解密
{

    QTextCodec *codec = QTextCodec::codecForName("GB2312");
    QTextCodec::setCodecForLocale(codec);
    unsigned char aes_keybuf[32];
    unsigned char aes_keybuf1[32];
    memset(aes_keybuf,0,sizeof(aes_keybuf));
    strcpy((char *)aes_keybuf, rc4_dencrypt_key);
    AES_KEY aeskey;
    AES_set_decrypt_key(aes_keybuf, 256, &aeskey);
    RC4_KEY rc4_key;
    QString md5;
    QString KEY=rc4_dencrypt_key;
    QByteArray bb;
    bb = QCryptographicHash::hash (KEY.toUtf8(), QCryptographicHash::Md5 );
    md5.append(bb.toHex());
    QByteArray ba = md5.toLatin1();
    char *mm = ba.data();
    RC4_set_key(&rc4_key, strlen(mm), (unsigned char *)mm);
    AES_KEY aeskey1;
    memset(aes_keybuf1,0,sizeof(aes_keybuf1));
    strcpy((char *)aes_keybuf1, mm);
    AES_set_decrypt_key(aes_keybuf1, 256, &aeskey1);

    char *in_data  = new char[encrypt_chunk_size + 1];
    char *out_data = new char[encrypt_chunk_size + 1];
    char *RC4_data = new char[encrypt_chunk_size + 1];
    char *aes_data = new char[encrypt_chunk_size + 1];


    QFile f(in_file_path);
    QFile x(out_file_path);

        if(!f.open(QIODevice::ReadOnly))
        {
            qDebug() << "打开文件失败" << endl;
            addItemContent(hang,2,"打开文件失败");
            return 0;
        }
        if(!x.open(QIODevice::ReadWrite))
        {
            qDebug() << "打开文件失败" << endl;
            addItemContent(hang,2,"打开文件失败");
            return 0;
        }

    //用指定密钥对一段内存进行加密，结果放在outbuffer中


    f.read(32);
    int test;
    f.read(in_data,2);
    test=atoi(in_data);
    qint64 in_1data = f.read(in_data,test);
    char *a_data = new char[test+2];
    AES_decrypt((unsigned char *)in_data, (unsigned char *)a_data, &aeskey);
    char *p=strstr(a_data,"a08500733dbf0052");
    if(NULL == p)
    {
        qDebug()<<a_data;
        addItemContent(hang,2,"解密失败");
        //return 0;
    }

    while(!f.atEnd())
    {
        qint64 in_1data = f.read(in_data,encrypt_chunk_size);
        if(in_1data >= 16)
        {
            AES_decrypt((unsigned char *)in_data, (unsigned char *)aes_data, &aeskey1);
            RC4(&rc4_key, (size_t)in_1data,(unsigned char *)aes_data, (unsigned char *)RC4_data);
            AES_decrypt((unsigned char *)RC4_data, (unsigned char *)out_data, &aeskey);

            x.write(out_data,in_1data);

        }
        else
        {
            x.write(in_data,in_1data);
        }




    }

    f.close();
    f.flush();
    x.close();
    x.flush();
    addItemContent(hang,2,"解密成功");


    RELESE_ARRAY(in_data);
    RELESE_ARRAY(out_data);

    return 0;
}
void MainWindow::on_pushButton_clicked() // 全部加密
{
        int row_count = ui->tableWidget->rowCount(); //获取表单行数
        qDebug() << "row_count:"<< row_count ;
        if(row_count == 0)
        {
            QMessageBox::about(NULL, "提示", "请添加一个 <font color='red'>文件</font>");
            return ;
        }
        if(ui->lineEdit->text() == "")
        {
            QMessageBox::about(NULL, "提示", "请输入 <font color='red'>密钥</font>");
            return ;
        }
        if(row_count == -1)
        {
            QMessageBox::about(NULL, "提示", "请添加一个 <font color='red'>文件</font>");
            return ;
        }


        for(int i=0;i<row_count;i++)

        {
            qDebug() << "name";

               QString name = ui->tableWidget->item(i,0)->text();
               qDebug() << name;
                  QFileInfo fileinfo (name);
                  if(!fileinfo.isFile()) //判断是否目录
                   {
                      chakan(name,0);
                  }
                  else
                  {
                      QString temp_key = ui->lineEdit->text(); //QString转char *
                      QByteArray tem_key = temp_key.toLatin1();
                      char *key = tem_key.data();
                      qDebug() << key;
                      if(ui->comboBox->currentIndex() == 0) //普通加密
                      {
                          AESencryptfile(name,name+".mt",key,16,i);
                      }
                      if(ui->comboBox->currentIndex() == 1) //高强度加密
                      {
                          AESXencryptfile(name,name+".mt",key,16,i);
                      }
                      if(ui->comboBox->currentIndex() == 2) //企业级加密
                      {
                          AES_S_encryptfile(name,name+".mt",key,16,i);
                      }
                      if(ui->comboBox->currentIndex() == 3) //国家级加密
                      {
                          AES_SS_encryptfile(name,name+".mt",key,16,i);
                      }

                      if(ui->checkBox->isChecked()) //判断加密删除文件复选框
                      {

                         string delfile = name.toStdString();
                         ofstream in(delfile, ios::out | ios::binary | ios::trunc);
                         in.write("/0", 5);
                         in.write("abc123456", 5);
                         in.write("", 5);
                         DeleteFileA(delfile.c_str());
                         in.close();
                         remove(delfile.c_str());
                         ofstream fout(delfile);
                         if (fout) {
                                         fout << "~!@#$%^&*(+_)(*&^%$@#!0123456789/*-+`QERTYUIOP{}ASDFGHJKL:" << endl;
                                     fout.close();
                                     }
                         QFile file(name);
                         if (file.exists())
                         {
                             file.remove();
                            }
                                     remove(delfile.c_str());
                         }
                  }
                   qDebug()<<"路径:"<<name;

               }




}

void MainWindow::on_commandLinkButton_clicked()
{
    QDesktopServices::openUrl(QUrl("https://jq.qq.com/?_wv=1027&k=5Jw4ETM"));
}

void MainWindow::on_commandLinkButton_2_clicked()
{
   qDebug() << ui->comboBox->currentIndex();
   QString md5;
   QString pwd="中国";
   QByteArray bb;
   bb = QCryptographicHash::hash (pwd.toUtf8(), QCryptographicHash::Md5 );
   md5.append(bb.toHex());
   qDebug() << md5;



}


void MainWindow::on_pushButton_2_clicked() // 全部解密

{
    int row_count = ui->tableWidget->rowCount(); //获取表单行数
    qDebug() << "row_count:"<< row_count ;
    if(row_count == 0)
    {
        QMessageBox::about(NULL, "提示", "请添加一个 <font color='red'>文件</font>");
        return ;
    }
    if(ui->lineEdit->text() == "")
    {
        QMessageBox::about(NULL, "提示", "请输入 <font color='red'>密钥</font>");
        return ;
    }
    if(row_count == -1)
    {
        QMessageBox::about(NULL, "提示", "请添加一个 <font color='red'>文件</font>");
        return ;
    }
    for(int i=0;i<row_count;i++)

    {
        QString name = ui->tableWidget->item(i,0)->text();
        qDebug() << name;
        QFileInfo fileinfo (name);
        if(!fileinfo.isFile()) //判断是否目录
         {
            chakan(name,1);

        }
        else
        {
        QString file_name, file_path;
        QFileInfo fi;
        fi = QFileInfo(name);
        file_name = fi.fileName();  //获取文件名
        file_path = fi.absolutePath(); //获取文件路径(不包含文件名)
        qDebug() << file_name;
        qDebug() << file_path;
        //file_name=file_name+".mt";
        //qDebug() << file_name;
        QString str2 = file_path+"/"+file_name.replace(QRegExp(".mt"), "");
        qDebug() << str2;
        QString temp_key = ui->lineEdit->text(); //QString转char *
        QByteArray tem_key = temp_key.toLatin1();
        char *key = tem_key.data();
        qDebug() << key;

         AesDecryptFile(name,str2,key,16,i);
         if(ui->checkBox_3->isChecked()) //判断解密删除文件复选框
          {

             string delfile = name.toStdString();
             ofstream in(delfile, ios::out | ios::binary | ios::trunc);
             in.write("/0", 5);
             in.write("abc123456", 5);
             in.write("", 5);
             DeleteFileA(delfile.c_str());
             in.close();
             remove(delfile.c_str());
             ofstream fout(delfile);
             if (fout) {
                             fout << "~!@#$%^&*(+_)(*&^%$@#!0123456789/*-+`QERTYUIOP{}ASDFGHJKL:" << endl;
                         fout.close();
                         }
             QFile file(name);
             if (file.exists())
             {
                 file.remove();
                }
                         remove(delfile.c_str());

         }
        }






    }


}
