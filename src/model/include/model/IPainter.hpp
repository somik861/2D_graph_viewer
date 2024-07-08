#pragma once

#include <QLineF>
#include <QPen>
#include <QPointF>

namespace model {
class IPainter {
  public:
    /**
     * Saves interal state of the painter
     * The behaviour shall be the same as QPainter::save()
     */
    virtual void save() = 0;
    /**
     * Restores painter to the state saved with 'save()'
     * The behaviour shall be the same as QPainter::restore()
     */
    virtual void restore() = 0;

    /* Viewport informs about the visible area on the graph. */
    virtual QRectF viewport() const = 0;

    /**
     * the minimum possible distance between two points (in each axis) that can be
     * recognized by the viewport (for example: minimal x and y distance between neighbouring pixels)
     */
    virtual QPointF resolution() const = 0;

    /*********** DRAW FUNCTIONS **********/
    virtual void drawLine(const QPointF& p1, const QPointF& p2) = 0;
    virtual void drawLines(const QPointF* point_pairs, int line_count) = 0;
    virtual void drawPolyline(const QPointF* points, int point_count) = 0;
    virtual void drawText(const QPointF& position, const QString& text, int flags) = 0;
    virtual void drawRect(const QRectF& rect) = 0;
    virtual qreal opacity() const = 0;
    /** opacity [0.0 .. 1.0] */
    virtual void setOpacity(qreal opacity) = 0;

    /*********** STYLING FUNCTIONS **********/
    virtual void setPen(const QPen& pen) = 0;
    virtual const QPen& pen() = 0;
    virtual void setBrush(const QBrush& brush) = 0;
    virtual const QBrush& brush() = 0;

    virtual ~IPainter() = default;
};
} // namespace model