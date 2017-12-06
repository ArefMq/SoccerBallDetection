#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "image.h"
#include "balldetector.h"

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
    void on_btn_referesh_clicked();
    void on_btn_next_clicked();
    void on_radio_image_clicked();
    void on_radio_edge_clicked();

private:
    Ui::MainWindow *ui;
    QImage monitor, edgeMonitor;

    MVision::BallDetector ballDetector;

    bool isEdgeImageSelected() const;
    void setImageToMonitor();

    void runBallDetector(const MVision::Image &image);
    void runWithAFileName(const QString& filename);

    // [TODO] : here might need a refactor as well
    void scanDatasetDirectory(const QString& dir_path);
    std::vector<QString> dataset;
};

#endif // MAINWINDOW_H
