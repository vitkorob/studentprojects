#include "pndopt.h"

PndOpt::PndOpt(QWidget *parent) : QDialog(parent)
{
    setupUi(this);
}

void PndOpt::okeyClick()
{
    bool s0, s1, s2, s3, s4, s5, s6;
    float q1, q2, q3, q4, q5, q6;

    s0 = radio_euler->isChecked();
    q1 = edt_eps->text().toFloat(&s1);
    q2 = edt_omg->text().toFloat(&s2);
    q3 = edt_err->text().toFloat(&s3);
    q4 = edt_x0->text().toFloat(&s4);
    q5 = edt_v0->text().toFloat(&s5);
    q6 = edt_t0->text().toFloat(&s6);

    if(q3 <= 0 && s3)
    {
        QMessageBox::information(this, tr("Invalid value"), tr("Error of coordinate can't be least zero."));
    }
    else if(s1 && s2 && s3 && s4 && s5 && s6)
    {
        emit this->setParam(s0, q1, q2, q3, q4, q5, q6);
        emit this->accept();
    }
    else
    {
        QMessageBox::information(this, tr("Format error"), tr("Check the format of numbers in all fields."));
    }
}

void PndOpt::setupUi(QDialog *parent)
{
    QGroupBox *grp_method = new QGroupBox(tr("Numerical method"));
    QGroupBox *grp_condit = new QGroupBox(tr("Initial conditions"));
    QGroupBox *grp_param = new QGroupBox(tr("Parameters"));

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(okeyClick()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    radio_rkutt = new QRadioButton(tr("&Runge-Kutta method"));
    radio_rkutt->setChecked(true);
    radio_euler = new QRadioButton(tr("&Euler method"));
    radio_euler->setChecked(false);

    edt_x0 = new QLineEdit();
    edt_x0->setAlignment(Qt::AlignRight);
    edt_x0->setMaxLength(32);

    edt_v0 = new QLineEdit();
    edt_v0->setAlignment(Qt::AlignRight);
    edt_v0->setMaxLength(32);

    edt_t0 = new QLineEdit();
    edt_t0->setAlignment(Qt::AlignRight);
    edt_t0->setMaxLength(32);

    edt_eps = new QLineEdit();
    edt_eps->setAlignment(Qt::AlignRight);
    edt_eps->setMaxLength(32);

    edt_omg = new QLineEdit();
    edt_omg->setAlignment(Qt::AlignRight);
    edt_omg->setMaxLength(32);

    edt_err = new QLineEdit();
    edt_err->setAlignment(Qt::AlignRight);
    edt_err->setMaxLength(32);

    QLabel *lbl_x0 = new QLabel(tr("Initial coordinate (&x<sub>0</sub>):"));
    lbl_x0->setBuddy(edt_x0);
    QLabel *lbl_v0 = new QLabel(tr("Initial velocity (&v<sub>0</sub>):"));
    lbl_v0->setBuddy(edt_v0);
    QLabel *lbl_t0 = new QLabel(tr("Observation time (&t<sub>end</sub>):"));
    lbl_t0->setBuddy(edt_t0);
    QLabel *lbl_eps = new QLabel(tr("Damping coefficient epsilon:"));
    QLabel *lbl_omg = new QLabel(tr("Coefficient omega:"));
    QLabel *lbl_err = new QLabel(tr("Error of coordinate x:"));

    QHBoxLayout *layout_method = new QHBoxLayout();
    layout_method->addWidget(radio_euler);
    layout_method->addWidget(radio_rkutt);
    grp_method->setLayout(layout_method);

    QGridLayout *layout_condit = new QGridLayout();
    layout_condit->addWidget(lbl_x0, 0, 0);
    layout_condit->addWidget(edt_x0, 0, 1);
    layout_condit->addWidget(lbl_v0, 1, 0);
    layout_condit->addWidget(edt_v0, 1, 1);
    layout_condit->addWidget(lbl_t0, 2, 0);
    layout_condit->addWidget(edt_t0, 2, 1);
    grp_condit->setLayout(layout_condit);

    QGridLayout *layout_param = new QGridLayout();
    layout_param->addWidget(lbl_eps, 0, 0);
    layout_param->addWidget(edt_eps, 0, 1);
    layout_param->addWidget(lbl_omg, 1, 0);
    layout_param->addWidget(edt_omg, 1, 1);
    layout_param->addWidget(lbl_err, 2, 0);
    layout_param->addWidget(edt_err, 2, 1);
    grp_param->setLayout(layout_param);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(grp_param, 0, 0);
    layout->addWidget(grp_condit, 0, 1);
    layout->addWidget(grp_method, 1, 0);
    layout->addWidget(buttonBox, 1, 1);
    parent->setLayout(layout);

    parent->setModal(true);
    parent->setWindowTitle(tr("Options"));
    parent->setFixedWidth(parent->sizeHint().width());
}
