#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "yldefine.h"
#include "ylrectangle.h"
#include "yltriangle.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void TabChecked();

private slots:
    void on_action_new_triggered();
    void on_action_quit_triggered();
    void chooseItem();
    void CloseTabItem(int p);
    void on_action_close_triggered();
    void on_action_save_triggered();
    void on_action_open_triggered();
    void on_action_clear_triggered();
    void on_action_delete_triggered();
    void recieve_scale_change(double);
private:
    Ui::MainWindow *ui;
    QActionGroup *Item;
    QTabWidget *tab = nullptr;
    QCursor *myCursor = nullptr;
    QString sheet = "\
        QTabWidget::tab-bar{\
                alignment:left;\
        }";
    QLabel *scale_status = new QLabel;

    void WriteXml(QString);
    void ReadXml(QString);

    void WriteYLC(QString);
    void ReadYLC(QString);
};
#endif // MAINWINDOW_H

