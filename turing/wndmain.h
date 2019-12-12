#ifndef WNDMAIN_H
#define WNDMAIN_H

#include <QtWidgets>
#include <stdio.h>

#include "wndparam.h"
#include "pixmapthread.h"
#include "euler.h"
#include "rkutt.h"

class WndMain : public QMainWindow
{
    Q_OBJECT
public:
    QAction *action_prm;
    QAction *action_rtn;

    QTimer *timer;

    bool metod;
    bool compute;
    bool resize_cmpt;
    bool first;
    char timerPhase;

    double aver;

    WndParam *asd;
    Euler *eulerThread;
    RKutt *rkuttThread;
    pixmapThread *imageDraw;

    explicit WndMain(QWidget *parent = 0);

private:
    void setupUi(QMainWindow *parent);
    void readSettings();
    void writeSettings();

protected:
    void paintEvent(QPaintEvent *);
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent *event);

public slots:
    void about();
    void parameters();
    void calculate();

    void timerOut();

    void metodReady();
    void allReady();

    void getParam(bool a0, double a1, double a2, double a3, double a4, double a5, double a6);
};

#endif  // WNDMAIN_H
