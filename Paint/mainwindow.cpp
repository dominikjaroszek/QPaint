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
#include <QColor>
#include <QPrinter>
#include <QImage>
#include <QPrintDialog>
#include <QPoint>

#include <QWidget>
#define DEFUALT_SIZE 5;
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


    modified = false;
 setWindowTitle(tr("Paint"));
    mImage->fill(Qt::white);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mPainter;
    delete mImage;

}


void MainWindow::on_actionQuit_triggered()
{
    if (maybeSave())
    QApplication::quit();
}

bool MainWindow::maybeSave()

{



    if (isModified()) {

       QMessageBox::StandardButton ret;




       ret = QMessageBox::warning(this, tr("Paint"),

                          tr("Rysunek został zmodyfikowany.\n"

                             "Czy chcesz zapisać zmiany?"),

                          QMessageBox::Save | QMessageBox::Discard

                          | QMessageBox::Cancel);





        if (ret == QMessageBox::Save) {

          return  on_actionSave_triggered();





        } else if (ret == QMessageBox::Cancel) {

            return false;

        }

    }

    return true;

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
modified = true;
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
    mColor2 = mColor;
    mColor = Qt::white;
}


bool MainWindow::on_actionSave_triggered()
{

    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Zapisz jako"), "",
            tr("PNG (*.png);;JPG (*.jpg) ;;All Files (*)"));
    if (fileName.isEmpty())
            return 0;
       else {
           QFile file(fileName);

           if (!file.open(QIODevice::WriteOnly)) {
               QMessageBox::information(this, tr("Plik niemożliwy do otwarcia"),
                   file.errorString());
               modified = false;
               return 1; }
    }




    mImage->save(fileName);
    return 1;
}



void MainWindow::on_actionUndo_triggered()
{

}

bool MainWindow::openImage(const QString &fileName)
{



     if (maybeSave()) {
    QImage loadedImage;





    if (!loadedImage.load(fileName))

        return false;



    QSize newSize = loadedImage.size().expandedTo(size());


    mImage = &loadedImage;

    modified = false;

    update();


}
     return true;
}


void MainWindow::on_actionPen_triggered()
{
 mColor = mColor2;
}


void MainWindow::on_actionPrint_triggered()
{





        QPrinter printer(QPrinter::HighResolution);


        QPrintDialog printDialog(&printer, this);
        if (printDialog.exec() == QDialog::Accepted) {
            QPainter painter(&printer);
            QRect rect = painter.viewport();
            QSize size = mImage->size();
            size.scale(rect.size(), Qt::KeepAspectRatio);
            painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
            painter.setWindow(mImage->rect());
            painter.drawImage(0, 0, *mImage);
        }

    }



void MainWindow::on_actionNew_triggered()
{
     if (maybeSave()) {
     mImage->fill(Qt::white);
      modified = true;
     update();
     }
}


void MainWindow::on_actionOpen_triggered()
{


 if (maybeSave()) {
      QPainter painter(this);
          QString filename = QFileDialog::getOpenFileName(this,tr("Wybierz"),"",tr("PNG (*.png);;JPG (*.jpg) ;;All Files (*)"));
          if(QString::compare(filename,QString())!= 0)
          {
              QImage image;
                      bool valid = mImage->load(filename);
              if(valid){
            //  ui->painter->setPixmap(QPixmap::fromImage(image));
              painter.fillRect(mImage->rect(),QPixmap::fromImage(*mImage));
             painter.drawImage(0, 0, *mImage);
             // mImage->load(filename);//tu problem z odczytem
          mEnabled = true;
         // mImage->fill(QPixmap::fromImage(*mImage));
          modified = false;
          update();
              }
}
      }
}


void MainWindow::on_actionAbout_At_triggered()
{






        QMessageBox::about(this, tr("About Paint"),

                tr("<p><b>Paint</b> aplikacja do rysowania</p>"));


}


void MainWindow::on_actionAbout_Qt_triggered()
{
    QApplication::aboutQt();
}




