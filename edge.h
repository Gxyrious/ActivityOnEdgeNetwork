#ifndef EDGE_H
#define EDGE_H

#include <QLine>

class Edge : public QLineF
{
public:
    bool tag;
    Edge();
    Edge(QPointF p1,QPointF p2);
    Edge(qreal x1, qreal y1, qreal x2, qreal y2);
    int getNum()const;
    void setNum(int num);
    void setNode(int sNode, int eNode);
    void setWeight(int weight);
    int getWeight()const;
    int getStartNode()const;
    int getEndNode()const;
private:
    int _num;
    int _sNode, _eNode, _weight;
};

#endif // EDGE_H
