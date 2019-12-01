#ifndef PNDOPT_H
#define PNDOPT_H

#include <QtWidgets>

class PndOpt : public QDialog
{
    Q_OBJECT
public:
    explicit PndOpt(QWidget *parent = 0);

    QRadioButton *radio_euler;
    QRadioButton *radio_rkutt;
    QLineEdit *edt_x0;
    QLineEdit *edt_v0;
    QLineEdit *edt_t0;
    QLineEdit *edt_eps;
    QLineEdit *edt_omg;
    QLineEdit *edt_err;

private:
    void setupUi(QDialog *parent);

signals:
    void setParam(bool a0, float a1, float a2, float a3, float a4, float a5, float a6);

private slots:
    void okeyClick();

};

#endif // PNDOPT_H
