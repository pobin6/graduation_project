#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <detectcv.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void update();  //超时处理函数

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *m_pTimer;
    QMediaPlayer *player;
    QVideoWidget *videoWidget;
    DetectCV *detectCV;
    bool isPause = false;
};

#endif // MAINWINDOW_H
