#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QPainter;
class QImage;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

   bool openImage(const QString &fileName);
 bool isModified() const { return modified; }
private slots:
    void on_actionQuit_triggered();
    void on_actionRozmiar_triggered();

    void on_actionKolor_triggered();

    void on_actionGumka_triggered();

    bool on_actionSave_triggered();

    void on_actionUndo_triggered();

    void on_actionOpen_triggered();

    void on_actionPen_triggered();

    void on_actionPrint_triggered();

    void on_actionNew_triggered();

    void on_actionAbout_At_triggered();

    void on_actionAbout_Qt_triggered();


protected:
    void paintEvent(QPaintEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;

private:
    void resizeImage(QImage *image, const QSize &newSize);
 QImage image;
 bool modified;
    Ui::MainWindow *ui;
    QPainter *mPainter;
    QImage *mImage;
    QPoint mBegin;
    QPoint mEnd;
    bool mEnabled;
    int mSize;
    QColor mColor;
     QColor mColor2;
    void saveToFile();
    void loadFromFile();
    bool maybeSave();

};
#endif // MAINWINDOW_H
