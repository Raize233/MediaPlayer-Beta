#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QStandardItemModel>
#include <QMediaPlayer>
#include <QMediaPlaylist>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_btn_add_clicked();
    void on_Loop_clicked();

    void on_btn_play_pressed();

    void on_Rand_clicked();

    void on_dura_valueChanged(int value);

    void on_action_triggered();

    void on_btn_maximize_clicked();

    void on_volume_valueChanged(int value);

    void on_btn_volume_clicked();

    void on_btn_next_clicked();

    void on_btn_previous_clicked();

private:
    void mousePressEvent(QMouseEvent *event) override;
      void mouseMoveEvent(QMouseEvent *event) override;
    Ui::MainWindow *ui;
    QStandardItemModel  *m_playListModel;
       QMediaPlayer        *m_player;
       QMediaPlaylist      *m_playlist;
QMediaPlaylist      *m_playlist2;
int m_nMouseClick_X_Coordinate;
    int m_nMouseClick_Y_Coordinate;
};
#endif // MAINWINDOW_H
