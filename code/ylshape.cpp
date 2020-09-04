#include "ylshape.h"

YLShape::YLShape():resizing(false), stretch_rect_state(NONE)
{}

YLShape::YLShape(int pnum):point_num(pnum), resizing(false), stretch_rect_state(NONE)
{}

YLShape::StretchRectState YLShape::getCurrentStretchState(const QPointF &cur)
{
    if(left_rect.contains(cur))
        return LEFT;
    if(left_top_rect.contains(cur))
        return LEFT_TOP;
    if(top_rect.contains(cur))
        return TOP;
    if(right_top_rect.contains(cur))
        return RIGHT_TOP;
    if(right_rect.contains(cur))
        return RIGHT;
    if(right_bottom_rect.contains(cur))
        return RIGHT_BOTTOM;
    if(bottom_rect.contains(cur))
        return BOTTOM;
    if(left_bottom_rect.contains(cur))
        return LEFT_BOTTOM;
    if(rotate_rect.contains(cur))
        return ROTATE;
    return NONE;
}

void YLShape::calculateCurrentStretchRect()
{
    calculateBoundary();
    left_rect = QRectF(left_bd, (top_bd+bottom_bd-STRETCH_SQUARE_LENGTH)/2.0, STRETCH_SQUARE_LENGTH, STRETCH_SQUARE_LENGTH);
    left_top_rect = QRectF(left_bd, top_bd, STRETCH_SQUARE_LENGTH, STRETCH_SQUARE_LENGTH);
    top_rect = QRectF((left_bd+right_bd-STRETCH_SQUARE_LENGTH)/2.0, top_bd, STRETCH_SQUARE_LENGTH, STRETCH_SQUARE_LENGTH);
    right_top_rect = QRectF(right_bd-STRETCH_SQUARE_LENGTH, top_bd, STRETCH_SQUARE_LENGTH, STRETCH_SQUARE_LENGTH);
    right_rect = QRectF(right_bd-STRETCH_SQUARE_LENGTH, (top_bd+bottom_bd-STRETCH_SQUARE_LENGTH)/2.0, STRETCH_SQUARE_LENGTH, STRETCH_SQUARE_LENGTH);
    right_bottom_rect = QRectF(right_bd-STRETCH_SQUARE_LENGTH, bottom_bd-STRETCH_SQUARE_LENGTH, STRETCH_SQUARE_LENGTH, STRETCH_SQUARE_LENGTH);
    bottom_rect = QRectF((left_bd+right_bd-STRETCH_SQUARE_LENGTH)/2.0, bottom_bd-STRETCH_SQUARE_LENGTH, STRETCH_SQUARE_LENGTH, STRETCH_SQUARE_LENGTH);
    left_bottom_rect = QRectF(left_bd, bottom_bd-STRETCH_SQUARE_LENGTH, STRETCH_SQUARE_LENGTH, STRETCH_SQUARE_LENGTH);
    rotate_rect = QRectF((left_bd + right_bd - STRETCH_SQUARE_LENGTH) / 2.0, top_bd + height_bd / 3 ,STRETCH_SQUARE_LENGTH, STRETCH_SQUARE_LENGTH);
}

qreal YLShape::normalizeAngle(qreal &angle){
    while (angle >= 2 * pi) angle -= 2 * pi;
    while (angle < 0) angle += 2 * pi;
    return angle;
}

