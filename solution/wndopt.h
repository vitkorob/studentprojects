#ifndef WNDOPT_H
#define WNDOPT_H

#include <QtWidgets>

class WndOpt : public QDialog
{
    Q_OBJECT
public:
    explicit WndOpt(QWidget *parent = 0);

    QRadioButton *rdoButton_RKutt;
    QRadioButton *rdoButton_Euler;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QLineEdit *lineEdit_5;
    QLineEdit *lineEdit_6;
    QLineEdit *lineEdit_7;
    QLineEdit *lineEdit_8;
    QLineEdit *lineEdit_9;

private:
    void setupUi(QDialog *parent);

signals:
    void pereDay(bool a0, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, float a9);

public slots:
    void okClick();

};

#endif  // WNDOPT_H
