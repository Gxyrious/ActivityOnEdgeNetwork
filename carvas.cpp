#include <QDebug>
#include <QMessageBox>

#include "carvas.h"
#include "LinkedList.hpp"

Carvas::Carvas(QWidget *parent) :
    QWidget(parent)
{
    //构造函数
    numVertex = 0;
    numEdge = 0;
    graph = new Graph<int, int>(-1, -1, 20);
}

void Carvas::mouseReleaseEvent(QMouseEvent *event)
{
    //鼠标释放事件
    //将原来的边全部置为黑色
    for(int iter = 0; iter <vecEdge.size(); iter++)
    {
        vecEdge[iter]->tag = false;
    }
    //新建结点
    Vertex *newVertex = new Vertex(event->pos());
    vecVertex.append(newVertex);
    newVertex->setNum(numVertex);
    graph->insertVertex(numVertex++);//将结点加入graph
    this->update();
}

void Carvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    int nodeRadius = 30;
    for(int iter = 0; iter < vecEdge.size(); iter++)
    {
        //绘制边
        if(vecEdge[iter]->tag)
        {
            //用特殊笔绘制
            painter.setPen(QPen(QColor(255,0,0),4));
            painter.setFont(QFont("黑体", 15));
        }
        else
        {
            //用普通笔绘制
            painter.setPen(QPen(QColor(0,0,0),4));
            painter.setFont(QFont("黑体", 15));
        }

        //箭头
        QLineF line(vecEdge[iter]->p1(), vecEdge[iter]->p2());
        double angle = atan2(-line.dy(), line.dx());
        qreal arrowSize = 10;
        QPointF arrowP1 = line.p2() - QPointF(sin(angle + M_PI / 3) * arrowSize,
                                              cos(angle + M_PI / 3) * arrowSize);
        QPointF arrowP2 = line.p2() - QPointF(sin(angle + 2 / 3 * M_PI) * arrowSize,
                                              cos(angle + 2 / 3 * M_PI) * arrowSize);
        QPolygonF arrowHead;
        arrowHead << line.p2() << arrowP2 << arrowP1;
        painter.drawLine(line);
        painter.drawLine(vecEdge[iter]->p2(), arrowP1);
        painter.drawLine(vecEdge[iter]->p2(), arrowP2);

        //权值
        QPointF location = QPointF((vecEdge[iter]->p1() + vecEdge[iter]->p2()) / 2);
        painter.drawText(location, QString::number(vecEdge[iter]->getWeight()));

    }
    for (int iter = 0; iter < vecVertex.size(); iter++)
    {
        //绘制结点
        int num = vecVertex[iter]->getNum();
        painter.setPen(QPen(QColor(0,0,0),3));
        if (num < 10)
        {
            painter.setFont(QFont("黑体",15, QFont::Bold));
        }
        else if (num < 100)
        {
            painter.setFont(QFont("黑体",10, QFont::Bold));
        }
        else
        {
            painter.setFont(QFont("黑体",8, QFont::Bold));
        }
        painter.drawEllipse(vecVertex[iter]->x() - nodeRadius/2, vecVertex[iter]->y() - nodeRadius/2, nodeRadius, nodeRadius);
        //Vertex继承了Point类
        painter.drawText(QRect(QPoint(vecVertex[iter]->x() - nodeRadius/2, vecVertex[iter]->y() - nodeRadius/2),
                               QPoint(vecVertex[iter]->x() + nodeRadius/2, vecVertex[iter]->y() + nodeRadius/2)),
                         QString::number(num), QTextOption(Qt::AlignCenter));
    }
}

Carvas::~Carvas()
{

}

bool Carvas::addEdge(int n1,int n2,int weight)
{
    //在序号为n1和n2的结点之间连接一条边，若结点不存在则返回false
    if(!graph->insertEdge(n1, n2, weight))
    {
        //边已存在
        return false;
    }
    //将原来的边全部置为黑色
    for(int iter = 0; iter <vecEdge.size(); iter++)
    {
        vecEdge[iter]->tag = false;
    }
    //新建边
    int x1 = vecVertex[n1]->x(), y1 = vecVertex[n1]->y();
    int x2 = vecVertex[n2]->x(), y2 = vecVertex[n2]->y();
    Edge *newEdge = new Edge(x1, y1, x2, y2);
    newEdge->setNode(n1, n2);//设置结点
    newEdge->setWeight(weight);//设置权值
    newEdge->setNum(numEdge++);//设置编号
    vecEdge.append(newEdge);
    this->update();//更新画布
    return true;
}

bool Carvas::removeEdge(int n1,int n2)
{
    //删除n1与n2之间的边，若边不存在，则返回false
    if(!graph->removeEdge(n1, n2))
    {
        return false;
    }
    //clearCriticalEdge();
    for(int iter = 0; iter < vecEdge.size();)
    {
        //遍历所有边，找出要删除的那条
        if(vecEdge[iter]->getStartNode() == n1 && vecEdge[iter]->getEndNode() == n2)
        {
            //删除该边，直接跳出
            vecEdge.remove(iter);
            continue;
        }
        else
        {
            vecEdge[iter]->tag = false;
        }
        iter++;
    }
    this->update();
    return true;
}

int Carvas::getNumVertex() const
{
    return numVertex;
}

int Carvas::getNumEdge() const
{
    return numEdge;
}

void Carvas::generateCriticalEdge()
{
    for(int iter = 0; iter < vecEdge.size(); iter++)
    {
        //先全部置为false
        vecEdge[iter]->tag = false;
        //std::cout<<vecEdge[i]->getStartNode()<<" "<<vecEdge[i]->getEndNode()<<"\n";
    }
    //生成关键路径
    if(!graph->TopologicalSort())
    {
        graph->startNode.Erase();
        graph->endNode.Erase();
        QMessageBox::warning(this, "ERROR", "该网络不存在关键路径");
    }
    else
    {
        CriticalPath(graph);
        LinkedList<int> sNodes = graph->startNode;
        LinkedList<int> eNodes = graph->endNode;
        LinkNode<int>* siter = sNodes.getHead();
        LinkNode<int>* eiter = eNodes.getHead();
        for(int iter = 0; iter < vecEdge.size(); iter++)
        {
            siter = sNodes.getHead()->_next;
            eiter = eNodes.getHead()->_next;
            while(siter != NULL && eiter != NULL)
            {
                //_data是在_NodeTable的位置，不一定是实际的编号
                if(siter->_data == vecEdge[iter]->getStartNode()
                        && eiter->_data == vecEdge[iter]->getEndNode())
                {
                    vecEdge[iter]->tag = true;
                    break;
                }
                siter = siter->_next;
                eiter = eiter->_next;
            }
        }
        graph->startNode.Erase();
        graph->endNode.Erase();
        this->update();
    }
}

void Carvas::clearGraph()
{
    graph->clear();
    vecEdge.clear();
    vecVertex.clear();
    this->update();
    numVertex = 0;
    numEdge = 0;
}
