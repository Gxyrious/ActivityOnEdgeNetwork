#include "vertex.h"

Vertex::Vertex()
{
    _num = 0;
}

Vertex::Vertex(QPointF point)
{
    setX(point.x());
    setY(point.y());
}

int Vertex::getNum()const
{
    return _num;
}

void Vertex::setNum(int num)
{
    _num = num;
}
