#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "image.h"
#include "balldetector.h"
#include "streams/singleimageloader.h"
#include "streams/singleimageloader.h"
#include "debugger.h"

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
    void on_stream_selector_currentIndexChanged(const QString &arg1);
    void on_btn_open_clicked();
    void on_btn_reload_clicked();
    void on_btn_previous_clicked();
    void on_btn_pause_clicked();
    void on_btn_play_clicked();
    void on_btn_forward_clicked();
    void on_frame_ready();
    void on_chk_ri_clicked();

    void on_chk_ca_clicked();

    void on_chk_edg_clicked();

    void on_chk_frht_clicked();

private:
    Ui::MainWindow *ui;
    MVision::BallDetector ballDetector;
    StreamLoader* streamLoader;
    Debugger debugger;

    void update_contexts();
};

#endif // MAINWINDOW_H
