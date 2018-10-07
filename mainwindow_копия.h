#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../QCP/qcustomplot/qcustomplot.h"
#include "axistag.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void timerSlot();

private:
  Ui::MainWindow *ui;
  QCustomPlot *mPlot;
  QPointer<QCPGraph> mGraph1;
  QPointer<QCPGraph> mGraph2;
  AxisTag *mTag1;
  AxisTag *mTag2;
  QTimer mDataTimer;
};


#endif // MAINWINDOW_H
