#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QMediaObject>
#include <memory>
#include <QVBoxLayout>
#include "QGraphicsDropShadowEffect"
#include "QDrag"
#include "QMouseEvent"
#include "QRect"
int index,state=0;
std::unique_ptr<QMetaObject::Connection> pconn{new QMetaObject::Connection};
QMetaObject::Connection &conn = *pconn;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Beta v1.0");
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setStyleSheet("background-color: #454545; "
                        "border: 1px solid black; ");
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(9); // Устанавливаем радиус размытия
    shadowEffect->setOffset(0);     // Устанавливаем смещение тени
    ui->centralwidget->setGraphicsEffect(shadowEffect);
    m_playListModel = new QStandardItemModel(this);
    ui->playlistView->setModel(m_playListModel);
    m_playListModel->setHorizontalHeaderLabels(QStringList()  << tr("Your Playlist") << tr("File Path"));
    ui->playlistView->horizontalHeader()->setStyleSheet("color: black;");
    ui->playlistView->setStyleSheet("color: white");
    ui->playlistView->hideColumn(1);
    ui->playlistView->horizontalHeader()->setHidden(true);
    ui->playlistView->verticalHeader()->setVisible(false);
    ui->playlistView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->playlistView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->playlistView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->playlistView->horizontalHeader()->setStretchLastSection(true);
    m_player = new QMediaPlayer(this);
    m_playlist = new QMediaPlaylist(m_player);
        m_player->setPlaylist(m_playlist);
        m_player->setVolume(50);
  m_playlist->setPlaybackMode(QMediaPlaylist::Loop);

  QString path;
  QFile file("folder.txt");
  if (file.open(QIODevice::ReadWrite)){
      QTextStream stream(&file);
      stream>>path;
  }
if(!path.isEmpty()){
  QDir dir(path);
      bool ok = dir.exists();
      if (ok)
      {

dir.setNameFilters(QStringList()<<"*.mp3");
          QStringList files = dir.entryList();
          foreach (QString filePath, files) {
          QList<QStandardItem *> items;
          items.append(new QStandardItem(QDir(filePath).dirName()));
          items.append(new QStandardItem(filePath));
          m_playListModel->appendRow(items);
          m_playlist->addMedia(QUrl(path+filePath));

        }

        }}
        connect(ui->btn_stop, &QToolButton::clicked, m_player, &QMediaPlayer::stop);
        connect(ui->btn_close, SIGNAL(clicked()),this,SLOT(close()));
        connect(ui->btn_minimize, SIGNAL(clicked()),this,SLOT(showMinimized()));
        connect(ui->playlistView, &QTableView::doubleClicked, [this](const QModelIndex &index){
             m_playlist->setCurrentIndex(index.row());
         });
        conn = QObject::connect(m_playlist, &QMediaPlaylist::currentIndexChanged, [this](int index){
            ui->currentTrack->setText(m_playListModel->data(m_playListModel->index(index, 0)).toString());
        });
ui->volume->setTickInterval(1);ui->dura->setTickInterval(1);
ui->volume->setMinimum(0);ui->volume->setMaximum(100);ui->dura->setMinimum(0);
ui->volume->setSliderPosition(m_player->volume());
connect(ui->volume, SIGNAL(valueChanged(int)), m_player, SLOT(setVolume(int)));
connect(ui->volume, SIGNAL(valueChanged(int)),ui->currentvolume,SLOT(setNum(int)));
QObject::connect(m_player,&QMediaPlayer::durationChanged,ui->dura,&QSlider::setMaximum);
QObject::connect(m_player, &QMediaPlayer::positionChanged, ui->dura, static_cast<void(QSlider::*)(int)>(&QSlider::setValue));
QObject::connect(ui->dura, &QSlider::sliderMoved, m_player, &QMediaPlayer::setPosition);
}
MainWindow::~MainWindow()
{
    delete ui;
    delete m_playListModel;
       delete m_playlist;
       delete m_player;
}
void MainWindow::on_btn_add_clicked(){
    QString path = QFileDialog::getExistingDirectoryUrl(this).toString() + "/";
    path.remove(0,8);
    QFile file("folder.txt");
    if (file.open(QIODevice::ReadWrite)){
        QTextStream stream(&file);
        stream<<path;
    }
    if(!path.isEmpty()){
      QDir dir(path);
          bool ok = dir.exists();
          if (ok)
          {
              dir.setNameFilters(QStringList()<<"*.mp3");
              QStringList files = dir.entryList();
              foreach (QString filePath, files) {
                  bool check =false;
                  for(int i = 0;i<ui->playlistView->height();i++){
               if(filePath ==ui->playlistView->rowAt(i) ){check=true;}}
                  if(check==false){
              QList<QStandardItem *> items;
              items.append(new QStandardItem(QDir(filePath).dirName()));
              items.append(new QStandardItem(filePath));
              m_playListModel->appendRow(items);
              m_playlist->addMedia(QUrl(path+filePath));}check=false;
}
            }
}}

double posit;
void MainWindow::on_Loop_clicked()
{
    if(ui->Loop->isChecked()){
     m_playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);

    }else {
       m_playlist->setPlaybackMode(QMediaPlaylist::Loop);
    }
}
void MainWindow::on_btn_play_pressed()
{
    if(state==0){
 m_player->play();state=1;
 QPixmap pixmap(":/icon/pause.png");
 QIcon ButtonIcon(pixmap);
    ui->btn_play->setIcon(ButtonIcon);
    }else{m_player->pause();state=0;
        QPixmap pixmap(":/icon/play.png");
        QIcon ButtonIcon(pixmap);
           ui->btn_play->setIcon(ButtonIcon);
    }
}
void MainWindow::on_Rand_clicked()
{
    if(ui->Rand->isChecked()){
        m_playlist->setPlaybackMode(QMediaPlaylist::Random);
    }else{  m_playlist->setPlaybackMode(QMediaPlaylist::Loop);}
}
void MainWindow::on_dura_valueChanged(int value)
{
    value=value/1000;
     int a=0,b=value,sec,min;
    if(value>=60){
        a=value/60;
        b=value-a*60;
    }
    if(b<10){
          ui->currentdura->setText(QString::number(a)+":0"+QString::number(b));
    }else{
    ui->currentdura->setText(QString::number(a)+":"+QString::number(b));
}
    min=(int)m_player->duration()/60000;
    sec=((int)m_player->duration()/1000)-min*60;
    if(sec<10){
          ui->maxdura->setText(QString::number(min)+":0"+QString::number(sec));
    }else{
ui->maxdura->setText(QString::number(min)+":"+QString::number(sec));}
}

void MainWindow::on_action_triggered()
{
    close();
}
void MainWindow::on_btn_maximize_clicked()
{
if(!this->isMaximized()){
    showMaximized();
}else{showNormal();}
}
void MainWindow::mousePressEvent(QMouseEvent *event) {
    m_nMouseClick_X_Coordinate = event->x();
    m_nMouseClick_Y_Coordinate = event->y();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
        this->showNormal();
    move(event->globalX()-m_nMouseClick_X_Coordinate,event->globalY()-m_nMouseClick_Y_Coordinate);

}
    bool control = true;
void MainWindow::on_volume_valueChanged(int value)
{
    if(m_player->isMuted()){
        QPixmap pixmap(":/icon/volume0.png");
        QIcon ButtonIcon(pixmap);
        ui->btn_volume->setIcon(pixmap);
    }else{
    if(value>65){
        QPixmap pixmap(":/icon/volume3.png");
        QIcon ButtonIcon(pixmap);
        ui->btn_volume->setIcon(pixmap);
    }else if(value>33){
        QPixmap pixmap(":/icon/volume2.png");
        QIcon ButtonIcon(pixmap);
        ui->btn_volume->setIcon(pixmap);
    }else if(value>0){
        QPixmap pixmap(":/icon/volume1.png");
        QIcon ButtonIcon(pixmap);
        ui->btn_volume->setIcon(pixmap);
    }else{
        QPixmap pixmap(":/icon/volume0.png");
        QIcon ButtonIcon(pixmap);
        ui->btn_volume->setIcon(pixmap);
    }
}}

void MainWindow::on_btn_volume_clicked()
{
    if(control==true){
        m_player->setMuted(control);control=false;
        QPixmap pixmap(":/icon/volume0.png");
        QIcon ButtonIcon(pixmap);
        ui->btn_volume->setIcon(pixmap);
    }else {
        m_player->setMuted(control);control=true;
        if(ui->volume->value()>65){
            QPixmap pixmap(":/icon/volume3.png");
            QIcon ButtonIcon(pixmap);
            ui->btn_volume->setIcon(pixmap);
        }else if(ui->volume->value()>33){
            QPixmap pixmap(":/icon/volume2.png");
            QIcon ButtonIcon(pixmap);
            ui->btn_volume->setIcon(pixmap);
        }else if(ui->volume->value()>0){
            QPixmap pixmap(":/icon/volume1.png");
            QIcon ButtonIcon(pixmap);
            ui->btn_volume->setIcon(pixmap);
        }else{
            QPixmap pixmap(":/icon/volume0.png");
            QIcon ButtonIcon(pixmap);
            ui->btn_volume->setIcon(pixmap);
        }
    }
}

void MainWindow::on_btn_next_clicked()
{
    if(ui->Loop->isChecked()){
        if(ui->Rand->isChecked()){ m_playlist->setPlaybackMode(QMediaPlaylist::Random);}else{
        m_playlist->setPlaybackMode(QMediaPlaylist::Loop);}
m_playlist->next();
m_playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    }else {m_playlist->next();}
}

void MainWindow::on_btn_previous_clicked()
{
    if(ui->Loop->isChecked()){
        if(ui->Rand->isChecked()){ m_playlist->setPlaybackMode(QMediaPlaylist::Random);}else{
        m_playlist->setPlaybackMode(QMediaPlaylist::Loop);}
m_playlist->previous();
m_playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    }else {m_playlist->previous();}
}
