#include "nrmainwnd.h"

NRMainWnd::NRMainWnd(QWidget *parent) : QMainWindow(parent)
{
    setupUi();
    readSettings();

    nropt = 0;
    nrthr = 0;

    computeFlag = false;
}

void NRMainWnd::writeSettings()
{
    QSettings settings("Dmitri Diakonov", "Rings");

    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
}

void NRMainWnd::readSettings()
{
    QSettings settings("Dmitri Diakonov", "Rings");

    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("state").toByteArray());
}

void NRMainWnd::closeEvent(QCloseEvent *event)
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

void NRMainWnd::about()
{
    QMessageBox::about(this, tr("About Rings"), tr("<h2>Rings 0.98</h2>"
                                                   "<p>Copyright &copy; 2011 Victor Korobkovsky</p>"));
}

void NRMainWnd::compute()
{
    if(!nrthr)
    {
        nrthr = new NRThread();
        connect(nrthr, SIGNAL(finished()), this, SLOT(computeEnd()));
    }

    statusBar()->showMessage("Please wait");

    act_cmp->setEnabled(computeFlag = false);
    act_opt->setEnabled(computeFlag);

    nrthr->start();
}

void NRMainWnd::options()
{
    if(!nrthr)
    {
        nrthr = new NRThread();
        connect(nrthr, SIGNAL(finished()), this, SLOT(computeEnd()));
    }

    if(!nropt)
    {
        nropt = new NROptions(this);
        connect(nropt, SIGNAL(setParam(float,float,float,float,float,float,float,float)),
                this, SLOT(getParam(float,float,float,float,float,float,float,float)));
    }

    nropt->edt_angle->setText(QString::number(nrthr->angle));
    nropt->edt_n0->setText(QString::number(nrthr->n0));
    nropt->edt_n1->setText(QString::number(nrthr->n1));
    nropt->edt_radius->setText(QString::number(nrthr->r));
    nropt->edt_d->setText(QString::number(nrthr->d));
    nropt->edt_lamda->setText(QString::number(nrthr->lamda));
    nropt->edt_amount->setText(QString::number(nrthr->rays));
    nropt->edt_pix->setText(QString::number(nrthr->pix));

    nropt->show();
    nropt->activateWindow();
}

void NRMainWnd::computeEnd()
{
    statusBar()->showMessage("Successful!");

    act_cmp->setEnabled(computeFlag = true);
    act_opt->setEnabled(computeFlag);

    label_pix->setPixmap(QPixmap::fromImage(*nrthr->pixmap));
    label_pix->resize(nrthr->pix, nrthr->pix);
    resize(nrthr->pix, nrthr->pix + 40);
}

void NRMainWnd::getParam(float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8)
{
    nrthr->angle = a1;
    nrthr->n0 = a2;
    nrthr->n1 = a3;
    nrthr->r = a4;
    nrthr->d = a5;
    nrthr->lamda = a6;
    nrthr->rays = floor(a7);
    nrthr->pix = floor(a8);
}

void NRMainWnd::setupUi()
{
    QMenu *menu_file = menuBar()->addMenu(tr("&File"));
    QMenu *menu_edit = menuBar()->addMenu(tr("&Edit"));
    QMenu *menu_help = menuBar()->addMenu(tr("&Help"));

    act_cmp = menu_edit->addAction(tr("Compute"));
    act_cmp->setShortcut(Qt::Key_Space);
    act_cmp->setStatusTip(tr("Start computing"));
    connect(act_cmp, SIGNAL(triggered()), this, SLOT(compute()));

    QAction *act_quit = menu_file->addAction(tr("Quit"));
    act_quit->setShortcut(Qt::CTRL + Qt::Key_Q);
    act_quit->setStatusTip(tr("Quit the application"));
    connect(act_quit, SIGNAL(triggered()), this, SLOT(close()));

    act_opt = menu_edit->addAction(tr("Options"));
    act_opt->setShortcut(Qt::CTRL + Qt::Key_O);
    act_opt->setStatusTip(tr("Set parameters"));
    connect(act_opt, SIGNAL(triggered()), this, SLOT(options()));

    QAction *act_abt = menu_help->addAction(tr("About Rings"));
    act_abt->setShortcut(Qt::Key_F1);
    act_abt->setStatusTip(tr("About Rings"));
    connect(act_abt, SIGNAL(triggered()), this, SLOT(about()));

    QAction *act_abt_qt = menu_help->addAction(tr("About &Qt"));
    connect(act_abt_qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    statusBar()->showMessage(tr("Welcome to Rings"));

    QWidget *centralWidget = new QWidget();
    setCentralWidget(centralWidget);

    label_pix = new QLabel(centralWidget);
    label_pix->resize(600, 600);

    setWindowTitle(tr("Rings"));
    setMinimumSize(300, 225);
    resize(QSize(600, 600));

}
