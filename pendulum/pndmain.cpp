#include "pndmain.h"

PndMain::PndMain(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);
    readSettings();

    pndopt = 0;
    pndthread = 0;

    computeFlag = false;
}

void PndMain::compute()
{
    if(!pndthread)
    {
        pndthread = new PndThread();
        connect(pndthread, SIGNAL(finished()),
                this, SLOT(computeEnd()));
    }

    statusBar()->showMessage("Please wait");

    act_cmp->setEnabled(computeFlag = false);
    act_opt->setEnabled(computeFlag);

    graph_v_x->setLine(0, 0, 0);
    graph_x_t->setLine(0, 0, 0);

    pndthread->start();

}

void PndMain::getParam(bool a0, float a1, float a2, float a3, float a4, float a5, float a6)
{
    pndthread->method_euler = a0;
    pndthread->eps = a1;
    pndthread->omg = a2;
    pndthread->err = a3;
    pndthread->x0 = a4;
    pndthread->v0 = a5;
    pndthread->t0 = a6;
}

void PndMain::computeEnd()
{
    statusBar()->showMessage("Successful!");

    act_cmp->setEnabled(computeFlag = true);
    act_opt->setEnabled(computeFlag);

    graph_v_x->setLine(pndthread->pnt_x.data(), pndthread->pnt_v.data(), pndthread->pnt_x.size());
    graph_x_t->setLine(pndthread->pnt_t.data(), pndthread->pnt_x.data(), pndthread->pnt_x.size());

}

void PndMain::options()
{
    if(!pndthread)
    {
        pndthread = new PndThread();
        connect(pndthread, SIGNAL(finished()),
                this, SLOT(computeEnd()));
    }

    if(!pndopt)
    {
        pndopt = new PndOpt(this);
        connect(pndopt, SIGNAL(setParam(bool,float,float,float,float,float,float)),
                this, SLOT(getParam(bool,float,float,float,float,float,float)));
    }

    pndopt->radio_euler->setChecked(pndthread->method_euler);
    pndopt->radio_rkutt->setChecked(!pndthread->method_euler);
    pndopt->edt_eps->setText(QString::number(pndthread->eps));
    pndopt->edt_omg->setText(QString::number(pndthread->omg));
    pndopt->edt_err->setText(QString::number(pndthread->err));
    pndopt->edt_x0->setText(QString::number(pndthread->x0));
    pndopt->edt_v0->setText(QString::number(pndthread->v0));
    pndopt->edt_t0->setText(QString::number(pndthread->t0));

    pndopt->show();
    pndopt->activateWindow();
}

void PndMain::about()
{
    QMessageBox::about(this, tr("About Pendulum"),
                       tr("<h2>Pendulum 0.98</h2>"
                          "<p>Copyright &copy; 2011 Victor Korobkovsky</p>"));
}

void PndMain::writeSettings()
{
    QSettings settings("Victor Korobkovsky", "Pendulum");

    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
}

void PndMain::readSettings()
{
    QSettings settings("Victor Korobkovsky", "Pendulum");

    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("state").toByteArray());
}

void PndMain::closeEvent(QCloseEvent *event)
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

void PndMain::setupUi(QMainWindow *parent)
{
    QMenu *menu_file = parent->menuBar()->addMenu(tr("&File"));
    QMenu *menu_edit = parent->menuBar()->addMenu(tr("&Experiment"));
    QMenu *menu_help = parent->menuBar()->addMenu(tr("&Help"));

    act_cmp = menu_edit->addAction(tr("Compute"));
    act_cmp->setShortcut(Qt::Key_Space);
    act_cmp->setStatusTip(tr("Start solution"));
    connect(act_cmp, SIGNAL(triggered()), parent, SLOT(compute()));

    QAction *act_quit = menu_file->addAction(tr("Quit"));
    act_quit->setShortcut(Qt::CTRL + Qt::Key_Q);
    act_quit->setStatusTip(tr("Quit the application"));
    connect(act_quit, SIGNAL(triggered()), parent, SLOT(close()));

    act_opt = menu_edit->addAction(tr("Options"));
    act_opt->setShortcut(Qt::CTRL + Qt::Key_O);
    act_opt->setStatusTip(tr("Set parameters of method"));
    connect(act_opt, SIGNAL(triggered()), parent, SLOT(options()));

    QAction *act_abt = menu_help->addAction(tr("About Pendulum"));
    act_abt->setShortcut(Qt::Key_F1);
    act_abt->setStatusTip(tr("About Pendulum"));
    connect(act_abt, SIGNAL(triggered()), parent, SLOT(about()));

    QAction *act_abt_qt = menu_help->addAction(tr("About &Qt"));
    connect(act_abt_qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    parent->statusBar()->showMessage(tr("Welcome to Pendulum"));

    QWidget *centralWidget = new QWidget();
    parent->setCentralWidget(centralWidget);

    graph_x_t = new Plotter();
    graph_x_t->setNameX("t");
    graph_x_t->setNameY("x");

    graph_v_x = new Plotter();
    graph_v_x->setNameX("x");
    graph_v_x->setNameY("v");

    QHBoxLayout *centralLayout = new QHBoxLayout(centralWidget);
    centralLayout->addWidget(graph_x_t);
    centralLayout->addWidget(graph_v_x);

    parent->setWindowTitle(tr("Pendulum"));
    parent->setMinimumSize(300, 225);
    parent->resize(QSize(800, 600));
}

