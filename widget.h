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

#include "carvas.h"
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
    Carvas *carvas;

public slots:
    void submitEdge();
    void removeEdge();

protected:


private:
    Ui::Widget *ui;
    QLineEdit *sNodeLE, *eNodeLE, *weightLE;//添加边的起点终点和权值输入
    QLineEdit *rEdgeNode1LE, *rEdgeNode2LE;//删除边的起点和终点输入
    QLineEdit *rNodeLE;//删除的结点输入
    QPushButton *edgeSubmitButton, *edgeRemoveButton;
    QPushButton *aoeGenerateButton, *clearGraphButton;//生成AOE网络
};

#endif // WIDGET_H
