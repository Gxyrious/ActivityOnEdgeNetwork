#ifndef EDGE_H
#define EDGE_H

#include <QLine>

class Edge : public QLine
{
public:
    bool tag;
    Edge();
    Edge(QLine line);
    Edge(int x1, int y1, int x2, int y2);
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
