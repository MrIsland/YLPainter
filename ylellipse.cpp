#include "ylellipse.h"

YLEllipse::YLEllipse():YLShape(1){
    QPen Pen = pen();
    inner_color = QColor(240, 248, 255);
    setPen(Pen);
    setBrush(inner_color);
    setFlags(ItemIsSelectable|ItemIsMovable|ItemIsFocusable);
    qDebug() << "缓缓飘落的枫叶像思念";
    qDebug() << "我点燃烛火温暖岁末的秋天";
}

void YLEllipse::calculateBoundary()
{
    QRectF boundary_rect = rect();
    left_bd = boundary_rect.left();
    top_bd = boundary_rect.top();
    right_bd = boundary_rect.right();
    bottom_bd = boundary_rect.bottom();
    width_bd = boundary_rect.width();
    height_bd = boundary_rect.height();
}

void YLEllipse::setPoints(const QPointF& start, const QPointF& end)
{
    qreal left = qMin(start.x(), end.x());
    qreal top = qMin(start.y(), end.y());
    qreal right = qMax(start.x(), end.x());
    qreal bottom = qMax(start.y(), end.y());
    QGraphicsEllipseItem::setRect(left, top, right-left, bottom-top);

    points.clear();
    points << QPointF((left+right)/2, (top+bottom)/2);
    rx = (right - left) / 2;
    ry = (bottom - top) / 2;
}

void YLEllipse::setPoints(const QPointF &central, const double _rx, const double _ry){
    qreal left = central.x() - _rx;
    qreal top = central.y() - _ry;
    qreal right = central.x() + _rx;
    qreal bottom = central.y() + _ry;
    QGraphicsEllipseItem::setRect(left, top, right-left, bottom-top);

    points.clear();
    points << central;
    rx = _rx; ry = _ry;
}

void YLEllipse::updateMouseStyle()
{
    switch (stretch_rect_state) {
    case NONE:
        setCursor(Qt::ArrowCursor);
        break;
    case LEFT:
    case RIGHT:
        setCursor(Qt::SizeHorCursor);
        break;
    case LEFT_TOP:
    case RIGHT_BOTTOM:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case TOP:
    case BOTTOM:
        setCursor(Qt::SizeVerCursor);
        break;
    case RIGHT_TOP:
    case LEFT_BOTTOM:
        setCursor(Qt::SizeBDiagCursor);
        break;
    default:
        break;
    }
}

void YLEllipse::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    QGraphicsEllipseItem::paint(painter, option, widget);

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

void YLEllipse::rotate(const QPointF &a)
{
    QPointF central = boundingRect().center();
    QPointF cornor(central.x(), boundingRect().top());
    QLineF lttp(central, cornor), ltcs(central, a);

    qreal attp = acos(fabs(lttp.dx()) / lttp.length()), atcs = acos(fabs(ltcs.dx()) / ltcs.length());

    if (lttp.dx() <= 0 && lttp.dy() <= 0) attp = pi - attp;
    else if (lttp.dx() <= 0 && lttp.dy() > 0) attp += pi;
    while (attp < 0) attp += 2 * pi;
    while (attp > 2 * pi) attp -= 2 * pi;

    if (ltcs.dx() <= 0 && ltcs.dy() <= 0) atcs = pi - atcs;
    else if (ltcs.dx() <= 0 && ltcs.dy() > 0) atcs += pi;
    while (atcs < 0) atcs += 2 * pi;
    while (atcs > 2 * pi) atcs -= 2 * pi;

    auto angle = attp - atcs;
    QTransform tran = transform();
    tran.translate(central.x(), central.y());
    tran.rotateRadians(rotation() + angle, Qt::ZAxis);
    tran.translate(-central.x(), -central.y());
    setTransform(tran);
    rotate_rad += angle;
}

qreal YLEllipse::getRotateAngle(){
    qreal angle = normalizeAngle(rotate_rad);
    return angle * 180 / pi;
}

void YLEllipse::rotate(qreal angle)
{
    QPointF central = boundingRect().center();
    QTransform tran = transform();
    tran.translate(central.x(), central.y());
    tran.rotate(angle, Qt::ZAxis);
    tran.translate(-central.x(), -central.y());
    setTransform(tran);
}

void YLEllipse::mousePressEvent(QGraphicsSceneMouseEvent* event){
    stretch_rect_state = getCurrentStretchState(event->pos());
    if (event->button() == Qt::LeftButton){
        setSelected(true);
        setFocus();
        event->accept();
    }
}

void YLEllipse::mouseMoveEvent(QGraphicsSceneMouseEvent* event){
    if (stretch_rect_state != NONE){
        QPointF now = event->pos();
        switch (stretch_rect_state) {
        case LEFT_TOP:
            qDebug() << "Left-top!!!";
            setPoints(QPointF(right_bd, bottom_bd), now);
            break;
        case LEFT_BOTTOM:
            setPoints(QPointF(right_bd, top_bd), now);
            break;
        case RIGHT_TOP:
            setPoints(QPointF(left_bd, bottom_bd), now);
            break;
        case RIGHT_BOTTOM:
            setPoints(QPointF(left_bd, top_bd), now);
            break;
        case TOP:
            qDebug() << "TOP!";
            setPoints(QPointF(right_bd, bottom_bd), QPointF(left_bd, qMin(now.y(), bottom_bd-MIN_PIXEL)));
            break;
        case BOTTOM:
            setPoints(QPointF(left_bd, top_bd), QPointF(right_bd, qMax(now.y(), top_bd+MIN_PIXEL)));
            break;
        case RIGHT:
            setPoints(QPointF(left_bd, top_bd), QPointF(qMax(now.x(), left_bd+MIN_PIXEL), bottom_bd));
            break;
        case LEFT:
            setPoints(QPointF(right_bd, bottom_bd), QPointF(qMin(now.x(), right_bd-MIN_PIXEL), top_bd));
            break;
        case ROTATE:
            rotate(now);
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

void YLEllipse::focusInEvent(QFocusEvent *event)
{
    color_setting->setVisible(true);
    connect(color_setting, SIGNAL(triggered()), this, SLOT(setColor()));
    QGraphicsEllipseItem::focusInEvent(event);
}

void YLEllipse::focusOutEvent(QFocusEvent *event)
{
    disconnect(color_setting, SIGNAL(triggered()), this, SLOT(setColor()));
    color_setting->setVisible(false);
    QGraphicsEllipseItem::focusOutEvent(event);
}

void YLEllipse::setColor()
{
    QColorDialog dia;
    dia.exec();
    inner_color = dia.selectedColor();
    setBrush(inner_color);
}

QPainterPath YLEllipse::shape() const
{
    if(!isSelected()) {
        return QGraphicsEllipseItem::shape();
    }
    QPainterPath hexagon_path;
    hexagon_path.moveTo(left_bd, top_bd);
    hexagon_path.lineTo(right_bd, top_bd);
    hexagon_path.lineTo(right_bd, bottom_bd);
    hexagon_path.lineTo(left_bd, bottom_bd);
    hexagon_path.closeSubpath();
    return hexagon_path;
}

void YLEllipse::getR(double &x, double &y){
    x = rx; y = ry;
}

int YLEllipse::type() const{
    return Type;
}

int YLEllipse::getPoints(QVector<QPointF> &tmp)
{
    tmp.clear();
    for (int i=0; i<point_num; i++) tmp.push_back(mapToScene(points[i]));
    return point_num;
}

QString YLEllipse::getHexColor()
{
    QString red_hex = QString("%1").arg(inner_color.red(),2,16,QChar('0'));
    QString green_hex = QString("%1").arg(inner_color.green(),2,16,QChar('0'));
    QString blue_hex = QString("%1").arg(inner_color.blue(),2,16,QChar('0'));
    QString hex_str = "#"+red_hex+green_hex+blue_hex;
    return hex_str;
}

void YLEllipse::setColorFromString(QString color_str)
{
    inner_color.setNamedColor(color_str);
    setBrush(inner_color);

//    qDebug() << "谁在用琵琶弹奏一曲东风破";
//    qDebug() << inner_color;
//    qDebug() << "岁月在墙上剥落看见小时候";

}

