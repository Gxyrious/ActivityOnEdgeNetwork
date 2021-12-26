#ifndef CARVAS_H
#define CARVAS_H

#include <QMouseEvent>
#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QString>

#include "Graph.hpp"
#include "Vertex.h"
#include "edge.h"


class Carvas : public QWidget
{
    Q_OBJECT
public:
    explicit Carvas(QWidget *parent = 0);
    ~Carvas();

    int getNumVertex()const;
    int getNumEdge()const;

    bool addEdge(int n1,int n2,int weight);
    bool removeEdge(int n1,int n2);
public slots:
    void generateCriticalEdge();
    void clearGraph();
protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
private:
    //当前结点和边的总数，与vecVertex.size()和vecEdge.size()保持一致
    int numVertex, numEdge;
    Graph<int, int>* graph;

    QVector<Vertex*> vecVertex;
    QVector<Edge*> vecEdge;
};

#endif // CARVAS_H
