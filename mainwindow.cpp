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
  tsRange(100),
  epsilon(0.001)
  //mTag1(0)
  //mTag2(0)
{
  ui->setupUi(this);

  mPlotWidget = new QWidget(this);

  this->setMinimumSize(800,820);

  mPlot = new QCustomPlot(mPlotWidget);
  mPlot->setMinimumSize(600,600);
  mPlot->setMaximumSize(600,600);
  mPlot->setInteraction(QCP::iRangeZoom, false);
  mPlot->setInteraction(QCP::iRangeDrag, true);

  mPlot2 = new QCustomPlot(mPlotWidget);
  mPlot2->setMinimumSize(600,200);
  mPlot2->setMaximumSize(600,200);
  mPlot2->setInteraction(QCP::iRangeDrag, false);
  mPlot2->setInteraction(QCP::iRangeZoom, false);
  mPlot2->axisRect()->setRangeZoom(Qt::Horizontal);
  mPlot2->axisRect()->setRangeDrag(Qt::Horizontal);   // Enable only drag along the horizontal axis

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
  QLabel *labelSpeed = new QLabel("timer int.");
  QLabel *labelVariant = new QLabel("type");
  QLabel *labelRange = new QLabel("tsrange");
  QLabel *labelStopCrit = new QLabel("Stop Criteria:");
  QLabel *labelEpsilon = new QLabel("Eps");
  QLabel *labelCurIter =  new QLabel("cur iter.");
  QLabel *labelRatio =  new QLabel("ratio");
  QLabel *labelCurDist = new QLabel("dist");
  QLabel *labelPolarAngel = new QLabel("Polar ang.");

  QSplitter *splitter1 = new QSplitter(Qt::Vertical);
  splitter1->setMinimumHeight(350);

  mLineEditR1 = new QLineEdit("2");
  mLineEditR2 = new QLineEdit("1");
  mLineEditR3 = new QLineEdit("1");
  mLineEditIter = new QLineEdit("500");
  mLineEditTimerStep = new QLineEdit("40");
  mLineEditPlot2XAxisRange = new QLineEdit("100");
  mLineEditEpsilon = new QLineEdit("0.001");
  mLineEditPolarAngel = new QLineEdit("0.1");

  mLabelCrIter =  new QLabel("0");
  mlabelRatio  =  new QLabel("0.5");
  mlabelCurEps = new QLabel("0.0");

  mCheckboxRevert = new QCheckBox("Revert");
  mCheckboxRevert->setCheckState(Qt::CheckState::Unchecked);

  mComboVariant = new QComboBox;
  QStringList cyclStrList;
  cyclStrList << "hypocycloid" << "epicycloid";
  mComboVariant->addItems(cyclStrList);

  connect(mLineEditR1, &QLineEdit::editingFinished, this, &MainWindow::setMutexFlag);
  connect(mLineEditR2, &QLineEdit::editingFinished, this, &MainWindow::setMutexFlag);
  connect(mLineEditR3, &QLineEdit::editingFinished, this, &MainWindow::setMutexFlag);
  connect(mLineEditIter, &QLineEdit::editingFinished, this, &MainWindow::setMutexFlag);
  connect(mLineEditTimerStep, &QLineEdit::editingFinished, this, &MainWindow::setMutexFlag);
  connect(mLineEditPlot2XAxisRange, &QLineEdit::editingFinished, this, &MainWindow::setMutexFlag);
  connect(mLineEditEpsilon, &QLineEdit::editingFinished, this, &MainWindow::setMutexFlag);
  connect(mCheckboxRevert, &QCheckBox::stateChanged, this, &MainWindow::setMutexFlag);
  connect(mLineEditPolarAngel, &QLineEdit::editingFinished, this, &MainWindow::setMutexFlag);

  connect(mComboVariant, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index) {
      variantChanged(index);
  });

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(mStartButton);
  buttonLayout->addWidget(mStopButton);

  QGridLayout *greedLayout = new QGridLayout;
  {
      int nRow = 0;

      greedLayout->addWidget(labelR1,nRow,0,Qt::AlignRight);
      greedLayout->addWidget(mLineEditR1,nRow,1,Qt::AlignRight);
      nRow++;
      greedLayout->addWidget(labelR2,nRow,0,Qt::AlignRight);
      greedLayout->addWidget(mLineEditR2,nRow,1,Qt::AlignRight);
      nRow++;
      greedLayout->addWidget(labelR3,nRow,0,Qt::AlignRight);
      greedLayout->addWidget(mLineEditR3,nRow,1,Qt::AlignRight);
      nRow++;
      greedLayout->addWidget(mCheckboxRevert,nRow,0,1,2,Qt::AlignRight);
      nRow++;
      greedLayout->addWidget(labelSpeed,nRow,0,Qt::AlignRight);
      greedLayout->addWidget(mLineEditTimerStep,nRow,1,Qt::AlignRight);
      nRow++;
      greedLayout->addWidget(labelPolarAngel,nRow,0,Qt::AlignRight);
      greedLayout->addWidget(mLineEditPolarAngel,nRow,1,Qt::AlignRight);
      nRow++;
      greedLayout->addWidget(labelRatio,nRow,0,Qt::AlignRight);
      greedLayout->addWidget(mlabelRatio,nRow,1,Qt::AlignLeft);
      nRow++;
      greedLayout->addWidget(labelVariant,nRow,0,Qt::AlignRight);
      greedLayout->addWidget(mComboVariant,nRow,1,Qt::AlignRight);
      nRow++;
      greedLayout->addWidget(labelStopCrit,nRow,0,1,2,Qt::AlignHCenter);
      nRow++;
      greedLayout->addWidget(labelIteration,nRow,0,Qt::AlignRight);
      greedLayout->addWidget(mLineEditIter,nRow,1,Qt::AlignRight);
      nRow++;
      greedLayout->addWidget(labelEpsilon,nRow,0,Qt::AlignRight);
      greedLayout->addWidget(mLineEditEpsilon,nRow,1,Qt::AlignRight);
      nRow++;
      greedLayout->addWidget(labelCurIter,nRow,0,Qt::AlignRight);
      greedLayout->addWidget(mLabelCrIter,nRow,1,Qt::AlignLeft);
      nRow++;
      greedLayout->addWidget(labelCurDist,nRow,0,Qt::AlignRight);
      greedLayout->addWidget(mlabelCurEps,nRow,1,Qt::AlignLeft);
      nRow++;
      greedLayout->addWidget(splitter1,nRow,0,1,2,Qt::AlignRight);
      nRow++;
      greedLayout->addWidget(labelRange,nRow,0,Qt::AlignRight);
      greedLayout->addWidget(mLineEditPlot2XAxisRange,nRow,1,Qt::AlignRight);

  }
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

  // style of curve
  fermatSpiral->setLineStyle(QCPCurve::LineStyle::lsLine);
  // for plot point size of 3 on plot
  //fermatSpiral->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc,3));

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
    speedTm = mLineEditTimerStep->text().toInt();
    mDataTimer.start(speedTm);
    mClearButton->setDisabled(false);
    mStopButton->setDisabled(false);
    mStartButton->setDisabled(true);
    mLine->setVisible(true);
    mPlot2->setInteraction(QCP::iRangeDrag, false);
    mPlot2->setInteraction(QCP::iRangeZoom, false);
    mPlot->setInteraction(QCP::iRangeDrag, false);
    mPlot->setInteraction(QCP::iRangeZoom, false);
}

//====================================
//
//====================================
void MainWindow::stopPlot()
{
    mDataTimer.stop();
    mStartButton->setDisabled(false);
    mStopButton->setDisabled(true);
    mPlot2->setInteraction(QCP::iRangeDrag, true);
    mPlot2->setInteraction(QCP::iRangeZoom, true);
    mPlot->setInteraction(QCP::iRangeDrag, true);
    mPlot->setInteraction(QCP::iRangeZoom, true);

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
        double eps_tmp = mLineEditEpsilon->text().toDouble();
        int itr = mLineEditIter->text().toInt();
        int rangets = mLineEditPlot2XAxisRange->text().toInt();
        double revt = 1.0;
        double polar = mLineEditPolarAngel->text().toDouble();

        if(mCheckboxRevert->isChecked())
        {
            revt = -1.0;
        }

        if(polar > 0.0 && polar <=20.0 )
        {
            phi = polar;
        }
        else
        {
            mLineEditPolarAngel->setText(QString::number(phi));
        }

        if(r1>0.0)
            Rfixed = r1;
        if(r2>0.0)
            Rmoving = r2;
        if(r3>=0.0)
            Rpoint = revt * r3;
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

        mlabelRatio->setText(QString::number(Rmoving/Rfixed));

        if(eps_tmp > 0 && eps_tmp <= 2.0)
            epsilon = eps_tmp;
        else
            mLineEditEpsilon->setText(QString::number(epsilon));

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
    mDataTimer.setInterval(mLineEditTimerStep->text().toInt());
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
        axisRange = Rfixed+Rpoint;
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
        //if(qFuzzyCompare(startPointX,X) && qFuzzyCompare(startPointY,Y))
        double segLen = segmentLength(startPointX, startPointY, X, Y);
        mlabelCurEps->setText(QString::number(segLen));

        if(segLen <=epsilon)
        {
            //qDebug() << "StartPoint closed num iter = " << i;
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

    mLabelCrIter->setText(QString::number(i));

    if(i> numIteration){
        stopPlot();
    }
}

double MainWindow::segmentLength(double X1, double Y1, double X2, double Y2)
{
    double x1x2_2 = X2 - X1;
    x1x2_2 = x1x2_2*x1x2_2;

    double y1y2_2 = Y2 - Y1;
    y1y2_2 = y1y2_2 * y1y2_2;

    return qSqrt( y1y2_2 + x1x2_2 );
}

//ui->setupUi(this);
//customPlot = new QCustomPlot();
//ui->gridLayout->addWidget(customPlot,0,0,1,1);

//customPlot->setInteraction(QCP::iRangeZoom,true);
//customPlot->setInteraction(QCP::iRangeDrag, true);
//customPlot->axisRect()->setRangeDrag(Qt::Horizontal);   // Enable only drag along the horizontal axis
//customPlot->axisRect()->setRangeZoom(Qt::Horizontal);   // Enable zoom only on the horizontal axis
//customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);   // Labl coordinates along the X axis as the Date and Time
//customPlot->xAxis->setDateTimeFormat("hh:mm");  // Set the date and time format

// // Customizable fonts on the axes
//customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
//customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));

// // Automatic scaling ticks on the X-axis
//customPlot->xAxis->setAutoTickStep(true);

// /* We are making visible the X-axis and Y on the top and right edges of the graph,
// * but disable them tick and labels coordinates
// * */
//customPlot->xAxis2->setVisible(true);
//customPlot->yAxis2->setVisible(true);
//customPlot->xAxis2->setTicks(false);
//customPlot->yAxis2->setTicks(false);
//customPlot->xAxis2->setTickLabels(false);
//customPlot->yAxis2->setTickLabels(false);

//customPlot->yAxis->setTickLabelColor(QColor(Qt::red));
//customPlot->legend->setVisible(true);   // Enable Legend
// // Set the legend in the upper left corner of the chart
//customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);

// // Initialize the chart and bind it to the axes
//graphic = new QCPGraph(customPlot->xAxis, customPlot->yAxis);
//customPlot->addPlottable(graphic);
//graphic->setName("Доход, Р");
//graphic->setPen(QPen(QColor(Qt::red)));
//graphic->setAntialiased(false);
//graphic->setLineStyle(QCPGraph::lsImpulse); // Charts in a pulse ticks view

//connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)),
//        this, SLOT(slotRangeChanged(QCPRange)));

// // We will build schedule with today's day and the current second in a future
//double now = QDateTime::currentDateTime().toTime_t();
// // We declare a vector of time and income
//QVector <double> time(400), income(400);

//srand(15); // Initialize the random number generator

// // ЗFill in the values of the graph
//for (int i=0; i<400; ++i)
//  {
//    time[i] = now + 3600*i;
//    income[i] = qFabs(income[i-1]) + (i/50.0+1)*(rand()/(double)RAND_MAX-0.5);
//  }

//graphic->setData(time, income);
//customPlot->rescaleAxes();
//customPlot->replot();
//}

//MainWindow::~MainWindow()
//{
//delete ui;
//}

//void MainWindow::slotRangeChanged(const QCPRange &newRange)
//{
// /* If a scope chart is less than one day,
// * then display the hours and minutes in the Axis X,
// * otherwise display the date "Month Day Year"
// * */
//customPlot->xAxis->setDateTimeFormat((newRange.size() <= 86400)? "hh:mm" : "dd MMM yy");
//}
