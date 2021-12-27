#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QLabel>
#include <QVector>
#include <QString>
#include <QFont>
#include <QPainter>
#include <QLineEdit>
#include <QPushButton>
#include <QDebug>

#include "canvas.h"
#include "Graph.hpp"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    Canvas *carvas;

public slots:
    void submitEdge();
    void removeEdge();

private:
    QLineEdit *sNodeLE, *eNodeLE, *weightLE;//添加边的起点终点和权值输入
    QLineEdit *rEdgeNode1LE, *rEdgeNode2LE;//删除边的起点和终点输入
    QPushButton *edgeSubmitButton, *edgeRemoveButton;
    QPushButton *aoeGenerateButton, *clearGraphButton;//生成AOE网络
};

#endif // WIDGET_H
