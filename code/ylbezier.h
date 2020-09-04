 #ifndef YLBEZIER_H
#define YLBEZIER_H

#include "ylshape.h"
#include "yldefine.h"

class YLBezier : public YLShape, public QGraphicsPathItem
{
public:
    YLBezier();
    enum {Type = UserType + BEZIER};
    void calculateBoundary() override;
    void setPoints(const QPointF &, const QPointF &) override;
    void setPoints(const QPointF &, const QPointF &, const QPointF &, const QPointF &);
    void updateMouseStyle() override;
    int type() const override;
    void caculateCurrentRect();
    int getPoints(QVector<QPointF> &) override;
    void setPoints(QVector<QPointF> &);
private:
    QPainterPath paint_curve(const QPointF &, const QPointF &, const QPointF &, const QPointF &);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void rotate(const QPointF &) override{};
    void mousePressEvent(QGraphicsSceneMouseEvent*) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent*) override;
    QPainterPath shape() const override;
    StretchRectState getCurrentStretchState(const QPointF &);
};

#endif // YLBEZIER_H
