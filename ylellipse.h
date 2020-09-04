#ifndef YLELLIPSE_H
#define YLELLIPSE_H

#include "yldefine.h"

class YLEllipse : public YLShape, public QGraphicsEllipseItem
{
	Q_OBJECT
public:
    YLEllipse();
    enum {Type = UserType + ELLIPSE};
    void calculateBoundary() override;
    void setPoints(const QPointF &, const QPointF &) override;
    void setPoints(const QPointF &, const double, const double);
    void updateMouseStyle() override;
    void getR(double &, double &);
    int type() const override;
    int getPoints(QVector<QPointF> &) override;
    void setColorFromString(QString);
    void rotate(qreal);
    qreal getRotateAngle();
    QString getHexColor();
protected:
    QColor inner_color;
    qreal rx, ry;
    qreal rotate_rad;
private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void rotate(const QPointF &) override;
    void mousePressEvent(QGraphicsSceneMouseEvent*) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent*) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

private slots:
    void setColor();

    QPainterPath shape() const override;
};

#endif // YLELLIPSE_H
