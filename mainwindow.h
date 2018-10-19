#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include "../QCP/qcustomplot/qcustomplot.h"
#include "axistag.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void timerSlot();

  void startPlot();
  void stopPlot();

  void clearPlot();
  void getValues();

  void setMutexFlag();

  void variantChanged(int index);
private:
  Ui::MainWindow *ui;
  QCustomPlot *mPlot;
  QCustomPlot *mPlot2;
  //QCPItemEllipse *mEllipse;
  MyEllipse *mEllipse;
  MyEllipse *mEllipse2;
  MyEllipse *mEllipse3;

  QCPItemLine *mLine;



  QPointer<QCPGraph> mGraph1;
  //QPointer<QCPGraph> mGraph2;

  QCPCurve *fermatSpiral;

  AxisTag *mTag1;
  //AxisTag *mTag2;
  QTimer mDataTimer;
  QWidget *mPlotWidget;
  QLineEdit *mLineEditR1;
  QLineEdit *mLineEditR2;
  QLineEdit *mLineEditR3;
  QLineEdit *mLineEditIter;
  QLineEdit *mLineEditPolarAngel;
  QLineEdit *mLineEditTimerStep;
  QLineEdit *mLineEditPlot2XAxisRange;
  QLineEdit *mLineEditEpsilon;

  QLabel *mLabelCrIter;
  QLabel *mlabelRatio;
  QLabel *mlabelCurEps;

  QCheckBox *mCheckboxRevert;

  QPushButton *mStartButton;
  QPushButton *mStopButton;
  QPushButton *mClearButton;

  QComboBox *mComboVariant;

  double Rfixed;
  double Rmoving;
  double Rpoint;
  int numIteration;
  bool flag_mutex;
  double axisRange;
  int cyclVariant;
  double phi;
  int speedTm;
  double startPointX;
  double startPointY;
  int tsRange;
  double epsilon;

  double segmentLength(double X1, double Y1, double X2 = 0, double Y2 = 0);
};


#endif // MAINWINDOW_H
