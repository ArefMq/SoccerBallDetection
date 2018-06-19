#include "debugger.h"
#include <iostream>
using namespace std;

Debugger::Debugger()
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

}

void Debugger::draw_point(int x, int y)
{

}

void Debugger::draw_line(int x1, int y1, int x2, int y2)
{

}

void Debugger::draw_circle(int x1, int y1, int x2, int y2)
{

}

void Debugger::draw_circle(int cx, int cy, int r)
{

}

void Debugger::draw_rect(int x, int y, int w, int h)
{

}
