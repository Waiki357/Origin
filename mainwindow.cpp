#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mediaPlayer_  = new QMediaPlayer(this);
    camera = new QCamera;

    //Configuracion video y control de volumen
    mediaPlayer_->setVideoOutput(ui->VideoPlayer);
    mediaPlayer_->setVolume(100);
    ui->VideoPlayer->setAspectRatioMode(Qt::KeepAspectRatio);
    ui->VolumenSlider->setRange(0, 100);
    ui->VolumenSlider->setSliderPosition(100);


    //Iconos Botones
    ui->Pause->setIcon(QIcon(QPixmap(":/images/pause.png")));
    ui->Play->setIcon(QIcon(QPixmap(":/images/play.png")));
    ui->Stop->setIcon(QIcon(QPixmap(":/images/stop.png")));
    ui->Rewind->setIcon(QIcon(QPixmap(":/images/rewind.jpg")));
    ui->Forward->setIcon(QIcon(QPixmap(":/images/forward.jpg")));
    ui->WebCam->setIcon(QIcon(QPixmap(":/images/webcam.png")));

    //Configuracion botones
    ui->Play->setEnabled(false);
    ui->Stop->setEnabled(false);
    ui->Pause->setEnabled(false);
    ui->Rewind->setEnabled(false);
    ui->Forward->setEnabled(false);

    connect(ui->actionAbrir,    SIGNAL(triggered()),             this,          SLOT(onOpen()));
    connect(ui->Video_Slider,   SIGNAL(sliderReleased()),        this,          SLOT(onSeek()));
    connect(mediaPlayer_,       SIGNAL(durationChanged(qint64)), this,          SLOT(onDurationChanged(qint64)));
    connect(mediaPlayer_,       SIGNAL(positionChanged(qint64)), this,          SLOT(onPositionChanged(qint64)));
    connect(ui->VolumenSlider,  SIGNAL(sliderMoved(int)),        this,          SLOT(onVolumeChanged(int)));
    connect(mediaPlayer_,       SIGNAL(error(QMediaPlayer::Error)), this,       SLOT(errores()));
    connect(camera,             SIGNAL(error(QCamera::Error)),      this,       SLOT(errores_camara()));
    connect(mediaPlayer_,       SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(parada()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onOpen()
{
    ui->Play->setEnabled(true);
    ui->Stop->setEnabled(false);
    ui->Pause->setEnabled(false);
    ui->Rewind->setEnabled(false);
    ui->Forward->setEnabled(false);
    ui->Video_Slider->setEnabled(true);

    if(camera->state()==QCamera::ActiveState){
        camera->stop();
        mediaPlayer_->setVideoOutput(ui->VideoPlayer);
    }

    QString fileName = QFileDialog::getOpenFileName(this,
                                            tr("Abrir archivo"));
    if (fileName != "") {
        qDebug() << fileName;
        locationLabel = new QLabel;
        mediaPlayer_->setMedia(QUrl::fromLocalFile(fileName));
        if(mediaPlayer_->isMetaDataAvailable()){
            QString Title = mediaPlayer_->metaData(QMediaMetaData::Title).toString();
            if (Title.isEmpty()){
                locationLabel->setText(fileName);
            }
            else{
                locationLabel->setText(Title);
            }

        }
        else{
            locationLabel->setText(fileName);
        }
        locationLabel->setAlignment(Qt::AlignLeft);
        locationLabel->setMinimumSize(locationLabel->sizeHint());

        ui->statusBar->addWidget(locationLabel);
        ui->statusBar->show();
    }

}

void MainWindow::onSeek()
{
    mediaPlayer_->setPosition(ui->Video_Slider->sliderPosition());
}

void MainWindow::onDurationChanged(qint64 duration)
{
    ui->Video_Slider->setRange(0, duration);
}

void MainWindow::onMaximized(){

    this->showMaximized();
}

void MainWindow::onPositionChanged(qint64 position)
{
    ui->Video_Slider->setSliderPosition(position);
}

void MainWindow::onVolumeChanged(int volume)
{
    mediaPlayer_->setVolume(volume);
}

void MainWindow::on_Play_pressed()
{
    mediaPlayer_->play();
    mediaPlayer_->setPlaybackRate(1);
    ui->Play->setEnabled(false);
    ui->Stop->setEnabled(true);
    ui->Pause->setEnabled(true);
    ui->Rewind->setEnabled(true);
    ui->Forward->setEnabled(true);
}

void MainWindow::on_Pause_pressed()
{
    mediaPlayer_->pause();
    ui->Play->setEnabled(true);
    ui->Stop->setEnabled(true);
    ui->Pause->setEnabled(false);
}

void MainWindow::on_Stop_pressed()
{
    mediaPlayer_->stop();
    ui->Play->setEnabled(true);
    ui->Stop->setEnabled(false);
    ui->Pause->setEnabled(false);
}

void MainWindow::on_Rewind_clicked()
{
    mediaPlayer_->setPosition(mediaPlayer_->position()-2000);
    ui->Play->setEnabled(true);
}

void MainWindow::errores(){

     QMessageBox::warning(this,"Error",mediaPlayer_->errorString());

     ui->Play->setEnabled(false);
     ui->Stop->setEnabled(false);
     ui->Pause->setEnabled(false);
     ui->Rewind->setEnabled(false);
     ui->Forward->setEnabled(false);

}

void MainWindow::errores_camara(){

     QMessageBox::warning(this,"Error",camera->errorString());

     ui->Play->setEnabled(false);
     ui->Stop->setEnabled(false);
     ui->Pause->setEnabled(false);
     ui->Rewind->setEnabled(false);
     ui->Forward->setEnabled(false);

}

void MainWindow::on_Forward_clicked()
{
   mediaPlayer_->setPlaybackRate(1.5);
   ui->Play->setEnabled(true);
}

void MainWindow::on_actionSalir_triggered()
{
    qApp->exit();
}

void MainWindow::parada(){

    if(mediaPlayer_->mediaStatus()==QMediaPlayer::EndOfMedia){
        ui->Play->setEnabled(true);
        ui->Stop->setEnabled(false);
        ui->Pause->setEnabled(false);
        ui->Rewind->setEnabled(false);
        ui->Forward->setEnabled(false);
        mediaPlayer_->setPosition(0);
    }

}

void MainWindow::on_WebCam_clicked()
{


    if(mediaPlayer_->state()==QMediaPlayer::PlayingState){
        mediaPlayer_->stop();
    }

    //Configuracion WebCam

    if(camera->state()==QCamera::ActiveState){
        camera->stop();
        mediaPlayer_->setVideoOutput(ui->VideoPlayer);
        ui->Play->setEnabled(true);
        ui->Pause->setEnabled(false);
        ui->Stop->setEnabled(false);
        ui->Rewind->setEnabled(false);
        ui->Forward->setEnabled(false);
    }

    else{

        ui->Play->setEnabled(false);
        ui->Pause->setEnabled(false);
        ui->Stop->setEnabled(false);
        ui->Rewind->setEnabled(false);
        ui->Forward->setEnabled(false);
        QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
        foreach (const QCameraInfo &cameraInfo, cameras)
            qDebug() << cameraInfo.deviceName();

        if(cameras.empty()){
            QMessageBox::warning(this,"Error","No hay cámaras disponibles");
            ui->Play->setEnabled(true);
            ui->Pause->setEnabled(false);
            ui->Stop->setEnabled(false);
            ui->Rewind->setEnabled(false);
            ui->Forward->setEnabled(false);
        }

        else{
            camera->setViewfinder(ui->VideoPlayer);
            camera->setCaptureMode(QCamera::CaptureViewfinder);
            camera->start();
        }
    }

}

void MainWindow::on_actionPantalla_Completa_triggered()
{
    this->showMaximized();
//    if (ui->VideoPlayer->isFullScreen()) {
//        ui->VideoPlayer->setFullScreen(false);
//    }
//    else {
//        ui->VideoPlayer->setFullScreen(true);
//    }
}

