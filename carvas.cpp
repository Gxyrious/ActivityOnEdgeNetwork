#include <QMessageBox>
#include <QDebug>

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

/*鼠标释放事件*/
void Carvas::mouseReleaseEvent(QMouseEvent *event)
{
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

/*绘图事件*/
void Carvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing, true);
    int nodeRadius = 15;
    for(int iter = 0; iter < vecEdge.size(); iter++)
    {
        //绘制边
        QPen pen;
        pen.setWidth(3);
        pen.setCapStyle(Qt::RoundCap);
        if(vecEdge[iter]->tag)
        {
            //若是关键路径，则用特殊笔绘制
            pen.setColor(Qt::red);
        }
        else
        {
            //非关键路径，用普通笔绘制
            pen.setColor(Qt::black);
        }
        painter.setPen(pen);
        painter.setFont(QFont("黑体", 15));

        //绘制路径和箭头
        double arrowSize = 15;
        //起始点和终止点
        QPointF sPoint = vecEdge[iter]->p1(), ePoint = vecEdge[iter]->p2();

        //线段的长度
        double len = sqrt((sPoint.x() - ePoint.x()) * (sPoint.x() - ePoint.x())
                          + (sPoint.y() - ePoint.y()) * (sPoint.y() - ePoint.y()));

        //向量(dx, dy)是(sPoint, ePoint)方向上的单位向量
        double dx = (ePoint.x() - sPoint.x()) / len , dy = (ePoint.y() - sPoint.y()) / len;

        //实际要画的线
        QLineF line(QPointF(sPoint.x() + dx * nodeRadius, sPoint.y() + dy * nodeRadius),
                    QPointF(ePoint.x() - dx * nodeRadius, ePoint.y() - dy * nodeRadius));
        //角度
        double angle = atan2(line.dy(), line.dx());

        QPointF arrowP1 = line.p2() - QPointF(cos(angle + M_PI / 9) * arrowSize,
                                              sin(angle + M_PI / 9) * arrowSize),
                arrowP2 = line.p2() - QPointF(cos(angle - M_PI / 9) * arrowSize,
                                              sin(angle - M_PI / 9) * arrowSize);

        QPolygonF arrowHead;
        arrowHead << line.p2() << arrowP2 << arrowP1;
        painter.drawLine(line.p2(), arrowP1);
        painter.drawLine(line.p2(), arrowP2);
        pen.setWidth(2);
        painter.drawLine(line);

        //绘制路径权值
        QPointF location = QPointF((vecEdge[iter]->p1() + vecEdge[iter]->p2()) / 2);
        if(vecEdge[iter]->dx() * vecEdge[iter]->dy() < 0)
        {
            //斜率为负，向左上角偏移
            location.setX(location.x() - 10);
            location.setY(location.y() - 10);
            qDebug()<<vecEdge[iter]->dx() * vecEdge[iter]->dy()<<"\n";
        }
        else
        {
            //斜率为正，向右上角偏移
            location.setX(location.x() + 10);
            location.setY(location.y() - 10);
        }
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
        painter.drawEllipse(vecVertex[iter]->x() - nodeRadius, vecVertex[iter]->y() - nodeRadius,
                            2 * nodeRadius, 2 * nodeRadius);

        painter.drawText(QRect(QPoint(vecVertex[iter]->x() - nodeRadius, vecVertex[iter]->y() - nodeRadius),
                               QPoint(vecVertex[iter]->x() + nodeRadius, vecVertex[iter]->y() + nodeRadius)),
                         QString::number(num), QTextOption(Qt::AlignCenter));
    }
}

/*析构函数*/
Carvas::~Carvas()
{
    delete graph;
    for(int iter = 0; iter < vecEdge.size(); iter++)
    {
        delete vecEdge[iter];
    }
    for(int iter = 0; iter < vecVertex.size(); iter++)
    {
        delete vecVertex[iter];
    }

}

/*添加边*/
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

/*删除边*/
bool Carvas::removeEdge(int n1,int n2)
{
    //删除n1与n2之间的边，若边不存在，则返回false
    if(!graph->removeEdge(n1, n2))
    {
        return false;
    }
    for(int iter = 0; iter < vecEdge.size();)
    {
        //遍历所有边，找出要删除的那条
        if(vecEdge[iter]->getStartNode() == n1 && vecEdge[iter]->getEndNode() == n2)
        {
            //删除该边，直接跳出
            delete vecEdge[iter];
            vecEdge.remove(iter);
            continue;
        }
        else
        {
            vecEdge[iter]->tag = false;
        }
        iter++;
    }
    numEdge--;
    this->update();
    return true;
}

/*获取结点数*/
int Carvas::getNumVertex() const
{
    return numVertex;
}

/*获取边数*/
int Carvas::getNumEdge() const
{
    return numEdge;
}

/*生成关键路径*/
void Carvas::generateCriticalEdge()
{
    for(int iter = 0; iter < vecEdge.size(); iter++)
    {
        //先全部置为黑边
        vecEdge[iter]->tag = false;
    }
    //生成拓扑排序
    if(!graph->TopologicalSort())
    {
        //关键路径不存在，清空过程中可能产生的关键路径
        graph->startNode.Erase();
        graph->endNode.Erase();
        QMessageBox::warning(this, "ERROR", "该网络不存在关键路径");
    }
    else
    {
        //生成关键路径
        CriticalPath(graph);
        LinkedList<int>& sNodes = graph->startNode;
        LinkedList<int>& eNodes = graph->endNode;
        LinkNode<int> *siter = sNodes.getHead(), *eiter = eNodes.getHead();
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

/*清空图*/
void Carvas::clearGraph()
{
    //先删地图
    graph->clear();

    //逐个删除内存
    for(int iter = 0; iter< vecEdge.size(); iter++)
    {
        delete vecEdge[iter];
    }
    for(int iter = 0; iter < vecVertex.size(); iter++)
    {
        delete vecVertex[iter];
    }

    //清空指针vector
    vecEdge.clear();
    vecVertex.clear();
    numEdge = 0;
    numVertex = 0;

    //更新画布
    this->update();

}
