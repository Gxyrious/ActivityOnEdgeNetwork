#include "vertex.h"

Vertex::Vertex()
{
    QPoint();
    _num = 0;
}

Vertex::Vertex(QPoint point)
{
    QPoint();
    setX(point.x());
    setY(point.y());
}

QPoint Vertex::getPoint()const
{
    return QPoint(x(),y());
}

int Vertex::getNum()const
{
    return _num;
}

void Vertex::setNum(int num)
{
    _num = num;
}
