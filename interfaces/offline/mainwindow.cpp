#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qpainter.h>

#include "modules/edgeimage.h"
#include "util.h"

#include <iostream>
#include <dirent.h>
#include <sys/stat.h>

#include "streams/singleimageloader.h"
#define pl std::cout << __FILE__ << " :: " << __LINE__ << std::endl;

using namespace std;
using namespace MVision;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    ballDetector(BallDetector(&debugger)),
    streamLoader(0)
{
    ui->setupUi(this);
    on_stream_selector_currentIndexChanged("Single Image");
    on_btn_open_clicked(); // [FIXME]:  remove this line
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_stream_selector_currentIndexChanged(const QString &arg1)
{
    if (streamLoader)
        delete streamLoader;


    if (arg1 == "Single Image")
        streamLoader = new SingleImageLoader();
//    if (arg1 == "Image Directory")
//        streamLoader = new SingleImageLoader();
//    if (arg1 == "Camera Device")
//        streamLoader = new SingleImageLoader();
//    if (arg1 == "Video Stream")
//        streamLoader = new SingleImageLoader();

    if (!streamLoader)
        return;

    connect(streamLoader, SIGNAL(frame_ready()), this, SLOT(on_frame_ready()));

}

void MainWindow::on_btn_open_clicked()
{
    if (!streamLoader)
        return;

    // TODO : implement this
    streamLoader->open("/home/aref/Workspace/pp/SoccerBallDetection/interfaces/offline/images/a.jpg");
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
    debugger.setImage(streamLoader->frame());
    ballDetector.update(streamLoader->convertFrameToMVImage());

    typedef std::vector<Ball> Balls;
    const Balls& results = ballDetector.getResults();
    for (Balls::const_iterator itr=results.begin(); itr<results.end(); itr++)
    {
        const Ball& b = *itr;
        debugger.setPen(DebugHelperInterface::Green);
        int x = b.PositionInImage().translation().x();
        int y = b.PositionInImage().translation().y();
        int r = b.PositionInImage().radious();
        debugger.draw_circle(x, y, r);
    }

    //-- Show Frame
    ui->monitor->setText("");
    const QImage& image = debugger.getImage().scaled(ui->monitor->size(), Qt::KeepAspectRatio);
    ui->monitor->setPixmap(QPixmap::fromImage(image));
}
