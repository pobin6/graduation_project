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
    player->setMedia(QMediaContent(QUrl::fromLocalFile("/home/pobin6/文档/CPP/test.mp4")));
    videoWidget = new QVideoWidget();
    player->setVideoOutput(videoWidget);
    ui->verticalLayout->insertWidget(0,videoWidget);

    detectCV = new DetectCV();
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
    m_pTimer->start(1000);
    player->play();
}


void MainWindow::update()
{
    int x,y,width,height;
    x = videoWidget->mapToGlobal(videoWidget->pos()).x();
    y = videoWidget->mapToGlobal(videoWidget->pos()).y();
//    width = videoWidget->width();
//    height = videoWidget->height();
    //get screen
    detectCV->FullScreen(x+150, y+50, 248, 385);
    //get move
//    detectCV->detectMove("../../background.jpg");
    //get face
//    detectCV->detectFace();
    //get body
//    detectCV->detectBody();
    int p = ui->label->text().toInt();
    p++;
    cout << p << endl;
    ui->label->setText(QString::number(p,10));
}


