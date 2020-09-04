#include "ylhexagon.h"

YLHexagon::YLHexagon():YLPolygon(6)
{
    qDebug() << "我一路向北 离开有你的世界";
}

void YLHexagon::setPoints(const QPointF &start, const QPointF &end)
{
    points.clear();
    qreal midy = (start.y() + end.y()) / 2;
    qreal len = (end.x() - start.x()) * (4 - sqrt(7.0)) / 6;
    qreal one_thirdx = start.x() + len;
    qreal two_thirdsx = start.x() + (end.x() - start.x()) - len;
    QPointF p1(start.x(), midy);
    QPointF p2(one_thirdx, start.y());
    QPointF p3(two_thirdsx, start.y());
    QPointF p4(end.x(), midy);
    QPointF p5(two_thirdsx, end.y());
    QPointF p6(one_thirdx, end.y());
    points << p1 << p2 << p3 << p4 << p5 << p6;
    setPolygon(points);
}

void YLHexagon::setPoints(QVector <QPointF> &tmp){
    points.clear();
    for (int i=0; i<6; i++) points << tmp[i];
    setPolygon(points);
}

void YLHexagon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

QPainterPath YLHexagon::shape() const
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

int YLHexagon::type() const{
    return Type;
}
