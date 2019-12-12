#include "wndopt.h"

WndOpt::WndOpt(QWidget *parent) : QDialog(parent)
{
    setupUi(this);
}

void WndOpt::okClick()
{
    bool s0, s1, s2, s3, s4, s5, s6, s7, s8, s9;
    double q1, q2, q3, q4, q5, q6, q7, q8, q9;

    s0 = rdoButton_RKutt->isChecked();
    q1 = lineEdit->text().toFloat(&s1);
    q2 = lineEdit_2->text().toFloat(&s2);
    q3 = lineEdit_3->text().toFloat(&s3);
    q4 = lineEdit_4->text().toFloat(&s4);
    q5 = lineEdit_5->text().toFloat(&s5);
    q6 = lineEdit_6->text().toFloat(&s6);
    q7 = lineEdit_7->text().toFloat(&s7);
    q8 = lineEdit_8->text().toFloat(&s8);
    q9 = lineEdit_9->text().toFloat(&s9);

    if(s1 && s2 && s3 && s4 && s5 && s6 && s7 && s8 && s9)
    {
        emit pereDay(s0, q1, q2, q3, q4, q5, q6, q7, q8, q9);
        emit accept();
    }
    else
    {
        QMessageBox::information(this, tr("Error"), tr("Check the format of numbers in all fields."));
    }
}

void WndOpt::setupUi(QDialog *parent)
{
    parent->setModal(1);
    parent->setWindowTitle(tr("Options"));

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(okClick()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(close()));

    rdoButton_RKutt = new QRadioButton(tr("Runge-Kutta method"));
    rdoButton_RKutt->setChecked(true);
    rdoButton_Euler = new QRadioButton(tr("Euler method"));
    rdoButton_Euler->setChecked(false);

    QGroupBox *groupBox = new QGroupBox(tr("Parameters"));
    QGroupBox *groupBox_2 = new QGroupBox(tr("Initial conditions"));
    QGroupBox *groupBox_3 = new QGroupBox(tr("Numerical analysis"));

    QLabel *label = new QLabel(QApplication::translate("WndGlav", "Damping coefficient (2\316\263):", 0));
    QLabel *label_2 = new QLabel(tr("Acceleration of gravity (g):"));
    QLabel *label_3 = new QLabel(tr("Length of the pendulum (L):"));
    QLabel *label_4 = new QLabel(QApplication::translate("WndGlav", "Coefficient (\316\264):", 0));
    QLabel *label_5 = new QLabel(tr("Coefficient (p):"));
    QLabel *label_6 = new QLabel(QApplication::translate("WndGlav", "Precision of variable (\316\265<sub>x</sub>):", 0));
    QLabel *label_7 = new QLabel(tr("Initial coordinate (x<sub>0</sub>):"));
    QLabel *label_8 = new QLabel(tr("Initial velocity (v<sub>0</sub>):"));
    QLabel *label_9 = new QLabel(tr("Observation time (t):"));

    lineEdit = new QLineEdit;
    lineEdit->setAlignment(Qt::AlignRight);
    lineEdit->setMaxLength(32);

    lineEdit_2 = new QLineEdit;
    lineEdit_2->setAlignment(Qt::AlignRight);
    lineEdit_2->setMaxLength(32);

    lineEdit_3 = new QLineEdit;
    lineEdit_3->setAlignment(Qt::AlignRight);
    lineEdit_3->setMaxLength(32);

    lineEdit_4 = new QLineEdit;
    lineEdit_4->setAlignment(Qt::AlignRight);
    lineEdit_4->setMaxLength(32);

    lineEdit_5 = new QLineEdit;
    lineEdit_5->setAlignment(Qt::AlignRight);
    lineEdit_5->setMaxLength(32);

    lineEdit_6 = new QLineEdit;
    lineEdit_6->setAlignment(Qt::AlignRight);
    lineEdit_6->setMaxLength(32);

    lineEdit_7 = new QLineEdit;
    lineEdit_7->setAlignment(Qt::AlignRight);
    lineEdit_7->setMaxLength(32);

    lineEdit_8 = new QLineEdit;
    lineEdit_8->setAlignment(Qt::AlignRight);
    lineEdit_8->setMaxLength(32);

    lineEdit_9 = new QLineEdit;
    lineEdit_9->setAlignment(Qt::AlignRight);
    lineEdit_9->setMaxLength(32);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(groupBox);
    layout->addWidget(groupBox_2);
    layout->addWidget(groupBox_3);
    layout->addWidget(buttonBox);
    parent->setLayout(layout);

    QGridLayout *layoutGb_1 = new QGridLayout;
    layoutGb_1->addWidget(label, 0, 0);
    layoutGb_1->addWidget(lineEdit, 0, 1);
    layoutGb_1->addWidget(label_2, 1, 0);
    layoutGb_1->addWidget(lineEdit_2, 1, 1);
    layoutGb_1->addWidget(label_3, 2, 0);
    layoutGb_1->addWidget(lineEdit_3, 2, 1);
    layoutGb_1->addWidget(label_4, 3, 0);
    layoutGb_1->addWidget(lineEdit_4, 3, 1);
    layoutGb_1->addWidget(label_5, 4, 0);
    layoutGb_1->addWidget(lineEdit_5, 4, 1);
    layoutGb_1->addWidget(label_6, 5, 0);
    layoutGb_1->addWidget(lineEdit_6, 5, 1);
    groupBox->setLayout(layoutGb_1);

    QGridLayout *layoutGb_2 = new QGridLayout;
    layoutGb_2->addWidget(label_7, 0, 0);
    layoutGb_2->addWidget(lineEdit_7, 0, 1);
    layoutGb_2->addWidget(label_8, 1, 0);
    layoutGb_2->addWidget(lineEdit_8, 1, 1);
    layoutGb_2->addWidget(label_9, 2, 0);
    layoutGb_2->addWidget(lineEdit_9, 2, 1);
    groupBox_2->setLayout(layoutGb_2);

    QHBoxLayout *layoutGb_3 = new QHBoxLayout;
    layoutGb_3->addWidget(rdoButton_RKutt);
    layoutGb_3->addWidget(rdoButton_Euler);
    groupBox_3->setLayout(layoutGb_3);
}
