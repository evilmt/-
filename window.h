#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLayout>
#include <QLabel>
#include <QComboBox>
#include <QMessageBox>
#include <QDialog>


class Window : public QMainWindow
{
    Q_OBJECT

public:
    Window(QWidget *parent = NULL):QMainWindow(parent)
    {
        QGridLayout *gridLayout = new QGridLayout;
        gridLayout->setColumnStretch(0, 1);
        gridLayout->setColumnStretch(1, 3);

        gridLayout->setMargin(10);

        QLabel *lbl_caption = new QLabel(QWidget::tr("Sex:"));
        cbo_sex = new QComboBox();

        cbo_sex->addItem(QWidget::tr("male"));
        cbo_sex->addItem(QWidget::tr("female"));
        cbo_sex->insertItem(2, tr("Insert item"));
        cbo_sex->insertSeparator(2);

        gridLayout->addWidget(lbl_caption, 0, 0);
        gridLayout->addWidget(cbo_sex, 0, 1);

        QHBoxLayout *bomLayout = new QHBoxLayout;
        QPushButton *btn = new QPushButton(QWidget::tr("Select"));
        bomLayout->addStretch();
        bomLayout->addWidget(btn);
        bomLayout->addStretch();

        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->addLayout(gridLayout);
        mainLayout->addLayout(bomLayout);

        QWidget *mainWidget = new QWidget;
        mainWidget->setLayout(mainLayout);

        setCentralWidget(mainWidget);

        connect(cbo_sex, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(on_sel_sex(const QString &)));
        connect(btn, SIGNAL(clicked()), this, SLOT(on_click_sel()));
    }

private:
    QComboBox *cbo_sex;

private slots:
    void on_sel_sex(const QString &text)
    {
        QString str;
        str = "You select " + text;
        QMessageBox::information(this, tr("Info"), str);
    }

    void on_click_sel()
    {
        QString str;
        str = "You select " + cbo_sex->currentText();
        QMessageBox::information(this, tr("Info"), str);
    }

};


#endif
