#pragma once

#include "model/IPainter.hpp"

#include <QPainter>
#include <QPen>

namespace viewmodel {
class GraphPainter : public virtual model::IPainter {
  public:
    ~GraphPainter() override = default;

    void setData(QPainter* painter,
                 const QRect& widget_rect,
                 const QMargins& graph_area_margins,
                 const QPoint& graph_shift,
                 const QPointF& zoom);

    void save() override;
    /**
     * Restores painter to the state saved with 'save()'
     * The behaviour shall be the same as QPainter::restore()
     */
    void restore() override;

    /*********** DRAW METHODS **********/
    void drawLine(const QPointF& p1, const QPointF& p2) override;
    void drawLines(const QPointF* point_pairs, int line_count) override;
    void drawPolyline(const QPointF* points, int point_count) override;
    void drawText(const QPointF& position, const QString& text, int flags) override;
    void drawRect(const QRectF& rect) override;
    qreal opacity() const override;
    void setOpacity(qreal opacity) override;

    QRectF viewport() const override;
    QPointF resolution() const override;
    void applyClipping() const;
    /*********** STYLING METHODS **********/
    void setPen(const QPen& pen) override;
    const QPen& pen() override;
    void setBrush(const QBrush& brush) override;
    const QBrush& brush() override;

    /*********** CONVERSION METHODS **********/
    QPoint graph2QtCoordinates(const QPointF& p) const;
    QPointF qt2GraphCoordinates(const QPoint& p) const;
    std::vector<QPoint> graph2QtCoordinates(const QPointF* points, int count) const;
    std::vector<QPointF> qt2GraphCoordinates(const QPoint* points, int count) const;

  private:
    void computeViewportAndResolution();

    QPainter* m_painter;
    QRect m_widget_rect;
    QMargins m_graph_area_margins;
    QPointF m_zoom;
    QPoint m_graph_shift;

    // computed values
    QRectF m_viewport;
    QRect m_graph_area;
    QPointF m_resolution;
};
} // namespace viewmodel