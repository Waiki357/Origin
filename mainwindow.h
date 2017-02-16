#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QPixmap>
#include <QMessageBox>
#include <QStringList>
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QMediaPlaylist>
#include <QCamera>
#include <QCameraInfo>
#include <QLabel>
#include <QMediaMetaData>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QMediaPlayer   *mediaPlayer_;
    QCamera        *camera;
    QLabel         *locationLabel;

private slots:
    void onOpen();
    void onSeek();
    void onDurationChanged(qint64 duration);
    void onPositionChanged(qint64 position);
    void onVolumeChanged(int volume);
    void onMaximized();
    void on_Play_pressed();
    void on_Pause_pressed();
    void on_Stop_pressed();
    void on_Rewind_clicked();
    void on_Forward_clicked();
    void on_actionSalir_triggered();
    void on_WebCam_clicked();
    void errores();
    void on_actionPantalla_Completa_triggered();
    void errores_camara();
    void parada();
};

#endif // MAINWINDOW_H
