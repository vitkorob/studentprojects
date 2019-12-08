#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include "calc.h"
#include "plotter.h"
#include "demo.h"

class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);

    calc *calcObject;
    Plotter *plot_x_t;
    Plotter *plot_f_t;
    Plotter *plot_f_x;

    bool calcReady;
    bool startResize;

private:
    QDialog *param;

    QLineEdit *lineEdit1;
    QLineEdit *lineEdit2;
    QLineEdit *lineEdit3;
    QLineEdit *lineEdit4;
    QLineEdit *lineEdit5;
    QLineEdit *lineEdit6;
    QLineEdit *lineEdit7;
    QLineEdit *lineEdit8;
    QLineEdit *lineEdit9;
    QLineEdit *lineEdit10;

    QRadioButton *euler;
    QRadioButton *runge;

    QAction *actionStart;
    QAction *actionPreferences;

    Demo *demo;

    float *masXaxis;
    float *masYaxis;

    void setupUi(QMainWindow *parent);

private slots:
    void informationMessage();
    void showDemo();
    void setPreferences();
    void savePreferences();
    void calcCompute();

    void computeReady();
    void drawReady();
};

#endif // MAINWINDOW_H
