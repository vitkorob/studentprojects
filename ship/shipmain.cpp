#include "shipmain.h"

ShipMain::ShipMain()
{
    setupUi();
    readSettings();

    computeFlag = false;

    atropt = 0;
    atrthread = 0;
}

void ShipMain::about()
{
    QMessageBox::about(this, tr("About Ship"),
                       tr("<h2>Ship 0.81</h2>"
                          "<p>Copyright &copy; 2011 Victor Korobkovsky</p>"));
}

void ShipMain::getParam(bool s0, float q1, float q2, float q3, float q4, float q5)
{
    atrthread->method_euler = s0;
    atrthread->b = q1;
    atrthread->err = q2;
    atrthread->x0 = q3;
    atrthread->y0 = q4;
    atrthread->t0 = q5;
}

void ShipMain::computeEnd()
{
    statusBar()->showMessage("Successful!");

    act_cmp->setEnabled(computeFlag = true);
    act_opt->setEnabled(computeFlag);

    graph_x_t->setLine(atrthread->pnt_t.data(), atrthread->pnt_x.data(), atrthread->pnt_t.size());
    graph_y_t->setLine(atrthread->pnt_t.data(), atrthread->pnt_y.data(), atrthread->pnt_t.size());
    graph_y_x->setLine(atrthread->pnt_x.data(), atrthread->pnt_y.data(), atrthread->pnt_x.size());
}

void ShipMain::options()
{
    if(!atrthread)
    {
        atrthread = new ShipThread();
        connect(atrthread, SIGNAL(finished()), this, SLOT(computeEnd()));
    }
    if(!atropt)
    {
        atropt = new ShipOptions(this);
        connect(atropt, SIGNAL(setParam(bool,float,float,float,float,float)),
                this, SLOT(getParam(bool,float,float,float,float,float)));
    }

    atropt->radio_euler->setChecked(atrthread->method_euler);
    atropt->radio_rkutt->setChecked(!atrthread->method_euler);
    atropt->edt_b->setText(QString::number(atrthread->b));
    atropt->edt_err->setText(QString::number(atrthread->err));
    atropt->edt_x0->setText(QString::number(atrthread->x0));
    atropt->edt_y0->setText(QString::number(atrthread->y0));
    atropt->edt_t0->setText(QString::number(atrthread->t0));

    atropt->show();
    atropt->activateWindow();
}

void ShipMain::compute()
{
    if(!atrthread)
    {
        atrthread = new ShipThread();
        connect(atrthread, SIGNAL(finished()), this, SLOT(computeEnd()));
    }

    statusBar()->showMessage("Please wait");

    act_cmp->setEnabled(computeFlag = false);
    act_opt->setEnabled(computeFlag);

    graph_x_t->setLine(0, 0, 0);
    graph_y_t->setLine(0, 0, 0);
    graph_y_x->setLine(0, 0, 0);

    atrthread->start();
}

void ShipMain::closeEvent(QCloseEvent *event)
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

void ShipMain::writeSettings()
{
    QSettings settings("Victor Korobkovsky", "Ship");

    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
}

void ShipMain::readSettings()
{
    QSettings settings("Victor Korobkovsky", "Ship");

    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("state").toByteArray());
}

void ShipMain::setupUi()
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

    QAction *act_abt = menu_help->addAction(tr("About Ship"));
    act_abt->setShortcut(Qt::Key_F1);
    act_abt->setStatusTip(tr("About Ship"));
    connect(act_abt, SIGNAL(triggered()), this, SLOT(about()));

    QAction *act_abt_qt = menu_help->addAction(tr("About &Qt"));
    connect(act_abt_qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    statusBar()->showMessage(tr("Welcome to Ship"));

    QWidget *centralWidget = new QWidget();
    setCentralWidget(centralWidget);

    graph_x_t = new Plotter();
    graph_x_t->setNameX("t");
    graph_x_t->setNameY("x");
    graph_x_t->setLineColor(0xff0000);

    graph_y_t = new Plotter();
    graph_y_t->setNameX("t");
    graph_y_t->setNameY("y");
    graph_y_t->setLineColor(0x110033);

    graph_y_x = new Plotter();
    graph_y_x->setNameX("x");
    graph_y_x->setNameY("y");

    QGridLayout *centralLayout = new QGridLayout(centralWidget);
    centralLayout->addWidget(graph_x_t, 0, 0);
    centralLayout->addWidget(graph_y_t, 1, 0);
    centralLayout->addWidget(graph_y_x, 0, 1, 2, 1);

    setWindowTitle(tr("Ship"));
    setMinimumSize(300, 225);
    resize(QSize(800, 600));
}
