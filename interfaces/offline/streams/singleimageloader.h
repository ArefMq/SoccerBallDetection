#ifndef SINGLEIMAGELOADER_H
#define SINGLEIMAGELOADER_H

#include "streamloader.h"

class SingleImageLoader : public StreamLoader
{
public:
    SingleImageLoader();

    void open(const char* path);

    void start_stream() {}
    void stop_stream() {}
    void restart_stream() {}
    void next_frame() {}
    void previous_frame() {}
};

#endif // SINGLEIMAGELOADER_H
