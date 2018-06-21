#include "debugger.h"
#include <iostream>
using namespace std;

Debugger::Debugger() : qpn(0), _current_color(None)
{
}

void Debugger::print(const char* message, Color c)
{
    cout << message << endl;
}

void Debugger::info(const char* message, Color c)
{
    cout << "[INFO] " << message << endl;
}

void Debugger::error(const char* message, Color c)
{
    cout << "[ERROR] " << message << endl;
}

void Debugger::warning(const char* message, Color c)
{
    cout << "[WARNING] " << message << endl;
}

void Debugger::draw_pixel(int x, int y, char* color)
{
    if (qpn)
    {
        QColor c = QColor(color[0], color[1], color[2]);
        qpn->setPen(c);
        qpn->drawPoint(x, y);
    }
}

void Debugger::draw_point(int x, int y, Color c)
{
    if (qpn)
    {
        setPen(c);
        qpn->drawPoint(x, y);
    }
}

void Debugger::draw_line(int x1, int y1, int x2, int y2, Color c)
{
    if (qpn)
    {
        setPen(c);
        qpn->drawLine(x1, y1, x2, y2);
    }
}

void Debugger::draw_circle(int x, int y, int w, int h, Color c)
{
    if (qpn)
    {
        setPen(c);
        qpn->drawEllipse(x, y, w, h);
    }
}

void Debugger::draw_circle(int cx, int cy, int r, Color c)
{
    if (qpn)
    {
        setPen(c);
        qpn->drawEllipse(QPoint(cx, cy), r, r);
    }
}

void Debugger::draw_rect(int x, int y, int w, int h, Color c)
{
    if (qpn)
    {
        setPen(c);
        qpn->drawRect(x, y, w, h);
    }
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

void Debugger::setPen(Color c)
{
    if (_current_color == c || !qpn)
        return;

    _current_color = c;
    switch (c)
    {
    case Black:
        qpn->setPen(QPen(Qt::black, 4));
        break;
    case White:
        qpn->setPen(QPen(Qt::white, 4));
        break;
    case DarkGray:
        qpn->setPen(QPen(Qt::darkGray, 4));
        break;
    case Gray:
        qpn->setPen(QPen(Qt::gray, 4));
        break;
    case LightGray:
        qpn->setPen(QPen(Qt::lightGray, 4));
        break;
    case Red:
        qpn->setPen(QPen(Qt::red, 4));
        break;
    case Green:
        qpn->setPen(QPen(Qt::green, 4));
        break;
    case Blue:
        qpn->setPen(QPen(Qt::blue, 4));
        break;
    case Cyan:
        qpn->setPen(QPen(Qt::cyan, 4));
        break;
    case Magenta:
        qpn->setPen(QPen(Qt::magenta, 4));
        break;
    case Yellow:
        qpn->setPen(QPen(Qt::yellow, 4));
        break;
    case DarkRed:
        qpn->setPen(QPen(Qt::darkRed, 4));
        break;
    case DarkGreen:
        qpn->setPen(QPen(Qt::darkGreen, 4));
        break;
    case DarkBlue:
        qpn->setPen(QPen(Qt::darkBlue, 4));
        break;
    case DarkCyan:
        qpn->setPen(QPen(Qt::darkCyan, 4));
        break;
    case DarkMagenta:
        qpn->setPen(QPen(Qt::darkMagenta, 4));
        break;
    case DarkYellow:
        qpn->setPen(QPen(Qt::darkYellow, 4));
        break;
    case Transparent:
        qpn->setPen(QPen(Qt::transparent, 4));
        break;
    default:
    case None:
        break;
    }


}
