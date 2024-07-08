#pragma once

#include "model/IGraph.hpp"
#include "view/GraphWidget.h"
#include "viewmodel/GraphPainter.hpp"

namespace viewmodel {

class GraphConnector {
  public:
    GraphConnector(model::IGraph* graph, view::GraphWidget* widget);

  private:
    void paintEventCallback(QPaintEvent* e);
    void mousePressCallback(Qt::MouseButton b);
    void mouseReleaseCallback(Qt::MouseButton b);

    QMenu* generateMenu();
    void zoomToRect(const QRectF& target);
    /******** DRAW METHODS ********/
    void drawMouseDragRect(GraphPainter& painter);
   

    model::IGraph* m_graph;
    view::GraphWidget* m_widget;
    viewmodel::GraphPainter m_gpainter;

    std::optional<QPoint> m_mouse_drag_start;
    QPointF m_zoom = {1.0, 1.0};
    QPoint m_shift = {0, 0};
};
} // namespace viewmodel