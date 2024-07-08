#pragma once

#include "model/IGraphElement.hpp"
#include "model/IPainter.hpp"

#include <QString>
#include <QStringList>
#include <memory>
#include <set>

namespace model {
class IGraph {
  public:
    enum class Axis { X, Y };

    using pIGraphElement = std::shared_ptr<IGraphElement>;

    // returns pointer to allow for chaining
    virtual pIGraphElement addElement(pIGraphElement element) = 0;
    virtual int elementCount() const = 0;
    virtual void removeElement(int idx) = 0;
    virtual void clear() = 0;
    virtual pIGraphElement getElement(int idx) = 0;

    virtual void paintElements(IPainter* painter) const = 0;
    virtual void paintAxis(IPainter* painter, Axis axis) const = 0;
    virtual void paintLegend(IPainter* painter, Axis axis) const = 0;
    virtual void paintLegendLines(IPainter* painter, Axis axis) const = 0;

    virtual ~IGraph() = default;
};

std::unique_ptr<IGraph> makeGraph();

} // namespace model