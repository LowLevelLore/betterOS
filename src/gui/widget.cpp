#include "all.hpp"

using namespace better_os::gui;
using namespace better_os::lib;

Widget::Widget(Widget* parent,
               int32_t x,
               int32_t y,
               int32_t w,
               int32_t h,
               uint8_t r,
               uint8_t g,
               uint8_t b)
    : m_parent(parent), m_x(x), m_y(y), m_w(w), m_h(h), m_r(r), m_g(g), m_b(b), m_focusable(true) {}

Widget::~Widget() {}

void Widget::GetFocus(Widget* widget) {
    if (m_parent) {
        m_parent->GetFocus(widget);
    }
}

void Widget::ModelToScreen(int32_t& x, int32_t& y) {
    if (m_parent) {
        m_parent->ModelToScreen(x, y);
    }
    x += m_x;
    y += m_y;
}

void Widget::Draw(better_os::basics::GraphicsContext* gc) {
    int X = 0;
    int Y = 0;
    ModelToScreen(X, Y);
    gc->FillRectangle(X, Y, m_w, m_h, m_r, m_g, m_b);
}

void Widget::OnMouseDown(int32_t x, int32_t y) {
    if (m_focusable) {
        GetFocus(this);
    }
}

void Widget::OnMouseUp(int32_t x, int32_t y) {}

void Widget::OnMouseMove(int32_t oldx, int32_t oldy, int32_t newx, int32_t newy) {}

void Widget::OnKeyDown(char* str) {}

void Widget::OnKeyUp(char* str) {}

CompositeWidget::CompositeWidget(Widget* parent,
                                 int32_t x,
                                 int32_t y,
                                 int32_t w,
                                 int32_t h,
                                 uint8_t r,
                                 uint8_t g,
                                 uint8_t b)
    : Widget(parent, x, y, w, h, r, g, b) {
    m_focussedChild = 0;
    m_numChildren = 0;
}

CompositeWidget::~CompositeWidget() {}

void CompositeWidget::GetFocus(Widget* widget) {
    m_focussedChild = widget;
    if (m_parent) {
        m_parent->GetFocus(this);
    }
}

void CompositeWidget::ModelToScreen(int32_t& x, int32_t& y) {
    // If needed, implement composite-specific coordinate transformation.
}

void CompositeWidget::Draw(better_os::basics::GraphicsContext* gc) {
    Widget::Draw(gc);
    for (int i = 0; i < m_numChildren; i++) {
        m_children[i]->Draw(gc);
    }
}

void CompositeWidget::OnMouseDown(int32_t x, int32_t y) {
    for (int i = 0; i < m_numChildren; i++) {
        if (m_children[i]->ContainsCoordinate(x - m_x, y - m_y)) {
            m_children[i]->OnMouseDown(x - m_x, y - m_y);
        }
    }
}

void CompositeWidget::OnMouseUp(int32_t x, int32_t y) {
    for (int i = 0; i < m_numChildren; i++) {
        if (m_children[i]->ContainsCoordinate(x - m_x, y - m_y)) {
            m_children[i]->OnMouseUp(x - m_x, y - m_y);
        }
    }
}

void CompositeWidget::OnMouseMove(int32_t oldx, int32_t oldy, int32_t newx, int32_t newy) {
    int firstChild = -1;
    for (int i = 0; i < m_numChildren; ++i) {
        if (m_children[i]->ContainsCoordinate(oldx - m_x, oldy - m_y)) {
            m_children[i]->OnMouseMove(oldx - m_x, oldy - m_y, newx - m_x, newy - m_y);
            firstChild = i;
            break;
        }
    }
    for (int i = 0; i < m_numChildren; ++i) {
        if (m_children[i]->ContainsCoordinate(newx - m_x, newy - m_y)) {
            if (firstChild != i) {
                m_children[i]->OnMouseMove(oldx - m_x, oldy - m_y, newx - m_x, newy - m_y);
            }
            break;
        }
    }
}

void CompositeWidget::OnKeyDown(char* str) {
    if (m_focussedChild != 0) {
        m_focussedChild->OnKeyDown(str);
    }
}

void CompositeWidget::OnKeyUp(char* str) {
    if (m_focussedChild != 0) {
        m_focussedChild->OnKeyUp(str);
    }
}
