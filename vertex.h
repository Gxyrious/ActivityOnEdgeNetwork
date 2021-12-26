#ifndef VERTEX_H
#define VERTEX_H

#include <QPoint>
#include "vertex.h"


class Vertex : public QPointF
{
public:
    Vertex();
    Vertex(QPointF point);

    int getNum()const;
    void setNum(int num);
private:
    int _num;
};

#endif // VERTEX_H
