#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dmp.h"
#include "canonical.h"
#include "basis.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    plan = new qptrajectory();
    ui->qcustomplot->setInteractions(QCP::iRangeZoom |  QCP::iRangeDrag);
    ui->qcustomplot->legend->setVisible(true);
    ui->qcustomplot2->setInteractions(QCP::iRangeZoom |  QCP::iRangeDrag);
    ui->qcustomplot3->setInteractions(QCP::iRangeZoom |  QCP::iRangeDrag);
    ui->qcustomplot2->plotLayout()->insertRow(0);
    ui->qcustomplot2->plotLayout()->addElement(0, 0, new QCPTextElement(ui->qcustomplot2, "canonical function tau=3.0"));
    ui->qcustomplot3->plotLayout()->insertRow(0);
    ui->qcustomplot3->plotLayout()->addElement(0, 0, new QCPTextElement(ui->qcustomplot3, "50 Gaussion functions"));




    ui->qcustomplot2->xAxis->setRange(0.0,1.0);
    ui->qcustomplot2->yAxis->setRange(0.0,1.1);

    ui->qcustomplot3->yAxis->setRange(0.0,1.1);
    ui->qcustomplot3->xAxis->setRange(0.0,2.0);
    obstacle_curve =new QCPCurve(ui->qcustomplot->xAxis , ui->qcustomplot->yAxis);
    robot = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
    agent_curve = new QCPCurve(ui->qcustomplot->xAxis ,ui->qcustomplot->yAxis );
    path_curve = new QCPCurve(ui->qcustomplot->xAxis ,ui->qcustomplot->yAxis );
    phase_curve = new QCPCurve(ui->qcustomplot2->xAxis , ui->qcustomplot2->yAxis);

    double sig_size = 50;
    count=0.0;
    for(int i=0;i<sig_size;i++){
        ui->qcustomplot3->addGraph();
    }

    //generate the path
    std::vector<segments> path;
    trajectory_profile p1 , p2 ,p3 ,p4  ;

    //initial curve_datacondition
    a_x = 0.0 ;a_y =0.0;
    v_x = 0.0 ;v_y =0.0;
    p_x = 1.0 ;p_y =0.0;

    p1.pos << 1.0,0,0;
    p1.vel << 0.0,0.0,0;
    p1.acc << 0.00,-0.0,0;

    p2.pos<< 1.5,2,0;
    p2.vel<< -0.3,-0.2,0;
    p2.acc<< -0.0,-0.0,0;

    p3.pos<< 0.5,2,0;
    p3.vel<< 0.0,-0.0,0;
    p3.acc<< 0.0,0.0,0;


    p4.pos<< 1.0,-5.0,0;
    p4.vel<< 0.3,0.2,0;
    p4.acc<< -0.1,0.1,0;

   path.push_back(segments(p1,p2,0.5));
   path.push_back(segments(p2,p3,0.5));
   //path.push_back(segments(p3,p4,4));
   //path.push_back(segments(p4,p1,4));

   data = plan->get_profile(path,2.0,0.01);

    //g y0
    init_x =  data[0].pos[0]; //xy_0
    init_y =  data[0].pos[1]; //yy_0
    end_x = data[data.size()-1].pos[0]; //xg
    end_y = data[data.size()-1].pos[1]; //yg
    std::cout << "init " << init_x << "  " <<init_y<<std::endl;
    std::cout << "end " << end_x << "  " <<end_y<<std::endl;

    //plot the generate path
   for(int i=0;i < data.size();i++)
    {
       path_data.push_back(QCPCurveData(data.size()-1 , data[i].pos[0],data[i].pos[1] ));
   }
   //learn the path
    std::vector<double> y_d , y_dot_d ,y_ddot_d;
    std::vector<double> x_d , x_dot_d ,x_ddot_d;

    y_d .clear();
    y_dot_d.clear();
    y_ddot_d.clear();
    y_d.resize(data.size());
    y_dot_d.resize(data.size());
    y_ddot_d.resize(data.size());

    x_d .clear();
    x_dot_d.clear();
    x_ddot_d.clear();
    x_d.resize(data.size());
    x_dot_d.resize(data.size());
    x_ddot_d.resize(data.size());

    for(int i=0;i<data.size();i++){
        y_d[i] = data[i].pos[1];
        y_dot_d[i] = data[i].vel[1];
        y_ddot_d[i] = data[i].acc[1];
        x_d[i] = data[i].pos[0];
        x_dot_d[i] = data[i].vel[0];
        x_ddot_d[i] = data[i].acc[0];
    }

    //finish
    dmp_x =new dmp;
    dmp_y = new dmp;
    basis_vecx.clear();
    basis_vecy.clear();


    canonical can(1.0, 0.01 ,3.0);
    xs = can.get_evolution();
    phase_data.clear();
    phase_data.resize(xs.size());
    for(unsigned int i=0;i<phase_data.size();i++){
        phase_data[i] = QCPCurveData(i ,   i*0.01 ,xs[i]);
    }
    phase_curve->data()->set(phase_data,true);

    for(unsigned int i=0; i<sig_size ;i++){
        sig[i] = 0.01;
        center[i] =(i*(1/sig_size));
    }

    for(int i=0;i<sig_size  ;i++){
        basis_vecx.push_back(new basis( xs ,sig[i]   ,center[i] , end_x , init_x));
        basis_vecy.push_back(new basis( xs ,sig[i]   ,center[i] , end_y , init_y));
     std::cout << std::endl   << center[i]<< std::endl;
    }

    for(unsigned int i=0 ; i<sig_size ;i++){
        basis_vecy[i]->learn_demo(y_d ,y_dot_d , y_ddot_d);
        basis_vecx[i]->learn_demo(x_d ,x_dot_d , x_ddot_d);
    }
    for(unsigned int i=0;i<sig_size;i++){
        for(double t=0;t<4;t+=0.01){
            double x = t ;
            double y = basis_vecx[i]->psi(t,basis_vecx[i]->sig , basis_vecx[i]->c );

            ui->qcustomplot3->graph(i)->addData( x , y );
            QPen pen;
            int num =  i ;
            num = num%9;
            switch (num) {
            case 0: case 10:
                pen.setColor(Qt::red);
                break;
            case 1: case 11:
                pen.setColor(Qt::gray);
              break;
            case 2: case 13:
                pen.setColor(Qt::black);
                break;
            case 3:
                pen.setColor(Qt:: blue);
                break;
            case 4:
                pen.setColor(Qt::cyan);
                break;
            case 5:
                pen.setColor(Qt::magenta);
                break;
            case 6:
                pen.setColor(Qt::yellow);
                break;
            case 7:
                pen.setColor(Qt::darkBlue);
                break;
            case 8:
                pen.setColor(Qt::green);
                break;
            case 9:
                pen.setColor(Qt::darkCyan);
                break;
            default:
                break;
            }
            ui->qcustomplot3->graph(i)->setPen(QPen(i));
            ui->qcustomplot3->graph(i)->setPen(QPen(pen));

        }

//        std::cout <<  basis_vecx[i]->c  <<std::endl;
    }
//    for(unsigned int i=0;i<10;i++){
//        std::cout << basis_vecy[i]->get_weight()<<std::endl;
//    }

    double a = 1.0;
    double b = 1.0;

//    double f_x  =
//    double f_y   =nonlinear_term( v_y, p_y,end_y,init_y);
    std::cout << "ok" <<std::endl;
    dmp_x->load_basis(basis_vecx);
    dmp_y->load_basis(basis_vecy);

    //double delta_t = 0.01;
    curve_data.clear();
    curve_data.resize(3000);
    v_x =0.1;
    v_y = 0.1;

    p_x =1.0;
    p_y=0.0;


    std::cout << "init : "<<init_x << "    " <<init_y <<std::endl;
    std::cout <<"end : " << end_x  << "  "  << end_y <<std::endl;



    curve_data.clear();
    //curve_data.push_back(QCPCurveData(0 , p_x , p_y));
    //curve_data[0] = QCPCurveData(1 , p_x , p_y);
    init_x = 1.0;
    init_y = 0.0;

    p_x =1.0;
    p_y = 0.0;
//    end_x = 0.3;
//    end_y = 1.3;
//    end_x = 0.3;
//    end_y = 1.3;


//    for(unsigned int i=1;i<data.size()+1;i++){
//        //τ v̇ = k(g − x) − dv − k(g − x 0 )s + skF(s)
//        a_x = a*(b*(end_x -data[i-1].pos[0]) - data[i-1].vel[0])  + dmp_x->nonlinear_term( v_x , xs[i],end_x,init_x);
//        a_y = a*(b*(end_y -data[i-1].pos[1]) - data[i-1].vel[1])  + dmp_y->nonlinear_term( v_y , xs[i],end_y,init_y);

//        v_x += a_x * delta_t;
//        v_y += a_y * delta_t;

//        p_x += v_x * delta_t;
//        p_y += v_y * delta_t;
//       // curve_data[i] = QCPCurveData(i , p_x + 0.3*cos(2*3.14159 /100*i) , p_y+ 0.3*sin(2*3.14159 /100*i));
//        //curve_data[i] = QCPCurveData( i+1 , p_x , p_y );
//        curve_data.push_back(QCPCurveData(i , p_x , p_y));

//        // std::cout << p_x  << "  " << p_y<<std::endl;
//    }
    count=1;
    std::cout << data.size()<<std::endl;


    ui->qcustomplot->replot();


    timer = new QTimer(this);

    connect(timer , SIGNAL(timeout()) , this ,  SLOT(update()));
    timer->start(20);
}

void MainWindow::update(){

//    a_x = dmp_x->nonlinear_term( i*0.01,1,0);
//    a_y =  dmp_y->nonlinear_term( i*0.01,1,0);
//   std::cout << "output : x"<<  dmp_x->nonlinear_term( i*0.01,1,0) <<std::endl;
//   std::cout << "output : y"<<  dmp_y->nonlinear_term( i*0.01,1,0) <<std::endl;

//    curve_data.append(QCPCurveData(curve_data.size()-1 , p_x , p_y));


//    a_x = dmp_x->nonlinear_term( v_x , p_x,end_x,init_x);
//    a_y = dmp_y->nonlinear_term( v_y, p_y,end_y,init_y);


   // std::cout <<"x" << p_x  << "y" << p_y<<std::endl;

        //τ v̇ = k(g − x) − dv − k(g − x 0 )s + skF(s)
    //τ v˙ = k(g − x) − dv − k(g − x0)s + skF(s)
        double delta_t=0.01;
        double a=1.,b=1.;
        double tau = 1.1;
        std::cout<< "ok"<<std::endl;
        if(count < data.size()+300){

            double rep_x =0.0 ,rep_y = 0.0;
            double distance = sqrt( (p_x-1) * (p_x-1) + (p_y-1)*(p_y-1));
            if( distance < 1.0){
                rep_x = 1.0*(p_x-1)/(distance*distance ) ;
                rep_y = 1.0*(p_y-1)/(distance*distance ) ;
//                rep_x = 0.0 ;
//                rep_y = 0.0 ;
            }else{
                rep_x = 0.0 ;
                rep_y = 0.0 ;
            }

            a_x = a*(b*(end_x -data[count-1].pos[0]) - data[count-1].vel[0])/0.9  + dmp_x->nonlinear_term( v_x , xs[count],end_x,init_x) +rep_x;
            a_y = a*(b*(end_y -data[count-1].pos[1]) - data[count-1].vel[1])/0.9  + dmp_y->nonlinear_term( v_y , xs[count],end_y,init_y) +rep_y;

            v_x += a_x * delta_t;
            v_y += a_y * delta_t;

            p_x += v_x * delta_t;
            p_y += v_y * delta_t;
            curve_data.push_back(QCPCurveData(count , p_x , p_y));
            count++;
            robot_data.clear();
            robot_data.resize(100);
            for(int i=0;i<100;i++){
              robot_data[i] = QCPCurveData(i , p_x + 0.3*cos(2*3.14159 /100*i) , p_y+ 0.3*sin(2*3.14159 /100*i));
            }
            obstaclecurve_data.clear();
            obstaclecurve_data.resize(100);
            for(int i=0;i<100;i++){
             obstaclecurve_data[i] = QCPCurveData(i , 1.0+ 0.3*cos(2*3.14159 /100*i) , 1.0+ 0.3*sin(2*3.14159 /100*i));
            }
        }
            obstacle_curve->data()->set(obstaclecurve_data,true);
            obstacle_curve->setPen(QPen(Qt::red));
            obstacle_curve->setVisible(false);   //
            robot->data()->set(robot_data,true);

        // curve_data[i] = QCPCurveData(i , p_x + 0.3*cos(2*3.14159 /100*i) , p_y+ 0.3*sin(2*3.14159 /100*i));
        //curve_data[i] = QCPCurveData( i+1 , p_x , p_y );


    agent_curve->data()->set(  curve_data );
    path_curve->data() ->set(path_data,true);
    path_curve->setPen(QPen(Qt::black));

    agent_curve->setName("imitated path");
    path_curve->setName("original path ");



    ui->qcustomplot->replot();
    ui->qcustomplot2->replot();
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_savebutton_clicked()
{
    QString filename = QFileDialog::getSaveFileName(0,"Save file",QDir::currentPath(),
           "png files (*.png)",
               new QString("png files (*.png)"));
    ui->qcustomplot->savePng(filename+QString("1"), 0, 0, 5,100, -1);
    ui->qcustomplot2->savePng(filename+QString("2"), 0, 0, 5,100, -1);

    ui->qcustomplot3->savePng(filename+QString("3"), 0, 0, 5,100, -1);

    //ui->qcustomplot->savePng(filename+, 0, 0, 5,100, -1);

}



void MainWindow::on_switch_button_clicked()
{
    if(timer->isActive()){
        timer->stop();
        ui->switch_button->setText("stop");
    }else{
        timer->start(20);
        ui->switch_button->setText("start");
    }
}
