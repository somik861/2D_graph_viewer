#include "model/IGraph.hpp"
#include "view/GraphWidget.h"
#include "viewmodel/GraphConnector.hpp"

#include <QApplication>
#include <QColor>
#include <QPen>
#include <QtGlobal>
#include <QtWidgets>
#include <iostream>

int main(int argc, char** argv) {
    qputenv("QT_QPA_PLATFORM", "windows:darkmode=0");

    QApplication app(argc, argv);

    view::GraphWidget graph_wdg;
    auto graph_model = model::makeGraph();

    graph_model->addElement(model::GraphElements::makeFunction([](auto x) { return x * 2; }))
        ->setPen(QPen(Qt::GlobalColor::red));
    graph_model->addElement(model::GraphElements::makeFunction([](auto x) { return x; }))
        ->setPen(QPen(Qt::GlobalColor::blue));
    graph_model->addElement(model::GraphElements::makeFunction([](auto x) { return x / 2; }))
        ->setPen(QPen(Qt::GlobalColor::green));
    graph_model->addElement(model::GraphElements::makeFunction([](auto x) { return x <= 0 ? 0 : std::sqrt(x); }))
        ->setPen(QPen(Qt::GlobalColor::gray));

    viewmodel::GraphConnector connector(graph_model.get(), &graph_wdg);

    graph_wdg.show();
    return app.exec();
}