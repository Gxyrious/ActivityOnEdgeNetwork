#include <QtWidgets>

#include "widget.h"

Widget::Widget(QWidget* parent) :
    QWidget(parent)
{
    //设置总的窗体大小
    this->setFixedSize(1000, 600);

    //carvas继承QWidget的类
    carvas = new Carvas(this);

    //输入框
    QGroupBox* inputGroup = new QGroupBox(tr("Input"));

    //输入框布局，使用栅格布局
    QGridLayout* inputLayout = new QGridLayout;

    //    组合框
    //    QLabel *modeLabel = new QLabel(tr("Mode:"));
    //    QComboBox *modeSelectComboBox = new QComboBox;
    //    modeSelectComboBox->addItem("add edge");
    //    modeSelectComboBox->addItem("delete edge");
    //    modeSelectComboBox->addItem("delete vertex");

    //! [1] 添加边
    QGroupBox* addEdgeGroup = new QGroupBox(tr("Add Edge"));

    QLabel* sLabel = new QLabel(tr("Start Node:"));
    sNodeLE = new QLineEdit;
    sNodeLE->setPlaceholderText("请输入起始结点");
    QLabel* eLabel = new QLabel(tr("  End Node:"));
    eNodeLE = new QLineEdit;
    eNodeLE->setPlaceholderText("请输入终止结点");
    QLabel* wLabel = new QLabel(tr("    Weight:"));
    weightLE = new QLineEdit;
    weightLE->setPlaceholderText(("请输入权值"));

    edgeSubmitButton = new QPushButton("加载边");//边提交按钮
    edgeSubmitButton->setFixedSize(300, 40);//设置固定大小

    //设置栅格布局
    QGridLayout* addEdgeLayout = new QGridLayout;
    addEdgeLayout->addWidget(sLabel, 0, 0, 1, 1);
    addEdgeLayout->addWidget(sNodeLE, 0, 1, 1, 4);
    addEdgeLayout->addWidget(eLabel, 1, 0, 1, 1);
    addEdgeLayout->addWidget(eNodeLE, 1, 1, 1, 4);
    addEdgeLayout->addWidget(wLabel, 2, 0, 1, 1);
    addEdgeLayout->addWidget(weightLE, 2, 1, 1, 4);
    addEdgeLayout->addWidget(edgeSubmitButton, 3, 0, 2, 5);

    //添加栅格布局
    addEdgeGroup->setLayout(addEdgeLayout);
    //! [1] 添加边

    //! [2] 删除边
    QGroupBox* removeEdgeGroup = new QGroupBox(tr("Remove Edge"));

    QLabel* rEdgeLabel1 = new QLabel(tr("Start Node:"));
    rEdgeNode1LE = new QLineEdit;
    rEdgeNode1LE->setPlaceholderText("请输入起始结点");
    QLabel* rEdgeLabel2 = new QLabel(tr("  End Node:"));
    rEdgeNode2LE = new QLineEdit;
    rEdgeNode2LE->setPlaceholderText("请输入终止结点");

    edgeRemoveButton = new QPushButton("删除边");//边删除按钮
//    QSizePolicy edgeRemoveButtonPolicy = edgeRemoveButton->sizePolicy();
//    edgeRemoveButtonPolicy.setVerticalPolicy(QSizePolicy::Minimum);
//    edgeRemoveButton->setSizePolicy(edgeRemoveButtonPolicy);
    edgeRemoveButton->setFixedSize(300, 40);

    QGridLayout* removeEdgeLayout = new QGridLayout;
    removeEdgeLayout->addWidget(rEdgeLabel1, 0, 0, 1, 1);
    removeEdgeLayout->addWidget(rEdgeNode1LE, 0, 1, 1, 4);
    removeEdgeLayout->addWidget(rEdgeLabel2, 1, 0, 1, 1);
    removeEdgeLayout->addWidget(rEdgeNode2LE, 1, 1, 1, 4);
    removeEdgeLayout->addWidget(edgeRemoveButton, 3, 0, 1, 5);

    removeEdgeGroup->setLayout(removeEdgeLayout);
    //! [2] 删除边

    //! [3] 生成AOE
    aoeGenerateButton = new QPushButton("生成AOE网络");//AOE网络生成按钮
    //设置部件大小策略为拉伸
    QSizePolicy aoeGenerateButtonPolicy = aoeGenerateButton->sizePolicy();
    aoeGenerateButtonPolicy.setVerticalPolicy(QSizePolicy::Minimum);
    aoeGenerateButton->setSizePolicy(aoeGenerateButtonPolicy);
    //aoeGenerateButton->setFixedSize(300, 150);
//! [4] 生成AOE

//! [5] 清空图
    clearGraphButton = new QPushButton("清空图", this);//清空图按钮
    clearGraphButton->setFixedSize(60, 40);
    clearGraphButton->move(this->width() - 2 * clearGraphButton->width(),
        this->height() - 2 * clearGraphButton->height());
//! [5] 清空图

//! [6] 设置输入框布局
    inputLayout->addWidget(addEdgeGroup);
    inputLayout->addWidget(removeEdgeGroup);
    inputLayout->addWidget(aoeGenerateButton);

    inputGroup->setLayout(inputLayout);
//! [6] 设置输入框布局

//! [7] 设置总体布局
    QGridLayout* layout = new QGridLayout;
    layout->addWidget(inputGroup, 0, 0, 1, 3);
    layout->addWidget(carvas, 0, 1, 1, 7);
    this->setLayout(layout);
//! [7] 设置总体布局

    //! [8] 设置信号和槽 signal and slot
    connect(edgeSubmitButton, SIGNAL(clicked(bool)), this, SLOT(submitEdge()));
    connect(edgeRemoveButton, SIGNAL(clicked(bool)), this, SLOT(removeEdge()));
    connect(aoeGenerateButton, SIGNAL(clicked(bool)), carvas, SLOT(generateCriticalEdge()));
    connect(clearGraphButton, SIGNAL(clicked(bool)), carvas, SLOT(clearGraph()));
//! [8] 设置信号和槽 signal and slot
}

Widget::~Widget()
{
    delete carvas;
    delete sNodeLE;
    delete eNodeLE;
    delete weightLE;
    delete rEdgeNode1LE;
    delete rEdgeNode2LE;
    delete edgeSubmitButton;
    delete edgeRemoveButton;
    delete aoeGenerateButton;
    delete clearGraphButton;
}

/*添加边*/
void Widget::submitEdge()
{
    //获取输入框数字
    int sNum = sNodeLE->text().toInt();
    int eNum = eNodeLE->text().toInt();
    int weight = weightLE->text().toInt();
    int num = carvas->getNumVertex();
    if (sNum < 0 || eNum < 0 || weight <= 0 || sNum >= num || eNum >= num || sNum == eNum)
    {
        //输入错误
        QMessageBox::warning(this, "ERROR", "输入数据有误，请重新输入");
    }
    else if (!carvas->addEdge(sNum, eNum, weight))
    {
        //该边已存在
        QMessageBox::warning(this, "ERROR", "该边已存在");
    }
    //清空输入框
    sNodeLE->clear();
    eNodeLE->clear();
    weightLE->clear();
}

/*删除边*/
void Widget::removeEdge()
{
    int rNum1 = rEdgeNode1LE->text().toInt();
    int rNum2 = rEdgeNode2LE->text().toInt();
    int num = carvas->getNumVertex();
    if (rNum1 < 0 || rNum2 < 0 || rNum1 >= num || rNum2 >= num || rNum1 == rNum2)
    {
        QMessageBox::warning(this, "ERROR", "输入数据有误，请重新输入");
    }
    else if (!carvas->removeEdge(rNum1, rNum2))
    {
        //删除失败
        QMessageBox::warning(this, "ERROR", "边不存在");
    }
    rEdgeNode1LE->clear();
    rEdgeNode2LE->clear();
}
