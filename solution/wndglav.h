#ifndef WNDGLAV_H
#define WNDGLAV_H

#include <QtWidgets>
#include "wndopt.h"
#include "solutionthread.h"
#include "pixmapthread.h"

class WndGlav : public QMainWindow
{
    Q_OBJECT
public:
    explicit WndGlav(QWidget *parent = 0);

private:
    void setupUi(QMainWindow *parent);
    void readSettings();
    void writeSettings();

    WndOpt *qwe;
    solutionThread *comp;
    pixmapThread *risu;

    QAction *act_clt;
    QAction *act_opt;

    bool cmpReady;

protected:
    void closeEvent(QCloseEvent *event);
    void paintEvent(QPaintEvent *);

//signals:

public slots:
    void about();
    void options();
    void compute();

    void metodReady();
    void allReady();

    void getParam(bool a0, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, float a9);

};

#endif  // WNDGLAV_H
