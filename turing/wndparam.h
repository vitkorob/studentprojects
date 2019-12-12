#ifndef WNDPARAM_H
#define WNDPARAM_H

#include <QDialog>
#include <QtWidgets>

class WndParam : public QDialog
{
    Q_OBJECT
public:
    QDialogButtonBox *buttonBox;
    QRadioButton *radioButton_RKutt;
    QRadioButton *radioButton_Euler;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QLineEdit *lineEdit_5;
    QLineEdit *lineEdit_6;

    explicit WndParam(QWidget *parent = 0);

private:
    void setupUi(QDialog *parent);

signals:
    void pereDay(bool a0, double a1, double a2, double a3, double a4, double a5, double a6);

private slots:
    void okClick();
};

#endif  // WNDPARAM_H
