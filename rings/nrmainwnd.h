#ifndef NRMAINWND_H
#define NRMAINWND_H

#include <QtWidgets>
#include "nroptions.h"
#include "nrthread.h"

class NRMainWnd : public QMainWindow
{
    Q_OBJECT
public:
    explicit NRMainWnd(QWidget *parent = 0);

private:
    QAction *act_cmp;
    QAction *act_opt;

    NROptions *nropt;
    NRThread *nrthr;

    QLabel *label_pix;

    bool computeFlag;

    void setupUi();
    void readSettings();
    void writeSettings();

protected:
    void closeEvent(QCloseEvent *event);

public slots:
    void about();
    void compute();
    void options();

    void computeEnd();
    void getParam(float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8);
};

#endif // NRMAINWND_H
