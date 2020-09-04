#include "ylpolygon.h"
extern bool zoom;
YLPolygon::YLPolygon():YLShape(), QGraphicsPolygonItem()
{
    QPen Pen = pen();
    inner_color = QColor(240, 248, 255);
    setPen(Pen);
    setBrush(inner_color);
    setFlags(ItemIsSelectable|ItemIsMovable|ItemIsFocusable);
}

YLPolygon::YLPolygon(int pnum):YLShape(pnum), QGraphicsPolygonItem()
{
    QPen Pen = pen();
    inner_color = QColor(240, 248, 255);
    setPen(Pen);
    setBrush(inner_color);
    setFlags(ItemIsSelectable|ItemIsMovable|ItemIsFocusable);
}

void YLPolygon::updateMouseStyle()
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

void YLPolygon::calculateBoundary()
{
    QVector<QPointF>::iterator it = points.begin();
    if(it == points.end())
        return;
    left_bd = it->x(), top_bd = it->y(), right_bd = it->x(), bottom_bd = it->y();
    it++;
    while(it != points.end()) {
        if(it->x() < left_bd)
            left_bd = it->x();
        if(it->x() > right_bd)
            right_bd = it->x();
        if(it->y() < top_bd)
            top_bd = it->y();
        if(it->y() > bottom_bd)
            bottom_bd = it->y();
        it++;
    }
    width_bd = right_bd - left_bd;
    height_bd = bottom_bd - top_bd;

//    QRectF boundary_rect = boundingRect();
//    left_bd = boundary_rect.left();
//    top_bd = boundary_rect.top();
//    right_bd = boundary_rect.right();
//    bottom_bd = boundary_rect.bottom();
//    height_bd = boundary_rect.height();
//    width_bd = boundary_rect.width();
}

void YLPolygon::mousePressEvent(QGraphicsSceneMouseEvent* event){
    qDebug() << "青春属于表白 阳光属于窗台";
    if (zoom) return;
    qDebug() << "而我想我属于一个拥有你的未来";
    stretch_rect_state = getCurrentStretchState(event->pos());
    updateMouseStyle();
    QGraphicsItem::mousePressEvent(event);
}

void YLPolygon::mouseMoveEvent(QGraphicsSceneMouseEvent* event){
    if (stretch_rect_state != NONE){
        QPointF now = event->pos();
        switch (stretch_rect_state) {
        case LEFT_TOP:
            setPoints(now, QPointF(right_bd, bottom_bd));
            break;
        case LEFT_BOTTOM:
            setPoints(QPointF(right_bd, top_bd), now);
            break;
        case RIGHT_TOP:
            setPoints(now, QPointF(left_bd, bottom_bd));
            break;
        case RIGHT_BOTTOM:
            setPoints(QPointF(left_bd, top_bd), now);
            break;
        case TOP:
            setPoints(QPointF(left_bd, qMin(now.y(), bottom_bd-MIN_PIXEL)), QPointF(right_bd, bottom_bd));
            break;
        case BOTTOM:
            setPoints(QPointF(left_bd, top_bd), QPointF(right_bd, qMax(now.y(), top_bd+MIN_PIXEL)));
            break;
        case RIGHT:
            setPoints(QPointF(left_bd, top_bd), QPointF(qMax(now.x(), left_bd+MIN_PIXEL), bottom_bd));
            break;
        case LEFT:
            setPoints(QPointF(right_bd, top_bd), QPointF(qMin(now.x(), left_bd+MIN_PIXEL), bottom_bd));
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

void YLPolygon::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "而我心碎你受罪你的美";
    if (zoom) return;
    qDebug() << "我不配";
    calculateBoundary();
    stretch_rect_state = NONE;
    event->accept();
    QGraphicsItem::mouseReleaseEvent(event);
    updateMouseStyle();

//    QVector <QPointF> vec;
//    int num = this->getPoints(vec);
//    qreal mnx = 2000, mny = 2000, mxx = -2000, mxy = -2000;
    qDebug() << "刮风这天我试过握着你的手";
//    for (int i=0; i<num; i++){
//        mnx = qMin(mnx, mapToScene(vec[i]).x());
//        mny = qMin(mny, mapToScene(vec[i]).y());
//        mxx = qMax(mxx, mapToScene(vec[i]).x());
//        mxy = qMax(mxy, mapToScene(vec[i]).y());
//    }
//    this->setPoints(QPointF(mnx, mny), QPointF(mxx, mxy));
    qDebug() << "但偏偏风渐渐把距离吹的好远";
}

void YLPolygon::focusInEvent(QFocusEvent *event)
{
    color_setting->setVisible(true);
    connect(color_setting, SIGNAL(triggered()), this, SLOT(setColor()));
    QGraphicsPolygonItem::focusInEvent(event);
}

void YLPolygon::focusOutEvent(QFocusEvent *event)
{
    disconnect(color_setting, SIGNAL(triggered()), this, SLOT(setColor()));
    color_setting->setVisible(false);
    QGraphicsPolygonItem::focusOutEvent(event);
}

void YLPolygon::setColor()
{
    QColorDialog dia;
    dia.exec();
    inner_color = dia.selectedColor();
    setBrush(inner_color);
}

void YLPolygon::rotate(const QPointF &a)
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

qreal YLPolygon::getRotateAngle(){
    qreal angle = normalizeAngle(rotate_rad);
    return angle * 180 / pi;
}

void YLPolygon::rotate(qreal angle)
{
    QPointF central = boundingRect().topLeft();
    QTransform tran = transform();
    tran.translate(central.x(), central.y());
    tran.rotate(angle, Qt::ZAxis);
    tran.translate(-central.x(), -central.y());
    setTransform(tran);
}

int YLPolygon::getPoints(QVector<QPointF> &tmp)
{
    tmp.clear();
    for (int i=0; i<point_num; i++) tmp.push_back(mapToScene(points[i]));
    return point_num;
}

QString YLPolygon::getHexColor()
{
    QString red_hex = QString("%1").arg(inner_color.red(),2,16,QChar('0'));
    QString green_hex = QString("%1").arg(inner_color.green(),2,16,QChar('0'));
    QString blue_hex = QString("%1").arg(inner_color.blue(),2,16,QChar('0'));
    QString hex_str = "#"+red_hex+green_hex+blue_hex;
    return hex_str;
}

void YLPolygon::setColorFromString(QString color_str)
{
    inner_color.setNamedColor(color_str);
    setBrush(inner_color);

    qDebug() << "谁在用琵琶弹奏一曲东风破";
    qDebug() << "岁月在墙上剥落看见小时候";

}
