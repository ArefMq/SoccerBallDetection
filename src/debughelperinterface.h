#ifndef DEBUGHELPERINTERFACE_H
#define DEBUGHELPERINTERFACE_H

class DebugHelperInterface
{
protected:
    DebugHelperInterface() {}

public:
    // Text debugs
    virtual void print(const char* message)=0;
    virtual void info(const char* message)=0;
    virtual void error(const char* message)=0;
    virtual void warning(const char* message)=0;

    // Visual Debug
    virtual void draw_pixel(int x, int y, char* color)=0;
    virtual void draw_point(int x, int y)=0;
    virtual void draw_line(int x1, int y1, int x2, int y2)=0;
    virtual void draw_circle(int x1, int y1, int x2, int y2)=0;
    virtual void draw_circle(int cx, int cy, int r)=0;
    virtual void draw_rect(int x, int y, int w, int h)=0;
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
    void print(const char* message) { if (_helper_instance) _helper_instance->print(message); }
    void info(const char* message) { if (_helper_instance) _helper_instance->info(message); }
    void error(const char* message) { if (_helper_instance) _helper_instance->error(message); }
    void warning(const char* message) { if (_helper_instance) _helper_instance->warning(message); }

    // Visual Debug
    void draw_pixel(int x, int y, char* color) { if (_helper_instance) _helper_instance->draw_pixel(x, y, color); }
    void draw_point(int x, int y) { if (_helper_instance) _helper_instance->draw_point(x, y); }
    void draw_line(int x1, int y1, int x2, int y2) { if (_helper_instance) _helper_instance->draw_line(x1, y1, x2, y2); }
    void draw_circle(int x1, int y1, int x2, int y2) { if (_helper_instance) _helper_instance->draw_circle(x1, y1, x2, y2); }
    void draw_circle(int cx, int cy, int r) { if (_helper_instance) _helper_instance->draw_circle(cx, cy, r); }
    void draw_rect(int x, int y, int w, int h) { if (_helper_instance) _helper_instance->draw_rect(x, y, w, h); }
};

#endif // DEBUGHELPERINTERFACE_H
