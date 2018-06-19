#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "debughelperinterface.h"

class Debugger : public DebugHelperInterface
{
public:
    Debugger();

    // Text debugs
    void print(const char* message);
    void info(const char* message);
    void error(const char* message);
    void warning(const char* message);

    // Visual Debug
    void draw_pixel(int x, int y, char* color);
    void draw_point(int x, int y);
    void draw_line(int x1, int y1, int x2, int y2);
    void draw_circle(int x1, int y1, int x2, int y2);
    void draw_circle(int cx, int cy, int r);
    void draw_rect(int x, int y, int w, int h);
};

#endif // DEBUGGER_H
