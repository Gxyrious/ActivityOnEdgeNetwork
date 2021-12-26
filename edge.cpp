#include "edge.h"

Edge::Edge()
{
    QLine();
    tag = false;
    _num = 0;
}

Edge::Edge(QPointF p1, QPointF p2)
{
    tag = false;
    setLine(p1.x(), p1.y(), p2.x(), p2.y());
}

Edge::Edge(qreal x1, qreal y1, qreal x2, qreal y2)
{
    tag = false;
    setLine(x1, y1, x2, y2);
}

int Edge::getNum()const
{
    return _num;
}

void Edge::setNum(int num)
{
    _num = num;
}

void Edge::setNode(int sNode,int eNode)
{
    _sNode = sNode;
    _eNode = eNode;
}

void Edge::setWeight(int weight)
{
    _weight = weight;
}

int Edge::getWeight() const
{
    return _weight;
}

int Edge::getStartNode()const
{
    return _sNode;
}

int Edge::getEndNode()const
{
    return _eNode;
}
