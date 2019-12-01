#ifndef ATRMAIN_H
#define ATRMAIN_H

#include <QtWidgets>
#include "plotter.h"
#include "atroptions.h"
#include "atrthread.h"

class AtrMain : public QMainWindow
{
    Q_OBJECT
public:
    AtrMain();

private:
    bool computeFlag;

    QAction *act_cmp;
    QAction *act_opt;

    Plotter *graph_x_t;
    Plotter *graph_y_t;
    Plotter *graph_z_t;
    Plotter *graph_x_z;

    AtrOptions *atropt;
    AtrThread *atrthread;

    void setupUi();
    void readSettings();
    void writeSettings();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void about();
    void options();
    void compute();

    void getParam(bool s0, float q1, float q2, float q3, float q4, float q5, float q6, float q7, float q8);
    void computeEnd();

};

#endif // ATRMAIN_H
