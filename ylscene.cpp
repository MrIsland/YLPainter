#include "ylscene.h"
#include "yldefine.h"
//#include "mainwindow.h"

//using namespace QGraphicsScene;

ylscene::ylscene(){
}

void ylscene::setMode(const MODE tmp_mode){
    nw_mode = tmp_mode;
}

MODE ylscene::getMODE() const{
    return nw_mode;
}

void ylscene::mousePressEvent(QGraphicsSceneMouseEvent *event){
    QGraphicsScene::mousePressEvent(event);
    if (!event->isAccepted()){
        if (nw_mode == MAGNIFY && event->button() == Qt::LeftButton){
            QList <QGraphicsItem *> list = this->items();
            if (scale_rate < 32){
//                for (auto i:list){
//                    i->setSelected(false);
//                }
                this->views()[0]->scale(2.0, 2.0);
                scale_rate = 2 * scale_rate;
                emit(scale_change(scale_rate));
            }
        }else if (nw_mode == LESSON && event->button() == Qt::LeftButton){
            QList <QGraphicsItem *> list = this->items();

            if (scale_rate > 1.0 / 32){
                this->views()[0]->scale(1.0 / 2.0, 1.0 / 2.0);
                scale_rate = scale_rate / 2.0;
                emit(scale_change(scale_rate));
            }
        }else if (event->button() == Qt::LeftButton && !isCreating && nw_mode == NOTHING){
            pStart = event -> scenePos();
            isSelecting = true;
            outline = new QGraphicsRectItem;
            this -> addItem(outline);
        }else if (event->button() == Qt::LeftButton && !isCreating){
            pStart = event -> scenePos();
            isCreating = true;
            outline = new QGraphicsRectItem;
            this -> addItem(outline);
        }
    }
}

void ylscene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    qDebug() << "nan shen is painting";
    QGraphicsScene::mouseReleaseEvent(event);
    if (event->isAccepted()) return;
    if (isCreating){
        pEnd = event->scenePos();
        QGraphicsItem *item = nullptr;
        switch (nw_mode){
        case RECT:
           item = new YLRectangle();
           dynamic_cast<YLRectangle *>(item) -> setPoints(pStart, pEnd);
           dynamic_cast<YLRectangle *>(item) -> calculateCurrentStretchRect();
           break;
        case TRIANGLE:
            item = new YLTriangle();
            dynamic_cast<YLTriangle *>(item) -> setPoints(pStart, pEnd);
            dynamic_cast<YLTriangle *>(item) -> calculateCurrentStretchRect();
            break;
        case HEXAGON:
            item = new YLHexagon();
            dynamic_cast<YLHexagon *>(item) -> setPoints(pStart, pEnd);
            dynamic_cast<YLHexagon *>(item) -> calculateCurrentStretchRect();
            break;
        case ELLIPSE:
            item = new YLEllipse();
            dynamic_cast<YLEllipse *>(item) -> setPoints(pStart, pEnd);
            dynamic_cast<YLEllipse *>(item) -> calculateCurrentStretchRect();
            break;
        case LINE:
            item = new YLLine();
            dynamic_cast<YLLine *>(item) -> setPoints(pStart, pEnd);
            dynamic_cast<YLLine *>(item) -> calculateCurrentStretchRect();
            break;
        case BEZIER:
            item = new YLBezier();
            dynamic_cast<YLBezier *>(item) -> setPoints(pStart, pEnd);
            dynamic_cast<YLBezier *>(item) ->calculateCurrentStretchRect();
            break;
        default:
            break;
        }
        if (item != nullptr) addItem(item);
        isCreating = false;
        pStart = QPointF(0,0);
        pEnd = QPointF(0,0);
        removeItem(outline);
        delete outline;

    }else if (isSelecting){
        if (pStart == pEnd){
            isCreating = false;
            removeItem(outline);
            delete outline;
            return;
        }

        for (auto i:items(outline->rect())){
            if (i == nullptr) continue;
            else i->setSelected(true);

        }
        isSelecting = false;
        pStart = QPointF(0, 0);
        pEnd = QPointF(0, 0);
        if (outline != nullptr){
            removeItem(outline);
            delete outline;
        }
    }
}

void ylscene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mouseMoveEvent(event);
    if (event->isAccepted() || (!isCreating && !isSelecting)){
        this->update();
        return;
    }
    QPointF pNow = event->scenePos();
    if (isSelecting){
        QPen pen;
        pen.setStyle(Qt::DashLine);
        outline->setPen(pen);
    }
    outline->setRect(QRectF(qMin(pStart.x(),pNow.x()),qMin(pStart.y(),pNow.y()),qMax(pStart.x(),pNow.x())-qMin(pStart.x(),pNow.x()),qMax(pStart.y(),pNow.y())-qMin(pStart.y(),pNow.y())));
    this->update();
}

void ylscene::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Delete){
        while (!selectedItems().isEmpty()) removeItem(selectedItems().front());
    }else QGraphicsScene::keyPressEvent(event);
}

void ylscene::setScale(double s){
    scale_rate = s;
}

double ylscene::getScale(){
    return scale_rate;
}

