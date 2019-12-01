#ifndef SHIPOPTIONS_H
#define SHIPOPTIONS_H

#include <QtWidgets>

class ShipOptions : public QDialog
{
    Q_OBJECT
public:
    explicit ShipOptions(QWidget *parent = 0);

    QRadioButton *radio_rkutt;
    QRadioButton *radio_euler;

    QLineEdit *edt_x0;
    QLineEdit *edt_y0;
    QLineEdit *edt_t0;
    QLineEdit *edt_b;
    QLineEdit *edt_err;

private:
    void setupUi();

signals:
    void setParam(bool s0, float q1, float q2, float q3, float q4, float q5);

private slots:
    void okeyClick();

};

#endif // SHIPOPTIONS_H
