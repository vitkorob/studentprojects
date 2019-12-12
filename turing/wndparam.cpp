#include "wndparam.h"

WndParam::WndParam(QWidget *parent) : QDialog(parent)
{
    setupUi(this);
}

void WndParam::okClick()
{
    bool s0, s1, s2, s3, s4, s5, s6;
    double q1, q2, q3, q4, q5, q6;

    s0 = radioButton_RKutt->isChecked();
    q1 = lineEdit->text().toFloat(&s1);
    q2 = lineEdit_2->text().toFloat(&s2);
    q3 = lineEdit_3->text().toFloat(&s3);
    q4 = lineEdit_4->text().toFloat(&s4);
    q5 = lineEdit_5->text().toFloat(&s5);
    q6 = lineEdit_6->text().toFloat(&s6);

    if(s1 && s2 && s3 && s4 && s5 && s6)
    {
        emit pereDay(s0, q1, q2, q3, q4, q5, q6);
        emit accept();
    }
    else
    {
        QMessageBox::information(this, tr("Error"), tr("In one or more text fields is not true given the number!\n"
                                                       "Check the format of numbers in all fields."));
    }
}

void WndParam::setupUi(QDialog *parent)
{
    parent->setModal(1);
    parent->setWindowTitle(tr("Parameters"));

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(okClick()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(close()));

    QGroupBox *groupBox = new QGroupBox(tr("Numerical analysis"));
    QGroupBox *groupBox_2 = new QGroupBox(tr("Parameters"));
    QGroupBox *groupBox_3 = new QGroupBox(tr("Initial conditions"));

    radioButton_RKutt = new QRadioButton(tr("Runge-Kutta method"));
    radioButton_RKutt->setChecked(true);
    radioButton_Euler = new QRadioButton(tr("Euler method"));
    radioButton_Euler->setChecked(false);

    QLabel *label = new QLabel(tr("First parameter (A):"));
    QLabel *label_2 = new QLabel(tr("Second parameter (B):"));
    QLabel *label_6 = new QLabel(tr("Precision of variable (x):"));
    QLabel *label_3 = new QLabel(tr("Concentration of the first substance (x<sub>0</sub>):"));
    QLabel *label_4 = new QLabel(tr("Concentration of the second substance (y<sub>0</sub>):"));
    QLabel *label_5 = new QLabel(tr("Observation time (t):"));

    lineEdit = new QLineEdit;
    lineEdit->setAlignment(Qt::AlignRight);
    lineEdit->setMaxLength(32);

    lineEdit_2 = new QLineEdit;
    lineEdit_2->setAlignment(Qt::AlignRight);
    lineEdit_2->setMaxLength(32);

    lineEdit_6 = new QLineEdit;
    lineEdit_6->setAlignment(Qt::AlignRight);
    lineEdit_6->setMaxLength(32);

    lineEdit_3 = new QLineEdit;
    lineEdit_3->setAlignment(Qt::AlignRight);
    lineEdit_3->setMaxLength(32);

    lineEdit_4 = new QLineEdit;
    lineEdit_4->setAlignment(Qt::AlignRight);
    lineEdit_4->setMaxLength(32);

    lineEdit_5 = new QLineEdit;
    lineEdit_5->setAlignment(Qt::AlignRight);
    lineEdit_5->setMaxLength(32);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(groupBox, 0, 0);
    layout->addWidget(groupBox_2, 1, 0);
    layout->addWidget(groupBox_3, 2, 0);
    layout->addWidget(buttonBox, 3, 0);
    parent->setLayout(layout);

    QGridLayout *layoutGb_1 = new QGridLayout;
    layoutGb_1->addWidget(radioButton_RKutt, 0, 0);
    layoutGb_1->addWidget(radioButton_Euler, 0, 1);
    groupBox->setLayout(layoutGb_1);

    QGridLayout *layoutGb_2 = new QGridLayout;
    layoutGb_2->addWidget(label, 0, 0);
    layoutGb_2->addWidget(lineEdit, 0, 1);
    layoutGb_2->addWidget(label_2, 1, 0);
    layoutGb_2->addWidget(lineEdit_2, 1, 1);
    layoutGb_2->addWidget(label_6, 2, 0);
    layoutGb_2->addWidget(lineEdit_6, 2, 1);
    groupBox_2->setLayout(layoutGb_2);

    QGridLayout *layoutGb_3 = new QGridLayout;
    layoutGb_3->addWidget(label_3, 0, 0);
    layoutGb_3->addWidget(lineEdit_3, 0, 1);
    layoutGb_3->addWidget(label_4, 1, 0);
    layoutGb_3->addWidget(lineEdit_4, 1, 1);
    layoutGb_3->addWidget(label_5, 2, 0);
    layoutGb_3->addWidget(lineEdit_5, 2, 1);
    groupBox_3->setLayout(layoutGb_3);
}
