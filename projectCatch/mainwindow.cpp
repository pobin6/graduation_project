#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"

using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(update()));

    player = new QMediaPlayer;
    player->setMedia(QMediaContent(QUrl::fromLocalFile("/home/pobin6/文档/CPP/test3.mp4")));
    videoWidget = new QVideoWidget();
    player->setVideoOutput(videoWidget);
    ui->verticalLayout->insertWidget(0,videoWidget);
//    ui->horizontalSlider->setMaximum(player->position());
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_pTimer;
    delete player;
    delete videoWidget;
    delete detectCV;
}

//start simulation
void MainWindow::on_pushButton_clicked()
{
    m_pTimer->start(600);
    player->play();
    int x,y,width, height;
    x = videoWidget->mapToGlobal(videoWidget->pos()).x();
    y = videoWidget->mapToGlobal(videoWidget->pos()).y();
    width = videoWidget->width();
    height = videoWidget->height();
    detectCV = new DetectCV(x, y ,width, height);
}


void MainWindow::update()
{
    int x,y,width, height;
    x = videoWidget->mapToGlobal(videoWidget->pos()).x();
    y = videoWidget->mapToGlobal(videoWidget->pos()).y();
    width = videoWidget->width();
    height = videoWidget->height();
    //get screen
    detectCV->FullScreen(x, y, width, height);
    //get move
    detectCV->detectMove("../background.jpg");
    //get face
//    detectCV->detectFace();
    //get body
//    detectCV->detectBodyr();
//    ui->label->setText(QString::number(detectCV->per_num,10));
    ui->spinBox->setValue(detectCV->per_num);
}



void MainWindow::on_pushButton_2_clicked()
{
        if(isPause)
        {
                player->play();
                isPause = false;
        }
        else
        {
            player->pause();
            isPause = true;
        }
}


void MainWindow::on_pushButton_3_clicked()
{
        player->setPosition(player->position()-5000);
}

void MainWindow::on_pushButton_4_clicked()
{
        player->setPosition(player->position()+5000);
}
