#ifndef DEBUGHELPERINTERFACE_H
#define DEBUGHELPERINTERFACE_H

class DebugHelperInterface
{
public:
    enum Color {
        Black=0,
        White,
        DarkGray,
        Gray,
        LightGray,
        Red,
        Green,
        Blue,
        Cyan,
        Magenta,
        Yellow,
        DarkRed,
        DarkGreen,
        DarkBlue,
        DarkCyan,
        DarkMagenta,
        DarkYellow,
        Transparent=-2,
        None=-1,
    };

protected:
    DebugHelperInterface() {}

public:
    // Text debugs
    virtual void print(const char* message, Color c=None)=0;
    virtual void info(const char* message, Color c=None)=0;
    virtual void error(const char* message, Color c=None)=0;
    virtual void warning(const char* message, Color c=None)=0;

    // Visual Debug
    virtual void draw_pixel(int x, int y, char* color)=0;
    virtual void draw_point(int x, int y, Color c=None)=0;
    virtual void draw_line(int x1, int y1, int x2, int y2, Color c=None)=0;
    virtual void draw_circle(int x1, int y1, int x2, int y2, Color c=None)=0;
    virtual void draw_circle(int cx, int cy, int r, Color c=None)=0;
    virtual void draw_rect(int x, int y, int w, int h, Color c=None)=0;
};

class Debugable : public DebugHelperInterface
{
public:
    void setDebugHelper(DebugHelperInterface* helper) { _helper_instance = helper; }
    virtual ~Debugable() {}

private:
    static DebugHelperInterface* _helper_instance;

protected:
    Debugable() {}

    // Text debugs
    void print(const char* message, Color c=None) { if (_helper_instance) _helper_instance->print(message, c); }
    void info(const char* message, Color c=None) { if (_helper_instance) _helper_instance->info(message, c); }
    void error(const char* message, Color c=None) { if (_helper_instance) _helper_instance->error(message, c); }
    void warning(const char* message, Color c=None) { if (_helper_instance) _helper_instance->warning(message, c); }

    // Visual Debug
    void draw_pixel(int x, int y, char* color) { if (_helper_instance) _helper_instance->draw_pixel(x, y, color); }
    void draw_point(int x, int y, Color c=None) { if (_helper_instance) _helper_instance->draw_point(x, y, c); }
    void draw_line(int x1, int y1, int x2, int y2, Color c=None) { if (_helper_instance) _helper_instance->draw_line(x1, y1, x2, y2, c); }
    void draw_circle(int x1, int y1, int x2, int y2, Color c=None) { if (_helper_instance) _helper_instance->draw_circle(x1, y1, x2, y2, c); }
    void draw_circle(int cx, int cy, int r, Color c=None) { if (_helper_instance) _helper_instance->draw_circle(cx, cy, r, c); }
    void draw_rect(int x, int y, int w, int h, Color c=None) { if (_helper_instance) _helper_instance->draw_rect(x, y, w, h, c); }
};

#endif // DEBUGHELPERINTERFACE_H
