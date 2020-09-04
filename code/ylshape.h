#ifndef YLSHAPE_H
#define YLSHAPE_H

#include "yldefine.h"

class YLShape : public QObject
{
public:
    YLShape();
    YLShape(int);
    enum StretchRectState {
        NONE = 0,
        LEFT_TOP = 1,
        TOP = 2,
        RIGHT_TOP = 3,
        RIGHT = 4,
        RIGHT_BOTTOM = 5,
        BOTTOM = 6,
        LEFT_BOTTOM = 7,
        LEFT = 8,
        ROTATE = 9
    };

    virtual void calculateBoundary() = 0;
    virtual void setPoints(const QPointF&, const QPointF&) = 0;
    virtual void updateMouseStyle() = 0;
    virtual void rotate(const QPointF&) = 0;
    StretchRectState getCurrentStretchState(const QPointF&A);
    void calculateCurrentStretchRect();
    virtual int getPoints(QVector<QPointF> &) = 0;
    static qreal normalizeAngle(qreal &);

protected:
    int point_num;
    QVector<QPointF> points;
    QColor stroke_color, rect_stroke_color;

    qreal left_bd, top_bd, right_bd, bottom_bd, width_bd, height_bd;
    QRectF left_rect, top_rect, right_rect, bottom_rect;
    QRectF left_top_rect, right_top_rect, right_bottom_rect, left_bottom_rect;
    QRectF rotate_rect;

    bool resizing;
    StretchRectState stretch_rect_state;
};

#endif // YLSHAPE_H
