#ifndef STREAMLOADER_H
#define STREAMLOADER_H

#include <qimage.h>
#include <qobject.h>
#include "tools/image.h"

class StreamLoader : public QObject
{
public:
    Q_OBJECT

protected:
    StreamLoader() {}
    QImage _frame;

public:
    virtual void open(const char* path)=0;

    virtual void start_stream()=0;
    virtual void stop_stream()=0;
    virtual void restart_stream()=0;

    virtual void refresh_frame() { emit frame_ready(); }
    virtual void next_frame()=0;
    virtual void previous_frame()=0;

    const QImage& frame() const { return _frame; }
    MVision::Image convertFrameToMVImage() const;

signals:
    void frame_ready();
};



#endif // STREAMLOADER_H
