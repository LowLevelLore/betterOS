#ifndef BETTER_OS_GUI_WIDGET_H
#define BETTER_OS_GUI_WIDGET_H

#include "../basics/graphicsContext.hpp"
#include "../drivers/keyboard.hpp"
#include "../lib/types.hpp"

namespace better_os {
namespace gui {

class Widget : public better_os::drivers::KeyboardEventHandler {
   protected:
    Widget* m_parent;
    better_os::lib::int32_t m_x;
    better_os::lib::int32_t m_y;
    better_os::lib::int32_t m_w;
    better_os::lib::int32_t m_h;

    better_os::lib::uint8_t m_r;
    better_os::lib::uint8_t m_g;
    better_os::lib::uint8_t m_b;

    bool m_focusable;

   public:
    Widget(Widget* parent,
           better_os::lib::int32_t x,
           better_os::lib::int32_t y,
           better_os::lib::int32_t w,
           better_os::lib::int32_t h,
           better_os::lib::uint8_t r,
           better_os::lib::uint8_t g,
           better_os::lib::uint8_t b);

    ~Widget();

    virtual void GetFocus(Widget* widget);
    virtual void ModelToScreen(better_os::lib::int32_t& x, better_os::lib::int32_t& y);

    virtual void Draw(better_os::basics::GraphicsContext* gc);

    inline bool ContainsCoordinate(better_os::lib::int32_t x,
                                   better_os::lib::int32_t y) {
        return (m_x <= x) && ((m_x + m_w) >= x) &&
               (m_y <= y) && ((m_y + m_h) >= y);
    }

    virtual void OnMouseDown(better_os::lib::int32_t x,
                             better_os::lib::int32_t y);
    virtual void OnMouseUp(better_os::lib::int32_t x,
                           better_os::lib::int32_t y);
    virtual void OnMouseMove(better_os::lib::int32_t oldx,
                             better_os::lib::int32_t oldy,
                             better_os::lib::int32_t newx,
                             better_os::lib::int32_t newy);

    virtual void OnKeyDown(char* str);
    virtual void OnKeyUp(char* str);
};

class CompositeWidget : public Widget {
   protected:
    Widget* m_children[100];
    int m_numChildren;
    Widget* m_focussedChild;

   public:
    CompositeWidget(Widget* parent,
                    better_os::lib::int32_t x,
                    better_os::lib::int32_t y,
                    better_os::lib::int32_t w,
                    better_os::lib::int32_t h,
                    better_os::lib::uint8_t r,
                    better_os::lib::uint8_t g,
                    better_os::lib::uint8_t b);

    ~CompositeWidget();

    virtual void GetFocus(Widget* widget) override;
    virtual void ModelToScreen(better_os::lib::int32_t& x,
                               better_os::lib::int32_t& y) override;

    virtual void Draw(better_os::basics::GraphicsContext* gc) override;

    virtual void OnMouseDown(better_os::lib::int32_t x,
                             better_os::lib::int32_t y) override;
    virtual void OnMouseUp(better_os::lib::int32_t x,
                           better_os::lib::int32_t y) override;
    virtual void OnMouseMove(better_os::lib::int32_t oldx,
                             better_os::lib::int32_t oldy,
                             better_os::lib::int32_t newx,
                             better_os::lib::int32_t newy) override;

    virtual void OnKeyDown(char* str) override;
    virtual void OnKeyUp(char* str) override;
};

}  // namespace gui
}  // namespace better_os

#endif  // BETTER_OS_GUI_WIDGET_H
