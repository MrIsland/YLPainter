#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ylscene.h"
#include "yldefine.h"
#include "ylhexagon.h"
#include "yltriangle.h"
#include "ylellipse.h"
#include "ylrectangle.h"
#include <QtXml/QDomDocument>

bool zoom;
QAction *color_setting;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(800, 600);

    Item = new QActionGroup(this);

    Item -> addAction(ui->action_rect);
    Item -> addAction(ui->action_line);
    Item -> addAction(ui->action_curve);
    Item -> addAction(ui->action_mouse);
    Item -> addAction(ui->action_triangle);
    Item -> addAction(ui->action_hexagon);
    Item -> addAction(ui->action_ellipse);
    Item -> addAction(ui->action_clear);
    Item -> addAction(ui->action_close);
    Item -> addAction(ui->action_magnifier);
    Item -> addAction(ui->action_lesson);
    Item -> addAction(ui->action_color);
	
    ui->action_color->setVisible(false);
	color_setting = ui->action_color;

    connect(ui->action_rect, SIGNAL(triggered()), this, SLOT(chooseItem()));
    connect(ui->action_line, SIGNAL(triggered()), this, SLOT(chooseItem()));
    connect(ui->action_triangle, SIGNAL(triggered()), this, SLOT(chooseItem()));
    connect(ui->action_mouse, SIGNAL(triggered()), this, SLOT(chooseItem()));
    connect(ui->action_hexagon, SIGNAL(triggered()), this, SLOT(chooseItem()));
    connect(ui->action_ellipse, SIGNAL(triggered()), this, SLOT(chooseItem()));
    connect(ui->action_curve, SIGNAL(triggered()), this, SLOT(chooseItem()));
    connect(ui->action_magnifier, SIGNAL(triggered()), this, SLOT(chooseItem()));
    connect(ui->action_lesson, SIGNAL(triggered()), this, SLOT(chooseItem()));

    scale_status->setText("100%");
    ui->statusbar->addPermanentWidget(scale_status);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::chooseItem(){
    qDebug() << "da shi xiong is choosing Item";
    if(tab == nullptr || tab->currentWidget() == nullptr)
        return;
    if (myCursor != nullptr) {
        delete myCursor;
        myCursor = nullptr;
    }
    QAction *checked = Item->checkedAction();
    zoom = false;
    if (checked != ui->action_mouse && checked != ui->action_magnifier && checked != ui->action_lesson) setCursor(Qt::CrossCursor);
    if(checked == ui->action_rect)
        dynamic_cast<ylscene*>(dynamic_cast<QGraphicsView*>(tab->currentWidget())->scene())->setMode(RECT);
    else if(checked == ui->action_triangle)
        dynamic_cast<ylscene*>(dynamic_cast<QGraphicsView*>(tab->currentWidget())->scene())->setMode(TRIANGLE);
    else if(checked == ui->action_hexagon)
        dynamic_cast<ylscene*>(dynamic_cast<QGraphicsView*>(tab->currentWidget())->scene())->setMode(HEXAGON);
    else if(checked == ui->action_ellipse)
        dynamic_cast<ylscene*>(dynamic_cast<QGraphicsView*>(tab->currentWidget())->scene())->setMode(ELLIPSE);
    else if (checked == ui->action_line)
        dynamic_cast<ylscene*>(dynamic_cast<QGraphicsView*>(tab->currentWidget())->scene())->setMode(LINE);
    else if (checked == ui->action_curve)
        dynamic_cast<ylscene*>(dynamic_cast<QGraphicsView*>(tab->currentWidget())->scene())->setMode(BEZIER);
    else if(checked == ui->action_mouse) {
        setCursor(Qt::ArrowCursor);
        dynamic_cast<ylscene*>(dynamic_cast<QGraphicsView*>(tab->currentWidget())->scene())->setMode(NOTHING);
    }else if (checked == ui->action_magnifier){
        zoom = true;
        qDebug() << "| That is your cursor sheet ? |";
        myCursor = new QCursor(QPixmap("://icon/shubiaofangfa.png"),-1,-1);
        setCursor(*myCursor);
        dynamic_cast<ylscene*>(dynamic_cast<QGraphicsView*>(tab->currentWidget())->scene())->setMode(MAGNIFY);
    }else if (checked == ui->action_lesson){
        zoom = true;
        myCursor=new QCursor(QPixmap("://icon/suoxiaojing-.png"),-1,-1);
        setCursor(*myCursor);
        dynamic_cast<ylscene*>(dynamic_cast<QGraphicsView*>(tab->currentWidget())->scene())->setMode(LESSON);
    }
}

void MainWindow::on_action_new_triggered()
{
    TabChecked();

    ylscene *scene = new ylscene;
    QGraphicsView *view = new QGraphicsView(this);
    scene->setSceneRect(0, 0, 800 ,600);
    view->setScene(scene);
    tab->addTab(view, "New Scene");
    connect(scene, SIGNAL(scale_change(double)), this, SLOT(recieve_scale_change(double)));
    chooseItem();

}

void MainWindow::TabChecked(){
    if (tab == nullptr){
        tab = new QTabWidget(this);
        tab->setTabsClosable(true);
        ui->horizontalLayout->addWidget(tab);
        connect(tab,SIGNAL(tabCloseRequested(int)),this, SLOT(CloseTabItem(int)));
        ui->action_rect->setChecked(true);
        setCursor(Qt::CrossCursor);
        tab->setMovable(true);
        tab->setStyleSheet(sheet);
    }
}

void MainWindow::on_action_quit_triggered()
{
    if (!(QMessageBox::information(this,tr("exit tip"),tr("Do you really want exit ?"),tr("Yes"),tr("No")))){
          QApplication* app = nullptr;
           app->exit(0);
     }
}

void MainWindow::CloseTabItem(int p){
    tab->removeTab(p);
}

void MainWindow::on_action_close_triggered()
{
    CloseTabItem(tab->currentIndex());
}

void MainWindow::on_action_save_triggered()
{
    QString caption = "save File", dir = ".", filter = "svg files(*.svg);;ylc files(*.ylc)";
    QString filename = QFileDialog::getSaveFileName(this, caption, dir, filter);
    if (filename == ""){
        QMessageBox warning(QMessageBox::Warning, "Warning!!!", "你没有选中任何文件");
        warning.setStandardButtons(QMessageBox::Close);
        warning.exec();
        return;
    }

   QFileInfo fileinfo = QFileInfo(filename);
   qDebug() << "真希望雨能下不停";
//   qDebug() << fileinfo.suffix() << " " << fileinfo.fileName();
   qDebug() << "让想念继续 让爱变透明";
   if (fileinfo.suffix() == "svg")  WriteXml(filename);
   else if (fileinfo.suffix() == "ylc") WriteYLC(filename);
}

void MainWindow::WriteXml(QString filename){
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
//    qDebug() << filename;
    ylscene *scene = dynamic_cast<ylscene*>(dynamic_cast<QGraphicsView*>(tab->currentWidget())->scene());
    QList <QGraphicsItem *> list = scene->items();

    QDomDocument document;
    QDomProcessingInstruction instruction;

    QDomElement svg = document.createElement("svg");
    instruction = document.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"");

    document.appendChild(instruction);

    QString scale = "100%";
    svg.setAttribute("width", scale);
    svg.setAttribute("height", scale);
    svg.setAttribute("version", "1.1");
    svg.setAttribute("xmlns", "http://www.w3.org/2000/svg");

    QListIterator<QGraphicsItem*> it(list);
    while(it.hasNext())
        it.next();

    //for (auto i:list){
    while(it.hasPrevious()) {
        QGraphicsItem *i = it.previous();
        if (i->type() == YLRectangle::Type){
            YLRectangle *rect = dynamic_cast<YLRectangle*>(i);

            QDomElement xmlItem = document.createElement("rect");
            double width, height, x, y;
            rect->getRect(x, y, width, height);
            QString fill = rect->getHexColor(), stroke = "rgb(25,25,112)";
            QString transform = QString::asprintf("rotate(%lf %lf,%lf)", rect->getRotateAngle(), x, y);
            xmlItem.setAttribute("fill", fill);
            xmlItem.setAttribute("stroke", stroke);
            xmlItem.setAttribute("width", QString::number(width));
            xmlItem.setAttribute("height", QString::number(height));
            xmlItem.setAttribute("x", QString::number(x));
            xmlItem.setAttribute("y", QString::number(y));
			xmlItem.setAttribute("transform", transform);
            svg.appendChild(xmlItem);
        }else if (i->type() == YLTriangle::Type){
            YLTriangle *tri = dynamic_cast<YLTriangle*>(i);

            QDomElement xmlItem = document.createElement("polygon");
            QVector <QPointF> tmp;
            QString point_information = "";
            int point_num = tri->getPoints(tmp);
            qreal x = 10000, y = 10000;
            for (int i=0; i<point_num; i++){
                x = qMin(x, tmp[i].x()); y = qMin(y, tmp[i].y());
            }
            QString fill = tri->getHexColor(), stroke = "rgb(25,25,112)";
            for (int i=0; i<point_num; i++){
                if (!i) point_information += ",";
                point_information += QString::number(tmp[i].x()) + "," + QString::number(tmp[i].y());
            }
            xmlItem.setAttribute("points", QString::number(tmp[0].x()) + "," + QString::number(tmp[0].y()) + ","
                                  + QString::number(tmp[1].x()) + "," +QString::number(tmp[1].y())
                                 + "," + QString::number(tmp[2].x()) + "," + QString::number(tmp[2].y()));
            xmlItem.setAttribute("fill", fill);
            xmlItem.setAttribute("stroke", stroke);
            svg.appendChild(xmlItem);
        }else if (i->type() == YLHexagon::Type){
            YLHexagon *hex = dynamic_cast<YLHexagon*>(i);
            QDomElement xmlItem = document.createElement("polygon");
            QVector <QPointF> tmp;
            QString point_information = "";

            int point_num;
            point_num = hex->getPoints(tmp);
            QString fill = hex->getHexColor(), stroke = "rgb(25,25,112)";
            for (int i=0; i<point_num; i++){
                if (i) point_information += ",";
                point_information += QString::number(tmp[i].x()) + "," + QString::number(tmp[i].y());
            }
            xmlItem.setAttribute("points", point_information);
            xmlItem.setAttribute("fill", fill);
            xmlItem.setAttribute("stroke", stroke);
            svg.appendChild(xmlItem);
        }else if (i->type() == YLEllipse::Type){
            YLEllipse *elps = dynamic_cast<YLEllipse*>(i);
            QDomElement xmlItem = document.createElement("ellipse");
            QVector <QPointF> tmp;
            QString point_information = "";
            double rx, ry;
            elps->getPoints(tmp);
            QString fill = elps->getHexColor(), stroke = "rgb(25,25,112)";
            QString transform = QString::asprintf("rotate(%lf %lf %lf)", elps->getRotateAngle(), tmp[0].x(), tmp[0].y());

            xmlItem.setAttribute("cx", tmp[0].x());
            xmlItem.setAttribute("cy", tmp[0].y());
            elps->getR(rx, ry);
            xmlItem.setAttribute("rx", rx);
            xmlItem.setAttribute("ry", ry);
            xmlItem.setAttribute("fill", fill);
            xmlItem.setAttribute("stroke", stroke);
            xmlItem.setAttribute("transform", transform);
            svg.appendChild(xmlItem);
        }else if (i->type() == YLLine::Type){
            YLLine *line = dynamic_cast<YLLine*>(i);
            QDomElement xmlItem = document.createElement("line");

            QVector <QPointF> tmp;
            line->getPoints(tmp);
            double x1 = tmp[0].x(), y1 = tmp[0].y();
            double x2 = tmp[1].x(), y2 = tmp[1].y();

            QString stroke = "rgb(25,25,112)", stroke_width = "2px";
            QString style = "stroke:" + stroke + ";stroke-width:" + stroke_width;

            xmlItem.setAttribute("x1", x1);
            xmlItem.setAttribute("y1", y1);
            xmlItem.setAttribute("x2", x2);
            xmlItem.setAttribute("y2", y2);
            xmlItem.setAttribute("style", style);
            svg.appendChild(xmlItem);
        }else if (i->type() == YLBezier::Type){
            YLBezier *bezier = dynamic_cast<YLBezier*>(i);
            QDomElement xmlItem = document.createElement("path");

            QVector <QPointF> tmp;
            int num = bezier->getPoints(tmp);

            QString point_info;
            QString stroke = "rgb(25,25,112)", stroke_width = "2px";
            QString style = "stroke:" + stroke + ";stroke-width:" + stroke_width;

            for (int i=0; i<num; i++){
                if (!i) point_info += "M" + QString::number(tmp[i].x()) + "," + QString::number(tmp[i].y()) + " C";
                else point_info += QString::number(tmp[i].x()) + "," + QString::number(tmp[i].y()) + " ";
                xmlItem.setAttribute("x" + QString::number(i+1), tmp[i].x());
                xmlItem.setAttribute("y" + QString::number(i+1), tmp[i].y());
            }

            xmlItem.setAttribute("fill", "none");
            xmlItem.setAttribute("style", style);
            xmlItem.setAttribute("d", point_info);

            svg.appendChild(xmlItem);
        }
    }
    document.appendChild(svg);
    QTextStream qout(&file);
    document.save(qout, 4);

    QFileInfo fileinfo = QFileInfo(filename);
    tab->setTabText(tab->currentIndex(), fileinfo.fileName());
    file.close();
}

void MainWindow::on_action_open_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "open", ".", "svg file(*.svg);;ylc file(*.ylc)");
    if (filename == ""){
        QMessageBox warning(QMessageBox::Warning, "Warning!!!", "你没有选中任何文件");
        warning.setStandardButtons(QMessageBox::Close);
        warning.exec();
        return;
    }

    TabChecked();
    QFileInfo fileinfo = QFileInfo(filename);

    if (fileinfo.suffix() == "svg") ReadXml(filename);
    else if (fileinfo.suffix() == "ylc")
        ReadYLC(filename);
}

void MainWindow::ReadXml(QString filename){
    ylscene *scene = new ylscene();
    connect(scene, SIGNAL(scale_change(double)), this, SLOT(recieve_scale_change(double)));
    scene->setSceneRect(0, 0, 800, 600);
    QFile file(filename);
    QDomDocument document;
    document.setContent(&file);
    QDomElement ele_doc = document.documentElement();
    QDomNode head = ele_doc.firstChild();
    while (!head.isNull()){
        if (head.isElement()){
            QString style, fill, stroke;
            QDomElement tmp = head.toElement();
            QGraphicsItem *item = nullptr;
            if (tmp.tagName() == "rect"){
                double x = tmp.attribute("x").toDouble();
                double y = tmp.attribute("y").toDouble();
                double height = tmp.attribute("height").toDouble();
                double width = tmp.attribute("width").toDouble();

                QString transform = tmp.attribute("transform");
                QString fill_color = tmp.attribute("fill");
                QByteArray buf = transform.toLatin1();
                char *tran_str = buf.data();
                qreal rotate_angle;
                sscanf(tran_str, "rotate(%lf", &rotate_angle);

                item = new YLRectangle();
                dynamic_cast<YLRectangle*>(item)->setPoints(QPointF(x, y), QPointF(x + width, y + height));
                dynamic_cast<YLRectangle*>(item)->calculateCurrentStretchRect();
                dynamic_cast<YLRectangle*>(item)->rotate(rotate_angle);
                dynamic_cast<YLRectangle*>(item)->setColorFromString(fill_color);
            }else if (tmp.tagName() == "polygon"){
                QString point_information;

                point_information = tmp.attribute("points");
                QStringList split = point_information.split(",");
                int num = split.size();
                QString fill_color = tmp.attribute("fill");

                if (num == 6){
                    QVector <QPointF> input;
                    for (int i=0; i<num; i+=2) input << QPointF(split[i].toDouble(), split[i|1].toDouble());
                    item = new YLTriangle();
                    dynamic_cast<YLTriangle*>(item)->setPoints(input);
                    dynamic_cast<YLTriangle*>(item)->calculateCurrentStretchRect();
                    dynamic_cast<YLTriangle*>(item)->setColorFromString(fill_color);
                }else{
                    QVector <QPointF> input;
                    for (int i=0; i<num; i+=2) input << QPointF(split[i].toDouble(), split[i|1].toDouble());
                    item = new YLHexagon();
                    dynamic_cast<YLHexagon*>(item)->setPoints(input);
                    dynamic_cast<YLHexagon*>(item)->calculateCurrentStretchRect();
                    dynamic_cast<YLHexagon*>(item)->setColorFromString(fill_color);
                }
            }else if (tmp.tagName() == "ellipse"){
                QString transform = tmp.attribute("transform");
                QByteArray buf = transform.toLatin1();
                char *tran_str = buf.data();
                qreal rotate_angle;
                sscanf(tran_str, "rotate(%lf", &rotate_angle);

                QString fill_color = tmp.attribute("fill");
                double cx = tmp.attribute("cx").toDouble();
                double cy = tmp.attribute("cy").toDouble();
                double rx = tmp.attribute("rx").toDouble();
                double ry = tmp.attribute("ry").toDouble();

                item = new YLEllipse();
                dynamic_cast<YLEllipse*>(item)->setPoints(QPointF(cx, cy), rx, ry);
                dynamic_cast<YLEllipse*>(item)->calculateCurrentStretchRect();
                dynamic_cast<YLEllipse*>(item)->rotate(rotate_angle);
                dynamic_cast<YLEllipse*>(item)->setColorFromString(fill_color);
            }else if(tmp.tagName() == "line"){
                double x1 = tmp.attribute("x1").toDouble();
                double y1 = tmp.attribute("y1").toDouble();
                double x2 = tmp.attribute("x2").toDouble();
                double y2 = tmp.attribute("y2").toDouble();

                item = new YLLine();
                dynamic_cast<YLLine*>(item)->setPoints(QPointF(x1, y1), QPointF(x2, y2));
                dynamic_cast<YLLine*>(item)->calculateCurrentStretchRect();
            }else if (tmp.tagName() == "path"){
                QVector <QPointF> v;
                for (int i=1; i<=4; i++)
                    v << QPointF(tmp.attribute("x" + QString::number(i)).toDouble(), tmp.attribute("y" + QString::number(i)).toDouble());

                item = new YLBezier();
                dynamic_cast<YLBezier*>(item) -> setPoints(v);
                dynamic_cast<YLBezier*>(item)->calculateCurrentStretchRect();
            }
            if (item != nullptr) scene->addItem(item);
        }
        head = head.nextSibling();
    }

    QGraphicsView *view = new QGraphicsView(this);
    view->setScene(scene);
    QFileInfo fileinfo = QFileInfo(filename);
    tab->addTab(view, fileinfo.fileName());
    file.close();
}

void MainWindow::on_action_clear_triggered()
{
    ylscene *scene = dynamic_cast<ylscene*>(dynamic_cast<QGraphicsView*>(tab->currentWidget())->scene());
    QList <QGraphicsItem *> list = scene->items();

    for (auto i:list){
        scene->removeItem(i);
        delete i;
    }
}

void MainWindow::on_action_delete_triggered()
{
    ylscene *scene = dynamic_cast<ylscene*>(dynamic_cast<QGraphicsView*>(tab->currentWidget())->scene());
    QList <QGraphicsItem *> list = scene->items();
    for (auto i:list){
        if (i->isSelected()){
            scene->removeItem(i);
            delete i;
        }
    }
}

void MainWindow::recieve_scale_change(double _scale){
    scale_status->setText(QString::number(100 * _scale) + "%");
}

void MainWindow::WriteYLC (QString filename){
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    QTextStream YLCwriter(&file);

    ylscene *scene = dynamic_cast<ylscene*>(dynamic_cast<QGraphicsView*>(tab->currentWidget())->scene());
    QList <QGraphicsItem *> list = scene->items();

    QListIterator<QGraphicsItem*> it(list);
    while(it.hasNext())
        it.next();

    QString head = "Create by YLC> hello? I am strving = =.";
    YLCwriter << head << endl;
    //for (auto i:list){
    while(it.hasPrevious()) {
        QGraphicsItem *i = it.previous();
        if (i->type() == YLRectangle::Type){
            YLRectangle *rect = dynamic_cast<YLRectangle*>(i);
            YLCwriter << YLRectangle::Type << endl;

            double x, y, width, height;
            rect->getRect(x, y, width, height);

            YLCwriter << x << "-" << y << "-" << width << "-" <<height << endl;

            double rad, rotate_x, rotate_y;
            rad = rect->getRotateAngle();
            rotate_x = x; rotate_y = y;
            YLCwriter << rad << "-" << rotate_x << "-" << rotate_y << endl;

            QString fill = rect->getHexColor();
            YLCwriter << fill << endl;
        }else if (i->type() == YLEllipse::Type){
            YLEllipse *elps = dynamic_cast<YLEllipse*>(i);
            YLCwriter << YLEllipse::Type << endl;

            QVector <QPointF> v;
            elps->getPoints(v);
            double cx, cy, rx, ry;
            cx = v[0].x(); cy = v[0].y();
            elps->getR(rx, ry);
            YLCwriter << cx << "-" << cy << "-" << rx << "-" << ry << endl;

            double rad, rotate_x, rotate_y;
            rad = elps->getRotateAngle();
            rotate_x = cx; rotate_y = cy;
            YLCwriter << rad << "-" << rotate_x << "-" << rotate_y << endl;

            QString fill = elps->getHexColor();
            YLCwriter << fill << endl;
        }else if (i->type() == YLHexagon::Type){
            YLHexagon *Hexagon = dynamic_cast<YLHexagon*>(i);
            YLCwriter << YLHexagon::Type << endl;

            QVector <QPointF> v;
            int num = Hexagon->getPoints(v);
            for (int i=0; i<num; i++){
                if (i) YLCwriter << "-";
                YLCwriter << v[i].x() << "-" << v[i].y();
            }
            YLCwriter << endl;

            QString fill = Hexagon->getHexColor();
            YLCwriter << fill << endl;
        }else if (i->type() == YLTriangle::Type){
            YLTriangle *tri = dynamic_cast<YLTriangle*>(i);
            YLCwriter << YLTriangle::Type << endl;

            QVector <QPointF> v;
            int num = tri->getPoints(v);
            for (int i=0; i<num; i++){
                if (i) YLCwriter << "-";
                YLCwriter << v[i].x() << "-" << v[i].y();
            }
            YLCwriter << endl;

            QString fill = tri->getHexColor();
            YLCwriter << fill << endl;
        }else if (i->type() == YLLine::Type){
            YLLine *line = dynamic_cast<YLLine*>(i);
            YLCwriter << YLLine::Type << endl;

            QVector <QPointF> v;
            int num = line->getPoints(v);
            for (int i=0; i<num; i++){
                if (i) YLCwriter << "-";
                YLCwriter << v[i].x() << "-" << v[i].y();
            }
            YLCwriter << endl;
        }else if (i->type() == YLBezier::Type){
            YLBezier *bezier = dynamic_cast<YLBezier*>(i);
            YLCwriter << YLBezier::Type << endl;

            QVector <QPointF> v;
            int num = bezier->getPoints(v);
            for (int i=0; i<num; i++){
                if (i) YLCwriter << "-";
                YLCwriter << v[i].x() << "-" << v[i].y();
            }
            YLCwriter << endl;
        }
    }
    QGraphicsView *view = new QGraphicsView(this);
    view->setScene(scene);
    QFileInfo fileinfo = QFileInfo(filename);
    tab->addTab(view, fileinfo.fileName());
    file.close();
}


void MainWindow::ReadYLC (QString filename) {
    ylscene *scene = new ylscene();
    connect(scene, SIGNAL(scale_change(double)), this, SLOT(recieve_scale_change(double)));
    scene->setSceneRect(0, 0, 800, 600);
    QGraphicsItem *item = nullptr;
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QTextStream YLCreader(&file);

    int type;
    QByteArray buf;
    char *buf_str;
    QString pos, rotate, color;
    double x, y, width, height, rad, rotate_x, rotate_y;
    double cx, cy, rx, ry;
    double x1, x2, x3, x4, x5, x6, y1, y2, y3, y4, y5, y6;
    QVector<QPointF> points;
    QString filter;

    while(!YLCreader.atEnd()) {
        filter = YLCreader.readLine();
        bool flag;
        type = filter.toInt(&flag);
        if(!flag)
            continue;
        switch(type) {
        case YLRectangle::Type :
            YLCreader >> pos;
            buf = pos.toLatin1();
            buf_str = buf.data();
            sscanf(buf_str, "%lf-%lf-%lf-%lf", &x, &y, &width, &height);

            YLCreader >> rotate;
            buf = rotate.toLatin1();
            buf_str = buf.data();
            sscanf(buf_str, "%lf-%lf-%lf", &rad, &rotate_x, &rotate_y);

            YLCreader >> color;

            item = new YLRectangle();
            dynamic_cast<YLRectangle*>(item)->setPoints(QPointF(x, y), QPointF(x + width, y + height));
            dynamic_cast<YLRectangle*>(item)->calculateCurrentStretchRect();
            dynamic_cast<YLRectangle*>(item)->rotate(rad);
            dynamic_cast<YLRectangle*>(item)->setColorFromString(color);
            break;
        case YLEllipse::Type :
            YLCreader >> pos;
            buf = pos.toLatin1();
            buf_str = buf.data();
            sscanf(buf_str, "%lf-%lf-%lf-%lf", &cx, &cy, &rx, &ry);

            YLCreader >> rotate;
            buf = rotate.toLatin1();
            buf_str = buf.data();
            sscanf(buf_str, "%lf-%lf-%lf", &rad, &rotate_x, &rotate_y);

            YLCreader >> color;

            item = new YLEllipse();
            dynamic_cast<YLEllipse*>(item)->setPoints(QPointF(cx, cy), rx, ry);
            dynamic_cast<YLEllipse*>(item)->calculateCurrentStretchRect();
            dynamic_cast<YLEllipse*>(item)->rotate(rad);
            dynamic_cast<YLEllipse*>(item)->setColorFromString(color);
            break;
        case YLHexagon::Type :
            YLCreader >> pos;
            buf = pos.toLatin1();
            buf_str = buf.data();
            sscanf(buf_str, "%lf-%lf-%lf-%lf-%lf-%lf-%lf-%lf-%lf-%lf-%lf-%lf",
                             &x1,&y1,&x2,&y2,&x3,&y3,&x4,&y4,&x5,&y5,&x6,&y6);
            points.clear();
            points<<QPointF(x1,y1)<<QPointF(x2,y2)<<QPointF(x3,y3)<<QPointF(x4,y4)<<QPointF(x5,y5)<<QPointF(x6,y6);

            YLCreader >> color;

            item = new YLHexagon();
            dynamic_cast<YLHexagon*>(item)->setPoints(points);
            dynamic_cast<YLHexagon*>(item)->setColorFromString(color);
            break;
        case YLTriangle::Type :
            YLCreader >> pos;
            buf = pos.toLatin1();
            buf_str = buf.data();
            sscanf(buf_str, "%lf-%lf-%lf-%lf-%lf-%lf", &x1, &y1, &x2, &y2, &x3, &y3);
            points.clear();
            points << QPointF(x1, y1) << QPointF(x2, y2) << QPointF(x3, y3);

            YLCreader >> color;

            item = new YLTriangle();
            dynamic_cast<YLTriangle*>(item)->setPoints(points);
            dynamic_cast<YLTriangle*>(item)->setColorFromString(color);
            break;
        case YLLine::Type :
            YLCreader >> pos;
            buf = pos.toLatin1();
            buf_str = buf.data();
            sscanf(buf_str, "%lf-%lf-%lf-%lf", &x1, &y1, &x2, &y2);

            item = new YLLine();
            dynamic_cast<YLLine*>(item)->setPoints(QPointF(x1, y1), QPointF(x2, y2));
            break;
        case YLBezier::Type :
            YLCreader >> pos;
            buf = pos.toLatin1();
            buf_str = buf.data();
            sscanf(buf_str, "%lf-%lf-%lf-%lf-%lf-%lf-%lf-%lf", &x1,&y1,&x2,&y2,&x3,&y3,&x4,&y4);
            points.clear();
            points << QPointF(x1,y1) << QPointF(x2,y2) << QPointF(x3,y3) << QPointF(x4,y4);

            item = new YLBezier();
            dynamic_cast<YLBezier*>(item)->setPoints(points);
            break;
        default:
            break;
        }
        if(item != nullptr) scene->addItem(item);
    }
    QGraphicsView *view = new QGraphicsView(this);
    view->setScene(scene);
    QFileInfo fileinfo = QFileInfo(filename);
    tab->addTab(view, fileinfo.fileName());
    file.close();
}
