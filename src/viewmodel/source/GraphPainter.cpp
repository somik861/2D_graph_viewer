#include "viewmodel/GraphPainter.hpp"

#include <vector>

using namespace viewmodel;

void GraphPainter::setData(QPainter* painter,
                           const QRect& widget_rect,
                           const QMargins& graph_area_margins,
                           const QPoint& graph_shift,
                           const QPointF& zoom) {
    m_painter = painter;
    m_widget_rect = widget_rect;
    m_graph_area_margins = graph_area_margins;
    m_graph_shift = graph_shift;
    m_zoom = zoom;

    m_painter->setRenderHint(QPainter::Antialiasing);
    computeViewportAndResolution();
}

void GraphPainter::save() /* override */ { m_painter->save(); }
void GraphPainter::restore() /* override */ { m_painter->restore(); }

/*********** DRAW METHODS **********/
void GraphPainter::drawLine(const QPointF& p1, const QPointF& p2) /* override */ {
    m_painter->drawLine(graph2QtCoordinates(p1), graph2QtCoordinates(p2));
}
void GraphPainter::drawLines(const QPointF* point_pairs, int line_count) /* override */ {
    auto converted = graph2QtCoordinates(point_pairs, line_count * 2);
    m_painter->drawLines(converted.data(), line_count);
}
void GraphPainter::drawPolyline(const QPointF* points, int point_count) /* override */ {
    auto converted = graph2QtCoordinates(points, point_count);
    m_painter->drawPolyline(converted.data(), point_count);
}

void GraphPainter::drawText(const QPointF& position, const QString& text, int flags) /* override */ {
    QPoint center = graph2QtCoordinates(position);

    QRect boundingRect;
    m_painter->drawText(QRect(0, 0, 0, 0), flags, text, &boundingRect);
    boundingRect.moveCenter(center);
    m_painter->drawText(boundingRect, flags, text);
}
void GraphPainter::drawRect(const QRectF& rect) /* override */ {
    QRect converted(graph2QtCoordinates(rect.topLeft()), graph2QtCoordinates(rect.bottomRight()));
    m_painter->drawRect(converted);
}

qreal GraphPainter::opacity() const /* override */ { return m_painter->opacity(); }
void GraphPainter::setOpacity(qreal opacity) /* override */ { m_painter->setOpacity(opacity); }
QRectF GraphPainter::viewport() const /* override */ { return m_viewport; }
QPointF GraphPainter::resolution() const /* override */ { return m_resolution; }
void GraphPainter::applyClipping() const {
    m_painter->setClipRect(m_graph_area);
    m_painter->setClipping(true);
}

/*********** STYLING METHODS **********/
void GraphPainter::setPen(const QPen& pen) /* override */ { m_painter->setPen(pen); }
const QPen& GraphPainter::pen() /* override */ { return m_painter->pen(); }
void GraphPainter::setBrush(const QBrush& brush) /* override */ { m_painter->setBrush(brush); }
const QBrush& GraphPainter::brush() /* override */ { return m_painter->brush(); }

void GraphPainter::computeViewportAndResolution() {
    auto rect = m_widget_rect;

    // remove margins
    rect = rect.marginsRemoved(m_graph_area_margins);
    m_graph_area = rect;

    // apply shift
    rect.moveTopLeft(m_graph_shift);

    // apply zoom
    m_viewport = rect.toRectF();
    m_viewport.setHeight(m_viewport.height() / m_zoom.y());
    m_viewport.setWidth(m_viewport.width() / m_zoom.x());

    m_viewport.moveCenter(rect.toRectF().center());

    m_resolution = {m_viewport.width() / rect.width(), m_viewport.height() / rect.height()};
}

/*********** CONVERSION METHODS **********/
QPoint GraphPainter::graph2QtCoordinates(const QPointF& p) const {
    qreal x = p.x(), y = p.y();
    qreal min_x = m_viewport.topLeft().x();
    qreal min_y = m_viewport.topLeft().y();
    int min_qx = m_graph_area.topLeft().x();
    int min_qy = m_graph_area.topLeft().y();

    // calculate viewport percantage
    qreal perc_x = (x - min_x) / m_viewport.width();
    qreal perc_y = (y - min_y) / m_viewport.height();

    // calculate coords in qt space
    int qx = std::lround(m_graph_area.width() * perc_x);
    int qy = std::lround(m_graph_area.height() * perc_y);

    // flip around X axis to set origin to correct origin position
    qy = m_graph_area.height() - qy;

    // shift into the graph area
    qx += min_qx;
    qy += min_qy;

    return QPoint(qx, qy);
}

QPointF GraphPainter::qt2GraphCoordinates(const QPoint& p) const {
    int qx = p.x(), qy = p.y();
    qreal min_x = m_viewport.topLeft().x();
    qreal min_y = m_viewport.topLeft().y();
    int min_qx = m_graph_area.topLeft().x();
    int min_qy = m_graph_area.topLeft().y();

    qx -= min_qx;
    qy -= min_qy;

    // flip around X axis to set origin to correct origin position
    qy = m_graph_area.height() - qy;

    // calculate percentages in graph area
    qreal perc_x = qreal(qx) / m_graph_area.width();
    qreal perc_y = qreal(qy) / m_graph_area.height();

    // calculate coordinates in graph space
    qreal x = (m_viewport.width() * perc_x) + min_x;
    qreal y = (m_viewport.height() * perc_y) + min_y;

    return {x, y};
}

std::vector<QPoint> GraphPainter::graph2QtCoordinates(const QPointF* points, int count) const {
    std::vector<QPoint> converted;
    converted.resize(count);
    for (int i = 0; i < count; ++i)
        converted[i] = graph2QtCoordinates(points[i]);

    return converted;
}
std::vector<QPointF> GraphPainter::qt2GraphCoordinates(const QPoint* points, int count) const {
    std::vector<QPointF> converted;
    converted.resize(count);
    for (int i = 0; i < count; ++i)
        converted[i] = qt2GraphCoordinates(points[i]);

    return converted;
}