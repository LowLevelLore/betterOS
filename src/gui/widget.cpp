#include "all.hpp"

using namespace better_os::gui;

Widget::Widget(Widget* parent,
               better_os::lib::int32_t x,
               better_os::lib::int32_t y,
               better_os::lib::int32_t w,
               better_os::lib::int32_t h,
               better_os::lib::uint8_t r,
               better_os::lib::uint8_t g,
               better_os::lib::uint8_t b) : parent(parent), x(x), y(y), w(w), h(h), r(r), g(g), b(b), focusable(true) {};

Widget::~Widget() {};

void Widget::GetFocus(Widget* widget) {
    if (parent) {
        parent->GetFocus(widget);
    }
};
void Widget::ModelToScreen(better_os::lib::int32_t& x, better_os::lib::int32_t& y) {
    if (parent) {
        parent->ModelToScreen(x, y);
    }
    x += this->x;
    y += this->y;
};

void Widget::Draw(GraphicsContext* gc) {
    int X = 0;
    int Y = 0;
    ModelToScreen(X, Y);
    gc->FillRectangle(X, Y, w, h, r, g, b);
};

void Widget::OnMouseDown(better_os::lib::int32_t x,
                         better_os::lib::int32_t y) {
    if (focusable) {
        GetFocus(this);
    }
};
void Widget::OnMouseUp(better_os::lib::int32_t x,
                       better_os::lib::int32_t y) {};
void Widget::OnMouseMove(better_os::lib::int32_t oldx,
                         better_os::lib::int32_t oldy, better_os::lib::int32_t newx, better_os::lib::int32_t newy) {};

void Widget::OnKeyDown(char* str) {};
void Widget::OnKeyUp(char* str) {};

CompositeWidget::CompositeWidget(Widget* parent, better_os::lib::int32_t x, better_os::lib::int32_t y,
                                 better_os::lib::int32_t w, better_os::lib::int32_t h,
                                 better_os::lib::uint8_t r, better_os::lib::uint8_t g,
                                 better_os::lib::uint8_t b) : Widget(parent, x, y, w, h, r, g, b) {
    focussedChild = 0;
    numChildren = 0;
};

CompositeWidget::~CompositeWidget() {};

void CompositeWidget::GetFocus(Widget* widget) {
    this->focussedChild = widget;
    if (parent) {
        parent->GetFocus(this);
    }
};
void CompositeWidget::ModelToScreen(better_os::lib::int32_t& x,
                                    better_os::lib::int32_t& y) {};

void CompositeWidget::Draw(GraphicsContext* gc) {
    Widget::Draw(gc);
    for (int i = 0; i < numChildren; i++) {
        children[i]->Draw(gc);
    }
};

void CompositeWidget::OnMouseDown(better_os::lib::int32_t x,
                                  better_os::lib::int32_t y) {
    for (int i = 0; i < numChildren; i++) {
        if (children[i]->ContainsCoordinate(x - this->x, y - this->y)) {
            children[i]->OnMouseDown(x - this->x, y - this->y);
        }
    }
};
void CompositeWidget::OnMouseUp(better_os::lib::int32_t x, better_os::lib::int32_t y) {
    for (int i = 0; i < numChildren; i++) {
        if (children[i]->ContainsCoordinate(x - this->x, y - this->y)) {
            children[i]->OnMouseUp(x - this->x, y - this->y);
        }
    }
};
void CompositeWidget::OnMouseMove(better_os::lib::int32_t oldx,
                                  better_os::lib::int32_t oldy,
                                  better_os::lib::int32_t newx,
                                  better_os::lib::int32_t newy) {
    int firstchild = -1;
    for (int i = 0; i < numChildren; ++i)
        if (children[i]->ContainsCoordinate(oldx - this->x, oldy - this->y)) {
            children[i]->OnMouseMove(oldx - this->x, oldy - this->y, newx - this->x, newy - this->y);
            firstchild = i;
            break;
        }

    for (int i = 0; i < numChildren; ++i)
        if (children[i]->ContainsCoordinate(newx - this->x, newy - this->y)) {
            if (firstchild != i)
                children[i]->OnMouseMove(oldx - this->x, oldy - this->y, newx - this->x, newy - this->y);
            break;
        }
};

void CompositeWidget::OnKeyDown(char* str) {
    if (focussedChild != 0)
        focussedChild->OnKeyDown(str);
};

void CompositeWidget::OnKeyUp(char* str) {
    if (focussedChild != 0)
        focussedChild->OnKeyUp(str);
};