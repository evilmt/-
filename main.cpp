#include "mainwindow.h"
#include <QApplication>
#include <QDialog>
#include <QTextCodec>
#include <QDebug>
#include <QFile>

#pragma comment(lib,"libeay32.lib")
#pragma comment(lib,"ssleay32.lib")

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

      QTextCodec *codec = QTextCodec::codecForName("utf-8");
      QTextCodec::setCodecForLocale(codec);
      QString str_hanzi("百度"); // 汉字
      QString str_ascii("baidu.com"); // 字母
      qDebug() << QString::fromLocal8Bit("百度");
      qDebug() << str_ascii;
      qDebug() << QObject::tr("百度");
     // qDebug() << "百度";
       w.show();
    return a.exec();
}
