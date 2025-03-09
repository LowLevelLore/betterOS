#ifndef __BETTER_OS_GUI_WIDGET_H
#define __BETTER_OS_GUI_WIDGET_H

#include "../basics/graphicsContext.hpp"
#include "../lib/types.hpp"

namespace better_os {
namespace gui {

class Widget {
   protected:
    Widget *parent;
    better_os::lib::int32_t x;
    better_os::lib::int32_t y;
    better_os::lib::int32_t w;
    better_os::lib::int32_t h;

    better_os::lib::uint8_t r;
    better_os::lib::uint8_t g;
    better_os::lib::uint8_t b;

    bool focusable;

   public:
    Widget(Widget *parent,
           better_os::lib::int32_t x,
           better_os::lib::int32_t y,
           better_os::lib::int32_t w,
           better_os::lib::int32_t h,
           better_os::lib::uint8_t r,
           better_os::lib::uint8_t g,
           better_os::lib::uint8_t b);

    ~Widget();

    virtual void GetFocus(Widget *widget);
    virtual void ModelToScreen(better_os::lib::int32_t &x, better_os::lib::int32_t &y);

    virtual void Draw(GraphicsContext *gc);

    inline bool ContainsCoordinate(better_os::lib::int32_t x,
                                   better_os::lib::int32_t y) {
        return (this->x <= x) and (this->x + this->w >= x) and (this->y <= y) and (this->y + this->h >= y);
    }

    virtual void OnMouseDown(better_os::lib::int32_t x,
                             better_os::lib::int32_t y);
    virtual void OnMouseUp(better_os::lib::int32_t x,
                           better_os::lib::int32_t y);
    virtual void OnMouseMove(better_os::lib::int32_t oldx,
                             better_os::lib::int32_t oldy, better_os::lib::int32_t newx, better_os::lib::int32_t newy);

    virtual void OnKeyDown(char *str);
    virtual void OnKeyUp(char *str);
};

class CompositeWidget : public Widget {
   protected:
    Widget *children[100];
    int numChildren;
    Widget *focussedChild;

   public:
    CompositeWidget(Widget *parent, better_os::lib::int32_t x, better_os::lib::int32_t y,
                    better_os::lib::int32_t w, better_os::lib::int32_t h,
                    better_os::lib::uint8_t r, better_os::lib::uint8_t g,
                    better_os::lib::uint8_t b);

    ~CompositeWidget();

    virtual void GetFocus(Widget *widget);
    virtual void ModelToScreen(better_os::lib::int32_t &x,
                               better_os::lib::int32_t &y);

    virtual void Draw(GraphicsContext *gc);

    virtual void OnMouseDown(better_os::lib::int32_t x,
                             better_os::lib::int32_t y);
    virtual void OnMouseUp(better_os::lib::int32_t x, better_os::lib::int32_t y);
    virtual void OnMouseMove(better_os::lib::int32_t oldx,
                             better_os::lib::int32_t oldy,
                             better_os::lib::int32_t newx,
                             better_os::lib::int32_t newy);

    virtual void OnKeyDown(char *str);
    virtual void OnKeyUp(char *str);
};

}  // namespace gui
}  // namespace better_os

#endif  // !__BETTER_OS_GUI_WIDGET_H