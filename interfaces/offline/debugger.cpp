#include "debugger.h"
#include <iostream>
using namespace std;

Debugger::Debugger() : qpn(0)
{
}

void Debugger::print(const char* message)
{
    cout << message << endl;
}

void Debugger::info(const char* message)
{
    cout << "[INFO] " << message << endl;
}

void Debugger::error(const char* message)
{
    cout << "[ERROR] " << message << endl;
}

void Debugger::warning(const char* message)
{
    cout << "[WARNING] " << message << endl;
}

void Debugger::draw_pixel(int x, int y, char* color)
{
    if (qpn)
        qpn->drawPoint(x, y);
}

void Debugger::draw_point(int x, int y)
{
    if (qpn)
        qpn->drawPoint(x, y);
}

void Debugger::draw_line(int x1, int y1, int x2, int y2)
{
    if (qpn)
        qpn->drawLine(x1, y1, x2, y2);
}

void Debugger::draw_circle(int x, int y, int w, int h)
{
    if (qpn)
        qpn->drawEllipse(x, y, w, h);
}

void Debugger::draw_circle(int cx, int cy, int r)
{
    if (qpn)
        qpn->drawEllipse(QPoint(cx, cy), r, r);
}

void Debugger::draw_rect(int x, int y, int w, int h)
{
    if (qpn)
        qpn->drawRect(x, y, w, h);
}

void Debugger::setImage(const QImage& image)
{
    _image = image;
    if (qpn)
        delete qpn;
    qpn = new QPainter(&_image);
    qpn->setPen(QPen(Qt::red, 3));
}

const QImage& Debugger::getImage()
{
    return _image;
}
