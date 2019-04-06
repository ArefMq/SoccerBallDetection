#include "imagedirectoryloader.h"
#include <QDir>
#include <unistd.h>
#include <iostream>
using namespace std;

void* p_thread_run(void* void_ptr)
{
    ImageDirectoryLoader* ptr = (ImageDirectoryLoader*)void_ptr;
    ptr->run();
    return 0;
}

ImageDirectoryLoader::ImageDirectoryLoader() : StreamLoader(), thread()
{

}

void ImageDirectoryLoader::open(const char* path)
{
    this->path = path;
    pthread_create(&thread, NULL, p_thread_run, this);
}

void ImageDirectoryLoader::run()
{
    QDir directory(path);
    QStringList images = directory.entryList(QStringList() << "*.jpg" << "*.JPG", QDir::Files);
    foreach(QString filename, images)
    {
        QString full_path = QString(path) + filename;
//        QString save_full_path = QString(SAVE_FOLDER) + filename;

        cout << "FILE NAME: " << full_path.toStdString().c_str() << "\n" << flush;
        _frame = QImage(full_path);
        emit frame_ready();
        usleep(3*1000*1000);
        cout << "\n---------------------------------------------------\n";
    }
}

