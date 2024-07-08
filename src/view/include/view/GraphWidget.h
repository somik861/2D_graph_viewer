#pragma once

#include <QMouseEvent>
#include <QPaintEvent>
#include <QPoint>
#include <QWidget>
#include <functional>

namespace view {

class GraphWidget : public QWidget {
    Q_OBJECT
  public:
    GraphWidget(QWidget* parent = nullptr);
    void registerPaintEventCallback(std::function<void(QPaintEvent*)> cb);
    void registerMouseButtonPressCallback(std::function<void(Qt::MouseButton)> cb);
    void registerMouseButtonReleaseCallback(std::function<void(Qt::MouseButton)> cb);

    void paintEvent(QPaintEvent* e) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    const QPoint& mousePosition() const;

  private:
    QPoint m_mouse_position;
    std::function<void(QPaintEvent*)> m_paint_cb;
    std::function<void(Qt::MouseButton)> m_mouse_press_cb;
    std::function<void(Qt::MouseButton)> m_mouse_release_cb;
};
} // namespace view