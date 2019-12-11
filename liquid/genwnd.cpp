#include "genwnd.h"

GenWnd::GenWnd(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);
    readSettings();

    dlg_options = 0;

    sln_thread = new SolutionThread();
    connect(sln_thread, SIGNAL(finished()), this, SLOT(computeEnd()));

    computeEndFlag = false;
}

void GenWnd::about()
{
    QMessageBox::about(this, tr("About Liquid"),
                       tr("<h2>Liquid 1.1</h2>"
                          "<p>Copyright &copy; 2011 Victor Korobkovsky</p>"
                          "<p>Liquid is a small application that demonstrates "
                          "numeric methods solution of equation.</p>"));
}

void GenWnd::clean()
{
    plotter_v_x->setLine(0, 0, 0);
    plotter_x_t->setLine(0, 0, 0);
}

void GenWnd::compute()
{
    statusBar()->showMessage("Please wait");

    act_cmp->setEnabled(computeEndFlag = false);
    act_opt->setEnabled(computeEndFlag);

    plotter_v_x->setLine(0, 0, 0);
    plotter_x_t->setLine(0, 0, 0);

    sln_thread->start();
}

void GenWnd::computeEnd()
{
    statusBar()->showMessage("Successful!");

    act_cmp->setEnabled(computeEndFlag = true);
    act_opt->setEnabled(computeEndFlag);

    plotter_v_x->setLine(sln_thread->pnt_x.data(), sln_thread->pnt_v.data(), sln_thread->pnt_x.size());
    plotter_x_t->setLine(sln_thread->pnt_t.data(), sln_thread->pnt_x.data(), sln_thread->pnt_x.size());
}

void GenWnd::options()
{
    if(!dlg_options)
    {
        dlg_options = new DlgOpt(this);
        connect(dlg_options, SIGNAL(setParam(bool,float,float,float,float,float,float)),
                this, SLOT(getParam(bool,float,float,float,float,float,float)));
    }

    dlg_options->radio_euler->setChecked(sln_thread->method_euler);
    dlg_options->radio_rkutt->setChecked(!sln_thread->method_euler);
    dlg_options->edt_eps->setText(QString::number(sln_thread->eps));
    dlg_options->edt_omg->setText(QString::number(sln_thread->omg));
    dlg_options->edt_err->setText(QString::number(sln_thread->err));
    dlg_options->edt_x0->setText(QString::number(sln_thread->x0));
    dlg_options->edt_v0->setText(QString::number(sln_thread->v0));
    dlg_options->edt_t0->setText(QString::number(sln_thread->t0));

    dlg_options->show();
    dlg_options->activateWindow();
}

void GenWnd::getParam(bool a0, float a1, float a2, float a3, float a4, float a5, float a6)
{
    sln_thread->method_euler = a0;
    sln_thread->eps = a1;
    sln_thread->omg = a2;
    sln_thread->err = a3;
    sln_thread->x0 = a4;
    sln_thread->v0 = a5;
    sln_thread->t0 = a6;
}

void GenWnd::writeSettings()
{
    QSettings settings("Victor Korobkovsky", "Liquid");

    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());

    settings.setValue("statusbar", statusBar()->isVisible());
    settings.setValue("toolbar", toolbar->isVisible());
    settings.setValue("plotter_x_t", plotter_x_t->isVisible());
    settings.setValue("plotter_v_x", plotter_v_x->isVisible());
}

void GenWnd::readSettings()
{
    QSettings settings("Victor Korobkovsky", "Liquid");

    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("state").toByteArray());

    statusBar()->setVisible(settings.value("statusbar", true).toBool());
    toolbar->setVisible(settings.value("toolbar", true).toBool());
    plotter_x_t->setVisible(settings.value("plotter_x_t", true).toBool());
    plotter_v_x->setVisible(settings.value("plotter_v_x", true).toBool());

    act_statusbar->setChecked(settings.value("statusbar", true).toBool());
    act_graph1->setChecked(settings.value("plotter_x_t", true).toBool());
    act_graph2->setChecked(settings.value("plotter_v_x", true).toBool());
}

void GenWnd::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Close"), tr("Do you want to close the Liquid?"),
                                                              QMessageBox::Yes | QMessageBox::Cancel);

    if(reply == QMessageBox::Cancel) event->ignore();
    else
    {
        writeSettings();
        event->accept();
    }
}

void GenWnd::setupUi(QMainWindow *parent)
{
    plotter_v_x = new Plotter();
    plotter_v_x->setNameX("X");
    plotter_v_x->setNameY("V");
    plotter_v_x->setLineColor(0x058805);

    plotter_x_t = new Plotter();
    plotter_x_t->setNameX("T");
    plotter_x_t->setNameY("X");
    plotter_x_t->setLineColor(0xff0022);

    QMenu *menu_file = parent->menuBar()->addMenu(tr("&File"));
    QMenu *menu_edit = parent->menuBar()->addMenu(tr("&Edit"));
    QMenu *menu_view = parent->menuBar()->addMenu(tr("&View"));
    QMenu *menu_help = parent->menuBar()->addMenu(tr("&Help"));

    menu_view->addSeparator();

    act_cmp = menu_file->addAction(tr("Compute"));
    act_cmp->setShortcut(Qt::Key_Space);
    act_cmp->setIcon(QIcon(":/images/compute.png"));
    act_cmp->setStatusTip(tr("Start solution"));
    connect(act_cmp, SIGNAL(triggered()), parent, SLOT(compute()));

    menu_file->addSeparator();

    QAction *act_quit = menu_file->addAction(tr("Quit"));
    act_quit->setShortcut(Qt::CTRL + Qt::Key_Q);
    act_quit->setIcon(QIcon(":/images/delete.png"));
    act_quit->setStatusTip(tr("Quit the application"));
    connect(act_quit, SIGNAL(triggered()), parent, SLOT(close()));

    act_opt = menu_edit->addAction(tr("Options"));
    act_opt->setShortcut(Qt::CTRL + Qt::Key_O);
    act_opt->setIcon(QIcon(":/images/options.png"));
    act_opt->setStatusTip(tr("Set parameters of method"));
    connect(act_opt, SIGNAL(triggered()), parent, SLOT(options()));

    QAction *act_cln = menu_edit->addAction(tr("Clean"));
    act_cln->setShortcut(QKeySequence::Delete);
    act_cln->setIcon(QIcon(":/images/trash.png"));
    act_cln->setStatusTip(tr("Clean plots"));
    connect(act_cln, SIGNAL(triggered()), this, SLOT(clean()));

    QAction *act_abt = menu_help->addAction(tr("About Liquid"));
    act_abt->setShortcut(Qt::Key_F1);
    act_abt->setIcon(QIcon(":/images/icon.png"));
    connect(act_abt, SIGNAL(triggered()), parent, SLOT(about()));

    QAction *act_abt_qt = menu_help->addAction(tr("About &Qt"));
    connect(act_abt_qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    act_graph1 = menu_view->addAction(tr("Graph &x(t)"));
    act_graph1->setCheckable(true);
    act_graph1->setChecked(plotter_x_t->isVisible());
    connect(act_graph1, SIGNAL(toggled(bool)), plotter_x_t, SLOT(setVisible(bool)));

    act_graph2 = menu_view->addAction(tr("Graph &v(x)"));
    act_graph2->setCheckable(true);
    act_graph2->setChecked(plotter_v_x->isVisible());
    connect(act_graph2, SIGNAL(toggled(bool)), plotter_v_x, SLOT(setVisible(bool)));

    toolbar = new QToolBar("Tool");
    toolbar->setObjectName(tr("toolbar"));
    toolbar->addAction(act_cmp);
    toolbar->addAction(act_opt);
    toolbar->addAction(act_cln);
    toolbar->addAction(act_quit);
    parent->addToolBar(Qt::RightToolBarArea, toolbar);

    QMenu *menu_panels = createPopupMenu();
    menu_panels->setTitle(tr("Panels"));
    menu_view->addMenu(menu_panels);
    parent->statusBar()->showMessage(tr("Ready"));

    act_statusbar = menu_panels->addAction(tr("Statusbar"));
    act_statusbar->setCheckable(true);
    connect(act_statusbar, SIGNAL(toggled(bool)), statusBar(), SLOT(setVisible(bool)));

    QWidget *centralWidget = new QWidget();
    parent->setCentralWidget(centralWidget);

    QHBoxLayout *centralLayout = new QHBoxLayout(centralWidget);
    centralLayout->addWidget(plotter_x_t);
    centralLayout->addWidget(plotter_v_x);

    parent->setWindowIcon(QIcon(":/images/icon.png"));
    parent->setWindowTitle(tr("Liquid"));
    parent->setMinimumSize(300, 225);
    parent->resize(QSize(800, 600));
}
