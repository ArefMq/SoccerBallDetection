#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "debughelperinterface.h"
#include <qimage.h>
#include <qpainter.h>
#include <qhash.h>
#include <qstring.h>

class Debugger : public DebugHelperInterface
{
public:
    Debugger();

    // Text debugs
    void print(const char* message, Color c=None, const char* Context="");
    void info(const char* message, Color c=None, const char* Context="");
    void error(const char* message, Color c=None, const char* Context="");
    void warning(const char* message, Color c=None, const char* Context="");

    // Visual Debug
    void draw_pixel(int x, int y, char* color, const char* Context="");
    void draw_pixel(int x, int y, Color color, const char* Context="");
    void draw_point(int x, int y, Color c=None, const char* Context="");
    void draw_line(int x1, int y1, int x2, int y2, Color c=None, const char* Context="");
    void draw_circle(int x1, int y1, int x2, int y2, Color c=None, const char* Context="");
    void draw_circle(int cx, int cy, int r, Color c=None, const char* Context="");
    void draw_rect(int x, int y, int w, int h, Color c=None, const char* Context="");

    void setImage(const QImage& image);
    const QImage& getImage();

    void setPen(Color c);
    void enableContext(QString Context);
    void disableContext(QString Context);
    void setContexEnabled(QString Context, bool isEnabled);

private:
    QImage _image;
    QPainter* qpn;
    Color _current_color;
    QHash<QString, bool> _contex_map;
    int i;
};

#endif // DEBUGGER_H
