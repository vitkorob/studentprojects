#include "dlgopt.h"

DlgOpt::DlgOpt(QWidget *parent) : QDialog(parent)
{
    setupUi(this);
}

void DlgOpt::okeyClick()
{
    bool s0, s1, s2, s3, s4, s5, s6, s7, s8, s9;
    float q1, q2, q3, q4, q5, q6, q7, q8, q9;

    s0 = radio_euler->isChecked();
    q1 = edt_eps->text().toFloat(&s1);
    q2 = edt_omg->text().toFloat(&s2);
    q3 = edt_err->text().toFloat(&s3);
    q4 = edt_x0->text().toFloat(&s4);
    q5 = edt_v0->text().toFloat(&s5);
    q6 = edt_t0->text().toFloat(&s6);
    q7 = edt_frc->text().toFloat(&s7);
    q8 = edt_per->text().toFloat(&s8);
    q9 = edt_tau->text().toFloat(&s9);

    if(s3 && q3 <= 0)
    {
        QMessageBox::information(this, tr("Invalid value"), tr("Error of coordinate can't be least zero."));
    }
    else if(s8 && s9 && q9 >= q8)
    {
        QMessageBox::information(this, tr("Invalid value"), tr("Coefficient tau can't be larger than T<sub>0</sub>."));
    }
    else if(s1 && s2 && s3 && s4 && s5 && s6 && s7 && s8 && s9)
    {
        emit this->setParam(s0, q1, q2, q3, q4, q5, q6, q7, q8, q9);
        emit this->accept();
    }
    else
    {
        QMessageBox::information(this, tr("Format error"), tr("Check the format of numbers in all fields."));
    }
}

void DlgOpt::setupUi(QDialog *parent)
{
    QGroupBox *grp_method = new QGroupBox(tr("Numerical method"));
    QGroupBox *grp_condit = new QGroupBox(tr("Initial conditions"));
    QGroupBox *grp_param = new QGroupBox(tr("Parameters"));

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(okeyClick()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QLabel *lbl_img = new QLabel();
    lbl_img->setAlignment(Qt::AlignCenter);
    lbl_img->setPixmap(QPixmap(":/images/equation.png"));

    radio_rkutt = new QRadioButton(tr("&Runge-Kutta"));
    radio_rkutt->setChecked(true);
    radio_euler = new QRadioButton(tr("&Euler"));
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

    edt_frc = new QLineEdit();
    edt_frc->setAlignment(Qt::AlignRight);
    edt_frc->setMaxLength(32);

    edt_per = new QLineEdit();
    edt_per->setAlignment(Qt::AlignRight);
    edt_per->setMaxLength(32);

    edt_tau = new QLineEdit();
    edt_tau->setAlignment(Qt::AlignRight);
    edt_tau->setMaxLength(32);

    edt_err = new QLineEdit();
    edt_err->setAlignment(Qt::AlignRight);
    edt_err->setMaxLength(32);

    QLabel *lbl_x0 = new QLabel(tr("Initial coordinate (&x<sub>0</sub>):"));
    lbl_x0->setBuddy(edt_x0);
    QLabel *lbl_v0 = new QLabel(tr("Initial velocity (&v<sub>0</sub>):"));
    lbl_v0->setBuddy(edt_v0);
    QLabel *lbl_t0 = new QLabel(tr("Observation time (&t<sub>end</sub>):"));
    lbl_t0->setBuddy(edt_t0);
    QLabel *lbl_eps = new QLabel(tr("Coefficient epsilon:"));
    QLabel *lbl_omg = new QLabel(tr("Coefficient omega:"));
    QLabel *lbl_frc = new QLabel(tr("Acceleration (&F<sub>0</sub>):"));
    lbl_frc->setBuddy(edt_frc);
    QLabel *lbl_per = new QLabel(tr("Period (T<sub>0</sub>):"));
    QLabel *lbl_tau = new QLabel(tr("Coefficient tau:"));
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
    layout_param->addWidget(lbl_frc, 2, 0);
    layout_param->addWidget(edt_frc, 2, 1);
    layout_param->addWidget(lbl_per, 3, 0);
    layout_param->addWidget(edt_per, 3, 1);
    layout_param->addWidget(lbl_tau, 4, 0);
    layout_param->addWidget(edt_tau, 4, 1);
    layout_param->addWidget(lbl_err, 5, 0);
    layout_param->addWidget(edt_err, 5, 1);
    grp_param->setLayout(layout_param);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(grp_method, 0, 0);
    layout->addWidget(lbl_img, 1, 0, 2, 1);
    layout->addWidget(buttonBox, 3, 0);
    layout->addWidget(grp_param, 0, 1, 2, 1);
    layout->addWidget(grp_condit, 2, 1, 2, 1);
    parent->setLayout(layout);

    parent->setModal(true);
    parent->setWindowTitle(tr("Options"));
    parent->setFixedSize(parent->sizeHint());
}
