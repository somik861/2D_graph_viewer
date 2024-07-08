#pragma once

#include "model/IGraphElement.hpp"

namespace model::GraphElements {
class GraphElementBase : public virtual IGraphElement {
  public:
    ~GraphElementBase() override = default;

    const QPen& pen() const override { return p_pen; }
    IGraphElement* setPen(const QPen& pen) override {
        p_pen = pen;
        return this;
    }

    const QString& label() const override { return p_label; }
    IGraphElement* setLabel(const QString& label) override {
        p_label = label;
        return this;
    }

    IGraphElement* setVisible(bool visible) override {
        p_visible = visible;
        return this;
    };
    bool isVisible() const override { return p_visible; };

  protected:
    QPen p_pen;
    QString p_label;
    bool p_visible;
};
} // namespace model::GraphElements