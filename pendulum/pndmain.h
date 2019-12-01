#ifndef PNDMAIN_H
#define PNDMAIN_H

#include <QtWidgets>
#include "plotter.h"
#include "pndopt.h"
#include "pndthread.h"

class PndMain : public QMainWindow
{
    Q_OBJECT
public:
    explicit PndMain(QWidget *parent = 0);

private:
    QAction *act_cmp;
    QAction *act_opt;

    Plotter *graph_x_t;
    Plotter *graph_v_x;

    PndOpt *pndopt;
    PndThread *pndthread;

    bool computeFlag;

    void setupUi(QMainWindow *parent);
    void writeSettings();
    void readSettings();

protected:
    void closeEvent(QCloseEvent *);

signals:

private slots:
    void compute();
    void options();
    void about();
    void getParam(bool a0, float a1, float a2, float a3, float a4, float a5, float a6);
    void computeEnd();
};

#endif // PNDMAIN_H
