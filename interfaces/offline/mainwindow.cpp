#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qpainter.h>

#include "modules/edgeimage.h"
#include "util.h"

#include <iostream>
#include <dirent.h>
#include <sys/stat.h>

#include "singleimageloader.h"

using namespace std;
using namespace MVision;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    ballDetector(BallDetector())
{
    streamLoader = new SingleImageLoader();
    connect(streamLoader, SIGNAL(frame_ready), this, SLOT(on_frame_ready));

    ui->setupUi(this);
    on_stream_selector_currentIndexChanged("Single Image");
}

MainWindow::~MainWindow()
{
    delete ui;
}


//void MainWindow::setImageToMonitor()
//{
//    ui->monitor->setText("");
//    if (isEdgeImageSelected())
//        ui->monitor->setPixmap(QPixmap::fromImage(monitor));
//    else
//        ui->monitor->setPixmap(QPixmap::fromImage(edgeMonitor));
//}

//void MainWindow::runBallDetector(const Image &image)
//{
//    monitor = image2qimage(image);
//    QPainter* qpn = new QPainter(&monitor);

//    ballDetector.update(image);
//    edgeMonitor = image2qimage(ballDetector.debug_GetEdgeImage());

//    typedef std::vector<Ball> Balls;
//    const Balls& results = ballDetector.getResults();
//    for (Balls::const_iterator itr=results.begin(); itr<results.end(); itr++)
//    {
//        const Ball& b = *itr;
//        qpn->setPen(QPen(Qt::red, 3));
//        qpn->drawEllipse(QPointF(b.PositionInImage()._translation.x, b.PositionInImage()._translation.y), b.PositionInImage()._radious, b.PositionInImage()._radious);
//    }

//    delete qpn;
//}

//void MainWindow::runWithAFileName(const QString& filename)
//{
//    Image img = loadImage(filename);
//    runBallDetector(img);
//    setImageToMonitor();
//}

//void MainWindow::scanDatasetDirectory(const QString& dir_path)
//{
//    dataset.clear();

//    DIR *dir;
//    class dirent *ent;
//    class stat st;

//    dir = opendir(dir_path.toStdString().c_str());
//    while ((ent = readdir(dir)) != NULL)
//    {
//        const string file_name = ent->d_name;
//        const string full_file_name = dir_path.toStdString() + "/" + file_name;

//        if (file_name[0] == '.')
//            continue;

//        if (stat(full_file_name.c_str(), &st) == -1)
//            continue;

//        const bool is_directory = (st.st_mode & S_IFDIR) != 0;

//        if (is_directory)
//            continue;

//#ifdef DEBUG
//        cout << "processing : " << file_name.c_str() << "\n";
//#endif
//        dataset.push_back(QString(full_file_name.c_str()));
//        ui->btn_next->setEnabled(true);
//        ui->btn_referesh->setEnabled(true);
//    }
//    closedir(dir);
//}

//void MainWindow::on_btn_referesh_clicked()
//{
//    if (dataset.size())
//        runWithAFileName(dataset.back());
//}

//void MainWindow::on_btn_next_clicked()
//{
//    dataset.pop_back();
//    on_btn_referesh_clicked();

//    if (dataset.size() <= 1)
//        ui->btn_next->setEnabled(false);
//}

//void MainWindow::on_radio_image_clicked()
//{
//    setImageToMonitor();
//}

//void MainWindow::on_radio_edge_clicked()
//{
//    setImageToMonitor();
//}


void MainWindow::on_stream_selector_currentIndexChanged(const QString &arg1)
{

}

void MainWindow::on_btn_open_clicked()
{
    // TODO : implement this
    streamLoader->open("/home/parallels/Workspace/paper/SoccerBallDetection/interfaces/offline/images/a.jpg");
}

void MainWindow::on_btn_reload_clicked()
{
    streamLoader->refresh_frame();
}

void MainWindow::on_btn_previous_clicked()
{
    streamLoader->previous_frame();
}

void MainWindow::on_btn_pause_clicked()
{
    streamLoader->stop_stream();
}

void MainWindow::on_btn_play_clicked()
{
    streamLoader->start_stream();
}

void MainWindow::on_btn_forward_clicked()
{
    streamLoader->next_frame();
}

void MainWindow::on_frame_ready()
{
    cout << "frame recieved!" << endl;
}
