#ifndef YLLINE_H
#define YLLINE_H

#include "yldefine.h"

class YLLine : public YLShape, public QGraphicsLineItem
{
public:
    YLLine();
    enum {Type = UserType + LINE};
    void calculateBoundary() override;
    void setPoints(const QPointF &, const QPointF &) override;
    void updateMouseStyle() override;
    int type() const override;
    void calculateCurrentStretchRect();
    int getPoints(QVector<QPointF> &) override;
private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void rotate(const QPointF &) override{};
    void mousePressEvent(QGraphicsSceneMouseEvent*) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent*) override;
//    QPainterPath shape() const override;
    StretchRectState getCurrentStretchState(const QPointF&A);
};

#endif // YLLINE_H
