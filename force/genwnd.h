#ifndef GENWND_H
#define GENWND_H

#include <QtWidgets>
#include "dlgopt.h"
#include "plotter.h"
#include "solutionthread.h"

class GenWnd : public QMainWindow
{
    Q_OBJECT

public:
    explicit GenWnd(QWidget *parent = 0);

private:
    QToolBar *toolbar;

    QAction *act_opt;
    QAction *act_cmp;
    QAction *act_graph1;
    QAction *act_graph2;
    QAction *act_graph3;
    QAction *act_statusbar;

    Plotter *plotter_x_t;
    Plotter *plotter_v_x;
    Plotter *plotter_f_t;

    DlgOpt *dlg_options;
    SolutionThread *sln_thread;

    void setupUi(QMainWindow *parent);
    void writeSettings();
    void readSettings();

    bool computeEndFlag;

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void about();
    void clean();
    void compute();
    void options();
    void getParam(bool a0, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, float a9);
    void computeEnd();
};

#endif // GENWND_H
