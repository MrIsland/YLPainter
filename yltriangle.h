#ifndef YLTRIANGLE_H
#define YLTRIANGLE_H

#include "yldefine.h"
#include "ylpolygon.h"

class YLTriangle : public YLPolygon
{
public:
    YLTriangle();
    enum {Type = UserType + TRIANGLE};
    void setPoints(const QPointF &, const QPointF &) override;
    void setPoints(QVector <QPointF> &);
    int type() const override;
private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QPainterPath shape() const override;
};
qreal minThree(qreal, qreal, qreal);
qreal maxThree(qreal, qreal, qreal);

#endif // YLTRIANGLE_H
