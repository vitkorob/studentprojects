#include "wndglav.h"

WndGlav::WndGlav(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);
    readSettings();

    cmpReady = 0;

    qwe = new WndOpt();
    comp = new solutionThread();
    risu = new pixmapThread();

    connect(comp, SIGNAL(finished()), this, SLOT(metodReady()));
    connect(risu, SIGNAL(finished()), this, SLOT(allReady()));

    connect(qwe, SIGNAL(pereDay(bool,float,float,float,float,float,float,float,float,float)),
            this, SLOT(getParam(bool,float,float,float,float,float,float,float,float,float)));
}

void WndGlav::metodReady()
{
    risu->masX = comp->pnt_x.data();
    risu->masY = comp->pnt_y.data();
    risu->masZ = comp->pnt_t.data();
    risu->numMas = comp->pnt_t.size();

    risu->w = width();
    risu->h = height() - menuBar()->height();
    risu->start();
}

void WndGlav::allReady()
{
    act_opt->setEnabled(true);
    act_clt->setEnabled(true);
    cmpReady = 1;

    repaint();
}

void WndGlav::getParam(bool a0, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, float a9)
{
    comp->metRKu = a0;
    comp->gam = a1;
    comp->g = a2;
    comp->L = a3;
    comp->del = a4;
    comp->p2 = a5;
    comp->eps = a6;
    comp->x0 = a7;
    comp->v0 = a8;
    comp->t = a9;

    this->activateWindow();
}

void WndGlav::compute()
{
    act_clt->setEnabled(false);
    act_opt->setEnabled(false);
    cmpReady = 0;

    comp->start();
}

void WndGlav::about()
{
    QMessageBox::warning(this, tr("About Solution"),
                         tr("<h2>Solution 2.1</h2>"
                            "<p>Copyright &copy; 2013 Victor Korobkovsky</p>"));
}

void WndGlav::options()
{
    qwe->rdoButton_RKutt->setChecked(comp->metRKu);
    qwe->rdoButton_Euler->setChecked(!comp->metRKu);
    qwe->lineEdit->setText(QString::number(comp->gam));
    qwe->lineEdit_2->setText(QString::number(comp->g));
    qwe->lineEdit_3->setText(QString::number(comp->L));
    qwe->lineEdit_4->setText(QString::number(comp->del));
    qwe->lineEdit_5->setText(QString::number(comp->p2));
    qwe->lineEdit_6->setText(QString::number(comp->eps));
    qwe->lineEdit_7->setText(QString::number(comp->x0));
    qwe->lineEdit_8->setText(QString::number(comp->v0));
    qwe->lineEdit_9->setText(QString::number(comp->t));

    qwe->show();
}

void WndGlav::paintEvent(QPaintEvent *)
{
    QPainter canvas(this);
    canvas.setRenderHint(QPainter::Antialiasing);

    if(cmpReady) canvas.drawPixmap(0, menuBar()->height(), QPixmap::fromImage(*risu->pixmap));
}

void WndGlav::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton reply;

    reply = QMessageBox::question(this, tr("Close program"), tr("If you end the program immediately,\n"
                                                                "you will lose all unsaved data.\n"
                                                                "Do you want to close the program?"), QMessageBox::Yes | QMessageBox::Cancel);

    if (reply == QMessageBox::Cancel) event->ignore();
    else
    {
        writeSettings();
        event->accept();
    }
}

void WndGlav::writeSettings()
{
    QSettings settings("Victor Korobkovsky", "Solution");

    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
}

void WndGlav::readSettings()
{
    QSettings settings("Victor Korobkovsky", "Solution");

    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("state").toByteArray());
}

void WndGlav::setupUi(QMainWindow *parent)
{
    parent->resize(800, 600);
    parent->setMinimumSize(256, 192);
    parent->setWindowTitle(tr("Solution"));

    QAction *act_quit = new QAction(tr("Quit"), parent);
    act_quit->setShortcut(Qt::CTRL + Qt::Key_Q);
    connect(act_quit, SIGNAL(triggered()), parent, SLOT(close()));

    act_opt = new QAction(tr("Options"), parent);
    act_opt->setShortcut(Qt::CTRL + Qt::Key_O);
    connect(act_opt, SIGNAL(triggered()), parent, SLOT(options()));

    act_clt = new QAction(tr("Compute"), parent);
    act_clt->setShortcut(Qt::Key_Space);
    connect(act_clt, SIGNAL(triggered()), parent, SLOT(compute()));

    QAction *act_abt = new QAction(tr("About Solution"), parent);
    act_abt->setShortcut(Qt::Key_F1);
    connect(act_abt, SIGNAL(triggered()), parent, SLOT(about()));

    QMenu *menu = menuBar()->addMenu(tr("File"));
    menu->addAction(act_clt);
    menu->addAction(act_opt);
    menu->addSeparator();
    menu->addAction(act_quit);

    QMenu *menu_2 = menuBar()->addMenu(tr("Help"));
    menu_2->addAction(act_abt);
}
