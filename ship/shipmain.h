#ifndef SHIPMAIN_H
#define SHIPMAIN_H

#include <QtWidgets>
#include "plotter.h"
#include "shipoptions.h"
#include "shipthread.h"

class ShipMain : public QMainWindow
{
    Q_OBJECT
public:
    ShipMain();

private:
    bool computeFlag;

    QAction *act_cmp;
    QAction *act_opt;

    Plotter *graph_x_t;
    Plotter *graph_y_t;
    Plotter *graph_y_x;

    ShipOptions *atropt;
    ShipThread *atrthread;

    void setupUi();
    void readSettings();
    void writeSettings();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void about();
    void options();
    void compute();

    void getParam(bool s0, float q1, float q2, float q3, float q4, float q5);
    void computeEnd();

};

#endif // SHIPMAIN_H
