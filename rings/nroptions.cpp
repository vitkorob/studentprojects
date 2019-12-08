#include "nroptions.h"

NROptions::NROptions(QWidget *parent) : QDialog(parent)
{
    setupUi();
}

void NROptions::setupUi()
{
    QGroupBox *grp_param = new QGroupBox(tr("Parameters"));

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(okeyClick()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    edt_n0 = new QLineEdit();
    edt_n0->setAlignment(Qt::AlignRight);
    edt_n0->setMaxLength(32);

    edt_n1 = new QLineEdit();
    edt_n1->setAlignment(Qt::AlignRight);
    edt_n1->setMaxLength(32);

    edt_angle = new QLineEdit();
    edt_angle->setAlignment(Qt::AlignRight);
    edt_angle->setMaxLength(32);

    edt_radius = new QLineEdit();
    edt_radius->setAlignment(Qt::AlignRight);
    edt_radius->setMaxLength(32);

    edt_d = new QLineEdit();
    edt_d->setAlignment(Qt::AlignRight);
    edt_d->setMaxLength(32);

    edt_lamda = new QLineEdit();
    edt_lamda->setAlignment(Qt::AlignRight);
    edt_lamda->setMaxLength(32);

    edt_amount = new QLineEdit();
    edt_amount->setAlignment(Qt::AlignRight);
    edt_amount->setMaxLength(32);

    edt_pix = new QLineEdit();
    edt_pix->setAlignment(Qt::AlignRight);
    edt_pix->setMaxLength(32);

    QLabel *lbl_n0 = new QLabel(tr("Coefficient (n<sub>0</sub>):"));
    lbl_n0->setBuddy(edt_n0);

    QLabel *lbl_n1 = new QLabel(tr("Coefficient (n<sub>1</sub>):"));
    lbl_n1->setBuddy(edt_n1);

    QLabel *lbl_angle = new QLabel(tr("Angle:"));
    lbl_angle->setBuddy(edt_angle);

    QLabel *lbl_radius = new QLabel(tr("Radius:"));
    lbl_radius->setBuddy(edt_radius);

    QLabel *lbl_d = new QLabel(tr("Width:"));
    lbl_d->setBuddy(edt_d);

    QLabel *lbl_lamda = new QLabel(tr("Lamda:"));
    lbl_lamda->setBuddy(edt_lamda);

    QLabel *lbl_amount = new QLabel(tr("Amount of rays:"));
    lbl_amount->setBuddy(edt_amount);

    QLabel *lbl_pix = new QLabel(tr("Image width:"));
    lbl_pix->setBuddy(edt_pix);

    QGridLayout *layout_param = new QGridLayout();
    layout_param->addWidget(lbl_angle, 0, 0);
    layout_param->addWidget(edt_angle, 0, 1);
    layout_param->addWidget(lbl_n0, 1, 0);
    layout_param->addWidget(edt_n0, 1, 1);
    layout_param->addWidget(lbl_n1, 2, 0);
    layout_param->addWidget(edt_n1, 2, 1);
    layout_param->addWidget(lbl_radius, 3, 0);
    layout_param->addWidget(edt_radius, 3, 1);
    layout_param->addWidget(lbl_d, 4, 0);
    layout_param->addWidget(edt_d, 4, 1);
    layout_param->addWidget(lbl_lamda, 5, 0);
    layout_param->addWidget(edt_lamda, 5, 1);
    layout_param->addWidget(lbl_amount, 6, 0);
    layout_param->addWidget(edt_amount, 6, 1);
    layout_param->addWidget(lbl_pix, 7, 0);
    layout_param->addWidget(edt_pix, 7, 1);
    grp_param->setLayout(layout_param);

    QVBoxLayout *layout_options = new QVBoxLayout();
    layout_options->addWidget(grp_param);
    layout_options->addWidget(buttonBox);
    setLayout(layout_options);

    setModal(true);
    setWindowTitle(tr("Options"));
    setFixedWidth(sizeHint().width());
}

void NROptions::okeyClick()
{
    bool s1, s2, s3, s4, s5, s6, s7, s8;
    float q1, q2, q3, q4, q5, q6, q7, q8;

    q1 = edt_angle->text().toFloat(&s1);
    q2 = edt_n0->text().toFloat(&s2);
    q3 = edt_n1->text().toFloat(&s3);
    q4 = edt_radius->text().toFloat(&s4);
    q5 = edt_d->text().toFloat(&s5);
    q6 = edt_lamda->text().toFloat(&s6);
    q7 = edt_amount->text().toFloat(&s7);
    q8 = edt_pix->text().toFloat(&s8);

    if(s1 && s2 && s3 && s4 && s5 && s6 && s7 && s8)
    {
        emit this->setParam(q1, q2, q3, q4, q5, q6, q7, q8);
        emit this->accept();
    }
    else
    {
        QMessageBox::information(this, tr("Format error"), tr("Check the format of numbers in all fields."));
    }
}
