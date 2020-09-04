#include "ylrectangle.h"

YLRectangle::YLRectangle():YLPolygon(4)
{
    qDebug() << "听妈妈的话 别让她受伤";
}

void YLRectangle::setPoints(const QPointF &start, const QPointF &end)
{
    points.clear();
    QPointF p1(start.x(), start.y());
    QPointF p2(end.x(), start.y());
    QPointF p3(end.x(), end.y());
    QPointF p4(start.x(), end.y());
    points << p1 << p2 << p3 << p4;
    setPolygon(points);
}

void YLRectangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    QGraphicsPolygonItem::paint(painter, option, widget);

    if (option->state & QStyle::State_Selected){
        qreal pen_width = pen().widthF();
        const qreal pad = pen_width / 3;

        rect_stroke_color = QColor(qRgb(119, 136, 153));
        painter->setPen(QPen(stroke_color, 0, Qt::DashLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(boundingRect().adjusted(pad, pad, -pad, -pad));

        calculateCurrentStretchRect();

        QColor rectcolor = QColor(qRgb(105, 105, 105));
        painter->fillRect(left_rect, rectcolor);
        painter->fillRect(left_top_rect, rectcolor);
        painter->fillRect(left_bottom_rect, rectcolor);
        painter->fillRect(top_rect, rectcolor);
        painter->fillRect(right_top_rect, rectcolor);
        painter->fillRect(right_rect, rectcolor);
        painter->fillRect(right_bottom_rect, rectcolor);
        painter->fillRect(bottom_rect, rectcolor);


        QColor rotatecolor = QColor(qRgb(50, 205, 50));
        painter->fillRect(rotate_rect, rotatecolor);

    }
}

void YLRectangle::getRect(double &x, double &y, double &width, double &height){
    x = mapToScene(points[0]).x(); y = mapToScene(points[0]).y(); width = width_bd; height = height_bd;
}

int YLRectangle::type() const{
    return Type;
}
