#include "model/IGraph.hpp"

#include <vector>

namespace model {
class Graph : public virtual IGraph {
  public:
    pIGraphElement addElement(pIGraphElement element) override {
        m_elements.push_back(element);
        return element;
    }
    void removeElement(int idx) override { m_elements.erase(std::next(m_elements.begin(), idx)); }
    void clear() override { m_elements.clear(); }
    int elementCount() const override { return int(m_elements.size()); };
    pIGraphElement getElement(int idx) override { return m_elements[idx]; }

    void paintElements(IPainter* painter) const override {
        for (const auto& elem : m_elements) {
            if (!elem->isVisible())
                continue;
            painter->save();
            painter->setPen(elem->pen());
            elem->paint(painter);

            painter->restore();
        }
    }

    void paintAxis(IPainter* painter, Axis axis) const override {
        // because of Qt (topLeft is 0,0), we flip top/bottom

        painter->save();
        painter->setPen(QPen(Qt::GlobalColor::black));

        auto viewport = painter->viewport();
        QPointF from = viewport.topLeft(); // Graph origin
        QPointF to = (axis == Axis::X ? viewport.topRight() : viewport.bottomLeft());
        painter->drawLine(from, to);

        painter->restore();
    }

    void paintLegend(IPainter* painter, Axis axis) const override {
        painter->save();
        painter->setPen(QPen(Qt::GlobalColor::black));

        auto resolution = painter->resolution();

        QPointF marker_length = 5.0 * resolution;
        qreal dx = (axis == Axis::X ? 0.0 : marker_length.y() / 2);
        qreal dy = (axis == Axis::X ? marker_length.x() / 2 : 0.0);
        QPointF dmarker_length(dx, dy);

        for (auto marker_point : calculateLegendMarkers(painter, axis)) {
            painter->drawLine(marker_point - dmarker_length, marker_point + dmarker_length);
            QString text = QString("%1").arg(axis == Axis::X ? marker_point.x() : marker_point.y());
            painter->drawText(marker_point - (dmarker_length * (axis == Axis::X ? 4 : 8)), text, Qt::AlignHCenter);
        }

        painter->restore();
    }
    void paintLegendLines(IPainter* painter, Axis axis) const override {
        painter->save();

        QPen pen;
        pen.setColor(Qt::GlobalColor::gray);
        pen.setStyle(Qt::PenStyle::DashLine);
        painter->setPen(pen);

        auto viewport = painter->viewport();

        qreal dx = (axis == Axis::X ? 0.0 : viewport.width());
        qreal dy = (axis == Axis::X ? viewport.height() : 0.0);
        QPointF dlength(dx, dy);

        for (auto marker_point : calculateLegendMarkers(painter, axis))
            painter->drawLine(marker_point, marker_point + dlength);

        painter->restore();
    };

    ~Graph() override = default;

  private:
    std::vector<qreal> calculateLegendMarkers(qreal from, qreal to) const {
        // TODO: Implement
        return {from, from + (to - from) / 4, (from + to) / 2, from + 3 * (to - from) / 4, to};
    }
    std::vector<QPointF> calculateLegendMarkers(IPainter* painter, Axis axis) const {
        auto viewport = painter->viewport();
        auto resolution = painter->resolution();
        qreal from = (axis == Axis::X ? viewport.left() : viewport.top());
        qreal to = (axis == Axis::X ? viewport.right() : viewport.bottom());

        auto markers = calculateLegendMarkers(from, to);
        std::vector<QPointF> out;
        out.reserve(markers.size());

        for (auto marker : markers) {
            QPointF marker_point;
            marker_point.rx() = (axis == Axis::X ? marker : viewport.left());
            marker_point.ry() = (axis == Axis::X ? viewport.top() : marker);

            out.push_back(marker_point);
        }

        return out;
    }

    std::vector<pIGraphElement> m_elements;
};

std::unique_ptr<IGraph> model::makeGraph() { return std::make_unique<Graph>(); }
} // namespace model