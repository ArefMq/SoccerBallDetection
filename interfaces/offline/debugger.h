#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "debughelperinterface.h"
#include <qimage.h>
#include <qpainter.h>

class Debugger : public DebugHelperInterface
{
public:
    Debugger();

    // Text debugs
    void print(const char* message, Color c=None);
    void info(const char* message, Color c=None);
    void error(const char* message, Color c=None);
    void warning(const char* message, Color c=None);

    // Visual Debug
    void draw_pixel(int x, int y, char* color);
    void draw_point(int x, int y, Color c=None);
    void draw_line(int x1, int y1, int x2, int y2, Color c=None);
    void draw_circle(int x1, int y1, int x2, int y2, Color c=None);
    void draw_circle(int cx, int cy, int r, Color c=None);
    void draw_rect(int x, int y, int w, int h, Color c=None);

    void setImage(const QImage& image);
    const QImage& getImage();

    void setPen(Color c);

private:
    QImage _image;
    QPainter* qpn;
    Color _current_color;
};

#endif // DEBUGGER_H
