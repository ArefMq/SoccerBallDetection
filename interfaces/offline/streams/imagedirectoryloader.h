#ifndef IMAGEDIRECTORYLOADER_H
#define IMAGEDIRECTORYLOADER_H

#include "streamloader.h"
#include <pthread.h>

class ImageDirectoryLoader : public StreamLoader
{
public:
    ImageDirectoryLoader();

    void open(const char* path);

    void start_stream() {}
    void stop_stream() {}
    void restart_stream() {}
    void next_frame() {}
    void previous_frame() {}

private:
    pthread_t thread;
    QString path;

    void run();
    friend void* p_thread_run(void*);
};

#endif // IMAGEDIRECTORYLOADER_H
