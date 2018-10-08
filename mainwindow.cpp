#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  //mPlot(0),
  //mPlotWidget(0),
  Rfixed(2.0),
  Rmoving(1.0),
  Rpoint(1.0),
  flag_mutex(true),
  axisRange(2.0),
  cyclVariant(0),
  phi(0.1),
  speedTm(40),
  startPointX(-53287652.0),
  startPointY(-0.987432),
  tsRange(100)
  //mTag1(0)
  //mTag2(0)
{
  ui->setupUi(this);

  mPlotWidget = new QWidget(this);

  this->setMinimumSize(800,820);

  mPlot = new QCustomPlot(mPlotWidget);
  mPlot->setMinimumSize(600,600);
  mPlot->setMaximumSize(600,600);
  mPlot->setInteractions(QCP::iRangeZoom);

  mPlot2 = new QCustomPlot(mPlotWidget);
  mPlot2->setMinimumSize(600,200);
  mPlot2->setMaximumSize(600,200);

  mStartButton = new QPushButton(tr("Start"));
  connect(mStartButton, &QPushButton::clicked, this, &MainWindow::startPlot);

  mStopButton = new QPushButton(tr("Stop"));
  mStopButton->setDisabled(true);
  connect(mStopButton, &QPushButton::clicked, this, &MainWindow::stopPlot);

  mClearButton = new QPushButton(tr("Clear"));
  mClearButton->setDisabled(true);
  connect(mClearButton, &QPushButton::clicked, this, &MainWindow::clearPlot);

  QLabel *labelR1 = new QLabel("R");
  QLabel *labelR2 = new QLabel("r");
  QLabel *labelR3 = new QLabel("h");
  QLabel *labelIteration = new QLabel("iter.");
  QLabel *labelSpeed = new QLabel("speed");
  QLabel *labelVariant = new QLabel("type");
  QLabel *labelRange = new QLabel("tsrange");

  mLineEditR1 = new QLineEdit("2");
  mLineEditR2 = new QLineEdit("1");
  mLineEditR3 = new QLineEdit("1");
  mLineEditIter = new QLineEdit("500");
  mLineEditSpeed = new QLineEdit("40");
  mLineEditPlot2XAxisRange = new QLineEdit("100");

  connect(mLineEditR1, &QLineEdit::editingFinished, this, &MainWindow::setMutexFlag);
  connect(mLineEditR2, &QLineEdit::editingFinished, this, &MainWindow::setMutexFlag);
  connect(mLineEditR3, &QLineEdit::editingFinished, this, &MainWindow::setMutexFlag);
  connect(mLineEditIter, &QLineEdit::editingFinished, this, &MainWindow::setMutexFlag);
  connect(mLineEditSpeed, &QLineEdit::editingFinished, this, &MainWindow::setMutexFlag);
  connect(mLineEditPlot2XAxisRange, &QLineEdit::editingFinished, this, &MainWindow::setMutexFlag);

  mComboVariant = new QComboBox;

  QStringList cyclStrList;

  cyclStrList << "hypocycloid" << "epicycloid";
  mComboVariant->addItems(cyclStrList);

  connect(mComboVariant, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index) {
      variantChanged(index);
  });

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(mStartButton);
  buttonLayout->addWidget(mStopButton);

  QGridLayout *greedLayout = new QGridLayout;
  greedLayout->addWidget(labelR1,0,0,Qt::AlignRight);
  greedLayout->addWidget(labelR2,1,0,Qt::AlignRight);
  greedLayout->addWidget(labelR3,2,0,Qt::AlignRight);
  greedLayout->addWidget(labelIteration,3,0,Qt::AlignRight);
  greedLayout->addWidget(labelSpeed,4,0,Qt::AlignRight);
  greedLayout->addWidget(labelVariant,5,0,Qt::AlignRight);
  greedLayout->addWidget(labelRange,6,0,Qt::AlignRight);

  greedLayout->addWidget(mLineEditR1,0,1,Qt::AlignRight);
  greedLayout->addWidget(mLineEditR2,1,1,Qt::AlignRight);
  greedLayout->addWidget(mLineEditR3,2,1,Qt::AlignRight);
  greedLayout->addWidget(mLineEditIter,3,1,Qt::AlignRight);
  greedLayout->addWidget(mLineEditSpeed,4,1,Qt::AlignRight);
  greedLayout->addWidget(mComboVariant,5,1,Qt::AlignRight);
  greedLayout->addWidget(mLineEditPlot2XAxisRange,6,1,Qt::AlignRight);

  QVBoxLayout *controlLayout = new QVBoxLayout;
  controlLayout->addLayout(greedLayout);
  controlLayout->addStretch();
  controlLayout->addWidget(mClearButton);
  controlLayout->addLayout(buttonLayout);

  QVBoxLayout *graphLayout = new QVBoxLayout;
  graphLayout->addWidget(mPlot);
  graphLayout->addWidget(mPlot2);

  QHBoxLayout *mainLayout = new QHBoxLayout;
  mainLayout->addLayout(graphLayout);

  mainLayout->addLayout(controlLayout);

  mPlotWidget->setLayout(mainLayout);

  setCentralWidget(mPlotWidget);

  fermatSpiral = new QCPCurve(mPlot->xAxis, mPlot->yAxis);

  mLine = new QCPItemLine(mPlot);

  mLine->setPen( QPen(Qt::green) );
  mLine->setVisible(false);


  mEllipse = new MyEllipse(mPlot);
  mEllipse2 = new MyEllipse(mPlot);
  mEllipse3 = new MyEllipse(mPlot);

  mEllipse2->setPen(QPen(Qt::red));

  mEllipse3->setBrush(QBrush(QColor(0, 0, 255, 30)));

  mEllipse3->setRadius(0.05,0.05);

  // color the curves:
  fermatSpiral->setPen(QPen(Qt::blue));
  fermatSpiral->setBrush(QBrush(QColor(0, 0, 255, 20)));

  // configure plot to have two right axes:
  mPlot2->yAxis->setTickLabels(false);
  //connect(mPlot->yAxis2, SIGNAL(rangeChanged(QCPRange)), mPlot->yAxis, SLOT(setRange(QCPRange))); // left axis only mirrors inner right axis
  mPlot2->yAxis2->setVisible(true);
  mPlot2->axisRect()->addAxis(QCPAxis::atRight);
  mPlot2->axisRect()->axis(QCPAxis::atRight, 0)->setPadding(30); // add some padding to have space for tags
  mPlot2->axisRect()->axis(QCPAxis::atRight, 1)->setPadding(30); // add some padding to have space for tags

  // // create graphs:
  mGraph1 = mPlot2->addGraph(mPlot2->xAxis, mPlot2->axisRect()->axis(QCPAxis::atRight, 0));
  //mGraph2 = mPlot->addGraph(mPlot->xAxis, mPlot->axisRect()->axis(QCPAxis::atRight, 1));
  mGraph1->setPen(QPen(QColor(250, 120, 0)));
  mGraph1->setBrush(QBrush(QColor(0, 0, 255, 20)));
  //mGraph2->setPen(QPen(QColor(0, 180, 60)));

  // // create tags with newly introduced AxisTag class (see axistag.h/.cpp):
  mTag1 = new AxisTag(mGraph1->valueAxis());
  mTag1->setPen(mGraph1->pen());
  //mTag2 = new AxisTag(mGraph2->valueAxis());
  //mTag2->setPen(mGraph2->pen());

  connect(&mDataTimer, SIGNAL(timeout()), this, SLOT(timerSlot()));

}

MainWindow::~MainWindow()
{
  delete ui;
}

//====================================
//
//====================================
void MainWindow::startPlot()
{
    speedTm = mLineEditSpeed->text().toInt();
    mDataTimer.start(speedTm);
    mClearButton->setDisabled(false);
    mStopButton->setDisabled(false);
    mStartButton->setDisabled(true);
    mLine->setVisible(true);
}

//====================================
//
//====================================
void MainWindow::stopPlot()
{
    mDataTimer.stop();
    mStartButton->setDisabled(false);
    mStopButton->setDisabled(true);

}


//====================================
//
//====================================
void MainWindow::clearPlot()
{
    fermatSpiral->data().data()->clear();
    mGraph1->data().data()->clear();
    mPlot->replot();
    mPlot2->replot();
}

//====================================
//
//====================================
void MainWindow::getValues()
{
    if(flag_mutex)
    {

        double r1 = mLineEditR1->text().toDouble();
        double r2 = mLineEditR2->text().toDouble();
        double r3 = mLineEditR3->text().toDouble();
        int itr = mLineEditIter->text().toInt();
        int rangets = mLineEditPlot2XAxisRange->text().toInt();

        if(r1>0.0)
            Rfixed = r1;
        if(r2>0.0)
            Rmoving = r2;
        if(r3>=0.0)
            Rpoint = r3;
        if(itr>0)
            numIteration = itr;
        if(r2 < r1)
        {
            axisRange = r1;
        }
        else
        {
            axisRange = r2;
        }
        if(r3>axisRange)
          axisRange=r3;

        if(rangets>10 && rangets< 5000)
            tsRange = rangets;

        flag_mutex = false;
    }
}

//====================================
//
//====================================
void MainWindow::variantChanged(int index)
{
    cyclVariant = index;
    flag_mutex = true;
}

//====================================
//
//====================================
void MainWindow::setMutexFlag()
{
    flag_mutex = true;
    mDataTimer.setInterval(mLineEditSpeed->text().toInt());
}



void MainWindow::timerSlot()
{
    getValues();

    // calculate and add a new data point to each graph:
    int i = fermatSpiral->dataCount();

    double TpolarAngle = phi*i;

    double m = Rmoving/Rfixed;

    double X = 0.0;
    double Y = 0.0;

    double RmovingCenterX=0.0;
    double RmovingCenterY=0.0;

    switch (cyclVariant) {
    case 0:
        RmovingCenterX = Rfixed*(1 - m) * qCos(m * TpolarAngle);
        RmovingCenterY = Rfixed*(1 - m) * qSin(m * TpolarAngle);
        X = RmovingCenterX + Rpoint * qCos(TpolarAngle * (1 - m));
        Y = RmovingCenterY - Rpoint * qSin(TpolarAngle * (1 - m));
        //axisRange = Rfixed;

        break;
    case 1:
        RmovingCenterX = Rfixed*(1 + m) * qCos(m * TpolarAngle);
        RmovingCenterY = Rfixed*(1 + m) * qSin(m * TpolarAngle);
        X = RmovingCenterX - Rpoint * qCos(TpolarAngle * (1 + m));
        Y = RmovingCenterY - Rpoint * qSin(TpolarAngle * (1 + m));
        //axisRange = Rfixed+Rpoint;
        break;
    default:
        break;
    }

    if(i==0)
    {
        startPointX = X;
        startPointY = Y;
        qDebug() << "StartPoint" << X << "   " << Y;
    }
    else
    {
        //qDebug() << "StartPoint else" << X << "   " << Y;
        if(qAbs(startPointX-X)<=0.0009 && qAbs(startPointY)-Y<=0.0009)
        {
            qDebug() << "StartPoint closed num iter = " << i;
            stopPlot();
        }

    }

    mEllipse->setRadius(Rfixed,Rfixed);
    mEllipse2->setRadius(Rmoving,Rmoving);

    mEllipse2->moveTracerCenter(RmovingCenterX,RmovingCenterY);

    mEllipse3->moveTracerCenter(X,Y);

    mLine->start->setCoords(QPointF(X,Y));
    mLine->end->setCoords(QPointF(RmovingCenterX,RmovingCenterY));

    fermatSpiral->addData(i,X,Y);
    //fermatSpiral->addData(i,qSqrt(phi)*qCos(phi), qSqrt(phi)*qSin(phi));

    // make key axis range scroll with the data:
    mPlot->xAxis->rescale();
    fermatSpiral->rescaleValueAxis(true, true);
    mGraph1->rescaleValueAxis(false, true);
    //mGraph2->rescaleValueAxis(false, true);
    mPlot->xAxis->setRange(0, 4*axisRange, Qt::AlignCenter);
    mPlot->yAxis->setRange(0, 4*axisRange, Qt::AlignBottom);

    // calculate and add a new data point to each graph:
    mGraph1->addData(mGraph1->dataCount(), Y);
    //mGraph2->addData(mGraph2->dataCount(), Y);

    // make key axis range scroll with the data:
    mPlot2->xAxis->rescale();
    mGraph1->rescaleValueAxis(false, true);
    //mGraph2->rescaleValueAxis(false, true);
    mPlot2->xAxis->setRange(mPlot2->xAxis->range().upper, tsRange, Qt::AlignRight);

    // update the vertical axis tag positions and texts to match the rightmost data point of the graphs:
    double graph1Value = mGraph1->dataMainValue(mGraph1->dataCount()-1);
    // double graph2Value = mGraph2->dataMainValue(mGraph2->dataCount()-1);
    mTag1->updatePosition(graph1Value);
    // mTag2->updatePosition(graph2Value);
    mTag1->setText(QString::number(graph1Value, 'f', 2));
    // mTag2->setText(QString::number(graph2Value, 'f', 2));

    mPlot->replot();
    mPlot2->replot();
    if(i> numIteration){
        stopPlot();
    }
}
