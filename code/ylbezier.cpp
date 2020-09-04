#include "ylbezier.h"

YLBezier::YLBezier():YLShape(4){
    QPen Pen = pen();
    stroke_color = QColor(240, 248, 255);
    setPen(Pen);
    setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsMovable);
    qDebug() << "繁华如三千东流水 我只取一瓢爱了解";
}

void YLBezier::calculateBoundary(){
    left_bd = points[0].x();
    right_bd = points[0].x();
    top_bd = points[0].y();
    bottom_bd = points[0].y();
    for (int i=1; i<point_num; i++){
        left_bd = qMin(left_bd, points[i].x());
        right_bd = qMax(right_bd, points[i].x());
        top_bd = qMin(top_bd, points[i].y());
        bottom_bd = qMax(bottom_bd, points[i].y());
    }
    width_bd = right_bd - left_bd;
    height_bd = bottom_bd - top_bd;
}

void YLBezier::setPoints(const QPointF &start, const QPointF &end){
    qDebug() << "I am in the bezier";
    points.clear();
    QPointF right_top(end.x(), start.y());
    QPointF left_bottom(start.x(), end.y());
    points << start << right_top << left_bottom << end;

    QPainterPath path = paint_curve(start, right_top, left_bottom, end);
    QGraphicsPathItem::setPath(path);
}

void YLBezier::setPoints(const QPointF &a, const QPointF &b, const QPointF &c, const QPointF &d){
    points.clear();
    points << a << b << c << d;
    caculateCurrentRect();
    QPainterPath path = paint_curve(a, b, c, d);
    QGraphicsPathItem::setPath(path);
}

void YLBezier::setPoints(QVector<QPointF> &tmp){
    points.clear();
    for (int i=0; i<4; i++) points << tmp[i];
    caculateCurrentRect();
    QPainterPath path = paint_curve(tmp[0], tmp[1], tmp[2], tmp[3]);
    QGraphicsPathItem::setPath(path);
}

void YLBezier::updateMouseStyle(){
    switch (stretch_rect_state) {
    case NONE:
        setCursor(Qt::ArrowCursor);
        break;
    case LEFT_TOP:
    case RIGHT_BOTTOM:
    case LEFT_BOTTOM:
    case RIGHT_TOP:
        setCursor(Qt::SizeFDiagCursor);
        break;
    default:
        break;
    }
}

int YLBezier::type() const{
    return Type;
}

void YLBezier::caculateCurrentRect(){
    qreal x1 = points[0].x(), y1 = points[0].y();
    qreal x2 = points[1].x(), y2 = points[1].y();
    qreal x3 = points[2].x(), y3 = points[2].y();
    qreal x4 = points[3].x(), y4 = points[3].y();
    calculateBoundary();
    left_top_rect = QRectF(x1 - STRETCH_SQUARE_LENGTH / 2.0, y1 - STRETCH_SQUARE_LENGTH / 2.0, STRETCH_SQUARE_LENGTH, STRETCH_SQUARE_LENGTH);
    right_top_rect = QRectF(x2 - STRETCH_SQUARE_LENGTH / 2.0, y2 - STRETCH_SQUARE_LENGTH / 2.0, STRETCH_SQUARE_LENGTH, STRETCH_SQUARE_LENGTH);
    left_bottom_rect = QRectF(x3 - STRETCH_SQUARE_LENGTH / 2.0, y3 - STRETCH_SQUARE_LENGTH / 2.0, STRETCH_SQUARE_LENGTH, STRETCH_SQUARE_LENGTH);
    right_bottom_rect = QRectF(x4 - STRETCH_SQUARE_LENGTH / 2.0, y4 - STRETCH_SQUARE_LENGTH / 2.0, STRETCH_SQUARE_LENGTH, STRETCH_SQUARE_LENGTH);
}

void YLBezier::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setRenderHint(QPainter::Antialiasing, true);
    QStyleOptionGraphicsItem op;
    op.initFrom(widget);
    op.state = QStyle::State_None;
    QGraphicsPathItem::paint(painter, &op, widget);
    if (option->state & QStyle::State_Selected){

        qreal pen_width = pen().widthF();
        const qreal pad = pen_width / 3;

        rect_stroke_color = QColor(qRgb(119, 136, 153));
        painter->setPen(QPen(stroke_color, 0, Qt::DashLine));
        painter->drawRect(boundingRect().adjusted(pad, pad, -pad, -pad));

//      calculateCurrentStretchRect();

        QColor rectcolor = QColor(qRgb(105, 105, 105));
        painter->fillRect(left_top_rect, rectcolor);
        painter->fillRect(left_bottom_rect, rectcolor);
        painter->fillRect(right_top_rect, rectcolor);
        painter->fillRect(right_bottom_rect, rectcolor);
    }
}

void YLBezier::mousePressEvent(QGraphicsSceneMouseEvent* event){
    stretch_rect_state = getCurrentStretchState(event->pos());
    if (event->button() == Qt::LeftButton){
        setSelected(true);
        setFocus();
        event->accept();
    }
}
void YLBezier::mouseMoveEvent(QGraphicsSceneMouseEvent* event){
    if (stretch_rect_state != NONE){
        QPointF now = event->pos();
        switch (stretch_rect_state) {
        case LEFT_TOP:
            setPoints(now, points[1], points[2], points[3]);
            break;
        case RIGHT_TOP:
            setPoints(points[0], now, points[2], points[3]);
            break;
        case LEFT_BOTTOM:
            setPoints(points[0], points[1], now, points[3]);
            break;
        case RIGHT_BOTTOM:
            setPoints(points[0], points[1], points[2], now);
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

QPainterPath YLBezier::shape() const{
    if (!isSelected()) return QGraphicsPathItem::shape();
    QPainterPath path;
    path.moveTo(QPointF(left_bd - 4 * STRETCH_SQUARE_LENGTH, top_bd - 4 * STRETCH_SQUARE_LENGTH));
    path.lineTo(QPointF(right_bd + 4 * STRETCH_SQUARE_LENGTH, top_bd - 4 * STRETCH_SQUARE_LENGTH));
    path.lineTo(QPointF(right_bd + 4 * STRETCH_SQUARE_LENGTH, bottom_bd + 4 * STRETCH_SQUARE_LENGTH));
    path.lineTo(QPointF(left_bd - 4 * STRETCH_SQUARE_LENGTH, bottom_bd + 4 * STRETCH_SQUARE_LENGTH));
    path.closeSubpath();
    return path;
}

YLShape::StretchRectState YLBezier::getCurrentStretchState(const QPointF &cur){
    if (left_top_rect.contains(cur))
        return LEFT_TOP;
    if (left_bottom_rect.contains(cur))
        return LEFT_BOTTOM;
    if (right_top_rect.contains(cur))
        return RIGHT_TOP;
    if (right_bottom_rect.contains(cur))
        return RIGHT_BOTTOM;
    return NONE;
}

QPainterPath YLBezier::paint_curve(const QPointF &a, const QPointF &b, const QPointF &c, const QPointF &d){
    QPainterPath curve_path;
    curve_path.moveTo(a);
    qreal dt = 1.0 / 10000;
    for (qreal i=0; i<1; i+=dt){
        qreal x = a.x() * (1 - i) * (1 - i) * (1 - i) + 3 * b.x() * i * (1 - i) * (1 - i) + 3 * c.x() * i * i * (1 - i)
                + d.x() * i * i * i;
        qreal y = a.y() * (1 - i) * (1 - i) * (1 - i) + 3 * b.y() * i * (1 - i) * (1 - i) + 3 * c.y() * i * i * (1 - i)
                + d.y() * i * i * i;
        curve_path.lineTo(QPointF(x, y));
    }
    return curve_path;
}

int YLBezier::getPoints(QVector<QPointF> &tmp)
{
    tmp.clear();
    for (int i=0; i<point_num; i++) tmp.push_back(mapToScene(points[i]));
    return point_num;
}
