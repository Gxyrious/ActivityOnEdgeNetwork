#include <QtWidgets>

#include "widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    this->setFixedSize(1000, 600);//先设置总的窗体大小
    //carvas是自己写的一个继承了QWidget的类
    carvas = new Carvas(this);

    //输入框
    QGroupBox *inputGroup = new QGroupBox(tr("Input"));
    QGridLayout *inputLayout = new QGridLayout;
//    组合框
//    QLabel *modeLabel = new QLabel(tr("Mode:"));
//    QComboBox *modeSelectComboBox = new QComboBox;
//    modeSelectComboBox->addItem("add edge");
//    modeSelectComboBox->addItem("delete edge");
//    modeSelectComboBox->addItem("delete vertex");

//! [1]
    QGroupBox *addEdgeGroup = new QGroupBox(tr("add edge"));

    QLabel *sLabel = new QLabel(tr("Start Node:"));
    sNodeLE = new QLineEdit;
    sNodeLE->setPlaceholderText("start node");
    //sNodeLE->setFocus();
    QLabel *eLabel = new QLabel(tr("  End Node:"));
    eNodeLE = new QLineEdit;
    eNodeLE->setPlaceholderText("end node");
    QLabel *wLabel = new QLabel(tr("    Weight:"));
    weightLE = new QLineEdit;
    weightLE->setPlaceholderText(("edge weight"));

    edgeSubmitButton = new QPushButton("加载边");//边提交按钮
//    QSizePolicy edgeSubmitButtonPolicy = edgeSubmitButton->sizePolicy();
//    edgeSubmitButtonPolicy.setVerticalPolicy(QSizePolicy::Minimum);
//    edgeSubmitButton->setSizePolicy(edgeSubmitButtonPolicy);
    edgeSubmitButton->setFixedSize(300,40);

    QGridLayout *addEdgeLayout = new QGridLayout;
    addEdgeLayout->addWidget(sLabel,0,0,1,1);
    addEdgeLayout->addWidget(sNodeLE,0,1,1,4);
    addEdgeLayout->addWidget(eLabel,1,0,1,1);
    addEdgeLayout->addWidget(eNodeLE,1,1,1,4);
    addEdgeLayout->addWidget(wLabel,2,0,1,1);
    addEdgeLayout->addWidget(weightLE,2,1,1,4);
    addEdgeLayout->addWidget(edgeSubmitButton,3,0,2,5);

    addEdgeGroup->setLayout(addEdgeLayout);
//! [1]

//! [2]
    QGroupBox *removeEdgeGroup = new QGroupBox(tr("remove edge"));

    QLabel *rEdgeLabel1 = new QLabel(tr("Start Node:"));
    rEdgeNode1LE = new QLineEdit;
    rEdgeNode1LE->setPlaceholderText("start node");
    QLabel *rEdgeLabel2 = new QLabel(tr("  End Node:"));
    rEdgeNode2LE = new QLineEdit;
    rEdgeNode2LE->setPlaceholderText("end node");

    edgeRemoveButton = new QPushButton("删除边");//边删除按钮
//    QSizePolicy edgeRemoveButtonPolicy = edgeRemoveButton->sizePolicy();
//    edgeRemoveButtonPolicy.setVerticalPolicy(QSizePolicy::Minimum);
//    edgeRemoveButton->setSizePolicy(edgeRemoveButtonPolicy);
    edgeRemoveButton->setFixedSize(300,40);

    QGridLayout *removeEdgeLayout = new QGridLayout;
    removeEdgeLayout->addWidget(rEdgeLabel1,0,0,1,1);
    removeEdgeLayout->addWidget(rEdgeNode1LE,0,1,1,4);
    removeEdgeLayout->addWidget(rEdgeLabel2,1,0,1,1);
    removeEdgeLayout->addWidget(rEdgeNode2LE,1,1,1,4);
    removeEdgeLayout->addWidget(edgeRemoveButton,3,0,1,5);

    removeEdgeGroup->setLayout(removeEdgeLayout);
//! [2]

//! [3]
    aoeGenerateButton = new QPushButton("生成AOE网络");//AOE网络生成按钮
    QSizePolicy aoeGenerateButtonPolicy = aoeGenerateButton->sizePolicy();
    aoeGenerateButtonPolicy.setVerticalPolicy(QSizePolicy::Minimum);
    aoeGenerateButton->setSizePolicy(aoeGenerateButtonPolicy);
    //aoeGenerateButton->setFixedSize(300, 150);
//! [4]

//! [5]
    clearGraphButton = new QPushButton("清空图",this);//清空图按钮
    clearGraphButton->setFixedSize(60, 40);
    clearGraphButton->move(this->width() - 2 * clearGraphButton->width(),
                           this->height() - 2 * clearGraphButton->height());
//! [5]

//! [6]
    inputLayout->addWidget(addEdgeGroup);
    inputLayout->addWidget(removeEdgeGroup);
    inputLayout->addWidget(aoeGenerateButton);

    inputGroup->setLayout(inputLayout);
//! [6]

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(inputGroup,0,0,1,3);
    layout->addWidget(carvas,0,1,1,7);
    this->setLayout(layout);
    this->

    connect(edgeSubmitButton,SIGNAL(clicked(bool)),this,SLOT(submitEdge()));
    connect(edgeRemoveButton,SIGNAL(clicked(bool)),this,SLOT(removeEdge()));
    connect(aoeGenerateButton,SIGNAL(clicked(bool)),carvas,SLOT(generateCriticalEdge()));
    connect(clearGraphButton,SIGNAL(clicked(bool)),carvas,SLOT(clearGraph()));
}

Widget::~Widget()
{

}

void Widget::submitEdge()
{
    //获取数字
    int sNum = sNodeLE->text().toInt();
    int eNum = eNodeLE->text().toInt();
    int weight = weightLE->text().toInt();
    int num = carvas->getNumVertex();
    if(sNum < 0 || eNum < 0 || weight <= 0 || sNum >= num || eNum >= num || sNum == eNum)
    {
        //输入错误
        QMessageBox::warning(this, "ERROR", "输入数据有误，请重新输入");
    }
    else if(!carvas->addEdge(sNum, eNum, weight))
    {
        //该边已存在
        QMessageBox::warning(this, "ERROR", "该边已存在");
    }
    sNodeLE->clear();
    eNodeLE->clear();
    weightLE->clear();
    qDebug()<<"按下 "<<sNum<<" "<<eNum<<"\n";
}

void Widget::removeEdge()
{
    int rNum1 = rEdgeNode1LE->text().toInt();
    int rNum2 = rEdgeNode2LE->text().toInt();
    int num = carvas->getNumVertex();
    if(rNum1 < 0 || rNum2 < 0 || rNum1 >= num || rNum2 >= num ||rNum1 == rNum2)
    {
         QMessageBox::warning(this, "ERROR", "输入数据有误，请重新输入");
    }
    else if(!carvas->removeEdge(rNum1, rNum2))
    {
        //删除失败
        QMessageBox::warning(this, "ERROR", "边不存在");
    }
    rEdgeNode1LE->clear();
    rEdgeNode2LE->clear();
}
