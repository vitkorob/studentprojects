#include "wndmain.h"

WndMain::WndMain(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);
    readSettings();

    asd = new WndParam();
    eulerThread = new Euler();
    rkuttThread = new RKutt();
    imageDraw = new pixmapThread();
    timer = new QTimer(this);

    connect(rkuttThread, SIGNAL(finished()), this, SLOT(metodReady()));
    connect(eulerThread, SIGNAL(finished()), this, SLOT(metodReady()));

    connect(imageDraw, SIGNAL(finished()), this, SLOT(allReady()));

    connect(timer, SIGNAL(timeout()), this, SLOT(timerOut()));

    connect(asd, SIGNAL(pereDay(bool,double,double,double,double,double,double)),
            this, SLOT(getParam(bool,double,double,double,double,double,double)));

    metod = 1;
    compute = 0;
    resize_cmpt = 0;
    first = 1;
}

void WndMain::about()
{
    QMessageBox::information(this, tr("About Turing"),
                             tr("<h2>Turing 0.99</h2>"
                                "<p>Copyright &copy; 2011 Victor Korobkovsky</p>"));
}

void WndMain::timerOut()
{
    timerPhase++;
    repaint();
}

void WndMain::resizeEvent(QResizeEvent *event)
{
    if (!compute)
    {
        resize_cmpt = 1;
        event->ignore();
        return;
    }

    compute = 0;
    timer->start(50);

    emit metodReady();
}

void WndMain::paintEvent(QPaintEvent *)
{
    QPainter canvas(this);
    canvas.setRenderHint(QPainter::Antialiasing);

    if (first)
    {
        first = 0;
        return;
    }

    if (compute)
      canvas.drawPixmap(0, menuBar()->height(), QPixmap::fromImage(*imageDraw->pixmap));
    else
      Painter::drawLoader(&canvas, 0x33AA33, 8, timerPhase, QPoint(width()>>1, height()>>1), 4, 16, tr(""));
}

void WndMain::getParam(bool a0, double a1, double a2, double a3, double a4, double a5, double a6)
{
    if ((metod = a0))
    {
        rkuttThread->A = a1;
        rkuttThread->B = a2;
        rkuttThread->x0 = a3;
        rkuttThread->y0 = a4;
        rkuttThread->t = a5;
        rkuttThread->epsilon = a6;
    }
    else
    {
        eulerThread->A = a1;
        eulerThread->B = a2;
        eulerThread->x0 = a3;
        eulerThread->y0 = a4;
        eulerThread->t = a5;
        eulerThread->epsilon = a6;
    }

    this->activateWindow();
}

void WndMain::metodReady()
{
    if (metod)
    {
        imageDraw->masX = rkuttThread->pnt_x.data();
        imageDraw->masY = rkuttThread->pnt_y.data();
        imageDraw->masZ = rkuttThread->pnt_t.data();
        imageDraw->numMas = rkuttThread->pnt_t.size();

        aver = rkuttThread->t / rkuttThread->pnt_t.size();
    }
    else
    {
        imageDraw->masX = eulerThread->pnt_x.data();
        imageDraw->masY = eulerThread->pnt_y.data();
        imageDraw->masZ = eulerThread->pnt_t.data();
        imageDraw->numMas = eulerThread->pnt_t.size();

        aver = eulerThread->t / eulerThread->pnt_t.size();
    }

    imageDraw->w = width();
    imageDraw->h = height() - menuBar()->height();
    imageDraw->start();
}

void WndMain::allReady()
{
    action_prm->setEnabled(true);
    action_rtn->setEnabled(true);
    timer->stop();
    compute = 1;

    if (resize_cmpt)
    {
        compute = 0;
        resize_cmpt = 0;
        timer->start(50);
        emit metodReady();
        return;
    }

    repaint();
}

void WndMain::parameters()
{
    asd->radioButton_RKutt->setChecked(metod);
    asd->radioButton_Euler->setChecked(!metod);

    if (metod)
    {
        asd->lineEdit->setText(QString::number(rkuttThread->A));
        asd->lineEdit_2->setText(QString::number(rkuttThread->B));
        asd->lineEdit_3->setText(QString::number(rkuttThread->x0));
        asd->lineEdit_4->setText(QString::number(rkuttThread->y0));
        asd->lineEdit_5->setText(QString::number(rkuttThread->t));
        asd->lineEdit_6->setText(QString::number(rkuttThread->epsilon));
    }
    else
    {
        asd->lineEdit->setText(QString::number(eulerThread->A));
        asd->lineEdit_2->setText(QString::number(eulerThread->B));
        asd->lineEdit_3->setText(QString::number(eulerThread->x0));
        asd->lineEdit_4->setText(QString::number(eulerThread->y0));
        asd->lineEdit_5->setText(QString::number(eulerThread->t));
        asd->lineEdit_6->setText(QString::number(eulerThread->epsilon));
    }

    asd->show();
}

void WndMain::calculate()
{
    action_prm->setEnabled(false);
    action_rtn->setEnabled(false);

    compute = 0;
    timer->start(50);

    if (metod)
      rkuttThread->start();
    else
      eulerThread->start();
}

void WndMain::closeEvent(QCloseEvent *event)
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

void WndMain::writeSettings()
{
    QSettings settings("Victor Korobkovsky", "Turing");

    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
}

void WndMain::readSettings()
{
    QSettings settings("Victor Korobkovsky", "Turing");

    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("state").toByteArray());
}

void WndMain::setupUi(QMainWindow *parent)
{
    parent->resize(800, 600);
    parent->setMinimumSize(200, 150);
    parent->setWindowTitle(tr("Turing"));

    QAction *action_esc = new QAction(tr("Exit"), parent);
    action_esc->setShortcut(Qt::Key_Escape);
    connect(action_esc, SIGNAL(triggered()), parent, SLOT(close()));

    action_prm = new QAction(tr("Parameters"), parent);
    action_prm->setShortcut(Qt::CTRL + Qt::Key_P);
    connect(action_prm, SIGNAL(triggered()), parent, SLOT(parameters()));

    action_rtn = new QAction(tr("Compute"), parent);
    action_rtn->setShortcut(Qt::Key_Return);
    connect(action_rtn, SIGNAL(triggered()), parent, SLOT(calculate()));

    QAction *action_abt = new QAction(tr("About"), parent);
    action_abt->setShortcut(Qt::Key_F1);
    connect(action_abt, SIGNAL(triggered()), parent, SLOT(about()));

    QAction *action_aqt = new QAction(tr("About Qt"), parent);
    connect(action_aqt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    QMenu *menu = menuBar()->addMenu(tr("File"));
    menu->addAction(action_esc);

    QMenu *menu_2 = menuBar()->addMenu(tr("Experiment"));
    menu_2->addAction(action_prm);
    menu_2->addAction(action_rtn);

    QMenu *menu_3 = menuBar()->addMenu(tr("Help"));
    menu_3->addAction(action_abt);
    menu_3->addAction(action_aqt);
}
