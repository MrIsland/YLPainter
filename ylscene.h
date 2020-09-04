#ifndef YLSCENE_H
#define YLSCENE_H

#include "yldefine.h"
#include "ylrectangle.h"
#include "yltriangle.h"
#include "ylhexagon.h"
#include "ylline.h"
#include "ylellipse.h"
#include "ylbezier.h"

class ylscene : public QGraphicsScene
{
    Q_OBJECT
public:
    void setMode(const MODE);
    MODE getMODE() const;
    void setScale(double);
    double getScale();
    ylscene();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    bool isCreating = false, isSelecting = false;
    MODE nw_mode;
    QPointF pStart, pEnd;
    QGraphicsRectItem *outline;
    double scale_rate = 1.0;

signals:
    void scale_change(double);

};

#endif // YLSCENE_H
