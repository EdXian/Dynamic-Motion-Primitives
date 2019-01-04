#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dmp.h"
#include "qptrajectory.h"
#include "qcustomplot.h"
#include "QTimer"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
//    double sig[10] = {1, 0.6294, 0.3692, 0.2494, 0.1569, 0.0988, 0.0622, 0.0391 ,0.0246 ,0.0155};
//    double center[10] = {0.04167 , 0.01639, 0.0066 ,0.003 ,0.001, 0.0004, 0.0001, 0.00006, 0.00002, 0.00001};

//    double sig[10] = {0.04, 0.03, 0.01, 0.03, 0.05, 0.03, 0.02, 0.05,0.03 ,0.08};
//    double center[10] = {0.1 , .2, .3, .4 ,.5,.6, .7, .8, .9, 1.0};
    double sig[100];
    double center[100];
    dmp *dmp_x;
    dmp *dmp_y;

    std::vector<basis * > basis_vecx;
    std::vector<basis * > basis_vecy;

    qptrajectory *plan;

    QCPCurve  *robot;
    QVector<QCPCurveData> robot_data;

    QCPCurve *agent_curve;
    QVector<QCPCurveData> agentcurve_data;

    QCPCurve *obstacle_curve;
    QVector<QCPCurveData> obstaclecurve_data;

    QVector<QCPCurveData> curve_data;
    QCPCurve *path_curve;
    QVector<QCPCurveData> path_data;

    QVector<QCPCurveData> phase_data;
    QCPCurve * phase_curve;


    double a_x , a_y ;
    double v_x , v_y ;
    double p_x , p_y ;
    double init_x , init_y;
    double end_x , end_y;
    int count;
    QTimer *timer;
    std::vector<double> xs;
     std::vector<trajectory_profile> data;
private slots:
    void update();

    void on_savebutton_clicked();


    void on_switch_button_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
