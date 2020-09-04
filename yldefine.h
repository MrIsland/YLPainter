#ifndef YLDEFINE_H
#define YLDEFINE_H

#include <cmath>
#include <QString>
#include <QDebug>
#include <QCursor>
#include <QPointF>
#include <QPainter>
#include <QKeyEvent>
#include <QTabWidget>
#include <QMainWindow>
#include <QGraphicsView>
#include <QMessageBox>
#include <QtSvg/QSvgGenerator>
#include <QtXml/QXmlAttributes>
#include <QtXml/QDomText>
#include <QtXml/QtXml>
#include <QLabel>
#include <QtXml/QDomDocument>
#include <QDialog>
#include <qfiledialog.h>
#include <QActionGroup>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsSceneEvent>
#include <QStyleOptionGraphicsItem>
#include <QColorDialog>

#include "ylshape.h"

// 选中矩形8个拖拽点小正方形边长;
#define STRETCH_SQUARE_LENGTH 8.0
//图形的最小大小边长：2.0像素
#define MIN_PIXEL 2.0
const double pi = acos(-1);
extern bool zoom;
extern QAction *color_setting;

enum MODE{
    RECT = 0,
    ELLIPSE = 1,
    TRIANGLE = 2,
    LINE = 3,
    BEZIER = 4,
    HEXAGON = 5,
    NOTHING = 6,
    MAGNIFY = 7,
    LESSON = 8
};



#endif // YLDEFINE_H
