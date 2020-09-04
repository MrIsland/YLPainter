#ifndef YLPOLYGON_H
#define YLPOLYGON_H

#include "yldefine.h"

class YLPolygon : public YLShape, public QGraphicsPolygonItem
{
    Q_OBJECT
public:
    YLPolygon();
    YLPolygon(int);
    void updateMouseStyle() override;
    void calculateBoundary() override;
    qreal getRotateAngle();
    void rotate(qreal);
    int getPoints(QVector<QPointF> &) override;
    QString getHexColor();
    void setColorFromString(QString);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent*) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent*) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent*) override;
	void focusInEvent(QFocusEvent*) override;
    void focusOutEvent(QFocusEvent*) override;
    void rotate(const QPointF &) override;
    qreal rotate_rad;
    QColor inner_color;
protected slots:
    void setColor();
};

#endif // YLPOLYGON_H
