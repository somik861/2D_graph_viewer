#include "view/GraphWidget.h"

using namespace view;

GraphWidget::GraphWidget(QWidget* parent) : QWidget(parent) { setMouseTracking(true); }

void GraphWidget::registerPaintEventCallback(std::function<void(QPaintEvent*)> cb) { m_paint_cb = std::move(cb); }

void GraphWidget::registerMouseButtonPressCallback(std::function<void(Qt::MouseButton)> cb) {
    m_mouse_press_cb = std::move(cb);
}

void GraphWidget::registerMouseButtonReleaseCallback(std::function<void(Qt::MouseButton)> cb) {
    m_mouse_release_cb = std::move(cb);
}

void GraphWidget::paintEvent(QPaintEvent* e) /* override */ {
    if (m_paint_cb)
        m_paint_cb(e);
    QWidget::paintEvent(e);
}

void GraphWidget::mouseMoveEvent(QMouseEvent* event) /* override */ {
    m_mouse_position = event->pos();
    update();
}

void GraphWidget::mousePressEvent(QMouseEvent* event) {
    m_mouse_position = event->pos();
    if (m_mouse_press_cb)
        m_mouse_press_cb(event->button());
    update();
}

void GraphWidget::mouseReleaseEvent(QMouseEvent* event) {
    m_mouse_position = event->pos();
    if (m_mouse_release_cb)
        m_mouse_release_cb(event->button());
    update();
}

const QPoint& GraphWidget::mousePosition() const { return m_mouse_position; }
