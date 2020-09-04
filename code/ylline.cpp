#include "ylline.h"

YLLine::YLLine():YLShape(2){
    QPen Pen = pen();
    stroke_color = QColor(240, 248, 255);
    Pen.setWidthF(3);
    setPen(Pen);

    setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsMovable);
    qDebug() << "还记得你说家是唯一的城堡";
}

void YLLine::calculateBoundary(){
    qreal x1 = points[0].x(), y1 = points[0].y();
    qreal x2 = points[1].x(), y2 = points[1].y();

    left_bd = qMin(x1, x2);
    right_bd = qMax(x1, x2);
    top_bd = qMin(y1, y2);
    bottom_bd = qMax(y1, y2);
    width_bd = right_bd - left_bd;
    height_bd = bottom_bd - top_bd;
}

void YLLine::setPoints(const QPointF &start, const QPointF &end){
    points.clear();
    points << start << end;
    QGraphicsLineItem::setLine(QLineF(start, end));
}
void YLLine::updateMouseStyle(){
    switch (stretch_rect_state) {
    case NONE:
        setCursor(Qt::ArrowCursor);
        break;
    case LEFT:
    case RIGHT:
        setCursor(Qt::SizeFDiagCursor);
        break;
    default:
        break;
    }
}
int YLLine::type() const{
    return Type;
}

void YLLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setRenderHint(QPainter::Antialiasing, true);
    QGraphicsLineItem::paint(painter, option, widget);

    if (option->state & QStyle::State_Selected){
        qreal pen_width = pen().widthF();
        const qreal pad = pen_width / 3;

        rect_stroke_color = QColor(qRgb(119, 136, 153));
        painter->setPen(QPen(stroke_color, 0, Qt::DashLine));
        painter->drawRect(boundingRect().adjusted(pad, pad, -pad, -pad));

        calculateCurrentStretchRect();

        QColor rectcolor = QColor(qRgb(105, 105, 105));
        painter->fillRect(left_rect, rectcolor);
        painter->fillRect(right_rect, rectcolor);
    }
}

void YLLine::calculateCurrentStretchRect(){
    qreal x1 = points[0].x(), y1 = points[0].y();
    qreal x2 = points[1].x(), y2 = points[1].y();

    left_rect = QRectF(x1 - STRETCH_SQUARE_LENGTH / 2.0, y1 - STRETCH_SQUARE_LENGTH / 2.0, STRETCH_SQUARE_LENGTH, STRETCH_SQUARE_LENGTH);
    right_rect = QRectF(x2 - STRETCH_SQUARE_LENGTH / 2.0, y2 - STRETCH_SQUARE_LENGTH / 2.0, STRETCH_SQUARE_LENGTH, STRETCH_SQUARE_LENGTH);
}
YLShape::StretchRectState YLLine::getCurrentStretchState(const QPointF &cur){
    if (left_rect.contains(cur))
        return LEFT;
    if (right_rect.contains(cur))
        return RIGHT;
    return NONE;
}
void YLLine::mousePressEvent(QGraphicsSceneMouseEvent* event){
    stretch_rect_state = getCurrentStretchState(event->pos());
    if (event->button() == Qt::LeftButton){
        setSelected(true);
        setFocus();
        event->accept();
    }
}
void YLLine::mouseMoveEvent(QGraphicsSceneMouseEvent* event){
    if (stretch_rect_state != NONE){
        QPointF now = event->pos();
        switch (stretch_rect_state) {
        case LEFT:
            setPoints(now, points[1]);
            break;
        case RIGHT:
            setPoints(points[0], now);
            break;
        default:
            break;
        }
        event->isAccepted();
    }else {
        QGraphicsItem::mouseMoveEvent(event);
        event->isAccepted();
    }
}

int YLLine::getPoints(QVector<QPointF> &tmp)
{
    tmp.clear();
    for (int i=0; i<point_num; i++) tmp.push_back(mapToScene(points[i]));
    return point_num;
}

//QPainterPath YLLine::shape() const{
//    QPainterPath line_path;
//    line_path.moveTo(points[0]);
//    line_path.lineTo(points[1]);
//    return line_path;
//}
