#include "atroptions.h"

AtrOptions::AtrOptions(QWidget *parent) : QDialog(parent)
{
    setupUi();
}

void AtrOptions::okeyClick()
{
    bool s0, s1, s2, s3, s4, s5, s6, s7, s8;
    float q1, q2, q3, q4, q5, q6, q7, q8;

    s0 = radio_euler->isChecked();
    q1 = edt_sig->text().toFloat(&s1);
    q2 = edt_r->text().toFloat(&s2);
    q3 = edt_b->text().toFloat(&s3);
    q4 = edt_err->text().toFloat(&s4);
    q5 = edt_x0->text().toFloat(&s5);
    q6 = edt_y0->text().toFloat(&s6);
    q7 = edt_z0->text().toFloat(&s7);
    q8 = edt_t0->text().toFloat(&s8);

    if(q4 <= 0 && s4)
    {
        QMessageBox::information(this, tr("Invalid value"), tr("Error of coordinate can't be least zero."));
    }
    else if(s1 && s2 && s3 && s4 && s5 && s6 && s7 && s8)
    {
        emit this->setParam(s0, q1, q2, q3, q4, q5, q6, q7, q8);
        emit this->accept();
    }
    else
    {
        QMessageBox::information(this, tr("Format error"), tr("Check the format of numbers in all fields."));
    }
}

void AtrOptions::setupUi()
{
    QGroupBox *grp_method = new QGroupBox(tr("Numerical method"));
    QGroupBox *grp_condit = new QGroupBox(tr("Initial conditions"));
    QGroupBox *grp_param = new QGroupBox(tr("Parameters"));
    QGroupBox *grp_image = new QGroupBox(tr("Simultaneous equations"));

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

    edt_y0 = new QLineEdit();
    edt_y0->setAlignment(Qt::AlignRight);
    edt_y0->setMaxLength(32);

    edt_z0 = new QLineEdit();
    edt_z0->setAlignment(Qt::AlignRight);
    edt_z0->setMaxLength(32);

    edt_t0 = new QLineEdit();
    edt_t0->setAlignment(Qt::AlignRight);
    edt_t0->setMaxLength(32);

    edt_sig = new QLineEdit();
    edt_sig->setAlignment(Qt::AlignRight);
    edt_sig->setMaxLength(32);

    edt_r = new QLineEdit();
    edt_r->setAlignment(Qt::AlignRight);
    edt_r->setMaxLength(32);

    edt_b = new QLineEdit();
    edt_b->setAlignment(Qt::AlignRight);
    edt_b->setMaxLength(32);

    edt_err = new QLineEdit();
    edt_err->setAlignment(Qt::AlignRight);
    edt_err->setMaxLength(32);

    QLabel *pixmap = new QLabel(grp_image);
    pixmap->setAlignment(Qt::AlignCenter);
    pixmap->setPixmap(QPixmap(":/images/attractor.png"));

    QLabel *lbl_x0 = new QLabel(tr("Initial &x<sub>0</sub>:"));
    lbl_x0->setBuddy(edt_x0);
    QLabel *lbl_y0 = new QLabel(tr("Initial &y<sub>0</sub>:"));
    lbl_y0->setBuddy(edt_y0);
    QLabel *lbl_z0 = new QLabel(tr("Initial &z<sub>0</sub>:"));
    lbl_z0->setBuddy(edt_z0);
    QLabel *lbl_t0 = new QLabel(tr("Observation time &t<sub>end</sub>:"));
    lbl_t0->setBuddy(edt_t0);
    QLabel *lbl_sig = new QLabel(tr("Coefficient sigma:"));
    QLabel *lbl_r = new QLabel(tr("Coefficient r:"));
    QLabel *lbl_b = new QLabel(tr("Coefficient b:"));
    QLabel *lbl_err = new QLabel(tr("Error of coordinate x:"));

    QHBoxLayout *layout_method = new QHBoxLayout(grp_method);
    layout_method->addWidget(radio_euler);
    layout_method->addWidget(radio_rkutt);

    QGridLayout *layout_condit = new QGridLayout(grp_condit);
    layout_condit->addWidget(lbl_x0, 0, 0);
    layout_condit->addWidget(edt_x0, 0, 1);
    layout_condit->addWidget(lbl_y0, 1, 0);
    layout_condit->addWidget(edt_y0, 1, 1);
    layout_condit->addWidget(lbl_z0, 2, 0);
    layout_condit->addWidget(edt_z0, 2, 1);
    layout_condit->addWidget(lbl_t0, 3, 0);
    layout_condit->addWidget(edt_t0, 3, 1);

    QGridLayout *layout_param = new QGridLayout(grp_param);
    layout_param->addWidget(lbl_sig, 0, 0);
    layout_param->addWidget(edt_sig, 0, 1);
    layout_param->addWidget(lbl_r, 1, 0);
    layout_param->addWidget(edt_r, 1, 1);
    layout_param->addWidget(lbl_b, 2, 0);
    layout_param->addWidget(edt_b, 2, 1);
    layout_param->addWidget(lbl_err, 3, 0);
    layout_param->addWidget(edt_err, 3, 1);

    QHBoxLayout *layout_image = new QHBoxLayout(grp_image);
    layout_image->addWidget(pixmap);

    QVBoxLayout *layout_second = new QVBoxLayout();
    layout_second->addWidget(grp_method);
    layout_second->addWidget(grp_image);
    layout_second->addStretch();
    layout_second->addWidget(buttonBox);

    QGridLayout *layout_first = new QGridLayout(this);
    layout_first->addWidget(grp_param, 0, 0);
    layout_first->addWidget(grp_condit, 1, 0);
    layout_first->addLayout(layout_second, 0, 1, 2, 1);

    setModal(true);
    setWindowTitle(tr("Options"));
    setFixedWidth(sizeHint().width());
}
