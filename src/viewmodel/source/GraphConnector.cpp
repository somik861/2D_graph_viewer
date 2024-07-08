#include "viewmodel/GraphPainter.hpp"

#include "viewmodel/GraphConnector.hpp"

#include <QMenu>
#include <QPainter>
#include <chrono>

using namespace viewmodel;

GraphConnector::GraphConnector(model::IGraph* graph, view::GraphWidget* widget) : m_graph(graph), m_widget(widget) {
    widget->registerPaintEventCallback([this](QPaintEvent* e) { paintEventCallback(e); });
    widget->registerMouseButtonPressCallback([this](Qt::MouseButton b) { mousePressCallback(b); });
    widget->registerMouseButtonReleaseCallback([this](Qt::MouseButton b) { mouseReleaseCallback(b); });
}

void GraphConnector::paintEventCallback(QPaintEvent*) {
    auto start_tp = std::chrono::steady_clock::now();
    QPainter painter(m_widget);
    m_gpainter.setData(&painter, m_widget->rect(), QMargins(150, 20, 20, 100), m_shift, m_zoom);

    for (auto axis : {model::IGraph::Axis::X, model::IGraph::Axis::Y}) {
        m_graph->paintAxis(&m_gpainter, axis);
        m_graph->paintLegendLines(&m_gpainter, axis);
        m_graph->paintLegend(&m_gpainter, axis);
    }

    m_gpainter.save();
    m_gpainter.applyClipping();
    m_graph->paintElements(&m_gpainter);
    m_gpainter.restore();

    drawMouseDragRect(m_gpainter);

    auto end_tp = std::chrono::steady_clock::now();
    auto duration = end_tp - start_tp;
    int duration_us = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();

    painter.drawText(QPoint{20, 20}, QString("%1 us").arg(duration_us));

    auto mouse_position = m_gpainter.qt2GraphCoordinates(m_widget->mousePosition());
    painter.drawText(QPoint{20, 40}, QString("X: %1").arg(mouse_position.x()));
    painter.drawText(QPoint{20, 60}, QString("Y: %1").arg(mouse_position.y()));
}

void GraphConnector::mousePressCallback(Qt::MouseButton button) {
    switch (button) {
    case Qt::LeftButton:
        m_mouse_drag_start = m_widget->mousePosition();
        break;
    case Qt::RightButton:
        auto menu = generateMenu();
        menu->exec(m_widget->mapToGlobal(m_widget->mousePosition()));
        break;
    }
    m_widget->update();
}
void GraphConnector::mouseReleaseCallback(Qt::MouseButton button) {
    switch (button) {
    case Qt::LeftButton:
        auto gposition = m_gpainter.qt2GraphCoordinates(m_widget->mousePosition());
        auto gmouse_start = m_gpainter.qt2GraphCoordinates(m_mouse_drag_start.value());

        if (gposition.x() < gmouse_start.x())
            std::swap(gposition.rx(), gmouse_start.rx());
        if (gposition.y() < gmouse_start.y())
            std::swap(gposition.ry(), gmouse_start.ry());
        zoomToRect(QRectF(gmouse_start, gposition));
        m_mouse_drag_start = std::nullopt;
        break;
    }
    m_widget->update();
}

QMenu* GraphConnector::generateMenu() {
    auto menu = new QMenu();
    menu->addAction("Reset", [this]() {
        m_zoom = {1.0, 1.0};
        m_shift = {0, 0};
        m_widget->update();
    });
    return menu;
}

void GraphConnector::zoomToRect(const QRectF& target) {
    auto viewport = m_gpainter.viewport();
    auto resolution = m_gpainter.resolution();
    m_zoom.rx() *= viewport.width() / target.width();
    m_zoom.ry() *= viewport.height() / target.height();

    QPointF shift_f = (target.center() - viewport.center());
    shift_f.rx() += resolution.x();
    shift_f.ry() += resolution.y();

    m_shift += QPoint(std::lround(shift_f.x()), std::lround(shift_f.y()));
}

void GraphConnector::drawMouseDragRect(GraphPainter& painter) {
    if (!m_mouse_drag_start.has_value())
        return;

    painter.save();
    QBrush brush(Qt::GlobalColor::lightGray);
    QPen pen(Qt::GlobalColor::black);
    pen.setStyle(Qt::PenStyle::DashDotDotLine);

    painter.setPen(pen);
    painter.setBrush(brush);
    painter.setOpacity(0.5);

    painter.drawRect(QRectF(painter.qt2GraphCoordinates(m_mouse_drag_start.value()),
                            painter.qt2GraphCoordinates(m_widget->mousePosition())));

    painter.restore();
}
