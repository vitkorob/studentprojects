#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);

    calcObject = new calc();
    connect(calcObject, SIGNAL(finished()), this, SLOT(computeReady()));

    demo = new Demo();

    calcReady = 0;
    startResize = 0;
}

void MainWindow::computeReady()
{
    calcReady = 1;
    actionStart->setEnabled(calcReady);
    actionPreferences->setEnabled(calcReady);

    masXaxis = new float[calcObject->pnt_t.size()];
    masYaxis = new float[calcObject->pnt_t.size()];

    for(long cou = 0; cou < calcObject->pnt_t.size(); cou++)
    {
        *(masXaxis + cou) = *(calcObject->pnt_r.data() + cou) * cos((*(calcObject->pnt_fi.data() + cou) - 1.570796327));
        *(masYaxis + cou) = *(calcObject->pnt_r.data() + cou) * sin((*(calcObject->pnt_fi.data() + cou) - 1.570796327));
    }

    plot_x_t->setLine(calcObject->pnt_t.data(), calcObject->pnt_r.data(), calcObject->pnt_t.size());
    plot_f_t->setLine(calcObject->pnt_t.data(), calcObject->pnt_fi.data(), calcObject->pnt_t.size());
    plot_f_x->setLine(masXaxis, masYaxis, calcObject->pnt_t.size());
}

void MainWindow::drawReady()
{
    if(startResize)
    {
        startResize = false;
        emit computeReady();
        return;
    }

    actionStart->setEnabled(true);
    actionPreferences->setEnabled(true);
    calcReady = true;

    repaint();
}

void MainWindow::showDemo()
{
    if(demo->initDemo(calcObject->pnt_r.data(), calcObject->pnt_fi.data(), calcObject->pnt_t.data(),
                      calcObject->pnt_t.size(), calcObject->time)) demo->show();
}

void MainWindow::informationMessage()
{
    QMessageBox::information(this, tr("About Elastic 9.2"), "<p>This program offers a numerically solved "
                                                            "(using the methods of Euler and Runge-Kutta method) "
                                                            "the problem of oscillations of a pendulum "
                                                            "with two degrees of freedom.</p>"
                                                            "<p>Copyright &copy; 2012 Victor Korobkovsky</p>");
}

void MainWindow :: setPreferences()
{
    param = new QDialog(this);
    param->setWindowTitle("Preferences");
    param->setModal(true);

    QGridLayout *layout = new QGridLayout(param);

    QGroupBox *box = new QGroupBox("Please, choose method:");
    layout->addWidget(box, 0, 0, 1, 2);

    QHBoxLayout *labox = new QHBoxLayout(box);

    euler = new QRadioButton("Eul&er method");
    euler->setChecked(!calcObject->abc);
    labox->addWidget(euler);

    runge = new QRadioButton("Ru&nge-Kutta method");
    runge->setChecked(calcObject->abc);
    labox->addWidget(runge);

    QLabel *label1 = new QLabel("Mass, &m:");
    lineEdit1 = new QLineEdit(QString::number(calcObject->mass));
    label1->setBuddy(lineEdit1);
    layout->addWidget(label1, 1, 0);
    layout->addWidget(lineEdit1, 1, 1);

    QLabel *label2 = new QLabel("Stiffness, &k:");
    lineEdit2 = new QLineEdit(QString::number(calcObject->koef));
    label2->setBuddy(lineEdit2);
    layout->addWidget(label2, 2, 0);
    layout->addWidget(lineEdit2, 2, 1);

    QLabel *label3 = new QLabel("Length, &l:");
    lineEdit3 = new QLineEdit(QString::number(calcObject->length));
    label3->setBuddy(lineEdit3);
    layout->addWidget(label3, 3, 0);
    layout->addWidget(lineEdit3, 3, 1);

    QLabel *label4 = new QLabel("Initial angle, &u(0):");
    lineEdit4 = new QLineEdit(QString::number(calcObject->ygol));
    label4->setBuddy(lineEdit4);
    layout->addWidget(label4, 4, 0);
    layout->addWidget(lineEdit4, 4, 1);

    QLabel *label5 = new QLabel("Initial coordinate, &r(0):");
    lineEdit5 = new QLineEdit(QString::number(calcObject->coord));
    label5->setBuddy(lineEdit5);
    layout->addWidget(label5, 5, 0);
    layout->addWidget(lineEdit5, 5, 1);

    QLabel *label6 = new QLabel("Initial angular velocity, u'(0):");
    lineEdit6 = new QLineEdit(QString::number(calcObject->yglvel));
    layout->addWidget(label6, 6, 0);
    layout->addWidget(lineEdit6, 6, 1);

    QLabel *label7 = new QLabel("Initial radial velocity, r'(0):");
    lineEdit7 = new QLineEdit(QString::number(calcObject->coordvel));
    layout->addWidget(label7, 7, 0);
    layout->addWidget(lineEdit7, 7, 1);

    QLabel *label8 = new QLabel("Gravitational acceleration, &g:");
    lineEdit8 = new QLineEdit(QString::number(calcObject->grav));
    label8->setBuddy(lineEdit8);
    layout->addWidget(label8, 8, 0);
    layout->addWidget(lineEdit8, 8, 1);

    QLabel *label9 = new QLabel("Precision r, epsilon:");
    lineEdit9 = new QLineEdit(QString::number(calcObject->eps));
    layout->addWidget(label9, 9, 0);
    layout->addWidget(lineEdit9, 9, 1);

    QLabel *label10 = new QLabel("Observation time, &t:");
    lineEdit10 = new QLineEdit(QString::number(calcObject->time));
    label10->setBuddy(lineEdit10);
    layout->addWidget(label10, 10, 0);
    layout->addWidget(lineEdit10, 10, 1);

    QPushButton *ok = new QPushButton("Ok");
    connect(ok, SIGNAL(clicked()), this, SLOT(savePreferences()));
    layout->addWidget(ok, 11, 1);

    param->show();
}

void MainWindow::savePreferences()
{
    calcObject->abc = runge->isChecked();
    calcObject->mass = lineEdit1->text().toFloat();
    calcObject->koef = lineEdit2->text().toFloat();
    calcObject->length = lineEdit3->text().toFloat();
    calcObject->ygol = lineEdit4->text().toFloat();
    calcObject->coord = lineEdit5->text().toFloat();
    calcObject->yglvel = lineEdit6->text().toFloat();
    calcObject->coordvel = lineEdit7->text().toFloat();
    calcObject->grav = lineEdit8->text().toFloat();
    calcObject->eps = lineEdit9->text().toFloat();
    calcObject->time = lineEdit10->text().toFloat();

    param->close();
}

void MainWindow::calcCompute()
{
    calcReady = 0;
    actionStart->setEnabled(calcReady);
    actionPreferences->setEnabled(calcReady);

    calcObject->start();
}

void MainWindow::setupUi(QMainWindow *parent)
{
    parent->setWindowTitle(tr("Elastic 9.2"));
    parent->resize(QSize(800, 600));
    parent->setMinimumSize(256, 192);

    QMenu *menuFile = menuBar()->addMenu("File");

    plot_f_t = new Plotter();
    plot_f_t->setNameX("T");
    plot_f_t->setNameY("FI");
    plot_f_x = new Plotter();
    plot_x_t = new Plotter();
    plot_x_t->setNameX("T");
    plot_x_t->setNameY("R");

    QWidget *centerWidget = new QWidget();
    QGridLayout *centerLayout = new QGridLayout();
    centerLayout->addWidget(plot_x_t, 0, 0);
    centerLayout->addWidget(plot_f_t, 1, 0);
    centerLayout->addWidget(plot_f_x, 0, 1, 2, 1);
    centerWidget->setLayout(centerLayout);
    parent->setCentralWidget(centerWidget);

    actionStart = menuFile->addAction("Calculate");
    actionStart->setShortcut(Qt::Key_Space);
    connect(actionStart, SIGNAL(triggered()), this, SLOT(calcCompute()));

    actionPreferences = menuFile->addAction("Preferences");
    actionPreferences->setShortcut(Qt::CTRL + Qt::Key_P);
    connect(actionPreferences, SIGNAL(triggered()), this, SLOT(setPreferences()));

    QAction *actionDemo = menuFile->addAction("Show demo");
    actionDemo->setShortcut(Qt::CTRL + Qt::Key_D);
    connect(actionDemo, SIGNAL(triggered()), this, SLOT(showDemo()));

    menuFile->addSeparator();

    QAction *actionExit = menuFile->addAction("Exit");
    actionExit->setShortcut(Qt::Key_Escape);
    connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));

    QMenu *menu_help = menuBar()->addMenu(tr("&Help"));

    QAction *actionInfo = menu_help->addAction("About Elastic");
    connect(actionInfo, SIGNAL(triggered()), this, SLOT(informationMessage()));

    QAction *act_abt_qt = menu_help->addAction(tr("About &Qt"));
    connect(act_abt_qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}
