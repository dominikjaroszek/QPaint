#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QInputDialog>
#include <QColorDialog>
#include <QPainter>
#include <QImage>
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QAction>
#define DEFUALT_SIZE 5;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mImage = new QImage(QApplication::desktop()->size(),
            QImage::Format_ARGB32_Premultiplied);
    mPainter = new QPainter(mImage);
    mEnabled = false;
    mColor = QColor(Qt::black);
    mSize = DEFUALT_SIZE;
    //painters = new QPainter ;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mPainter;
    delete mImage;

}


void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.fillRect(mImage->rect(),Qt::white);
    painter.drawImage(0, 0, *mImage);
    e->accept();
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
mEnabled = true;
mBegin = e->pos();
e->accept();
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
if(!mEnabled)
{e->accept();
    return;
}
QPen pen(mColor);
pen.setCapStyle(Qt::RoundCap);
pen.setWidth(mSize);
mEnd = e->pos();
    mPainter->setPen(pen);
    mPainter->drawLine(mBegin, mEnd);
    mBegin = mEnd;
    update();
    e->accept();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
 mEnabled = false;
 e->accept();
}





void MainWindow::on_actionRozmiar_triggered()
{
    mSize = QInputDialog::getInt(this,"Edytowanie rozmiaru",
                                 "Rozmiar: " ,5,1);
}


void MainWindow::on_actionKolor_triggered()
{
    mColor = QColorDialog::getColor(Qt::black,this,"Edytowanie koloru");
}


void MainWindow::on_actionGumka_triggered()
{
    mColor = Qt::white;
}


void MainWindow::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save Address Book"), "",
            tr("PNG (*.png);;JPG (*.jpg) ;;All Files (*)"));
    if (fileName.isEmpty())
           return;
       else {
           QFile file(fileName);
           if (!file.open(QIODevice::WriteOnly)) {
               QMessageBox::information(this, tr("Unable to open file"),
                   file.errorString());
               return;
           }
    }
}


void MainWindow::on_actionLoad_triggered()
{
}


void MainWindow::on_actionUndo_triggered()
{

}


void MainWindow::on_actionOpen_triggered()
{
    QPainter painter(this);
        QString filename = QFileDialog::getOpenFileName(this,tr("Choose"),"",tr("Images (*.png *.jpg *.jpeg"));
        if(QString::compare(filename,QString())!= 0)
        {
            QImage image;
                    bool valid = image.load(filename);
            if(valid)
          //  ui->painter->setPixmap(QPixmap::fromImage(image));
            painter.fillRect(mImage->rect(),QPixmap::fromImage(image));
            painter.drawImage(0, 0, *mImage);

    }
}

