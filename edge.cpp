#include "edge.h"

Edge::Edge()
{
    QLine();
    tag = false;
    _num = 0;
}

Edge::Edge(QLine line)
{
    tag = false;
    setLine(line.x1(),line.y1(),line.x2(),line.y2());
}

Edge::Edge(int x1, int y1, int x2, int y2)
{
    tag = false;
    setLine(x1,y1,x2,y2);
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
