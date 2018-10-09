#ifndef AXISTAG_H
#define AXISTAG_H


#include <QObject>
#include "../QCP/qcustomplot/qcustomplot.h"

class AxisTag : public QObject
{
  Q_OBJECT
public:
  explicit AxisTag(QCPAxis *parentAxis);
  virtual ~AxisTag();

  // setters:
  void setPen(const QPen &pen);
  void setBrush(const QBrush &brush);
  void setText(const QString &text);

  // getters:
  QPen pen() const { return mLabel->pen(); }
  QBrush brush() const { return mLabel->brush(); }
  QString text() const { return mLabel->text(); }

  // other methods:
  void updatePosition(double value);

protected:
  QCPAxis *mAxis;
  QPointer<QCPItemTracer> mDummyTracer;
  QPointer<QCPItemLine> mArrow;
  QPointer<QCPItemText> mLabel;
};


class MyEllipse : public QCPItemEllipse
{
    Q_OBJECT
public:
    explicit MyEllipse(QCustomPlot *parentPlot);
    void moveCenter(double x, double y);
    void setRadius(const double &x, const double &y);
public slots:
    void moveTracerCenter(const double &x, const double &y);
private:
    double xRadius;
    double yRadius;
    QCPItemTracer *mCenterTracer;
};

#endif // AXISTAG_H
