#ifndef YLRECTANGLE_H
#define YLRECTANGLE_H

#include "yldefine.h"
#include "ylpolygon.h"

class YLRectangle : public YLPolygon
{
public:
    YLRectangle();
    void setPoints(const QPointF &, const QPointF &) override;
    enum{Type = UserType + RECT};
    void getRect(double &, double &, double &, double &);
    int type() const override;
private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

#endif // YLRECTANGLE_H
