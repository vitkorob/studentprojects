#include "atrmain.h"

AtrMain::AtrMain()
{
    setupUi();
    readSettings();

    computeFlag = false;

    atropt = 0;
    atrthread = 0;
}

void AtrMain::about()
{
    QMessageBox::about(this, tr("About Attractor"),
                       tr("<h2>Attractor 0.99</h2>"
                          "<p>Copyright &copy; 2013 Victor Korobkovsky</p>"));
}

void AtrMain::getParam(bool s0, float q1, float q2, float q3, float q4, float q5, float q6, float q7, float q8)
{
    atrthread->method_euler = s0;
    atrthread->sig = q1;
    atrthread->r = q2;
    atrthread->b = q3;
    atrthread->err = q4;
    atrthread->x0 = q5;
    atrthread->y0 = q6;
    atrthread->z0 = q7;
    atrthread->t0 = q8;
}

void AtrMain::computeEnd()
{
    statusBar()->showMessage("Successful!");

    act_cmp->setEnabled(computeFlag = true);
    act_opt->setEnabled(computeFlag);

    graph_x_t->setLine(atrthread->pnt_t.data(), atrthread->pnt_x.data(), atrthread->pnt_t.size());
    graph_y_t->setLine(atrthread->pnt_t.data(), atrthread->pnt_y.data(), atrthread->pnt_t.size());
    graph_z_t->setLine(atrthread->pnt_t.data(), atrthread->pnt_z.data(), atrthread->pnt_t.size());
    graph_x_z->setLine(atrthread->pnt_x.data(), atrthread->pnt_z.data(), atrthread->pnt_t.size());
}

void AtrMain::options()
{
    if(!atrthread)
    {
        atrthread = new AtrThread();
        connect(atrthread, SIGNAL(finished()), this, SLOT(computeEnd()));
    }
    if(!atropt)
    {
        atropt = new AtrOptions(this);
        connect(atropt, SIGNAL(setParam(bool,float,float,float,float,float,float,float,float)),
                this, SLOT(getParam(bool,float,float,float,float,float,float,float,float)));
    }

    atropt->radio_euler->setChecked(atrthread->method_euler);
    atropt->radio_rkutt->setChecked(!atrthread->method_euler);
    atropt->edt_sig->setText(QString::number(atrthread->sig));
    atropt->edt_r->setText(QString::number(atrthread->r));
    atropt->edt_b->setText(QString::number(atrthread->b));
    atropt->edt_err->setText(QString::number(atrthread->err));
    atropt->edt_x0->setText(QString::number(atrthread->x0));
    atropt->edt_y0->setText(QString::number(atrthread->y0));
    atropt->edt_z0->setText(QString::number(atrthread->z0));
    atropt->edt_t0->setText(QString::number(atrthread->t0));

    atropt->show();
    atropt->activateWindow();
}

void AtrMain::compute()
{
    if(!atrthread)
    {
        atrthread = new AtrThread();
        connect(atrthread, SIGNAL(finished()), this, SLOT(computeEnd()));
    }

    statusBar()->showMessage("Please wait");

    act_cmp->setEnabled(computeFlag = false);
    act_opt->setEnabled(computeFlag);

    graph_x_t->setLine(0, 0, 0);
    graph_y_t->setLine(0, 0, 0);
    graph_z_t->setLine(0, 0, 0);
    graph_x_z->setLine(0, 0, 0);

    atrthread->start();
}

void AtrMain::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Close"), tr("Do you want to close the program?"),
                                                              QMessageBox::Yes | QMessageBox::Cancel);

    if(reply == QMessageBox::Cancel) event->ignore();
    else
    {
        writeSettings();
        event->accept();
    }
}

void AtrMain::writeSettings()
{
    QSettings settings("Victor Korobkovsky", "Attractor");

    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
}

void AtrMain::readSettings()
{
    QSettings settings("Victor Korobkovsky", "Attractor");

    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("state").toByteArray());
}

void AtrMain::setupUi()
{
    QMenu *menu_file = menuBar()->addMenu(tr("&File"));
    QMenu *menu_edit = menuBar()->addMenu(tr("&Experiment"));
    QMenu *menu_help = menuBar()->addMenu(tr("&Help"));

    act_cmp = menu_edit->addAction(tr("Compute"));
    act_cmp->setShortcut(Qt::Key_Space);
    act_cmp->setStatusTip(tr("Start solution"));
    connect(act_cmp, SIGNAL(triggered()), this, SLOT(compute()));

    QAction *act_quit = menu_file->addAction(tr("Quit"));
    act_quit->setShortcut(Qt::CTRL + Qt::Key_Q);
    act_quit->setStatusTip(tr("Quit the application"));
    connect(act_quit, SIGNAL(triggered()), this, SLOT(close()));

    act_opt = menu_edit->addAction(tr("Options"));
    act_opt->setShortcut(Qt::CTRL + Qt::Key_O);
    act_opt->setStatusTip(tr("Set parameters of method"));
    connect(act_opt, SIGNAL(triggered()), this, SLOT(options()));

    QAction *act_abt = menu_help->addAction(tr("About Attractor"));
    act_abt->setShortcut(Qt::Key_F1);
    act_abt->setStatusTip(tr("About Attractor"));
    connect(act_abt, SIGNAL(triggered()), this, SLOT(about()));

    QAction *act_abt_qt = menu_help->addAction(tr("About &Qt"));
    connect(act_abt_qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    statusBar()->showMessage(tr("Welcome to Attractor"));

    QWidget *centralWidget = new QWidget();
    setCentralWidget(centralWidget);

    graph_x_t = new Plotter();
    graph_x_t->setNameX("t");
    graph_x_t->setNameY("x");
    graph_x_t->setLineColor(0xff0000);

    graph_y_t = new Plotter();
    graph_y_t->setNameX("t");
    graph_y_t->setNameY("y");
    graph_y_t->setLineColor(0x00aa77);

    graph_z_t = new Plotter();
    graph_z_t->setNameX("t");
    graph_z_t->setNameY("z");

    graph_x_z = new Plotter();
    graph_x_z->setNameX("x");
    graph_x_z->setNameY("z");
    graph_x_z->setLineColor(0x0000cc);

    QGridLayout *centralLayout = new QGridLayout(centralWidget);
    centralLayout->addWidget(graph_x_t, 0, 0);
    centralLayout->addWidget(graph_y_t, 1, 0);
    centralLayout->addWidget(graph_z_t, 0, 1);
    centralLayout->addWidget(graph_x_z, 1, 1);

    setWindowTitle(tr("Attractor"));
    setMinimumSize(300, 225);
    resize(QSize(800, 600));
}
