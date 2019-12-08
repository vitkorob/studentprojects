#ifndef NROPTIONS_H
#define NROPTIONS_H

#include <QtWidgets>

class NROptions : public QDialog
{
    Q_OBJECT
public:
    explicit NROptions(QWidget *parent = 0);

    QLineEdit *edt_n0;
    QLineEdit *edt_n1;
    QLineEdit *edt_angle;
    QLineEdit *edt_radius;
    QLineEdit *edt_d;
    QLineEdit *edt_lamda;
    QLineEdit *edt_amount;
    QLineEdit *edt_pix;

private:
    void setupUi();

signals:
    void setParam(float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8);

public slots:
    void okeyClick();

};

#endif // NROPTIONS_H
