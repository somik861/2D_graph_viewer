#include "GraphElementBase.hpp"

#include <vector>

namespace model::GraphElements {
class Function : public virtual IGraphElement, public GraphElementBase {
  public:
    using function_t = std::function<qreal(qreal)>;
    Function(function_t function) : m_function(std::move(function)) {}

    ~Function() override = default;

    void paint(IPainter* painter) const override {
        auto viewport = painter->viewport();
        auto resolution = painter->resolution();

        qreal from = viewport.topLeft().x();
        qreal to = viewport.bottomRight().x();

        qreal dx = resolution.x() * 2; // draw at smaller smoothness (bigger steps between points)

        int point_count = int((to - from) / dx) + 1;

        // nothing can be seen
        if (point_count < 2)
            return;

        std::vector<QPointF> points;
        points.reserve(point_count);

        for (int i = 0; i < point_count; ++i) {
            qreal x = from + i * dx;
            qreal y = m_function(x);
            points.push_back(QPointF(x, y));
        }

        painter->drawPolyline(points.data(), point_count);
    }

  private:
    function_t m_function;
};

std::unique_ptr<IGraphElement> makeFunction(std::function<qreal(qreal)> function) {
    return std::make_unique<GraphElements::Function>(std::move(function));
}
} // namespace model::GraphElements