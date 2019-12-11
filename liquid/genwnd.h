#ifndef GENWND_H
#define GENWND_H

#include <QtWidgets>
#include "dlgopt.h"
#include "plotter.h"
#include "solutionthread.h"

class GenWnd : public QMainWindow // sozdaem class dlya glavnogo okna
{
    Q_OBJECT  // qt trebuet etot macros chtobi rabotali SLOT i SIGNAL

public:
    explicit GenWnd(QWidget *parent = 0); // konstruktor glavnogo okna

private:
    QToolBar *toolbar; // panel instrumentov

    QAction *act_opt; // dlya dialogovogo okna s parametrami
    QAction *act_cmp; // dlya nachala obscheta
    QAction *act_graph1; // dlya scritiya/pokaza grafica x(t)
    QAction *act_graph2; // dlya scritiya/pokaza grafica v(x)
    QAction *act_statusbar; // panel' s podskazkami

    Plotter *plotter_x_t;  // setka dlya x(t)
    Plotter *plotter_v_x;  // setka dlya v(x)

    DlgOpt *dlg_options; // dialogovoe okno
    SolutionThread *sln_thread; // sozdaem otdel'nyy potok dlya resheniya uravneniy

    void setupUi(QMainWindow *parent); // inicializaciya graficheskih elementov
    void writeSettings(); // zapis' configuracii okna (nastroyki)
    void readSettings();  // chtenie configuracii okna (nastroyki)

    bool computeEndFlag; // flag nugen dlya ocenki gotovnosti dannih

protected:
    void closeEvent(QCloseEvent *event); // pri zacritii prilogeniya

private slots:
    void about();  // slot o programme
    void clean();  // slot dlya ochistki grafikov
    void compute(); // slot dlya nachala obscheta uravneniy
    void options(); // slot dlya vizova okna s opciyami
    void getParam(bool a0, float a1, float a2, float a3, float a4, float a5, float a6); // peredacha porametrov iz okna opciy v glavnoe okno
    void computeEnd(); // koda zakonchilsya obchtet
};

#endif // GENWND_H
