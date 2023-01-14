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
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mImage = QImage(QApplication::desktop()->size(),QImage::Format_ARGB32_Premultiplied);
    mColor = QColor(Qt::black);
    mSize = 5;
    modified = false;
    setWindowTitle(tr("Paint"));
    mImage.fill(Qt::white);
    ifco1 = false;
    ifco2 = false;
    style = Qt::RoundCap;
     ui->menuAbout->setCursor(Qt::PointingHandCursor);
     ui->menuFile->setCursor(Qt::PointingHandCursor);
     ui->menuShapes->setCursor(Qt::PointingHandCursor);
     ui->menuRysowanie->setCursor(Qt::PointingHandCursor);
     ui->toolBar->setCursor(Qt::PointingHandCursor);
     mColorBrush =Qt::white;


}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_actionQuit_triggered()
{
    if (maybeSave())
    QApplication::quit();
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
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
void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mBegin = e->pos();
        scribbling = true;
        mtmpImage = mImage.copy();
    }
}
void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    if ((e->buttons() & Qt::LeftButton) && scribbling){
        drawLineTo(e->pos());
        if(ifco6)
        mImage = mtmpImage.copy();
    }
}
void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton && scribbling) {
           drawLineTo(e->pos());
           scribbling = false;
       }
}
void MainWindow::drawLineTo(const QPoint &mEnd)
{
    QPainter painter(&mImage);
    QPen pen;

    pen.setStyle(Qt::SolidLine);
    pen.setWidth(mSize);
    pen.setColor(mColor);
    pen.setCapStyle(style);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);


    if(ifco1)
    {
        painter.setBrush(mColorBrush);
        painter.drawEllipse(mBegin, 40, 80 );

     }
    else if(ifco2)
    {
        update();
        painter.drawLine(mBegin,mEnd);  
        mBegin = mEnd;

    }
     else if(ifco3)
    {
    painter.setBrush(mColorBrush);
    painter.drawRect(mBegin.x(),mBegin.y(),85,50);

    }
    else if(ifco4)
    {
         painter.setBrush(mColorBrush);
        painter.drawRect(mBegin.x(),mBegin.y(),50,50);

    }
    else if(ifco5)
    {
         painter.setBrush(mColorBrush);
        QPointF *points = new QPointF[3];
        points[0] = QPointF(mBegin.x(),mBegin.y()),
        points[1] = QPointF(mBegin.x()-100,mBegin.y()+80),
        points[2] = QPointF(mBegin.x()+100,mBegin.y()+80),
        painter.drawPolygon(points,3);
    }
    else if(ifco6){
        painter.drawLine(mBegin,mEnd);
        update();
    }
    modified = true;
    update();
}
void MainWindow::on_actionRozmiar_triggered()
{
    mSize = QInputDialog::getInt(this,"Edytowanie rozmiaru","Rozmiar: " ,5,1);
}

void MainWindow::on_actionKolor_triggered()
{
    mColor = QColorDialog::getColor(Qt::black,this,"Edytowanie koloru");
     mColor2 = mColor ;
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
    mImage.save(fileName);
    return 1;
}

void MainWindow::on_actionPrint_triggered()
{
        QPrinter printer(QPrinter::HighResolution);
        QPrintDialog printDialog(&printer, this);
        if (printDialog.exec() == QDialog::Accepted) {
            QPainter painter(&printer);
            QRect rect = painter.viewport();
            QSize size = mImage.size();
            size.scale(rect.size(), Qt::KeepAspectRatio);
            painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
            painter.setWindow(mImage.rect());
            painter.drawImage(0, 0, mImage);
        }
    }
void MainWindow::on_actionNew_triggered()
{
     if (maybeSave()) {
     mImage.fill(Qt::white);
     modified = true;
     update();
     }
}
void MainWindow::resizeImage(QImage *image, const QSize &newSize)
{
    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}
void MainWindow::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.fillRect(mImage.rect(),Qt::white);
    painter.drawImage(0, 0, mImage);
    e->accept();
}

void MainWindow::on_actionAbout_At_triggered()
{
    QMessageBox::about(this, tr("About Paint"),tr("<p><b>Paint</b> aplikacja do rysowania</p>"));
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QApplication::aboutQt();
}

void MainWindow::on_actionGumka_triggered()
{
    QCursor cursorTarget = QCursor(QPixmap(":/new/prefix1/icons8-eraser-32.png"));
     ui->centralwidget->setCursor(cursorTarget);
     update();
    style = Qt::SquareCap;
    mColor2 = mColor;
    mColor = Qt::white;
    ifco1 = false;
     ifco2 = true;
     ifco3 = false;
      ifco4 = false;
       ifco5 = false;
        ifco6 = false;
}
void MainWindow::on_actionPen_triggered()
{
    style = Qt::RoundCap;
     ui->centralwidget->setCursor(Qt::CrossCursor);
 mColor = mColor2;
 ifco1 = false;
  ifco2 = true;
  ifco3 = false;
   ifco4 = false;
    ifco5 = false;
     ifco6 = false;
}
void MainWindow::on_actionelipsa_triggered()
{
    style = Qt::RoundCap;
     ui->centralwidget->setCursor(Qt::CrossCursor);
     mColor = mColor2;
    ifco1 = true;
     ifco2 = false;
     ifco3 = false;
      ifco4 = false;
       ifco5 = false;
        ifco6 = false;
}


void MainWindow::on_actionprostokat_triggered()
{
    style = Qt::RoundCap;
     ui->centralwidget->setCursor(Qt::CrossCursor);
     mColor = mColor2;
    ifco1 = false;
     ifco2 = false;
     ifco3 = true;
      ifco4 = false;
       ifco5 = false;
        ifco6 = false;
}


void MainWindow::on_actionkwadrat_triggered()
{
    style = Qt::RoundCap;
     ui->centralwidget->setCursor(Qt::CrossCursor);
     mColor = mColor2;
    ifco1 = false;
     ifco2 = false;
     ifco3 = false;
      ifco4 = true;
       ifco5 = false;
        ifco6 = false;
}


void MainWindow::on_actiontrojkat_triggered()
{
    style = Qt::RoundCap;
     ui->centralwidget->setCursor(Qt::CrossCursor);
     mColor = mColor2;
    ifco1 = false;
     ifco2 = false;
     ifco3 = false;
      ifco4 = false;
       ifco5 = true;
       ifco6 = false;
}
void MainWindow::on_actionLinia_triggered()
{
    style = Qt::RoundCap;
     ui->centralwidget->setCursor(Qt::CrossCursor);
     mColor = mColor2;
     ifco1 = false;
      ifco2 = false;
      ifco3 = false;
       ifco4 = false;
        ifco5 = false;
        ifco6 = true;
}
void MainWindow::on_actionOpen_triggered()
{
    if(maybeSave()){
    QPaintEvent *e;
      QImage loadedImage;
      QString fileName = QFileDialog::getOpenFileName(this,tr("Open File"), QDir::currentPath());
      if (!fileName.isEmpty())
      {
      loadedImage.load(fileName);
      QSize newSize = loadedImage.size().expandedTo(size());
      resizeImage(&loadedImage, newSize);
      mImage = loadedImage;
      modified = false;
      e->accept();
      update();
      }
    }
}

void MainWindow::on_actionwypelnienie_figury_triggered()
{
    mColorBrush=QColorDialog::getColor(Qt::black,this,"Edytowanie koloru");
}

