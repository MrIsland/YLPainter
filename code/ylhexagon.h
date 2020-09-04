#ifndef YLHEXAGON_H
#define YLHEXAGON_H

#include "yldefine.h"
#include "ylpolygon.h"

class YLHexagon : public YLPolygon
{
public:
    YLHexagon();
    enum {Type = UserType + HEXAGON};
    void setPoints(const QPointF &, const QPointF &) override;
    void setPoints(QVector <QPointF> &);
    int type() const override;
private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QPainterPath shape() const override;
};

#endif // YLHEXAGON_H
