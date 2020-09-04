#include "yltriangle.h"

YLTriangle::YLTriangle():YLPolygon(3)
{
    qDebug() << "最美的不是下雨天";
    qDebug() << "而是与你一起躲过的屋檐";
}

void YLTriangle::setPoints(const QPointF &start, const QPointF &end)
{
    points.clear();

    QPointF p1(start.x(), end.y());
    QPointF p2((start.x() + end.x()) / 2.0, start.y());
    QPointF p3(end.x(), end.y());
    points << p1 << p2 << p3;
    setPolygon(points);
}

void YLTriangle::setPoints(QVector <QPointF> &tmp)
{
    points.clear();
    points << tmp[0] << tmp[1] << tmp[2];
    setPolygon(points);
}

void YLTriangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

QPainterPath YLTriangle::shape() const
{
    if(!isSelected()) {
        return QGraphicsPolygonItem::shape();
    }
    QPainterPath hexagon_path;
    hexagon_path.moveTo(left_bd, top_bd);
    hexagon_path.lineTo(right_bd, top_bd);
    hexagon_path.lineTo(right_bd, bottom_bd);
    hexagon_path.lineTo(left_bd, bottom_bd);
    hexagon_path.closeSubpath();
    return hexagon_path;
}

int YLTriangle::type() const{
    return Type;
}
