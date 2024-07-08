#pragma once

#include "IPainter.hpp"

#include <QPen>
#include <QPointF>
#include <QRectF>
#include <QString>
#include <functional>
#include <memory>

namespace model {
class IGraphElement {
  public:
    /**
     * Draw the element to the graph via painter
     *
     * GraphElements are not required to set the correct Pen, it is done automatically before paint(...) call
     */
    virtual void paint(IPainter* painter) const = 0;

    /** return this pointer to support chaining */
    virtual IGraphElement* setPen(const QPen&) = 0;
    virtual IGraphElement* setVisible(bool visible) = 0;
    virtual IGraphElement* setLabel(const QString& label) = 0;

    virtual const QPen& pen() const = 0;
    virtual bool isVisible() const = 0;
    virtual const QString& label() const = 0;

    virtual ~IGraphElement() = default;
};

namespace GraphElements {
std::unique_ptr<IGraphElement> makeFunction(std::function<qreal(qreal)> function);
}

} // namespace model