#ifndef VERTEX_H
#define VERTEX_H

#include <QPoint>
#include "vertex.h"


class Vertex : public QPoint
{
public:
    Vertex();
    Vertex(QPoint point);
    QPoint getPoint()const;

    int getNum()const;
    void setNum(int num);
private:
    //the count of Vertices
    int _num;
};

#endif // VERTEX_H
