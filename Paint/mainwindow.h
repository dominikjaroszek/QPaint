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

private slots:
    void on_actionQuit_triggered();
    void on_actionRozmiar_triggered();

    void on_actionKolor_triggered();

    void on_actionGumka_triggered();

    void on_actionSave_triggered();

    void on_actionLoad_triggered();

    void on_actionUndo_triggered();

    void on_actionOpen_triggered();

protected:
    void paintEvent(QPaintEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;

private:
    Ui::MainWindow *ui;
    QPainter *mPainter;
    QImage *mImage;
    QPoint mBegin;
    QPoint mEnd;
    bool mEnabled;
    int mSize;
    QColor mColor;
    void saveToFile();
    void loadFromFile();
};
#endif // MAINWINDOW_H
