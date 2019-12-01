#ifndef ATROPTIONS_H
#define ATROPTIONS_H

#include <QtWidgets>

class AtrOptions : public QDialog
{
    Q_OBJECT
public:
    explicit AtrOptions(QWidget *parent = 0);

    QRadioButton *radio_rkutt;
    QRadioButton *radio_euler;

    QLineEdit *edt_x0;
    QLineEdit *edt_y0;
    QLineEdit *edt_z0;
    QLineEdit *edt_t0;
    QLineEdit *edt_sig;
    QLineEdit *edt_r;
    QLineEdit *edt_b;
    QLineEdit *edt_err;

private:
    void setupUi(void);

signals:
    void setParam(bool s0, float q1, float q2, float q3, float q4, float q5, float q6, float q7, float q8);

private slots:
    void okeyClick();

};

#endif // ATROPTIONS_H
