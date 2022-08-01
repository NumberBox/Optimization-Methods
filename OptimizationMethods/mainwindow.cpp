#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include "dialog_user.h"
#include "dialog_metod.h"
#include <QVector>
#include "dialog_question.h"
#include <QTimer>
#include <QFileDialog>
#include <ctime>
#include <QTextCodec>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->groupBox_11->hide();
    ui->groupBox_7->hide();
    ui->groupBox_8->hide();
    ui->groupBox_10->show();

    QFile file("MO_Kursa.db3");
        if((!file.exists())||(!file.open(QIODevice::ReadOnly))){
            if(file.open(QIODevice::WriteOnly | QIODevice::Text)){
                    file.close();
                    db = QSqlDatabase::addDatabase("QSQLITE");
                    db.setDatabaseName("MO_Kursa.db3");
                    db.open();
                    QMessageBox::warning(this,"Warning", "Отсутствует база данных\nДальнейшее использование невозможно");
                    db.close();
            }
        }
        else{
              file.close();
              db = QSqlDatabase::addDatabase("QSQLITE");
              db.setDatabaseName("MO_Kursa.db3");
        }

        connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slotEditRecord(QModelIndex)));
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableView->setMouseTracking(true);
        ui->tableView->setStyleSheet("QTableView {"
                                        "color: rgb(0, 0, 0);"
                                        "selection-background-color: rgb(255, 234, 197);"
                                        "background-color: rgb(255, 255, 255);"
                                        "selection-color: rgb(0, 0, 0);"
                                     "}"
                                     "QTableView::item:hover {"
                                         "background-color: rgb(249, 250, 150);"
                                     "}"
                                     "QHeaderView {"
                                        "background-color: rgb(255, 234, 197);"
                                        "font: italic;"
                                     "}"
                                     "QHeaderView::section:horizontal { "
                                        "color: rgb(0, 0, 0);"
                                        "font-style: italic;"
                                        "font-weight: bold;"
                                        "background-color: rgb(255, 234, 197);"
                                     "}");
        ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tableView->horizontalHeader()->setStretchLastSection(true);
        ui->tableView->verticalHeader()->hide();

        ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->tableWidget->setStyleSheet("QTableView { background-color: rgb(255, 255, 255); }"
                                       "QHeaderView::section:horizontal { "
                                        "color: rgb(0, 0, 0);"
                                      //  "font-size:14px;"
                                        "font: italic;"
                                       "border: 1px solid #000000;"
                                        "background-color: rgb(255, 234, 197);"

                                         "}");


        ui->tableWidget->setColumnCount(3);
        ui->tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("T1"));
        ui->tableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem("Т2"));
        ui->tableWidget->setHorizontalHeaderItem(2,new QTableWidgetItem("Целевая функция"));
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
        ui->tableWidget->setColumnWidth(0,ui->tableWidget->width()/3);
        ui->tableWidget->setColumnWidth(1,ui->tableWidget->width()/3);
        ui->tableWidget->setColumnWidth(2,ui->tableWidget->width()/3);
        ui->tableWidget->verticalHeader()->hide();
        ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
        ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

        container = QWidget::createWindowContainer(graph);
        ui->gridLayout_10->addWidget(container);

        graph->setSelectionMode(QtDataVisualization::QAbstract3DGraph::SelectionNone);
        m_sun = new QtDataVisualization::QCustom3DItem();
        graph->setAxisX(new QtDataVisualization::QValue3DAxis);
        graph->setAxisY(new QtDataVisualization::QValue3DAxis);
        graph->setAxisZ(new QtDataVisualization::QValue3DAxis);

        chartView  = new QtCharts::QChartView();
        chart_ = new QtCharts::QChart();
        chartView->setChart(chart_);
        axisX = new QtCharts::QValueAxis();
        axisY = new QtCharts::QValueAxis();
        chart_->addAxis(axisY, Qt::AlignLeft);
        chart_->addAxis(axisX, Qt::AlignBottom);
        axisY->setTickCount(10);
        axisX->setTickCount(10);
        chart_->legend()->setVisible(false);
        ui->gridLayout_4->addWidget(chartView);
        chartView->setRenderHint(QPainter::Antialiasing);



        QPen pen;
        pen.setWidth(1);
        pen.setColor(QColor(255,0,0));
        seriesA->setPen(pen);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_13_clicked()
{
    ui->lineEdit_18->clear();
    ui->lineEdit_19->clear();
}

void MainWindow::on_pushButton_14_clicked()//войти
{
    if(ui->lineEdit_18->text()==""||ui->lineEdit_19->text()==""){
        QMessageBox::information(this,"Ошибка","Не все поля не заполнены");
        return;
    }
    db.open();
    QSqlQuery query;
    QVector<QString> employ;
    query.exec("SELECT role, login, password FROM USER");
    while (query.next()){
        employ.push_back(query.value(0).toString());
        employ.push_back(query.value(1).toString());
        employ.push_back(query.value(2).toString());
    }
    db.close();
    for (int i = 1;i<employ.size();i+=3) {


        if((ui->lineEdit_18->text()==employ.at(i))&&(ui->lineEdit_19->text()==employ.at(i+1))){

            if(employ.at(i-1)=="Администратор"){
                   ui->groupBox_10->hide();
                   ui->groupBox_8->show();
                   this->on_pushButton_2_clicked();
                   return;
            }
            if(employ.at(i-1)=="Исследователь"){
                   ui->groupBox_10->hide();
                   ui->comboBox->clear();
                   ui->comboBox_2->clear();
                   db.open();
                   QSqlQuery query;
                   query.exec("SELECT var FROM QUESTION");
                   while (query.next()){
                       ui->comboBox->addItem(query.value(0).toString());
                   }
                   query.exec("SELECT name FROM METOD");
                   while (query.next()){
                       ui->comboBox_2->addItem(query.value(0).toString());
                   }
                   db.close();
                   this->on_comboBox_activated(ui->comboBox->currentText());

                   ui->groupBox_11->show();
                   return;

            }
        }

    }
    for (int i = 1;i<employ.size();i+=3) {
        if(ui->lineEdit_18->text()==employ.at(i)){
            QMessageBox::information(this,"Ошибка","Пароль не верен");
            return;
        }
    }
    QMessageBox::information(this,"Ошибка","Такого пользователя не существует");
}

double func_(double T1, double T2){
    double c1 = 1.0*1.0*(((T1-T2)*(T1-T2))+1.0*1.0/9.0*((T1+T2-1.0*10.0)*(T1+T2-1.0*10.0)));
    return round(c1*1000)/1000.0;
}
void MainWindow::box_max()
{
    double mint1 = ui->lineEdit_22->text().toDouble();
    double mint2 = ui->lineEdit_23->text().toDouble();
    double maxt1 = ui->lineEdit_21->text().toDouble();
    double maxt2 = ui->lineEdit_24->text().toDouble();
    double touc = ui->lineEdit_26->text().toDouble();//расчет
    //метод бокса
    srand(time(0));
    int _n = 2;
    int _N = 4;
    double _g[2] = {mint1, mint2};
    double _h[2] = {maxt1, maxt2};
    double _x[2][4];
    double _r[2][4];
    int _P = 0;

    while(true){
        int ___ = 0;
        bool ___bool = false;

        //1
        while (true){
            for (int i = 0;i<_n;i++) {
                for (int j=0;j<_N;j++) {
                  _r[i][j] = rand()%10/10.0;
                }
            }

            for (int i = 0;i<_n;i++) {
                for (int j=0;j<_N;j++) {
                    _x[i][j] = _g[i]+_r[i][j]*(_h[i]-_g[i]);
                }
            }
            bool check = false;
            for (int j=0;j<_N;j++) {
                if((_x[0][j]+_x[1][j])<=12){
                    check = true;
                    _P++;
                }
            }
            if (check){
                break;
            }
        }

        for (int j=0;j<_N;j++) {
            //qDebug()<<_x[0][j]+_x[1][j];
            if((_x[0][j]+_x[1][j])>12){
                while(true){
                   double sum = 0;
                   for (int k = 0;k<_P;k++) {
                      sum+=_x[0][k];
                   }
                   _x[0][j]=(1.0/2.0)*(_x[0][j]+(1/_P)*sum);
                   sum = 0;
                   for (int k = 0;k<_P;k++) {
                      sum+=_x[1][k];
                   }
                   _x[1][j]=(1.0/2.0)*(_x[1][j]+(1/_P)*sum);

                   if((_x[0][j]+_x[1][j])<=12){
                      _P++;
                      break;
                   }
                   ___++;
                   if(___ == 100){
                       ___ = 0;
                       ___bool = true;
                       break;
                   }
                }
            }
        }
        if(!___bool){
            break;
        }
    }

    //2
    double _F[4];
    for (int i = 0;i<_N;i++) {
        _F[i] = func_(_x[0][i],_x[1][i]);
        qDebug()<<_F[i]<<_x[0][i]<<_x[1][i];
    }
    int ____ = 0;
    while(true){
        //3
        int _G = 0;
        double _max_c = 0;
        for (int i = 0;i<_N;i++) {
            if(_max_c<_F[i]){
                _max_c = _F[i];
                _G = i;
            }
        }
        int _D = 0;
        double _min_c = 1000;
        for (int i = 0;i<_N;i++) {
            if(_min_c>_F[i]){
                _min_c = _F[i];
                _D = i;
            }
        }
        qDebug()<<_D<<_G;

        //4
        double _C[2];
        for (int i = 0;i<_n;i++) {
            double sum = 0;
            for (int j = 0;j<_N;j++) {
               sum+=_x[i][j];
            }
            _C[i] = 1.0/(_N-1)*(sum-_x[i][_D]);
            qDebug()<<_C[i];
        }

        //5
        double _B;
        double sum = 0;
        for (int i = 0;i<_n;i++) {
           sum+=(abs(_C[i]-_x[i][_D])+abs(_C[i]-_x[i][_G]));
        }
        _B = (1.0/(2.0*_n))*sum;
        if(_B<touc){
            qDebug()<<"end"<<func_(_x[0][_G],_x[1][_G])<<_x[0][_G]<<_x[1][_G];
            int znak = 2;
            if(ui->lineEdit_26->text().toDouble()*100.0==1.0){
                znak = 2;
            }
            else if(ui->lineEdit_26->text().toDouble()*100.0==10.0){
                znak = 1;
            }
            else if(ui->lineEdit_26->text().toDouble()*100.0==0.1){
                znak = 3;
            }
            else if(ui->lineEdit_26->text().toDouble()*100.0==100.0){
                znak = 0;
            }

            QString str = QString("%1").arg(func_(_x[0][_G],_x[1][_G]), 0, 'f', znak);
            ui->lineEdit_29->setText(str);
            str = QString("%1").arg(_x[0][_G], 0, 'f', znak);
            ui->lineEdit_28->setText(str);
            str = QString("%1").arg(_x[1][_G], 0, 'f', znak);
            ui->lineEdit_30->setText(str);

            double max_c_label = ui->lineEdit_29->text().toDouble();
            ui->label_2->setText("Максимальная прибль от реализации\nцелевого компонента состовляет "+QString("%1").arg(max_c_label*100, 0, 'f', 0)+" у.е.");

            break;
        }

        //6
        double _xO[2];
        for (int i = 0;i<_n;i++) {
            _xO[i] = 2.3*_C[i]-1.3*_x[i][_D];
        }
        for (int i = 0;i<_n;i++) {
            if(_xO[i]<_g[i]){
               _xO[i] = _g[i]+touc;
            }
            if(_xO[i]>_h[i]){
               _xO[i] = _h[i]-touc;
            }
        }
        qDebug()<<_xO[0]<<_xO[1];

        //7
        while(true){
            if(_xO[0]+_xO[1]>12){
                _xO[0] = (1.0/2.0)*(_xO[0]+_C[0]);
                _xO[1] = (1.0/2.0)*(_xO[1]+_C[1]);
            }
            else{
                break;
            }
        }
        qDebug()<<_xO[0]<<_xO[1];

        //8
        double _FO;
        _FO = func_(_xO[0],_xO[1]);

        //9
        while(true){
            if(_FO<_F[_D]){
                _xO[0] = (1.0/2.0)*(_xO[0]+_x[0][_G]);
                _xO[1] = (1.0/2.0)*(_xO[1]+_x[1][_G]);
            }
            else{
                break;
            }
            _FO = func_(_xO[0],_xO[1]);
        }
        qDebug()<<_FO;

        //10
        _x[0][_D] = _xO[0];
        _x[1][_D] = _xO[1];
        _F[_D] = _FO;

        ____++;
        if(____ == 100){

            qDebug()<<"end"<<func_(_x[0][_G],_x[1][_G])<<_x[0][_G]<<_x[1][_G];
            int znak = 2;
            if(ui->lineEdit_26->text().toDouble()*100.0==1.0){
                znak = 2;
            }
            else if(ui->lineEdit_26->text().toDouble()*100.0==10.0){
                znak = 1;
            }
            else if(ui->lineEdit_26->text().toDouble()*100.0==0.1){
                znak = 3;
            }
            else if(ui->lineEdit_26->text().toDouble()*100.0==100.0){
                znak = 0;
            }
            QString str = QString("%1").arg(func_(_x[0][_G],_x[1][_G]), 0, 'f', znak);
            ui->lineEdit_31->setText(str);
            str = QString("%1").arg(_x[0][_G], 0, 'f', znak);
            ui->lineEdit_32->setText(str);
            str = QString("%1").arg(_x[1][_G], 0, 'f', znak);
            ui->lineEdit_33->setText(str);
            break;
        }
    }
}

void MainWindow::box_min()
{
    double mint1 = ui->lineEdit_22->text().toDouble();
    double mint2 = ui->lineEdit_23->text().toDouble();
    double maxt1 = ui->lineEdit_21->text().toDouble();
    double maxt2 = ui->lineEdit_24->text().toDouble();
    double touc = ui->lineEdit_26->text().toDouble();//расчет
    //метод бокса
    srand(time(0));
    int _n = 2;
    int _N = 4;
    double _g[2] = {mint1, mint2};
    double _h[2] = {maxt1, maxt2};
    double _x[2][4];
    double _r[2][4];
    int _P = 0;

    while(true){
        int ___ = 0;
        bool ___bool = false;

        //1
        while (true){
            for (int i = 0;i<_n;i++) {
                for (int j=0;j<_N;j++) {
                  _r[i][j] = rand()%10/10.0;
                }
            }

            for (int i = 0;i<_n;i++) {
                for (int j=0;j<_N;j++) {
                    _x[i][j] = _g[i]+_r[i][j]*(_h[i]-_g[i]);
                }
            }
            bool check = false;
            for (int j=0;j<_N;j++) {
                if((_x[0][j]+_x[1][j])<=12){
                    check = true;
                    _P++;
                }
            }
            if (check){
                break;
            }
        }

        for (int j=0;j<_N;j++) {
            //qDebug()<<_x[0][j]+_x[1][j];
            if((_x[0][j]+_x[1][j])>12){
                while(true){
                   double sum = 0;
                   for (int k = 0;k<_P;k++) {
                      sum+=_x[0][k];
                   }
                   _x[0][j]=(1.0/2.0)*(_x[0][j]+(1/_P)*sum);
                   sum = 0;
                   for (int k = 0;k<_P;k++) {
                      sum+=_x[1][k];
                   }
                   _x[1][j]=(1.0/2.0)*(_x[1][j]+(1/_P)*sum);

                   if((_x[0][j]+_x[1][j])<=12){
                      _P++;
                      break;
                   }
                   ___++;
                   if(___ == 100){
                       ___ = 0;
                       ___bool = true;
                       break;
                   }
                }
            }
        }
        if(!___bool){
            break;
        }
    }

    //2
    double _F[4];
    for (int i = 0;i<_N;i++) {
        _F[i] = func_(_x[0][i],_x[1][i]);
        qDebug()<<_F[i]<<_x[0][i]<<_x[1][i];
    }
    int ____ = 0;
    while(true){


        //3
        int _G = 0;
        int _D = 0;
        double _max_c = 0;
        for (int i = 0;i<_N;i++) {
            if(_max_c<_F[i]){
                _max_c = _F[i];
                _D = i;
            }
        }

        double _min_c = 1000;
        for (int i = 0;i<_N;i++) {
            if(_min_c>_F[i]){
                _min_c = _F[i];
                _G = i;
            }
        }
        qDebug()<<_D<<_G;

        //4
        double _C[2];
        for (int i = 0;i<_n;i++) {
            double sum = 0;
            for (int j = 0;j<_N;j++) {
               sum+=_x[i][j];
            }
            _C[i] = 1.0/(_N-1)*(sum-_x[i][_D]);
            qDebug()<<_C[i];
        }

        //5
        double _B;
        double sum = 0;
        for (int i = 0;i<_n;i++) {
           sum+=(abs(_C[i]-_x[i][_D])+abs(_C[i]-_x[i][_G]));
        }
        _B = (1.0/(2.0*_n))*sum;
        if(_B<touc){
            qDebug()<<"end"<<func_(_x[0][_G],_x[1][_G])<<_x[0][_G]<<_x[1][_G];
            int znak = 2;
            if(ui->lineEdit_26->text().toDouble()*100.0==1.0){
                znak = 2;
            }
            else if(ui->lineEdit_26->text().toDouble()*100.0==10.0){
                znak = 1;
            }
            else if(ui->lineEdit_26->text().toDouble()*100.0==0.1){
                znak = 3;
            }
            else if(ui->lineEdit_26->text().toDouble()*100.0==100.0){
                znak = 0;
            }
            QString str = QString("%1").arg(func_(_x[0][_G],_x[1][_G]), 0, 'f', znak);
            ui->lineEdit_31->setText(str);
            str = QString("%1").arg(_x[0][_G], 0, 'f', znak);
            ui->lineEdit_32->setText(str);
            str = QString("%1").arg(_x[1][_G], 0, 'f', znak);
            ui->lineEdit_33->setText(str);
            break;
        }

        //6
        double _xO[2];
        for (int i = 0;i<_n;i++) {
            _xO[i] = 2.3*_C[i]-1.3*_x[i][_D];
        }
        for (int i = 0;i<_n;i++) {
            if(_xO[i]<_g[i]){
               _xO[i] = _g[i]+touc;
            }
            if(_xO[i]>_h[i]){
               _xO[i] = _h[i]-touc;
            }
        }
        qDebug()<<_xO[0]<<_xO[1];

        //7
        while(true){
            if(_xO[0]+_xO[1]>12){
                _xO[0] = (1.0/2.0)*(_xO[0]+_C[0]);
                _xO[1] = (1.0/2.0)*(_xO[1]+_C[1]);
            }
            else{
                break;
            }
        }
        qDebug()<<_xO[0]<<_xO[1];

        //8
        double _FO;
        _FO = func_(_xO[0],_xO[1]);

        //9
        while(true){
            if(_FO>_F[_D]){
                _xO[0] = (1.0/2.0)*(_xO[0]+_x[0][_G]);
                _xO[1] = (1.0/2.0)*(_xO[1]+_x[1][_G]);
            }
            else{
                break;
            }
            _FO = func_(_xO[0],_xO[1]);
        }
        qDebug()<<_FO;

        //10
        _x[0][_D] = _xO[0];
        _x[1][_D] = _xO[1];
        _F[_D] = _FO;
        ____++;
        if(____ == 100){

            qDebug()<<"end"<<func_(_x[0][_G],_x[1][_G])<<_x[0][_G]<<_x[1][_G];
            int znak = 2;
            if(ui->lineEdit_26->text().toDouble()*100.0==1.0){
                znak = 2;
            }
            else if(ui->lineEdit_26->text().toDouble()*100.0==10.0){
                znak = 1;
            }
            else if(ui->lineEdit_26->text().toDouble()*100.0==0.1){
                znak = 3;
            }
            else if(ui->lineEdit_26->text().toDouble()*100.0==100.0){
                znak = 0;
            }
            QString str = QString("%1").arg(func_(_x[0][_G],_x[1][_G]), 0, 'f', znak);
            ui->lineEdit_31->setText(str);
            str = QString("%1").arg(_x[0][_G], 0, 'f', znak);
            ui->lineEdit_32->setText(str);
            str = QString("%1").arg(_x[1][_G], 0, 'f', znak);
            ui->lineEdit_33->setText(str);
            break;
        }
    }
}

void MainWindow::scan_max()
{
    double mint1 = ui->lineEdit_22->text().toDouble();
    double mint2 = ui->lineEdit_23->text().toDouble();
    double maxt1 = ui->lineEdit_21->text().toDouble();
    double maxt2 = ui->lineEdit_24->text().toDouble();
    double touc = ui->lineEdit_26->text().toDouble();//расчет

    for (double i = mint1;i<=maxt1;i+=touc) {
        for (double j = mint2;j<=maxt2;j+=touc) {
            if((i+j)<=12.0){
                t1.push_back(i);
                t2.push_back(j);
                //double c1 = 1*pow((i*i+1*j-11),2)+pow((i+(j+j)-7),2);
                double c1 = 1.0*1.0*(((i-j)*(i-j))+1.0*1.0/9.0*((i+j-1.0*10.0)*(i+j-1.0*10.0)));
                c.push_back(round(c1*1000)/1000.0);
            }
        }
    }
    int ai = 0;
    double max_c = 0;
    for (int i = 0;i<c.size();i++) {
        if(max_c<c.at(i)){
            max_c = c.at(i);
            ai = i;
        }
    }
    int znak = 2;
    if(ui->lineEdit_26->text().toDouble()*100.0==1.0){
        znak = 2;
    }
    else if(ui->lineEdit_26->text().toDouble()*100.0==10.0){
        znak = 1;
    }
    else if(ui->lineEdit_26->text().toDouble()*100.0==0.1){
        znak = 3;
    }
    else if(ui->lineEdit_26->text().toDouble()*100.0==100.0){
        znak = 0;
    }

    QString str = QString("%1").arg(max_c, 0, 'f', znak);
    ui->lineEdit_29->setText(str);
    str = QString("%1").arg(t1.at(ai), 0, 'f', znak);
    ui->lineEdit_28->setText(str);
    str = QString("%1").arg(t2.at(ai), 0, 'f', znak);
    ui->lineEdit_30->setText(str);

    double max_c_label = ui->lineEdit_29->text().toDouble();
    ui->label_2->setText("Максимальная прибль от реализации\nцелевого компонента состовляет "+QString("%1").arg(max_c_label*100, 0, 'f', 0)+" у.е.");

    double min_c = 1000;
    for (int i = 0;i<c.size();i++) {
        if(min_c>c.at(i)){
            min_c = c.at(i);
            ai = i;
        }
    }

    str = QString("%1").arg(min_c, 0, 'f', znak);
    ui->lineEdit_31->setText(str);
    str = QString("%1").arg(t1.at(ai), 0, 'f', znak);
    ui->lineEdit_32->setText(str);
    str = QString("%1").arg(t2.at(ai), 0, 'f', znak);
    ui->lineEdit_33->setText(str);
}


void MainWindow::on_pushButton_15_clicked()//рассчитать
{


    this->on_pushButton_4_clicked();

    double mint1 = ui->lineEdit_22->text().toDouble();
    double mint2 = ui->lineEdit_23->text().toDouble();
    double maxt1 = ui->lineEdit_21->text().toDouble();
    double maxt2 = ui->lineEdit_24->text().toDouble();
    double touc = ui->lineEdit_26->text().toDouble();//расчет
    double touc_ = ui->lineEdit->text().toDouble();//таблица
    if(
    this->on_lineEdit_textChanged(ui->lineEdit->text())&&
    this->on_lineEdit_2_textChanged(ui->lineEdit_2->text())&&
    this->on_lineEdit_21_textChanged(ui->lineEdit_21->text())&&
    this->on_lineEdit_22_textChanged(ui->lineEdit_22->text())&&
    this->on_lineEdit_23_textChanged(ui->lineEdit_23->text())&&
    this->on_lineEdit_24_textChanged(ui->lineEdit_24->text())&&
    this->on_lineEdit_26_textChanged(ui->lineEdit_26->text())&&
    this->on_lineEdit_27_textChanged(ui->lineEdit_27->text())){

    }else{
        QMessageBox::information(this,"Ошибка","Проверьте правильность входных данных");
        return;
    }
    if(ui->lineEdit_22->text().toDouble()>=ui->lineEdit_21->text().toDouble()){
        QMessageBox::information(this,"Ошибка","Значение Min T1 должно быть меньше Max T1");
        return;
    }
    if(ui->lineEdit_23->text().toDouble()>=ui->lineEdit_24->text().toDouble()){
        QMessageBox::information(this,"Ошибка","Значение Min T2 должно быть меньше Max T2");
        return;
    }

    if(touc<0.01||touc_<0.01||ui->lineEdit_27->text().toDouble()<0.01)
        return;
    if (!(ui->comboBox_2->currentText()=="Метод Сканирования"||ui->comboBox_2->currentText()=="Метод Бокса")){
        QMessageBox::information(this,"Отсутствует метод","Выбранный метод будет релизован в ближайшее время");
        return;
    }

    ui->groupBox_11->hide();
    ui->groupBox_7->show();

    if (ui->comboBox_2->currentText()=="Метод Сканирования"){
        scan_max();
    }
    if (ui->comboBox_2->currentText()=="Метод Бокса"){
        scan_max();
        box_max();
        box_min();
    }






    for (double i = mint1;i<=maxt1;i+=touc_) {
        for (double j = mint2;j<=maxt2;j+=touc_) {
            if((i+j)<=12){
                t1_.push_back(i);
                t2_.push_back(j);
                //double c1 = 1*pow((i*i+1*j-11),2)+pow((i+(j+j)-7),2);
                double c1 = 1.0*1.0*(((i-j)*(i-j))+1.0*1.0/9.0*((i+j-1.0*10.0)*(i+j-1.0*10.0)));
                c_.push_back(c1);
            }
        }
    }
    for (double i = mint1;i<=maxt1;i+=0.1) {
        for (double j = mint2;j<=maxt2;j+=0.1) {
            if((i+j)<=12){
                t1__.push_back(i);
                t2__.push_back(j);
                //double c1 = 1*pow((i*i+1*j-11),2)+pow((i+(j+j)-7),2);
                double c1 = 1.0*1.0*(((i-j)*(i-j))+1.0*1.0/9.0*((i+j-1.0*10.0)*(i+j-1.0*10.0)));
                c__.push_back(c1);
            }
        }
    }

    time_i = 0;
    time1 = new QTimer();
    time1->start(1);
    connect(time1, SIGNAL(timeout()),this,SLOT(table()));

    int ai = 0;
    double max_c = 0;
    for (int i = 0;i<c.size();i++) {
        if(max_c<c.at(i)){
            max_c = c.at(i);
            ai = i;
        }
    }

    double min_c = 1000;
    for (int i = 0;i<c.size();i++) {
        if(min_c>c.at(i)){
            min_c = c.at(i);
            ai = i;
        }
    }

    int znak_graph = 2;
    if(ui->lineEdit_27->text().toDouble()*100.0==1.0){
        znak_graph = 100;
    }
    else if(ui->lineEdit_27->text().toDouble()*100.0==10.0){
        znak_graph = 10;
    }
    else if(ui->lineEdit_27->text().toDouble()*100.0==0.1){
        znak_graph = 1000;
    }
    else if(ui->lineEdit_27->text().toDouble()*100.0==100.0){
        znak_graph = 1;
    }

    float sampleMinX = static_cast<float>(mint1);
    float sampleMaxX = static_cast<float>(maxt1);
    float sampleMinZ = static_cast<float>(mint2);
    float sampleMaxZ = static_cast<float>(maxt2);
    int sampleCountX = (sampleMaxX-sampleMinX)*znak_graph;
    int sampleCountZ = (sampleMaxZ-sampleMinZ)*znak_graph;



    m_sqrtSinProxy = new  QtDataVisualization::QSurfaceDataProxy();
    m_sqrtSinSeries = new QtDataVisualization::QSurface3DSeries(m_sqrtSinProxy);
    m_sqrtSinSeries->setBaseColor(QColor(255,255,255));
//******
    float stepX = (sampleMaxX - sampleMinX) / float(sampleCountX - 1);
    float stepZ = (sampleMaxZ - sampleMinZ) / float(sampleCountZ - 1);

    float x1 = 0.0f;
    float y1 = 0.0f;
    float z1 = 0.0f;
    QtDataVisualization::QSurfaceDataArray *dataArray = new QtDataVisualization::QSurfaceDataArray;
    dataArray->reserve(sampleCountZ);
    for(int i = 0; i<=sampleCountZ;i+=1){
        int index = 0;
        QtDataVisualization::QSurfaceDataRow *newRow = new QtDataVisualization::QSurfaceDataRow(sampleCountX);
        float z = qMin(sampleMaxZ, (i * stepZ + sampleMinZ));
        for (int j = 0; j < sampleCountX; j++) {
            float x = qMin(sampleMaxX, (j * stepX + sampleMinX));
            float y = 1.0f*1.0f*(((x-z)*(x-z))+1.0f*1.0f/9.0f*((x+z-1.0f*10.0f)*(x+z-1.0f*10.0f)));
            //float y = 1*pow((x*x+1*z-11),2)+pow((x+(z+z)-7),2);

            if((x+z)<=12){
                (*newRow)[index++].setPosition(QVector3D(x, y, z));
                x1 = x;
                y1 = y;
                z1 = z;
            }
            else{
                (*newRow)[index++].setPosition(QVector3D(x1, y1, z1));
            }
        }
        *dataArray << newRow;

    }
    m_sqrtSinProxy->resetArray(dataArray);


//******
    m_sqrtSinSeries->setDrawMode(QtDataVisualization::QSurface3DSeries::DrawSurfaceAndWireframe);
            m_sqrtSinSeries->setFlatShadingEnabled(true);
            graph->axisX()->setLabelFormat("%.2f");
            graph->axisZ()->setLabelFormat("%.2f");
            graph->axisX()->setRange(sampleMinX, sampleMaxX);
            graph->axisY()->setRange(static_cast<float>(min_c), static_cast<float>(max_c));
            graph->axisZ()->setRange(sampleMinZ, sampleMaxZ);
            graph->axisX()->setLabelAutoRotation(30);
            graph->axisY()->setLabelAutoRotation(90);
            graph->axisZ()->setLabelAutoRotation(30);
    QtDataVisualization::QAbstract3DGraph::ShadowQuality sq = QtDataVisualization::QAbstract3DGraph::ShadowQuality(0);
    graph->setShadowQuality(sq);
    graph->addSeries(m_sqrtSinSeries);

    m_sun-> setScaling(QVector3D(0.007f, 0.007f, 0.007f));
    m_sun->setMeshFile(QStringLiteral(":/new/prefix1/largesphere.obj"));
    QImage sunColor = QImage(2, 2, QImage::Format_RGB32);
    sunColor.fill(QColor(0xff, 0xbb, 0x00));
    m_sun->setTextureImage(sunColor);
    graph->addCustomItem(m_sun);
    m_sun->setPosition(QVector3D(ui->lineEdit_28->text().toDouble(),ui->lineEdit_29->text().toDouble(), ui->lineEdit_30->text().toDouble()));
   // graph->set




    double step_y = (max_c-min_c)/ui->lineEdit_2->text().toDouble();
    double i_i = min_c;
    while (i_i<max_c) {
        for (int i = 0;i<c__.size();i++) {
            if((c__.at(i)<i_i+(step_y/10))&&(c__.at(i)>i_i)){
                vec_t1_2d.push_back(t1__.at(i));
                vec_t2_2d.push_back(t2__.at(i));
            }
        }
        i_i+=step_y;
    }

    for (int i = 0;i<vec_t1_2d.size();i++) {
        seriesA->append(vec_t1_2d.at(i), vec_t2_2d.at(i));
    }
    seriesB->append(ui->lineEdit_28->text().toDouble(),ui->lineEdit_30->text().toDouble());
    chart_->addSeries(seriesA);

    seriesA->setColor(QColor(255, 0, 0));
    seriesA->setMarkerSize(3);
    seriesA->attachAxis(axisY);
    seriesA->attachAxis(axisX);

    chart_->addSeries(seriesB);

    seriesB->setColor(QColor(255, 165, 0));
    seriesB->setMarkerSize(20);
    seriesB->attachAxis(axisY);
    seriesB->attachAxis(axisX);




    axisX->setRange(mint1-1,maxt1+1);
    axisY->setRange(mint2-1,maxt2+1);
    axisX->setTitleText("t1");
    axisY->setTitleText("t2");
}

void MainWindow::on_pushButton_17_clicked()//назад
{
    c.clear();
    t1.clear();
    t2.clear();
    chart_->removeSeries(seriesA);
    chart_->removeSeries(seriesB);
    vec_t1_2d.clear();
    vec_t2_2d.clear();
    seriesA->clear();
    seriesB->clear();
    c_.clear();
    t1_.clear();
    t2_.clear();
    graph->removeSeries(m_sqrtSinSeries);
    delete  m_sqrtSinProxy;
    delete m_sqrtSinSeries;
    delete time1;
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    ui->groupBox_7->hide();
    ui->groupBox_11->show();
}

void MainWindow::on_pushButton_16_clicked()//сменить пользователя
{
    ui->groupBox_11->hide();
    ui->groupBox_10->show();
}

void MainWindow::on_pushButton_9_clicked()//сменить пользователя
{
    ui->groupBox_8->hide();
    ui->groupBox_10->show();
}
void MainWindow::update_model()
{



    db.open();
    model->select();
    db.close();
}
void MainWindow::on_pushButton_2_clicked()//методы решения
{
    ui->label_72->setText("Методы решения");
    this->select_buttons();
    ui->pushButton_2->setStyleSheet("QPushButton {"
                                        "background-color: rgb(249, 250, 150);"
                                    "border: 1px solid #000000;"
                                    "border-radius: 20px;"
                                     "font: bold;"
                                    "}"
                                    "QPushButton::hover {"
                                        "background-color: rgb(227, 227, 136);"
                                    "}"
                                    "QPushButton::pressed {"
                                     "border: 3px solid #000000;"
                                    "}");
    list.clear();
    list_bd.clear();
    QSqlQuery query;
    db.open();
    model = new QSqlTableModel();
    model->setTable("METOD");
    list<<"Id"<<"Название";
    list_bd<<"METOD"<<"id"<<"name";
    for(int i = 0; i < model->columnCount(); i++){
        model->setHeaderData(i,Qt::Horizontal,list[i]);
    }
    model->setSort(0,Qt::AscendingOrder);
    ui->tableView->setModel(model);
    model->select();
    db.close();
}

void MainWindow::on_pushButton_8_clicked()//добавить
{
    QStringList list_present;
    if(list_bd.at(0)=="USER"){
         dialog_user *edit = new dialog_user(list,list_present,list_bd,db,this);
         edit->setWindowFlag(Qt::WindowContextHelpButtonHint,false);
         edit->setWindowTitle("Добавление");
         connect(edit,SIGNAL(close_dialog()),this,SLOT(update_model()));
         edit->exec();
    }
    if(list_bd.at(0)=="METOD"){
         dialog_metod *edit = new dialog_metod(list,list_present,list_bd,db,this);
         edit->setWindowFlag(Qt::WindowContextHelpButtonHint,false);
         edit->setWindowTitle("Добавление");
         connect(edit,SIGNAL(close_dialog()),this,SLOT(update_model()));
         edit->exec();
    }
    if(list_bd.at(0)=="QUESTION"){
         dialog_question *edit = new dialog_question(list,list_present,list_bd,db,this);
         edit->setWindowFlag(Qt::WindowContextHelpButtonHint,false);
         edit->setWindowTitle("Добавление");
         connect(edit,SIGNAL(close_dialog()),this,SLOT(update_model()));
         edit->exec();
    }

}
void MainWindow::slotEditRecord(QModelIndex index)//редактировать
{

    QStringList list_present;
    for(int i =0;i<list.size();i++){
        list_present<<ui->tableView->model()->data(ui->tableView->model()->index(index.row(),i)).toString();
    }
    if(list_bd.at(0)=="USER"){
         dialog_user *edit = new dialog_user(list,list_present,list_bd,db,this);
         edit->setWindowFlag(Qt::WindowContextHelpButtonHint,false);
         edit->setWindowTitle("Редактирование");
         connect(edit,SIGNAL(close_dialog()),this,SLOT(update_model()));
         edit->exec();
    }
    if(list_bd.at(0)=="METOD"){
         dialog_metod *edit = new dialog_metod(list,list_present,list_bd,db,this);
         edit->setWindowFlag(Qt::WindowContextHelpButtonHint,false);
         edit->setWindowTitle("Редактирование");
         connect(edit,SIGNAL(close_dialog()),this,SLOT(update_model()));
         edit->exec();
    }
    if(list_bd.at(0)=="QUESTION"){
         dialog_question *edit = new dialog_question(list,list_present,list_bd,db,this);
         edit->setWindowFlag(Qt::WindowContextHelpButtonHint,false);
         edit->setWindowTitle("Редактирование");
         connect(edit,SIGNAL(close_dialog()),this,SLOT(update_model()));
         edit->exec();
    }

}

void MainWindow::on_pushButton_10_clicked()//пользователи
{
    ui->label_72->setText("Пользователи");
    this->select_buttons();
    ui->pushButton_10->setStyleSheet("QPushButton {"
                                        "background-color: rgb(249, 250, 150);"
                                    "border: 1px solid #000000;"
                                    "border-radius: 20px;"
                                     "font: bold;"
                                    "}"
                                    "QPushButton::hover {"
                                        "background-color: rgb(227, 227, 136);"
                                    "}"
                                    "QPushButton::pressed {"
                                     "border: 3px solid #000000;"
                                    "}");
    list.clear();
    list_bd.clear();
    QSqlQuery query;
    db.open();
    model = new QSqlTableModel();
    model->setTable("USER");
    list<<"Id"<<"Роль"<<"Логин"<<"Пароль";
    list_bd<<"USER"<<"id"<<"role"<<"login"<<"password";
    for(int i = 0; i < model->columnCount(); i++){
        model->setHeaderData(i,Qt::Horizontal,list[i]);
    }
    model->setSort(0,Qt::AscendingOrder);
    ui->tableView->setModel(model);
    model->select();
    db.close();
}

void MainWindow::on_pushButton_12_clicked()//задания
{
    ui->label_72->setText("Задания");
    this->select_buttons();
    ui->pushButton_12->setStyleSheet("QPushButton {"
                                        "background-color: rgb(249, 250, 150);"
                                    "border: 1px solid #000000;"
                                    "border-radius: 20px;"
                                     "font: bold;"
                                    "}"
                                    "QPushButton::hover {"
                                        "background-color: rgb(227, 227, 136);"
                                    "}"
                                    "QPushButton::pressed {"
                                     "border: 3px solid #000000;"
                                    "}");
    list.clear();
    list_bd.clear();
    QSqlQuery query;
    db.open();
    model = new QSqlTableModel();
    model->setTable("QUESTION");
    list<<"Id"<<"Вариант"<<"Текст";
    list_bd<<"QUESTION"<<"id"<<"var"<<"text";
    for(int i = 0; i < model->columnCount(); i++){
        model->setHeaderData(i,Qt::Horizontal,list[i]);
    }
    model->setSort(0,Qt::AscendingOrder);
    ui->tableView->setModel(model);
    model->select();
    db.close();
}
void MainWindow::select_buttons()
{
    ui->pushButton_2->setStyleSheet("QPushButton {"
                                        "background-color: rgb(249, 250, 150);"
                                    "border: 1px solid #000000;"
                                    "border-radius: 20px;"
                                    "}"
                                    "QPushButton::hover {"
                                        "background-color: rgb(227, 227, 136);"
                                    "}"
                                    "QPushButton::pressed {"
                                     "border: 3px solid #000000;"
                                    "}");
    ui->pushButton_12->setStyleSheet("QPushButton {"
                                        "background-color: rgb(249, 250, 150);"
                                    "border: 1px solid #000000;"
                                    "border-radius: 20px;"
                                    "}"
                                    "QPushButton::hover {"
                                        "background-color: rgb(227, 227, 136);"
                                    "}"
                                    "QPushButton::pressed {"
                                     "border: 3px solid #000000;"
                                    "}");
    ui->pushButton_10->setStyleSheet("QPushButton {"
                                        "background-color: rgb(249, 250, 150);"
                                    "border: 1px solid #000000;"
                                    "border-radius: 20px;"
                                    "}"
                                    "QPushButton::hover {"
                                        "background-color: rgb(227, 227, 136);"
                                    "}"
                                    "QPushButton::pressed {"
                                     "border: 3px solid #000000;"
                                    "}");

}


void MainWindow::on_comboBox_activated(const QString &arg1)
{
    db.open();
    QSqlQuery query;
    query.exec("SELECT text FROM QUESTION WHERE var = \""+arg1+"\"");
    while (query.next()){
        ui->textEdit->setText(query.value(0).toString());
    }
    db.close();
}

void MainWindow::table()
{
    int znak = 1;
    if(ui->lineEdit->text().toDouble()*100.0==1.0){
        znak = 2;
    }
    else if(ui->lineEdit->text().toDouble()*100.0==10.0){
        znak = 1;
    }
    else if(ui->lineEdit->text().toDouble()*100.0==0.1){
        znak = 3;
    }
    else if(ui->lineEdit->text().toDouble()*100.0==100.0){
        znak = 0;
    }
    for (int i = 0;i<1000;i++) {
        ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
        QString str = QString("%1").arg(t1_.at(time_i), 0, 'f', znak);
        ui->tableWidget->setItem(0+ui->tableWidget->rowCount()-1,0,new QTableWidgetItem(str));
        str = QString("%1").arg(t2_.at(time_i), 0, 'f', znak);
        ui->tableWidget->setItem(0+ui->tableWidget->rowCount()-1,1,new QTableWidgetItem(str));
         str = QString("%1").arg(c_.at(time_i), 0, 'f', znak);
        ui->tableWidget->setItem(0+ui->tableWidget->rowCount()-1,2,new QTableWidgetItem(str));
        time_i++;

        if(time_i>=t1_.size()){
            time1->stop();
            return;
        }
    }

}



void MainWindow::on_pushButton_5_clicked()//3d график
{
    d3_or_2d = false;
    ui->label_64->setText("Поверхность отклика целевой функции");
    ui->groupBox_9->hide();
    ui->groupBox_12->show();
    ui->pushButton_5->setStyleSheet("QPushButton {"
                                    "background-color: rgb(249, 250, 150);"
                                "border: 1px solid #000000;"
                                "border-radius: 15px;"
                                    "font: bold;"
                                "}"
                                "QPushButton::hover {"

                                    "background-color: rgb(227, 227, 136);"
                                "}"
                                "QPushButton::pressed {"
                                 "border: 3px solid #000000;"
                                "}");
    ui->pushButton_4->setStyleSheet("QPushButton {"
                                    "background-color: rgb(249, 250, 150);"
                                "border: 1px solid #000000;"
                                "border-radius: 15px;"

                                "}"
                                "QPushButton::hover {"

                                    "background-color: rgb(227, 227, 136);"
                                "}"
                                "QPushButton::pressed {"
                                 "border: 3px solid #000000;"
                                "}");
}

void MainWindow::on_pushButton_4_clicked()//2d
{
    d3_or_2d = true;
   ui->label_64->setText("Линии равных значений целевой функции");
    ui->groupBox_12->hide();
    ui->groupBox_9->show();
    ui->pushButton_4->setStyleSheet("QPushButton {"
                                    "background-color: rgb(249, 250, 150);"
                                "border: 1px solid #000000;"
                                "border-radius: 15px;"
                                    "font: bold;"
                                "}"
                                "QPushButton::hover {"

                                    "background-color: rgb(227, 227, 136);"
                                "}"
                                "QPushButton::pressed {"
                                 "border: 3px solid #000000;"
                                "}");
    ui->pushButton_5->setStyleSheet("QPushButton {"
                                    "background-color: rgb(249, 250, 150);"
                                "border: 1px solid #000000;"
                                "border-radius: 15px;"

                                "}"
                                "QPushButton::hover {"

                                    "background-color: rgb(227, 227, 136);"
                                "}"
                                "QPushButton::pressed {"
                                 "border: 3px solid #000000;"
                                "}");
}

bool MainWindow::on_lineEdit_22_textChanged(const QString &arg1)
{
    bool check;
    double G_W = arg1.toDouble(&check);
    if(!check){
        ui->lineEdit_22->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 215, 223);"
                                "border: 1px solid #000000;"
                                "}");
    }
    else{
        ui->lineEdit_22->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 255, 255);"
                                "border: 1px solid #000000;"
                                "}");
    }
    return check;
}

bool MainWindow::on_lineEdit_21_textChanged(const QString &arg1)
{
    bool check;
    double G_W = arg1.toDouble(&check);
    if(!check){
        ui->lineEdit_21->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 215, 223);"
                                "border: 1px solid #000000;"
                                "}");
    }
    else{
        ui->lineEdit_21->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 255, 255);"
                                "border: 1px solid #000000;"
                                "}");
    }
    return check;
}

bool MainWindow::on_lineEdit_23_textChanged(const QString &arg1)
{
    bool check;
    double G_W = arg1.toDouble(&check);
    if(!check){
        ui->lineEdit_23->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 215, 223);"
                                "border: 1px solid #000000;"
                                "}");
    }
    else{
        ui->lineEdit_23->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 255, 255);"
                                "border: 1px solid #000000;"
                                "}");
    }
    return check;
}

bool MainWindow::on_lineEdit_24_textChanged(const QString &arg1)
{
    bool check;
    double G_W = arg1.toDouble(&check);
    if(!check){
        ui->lineEdit_24->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 215, 223);"
                                "border: 1px solid #000000;"
                                "}");
    }
    else{
        ui->lineEdit_24->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 255, 255);"
                                "border: 1px solid #000000;"
                                "}");
    }
    return check;
}

bool MainWindow::on_lineEdit_26_textChanged(const QString &arg1)
{
    bool check;
    double G_W = arg1.toDouble(&check);
    if(!check){
        ui->lineEdit_26->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 215, 223);"
                                "border: 1px solid #000000;"
                                "}");
    }
    else{
        ui->lineEdit_26->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 255, 255);"
                                "border: 1px solid #000000;"
                                "}");
    }
    return check;
}

bool MainWindow::on_lineEdit_27_textChanged(const QString &arg1)
{
    bool check;
    double G_W = arg1.toDouble(&check);
    if(!check){
        ui->lineEdit_27->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 215, 223);"
                                "border: 1px solid #000000;"
                                "}");
    }
    else{
        ui->lineEdit_27->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 255, 255);"
                                "border: 1px solid #000000;"
                                "}");
    }
    return check;
}

bool MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    bool check;
    double G_W = arg1.toDouble(&check);
    if(!check){
        ui->lineEdit->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 215, 223);"
                                "border: 1px solid #000000;"
                                "}");
    }
    else{
        ui->lineEdit->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 255, 255);"
                                "border: 1px solid #000000;"
                                "}");
    }
    return check;
}

bool MainWindow::on_lineEdit_2_textChanged(const QString &arg1)
{
    bool check;
    double G_W = arg1.toDouble(&check);
    if(!check){
        ui->lineEdit_2->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 215, 223);"
                                "border: 1px solid #000000;"
                                "}");
    }
    else{
        ui->lineEdit_2->setStyleSheet("QLineEdit{"
                                "background-color: rgb(255, 255, 255);"
                                "border: 1px solid #000000;"
                                "}");
    }
    return check;
}

void MainWindow::on_pushButton_clicked()
{
    QString file = QFileDialog::getSaveFileName(this,"Сохранить файл","","*.png");
    if(file=="")
        return;

    if(d3_or_2d){
          QPixmap p = chartView->grab();
          p.save(file,"PNG");
    }
    else{
        QImage image = graph->renderToImage();
        image.save(file);
    }

}


void MainWindow::on_pushButton_3_clicked()
{
    QString file = QFileDialog::getSaveFileName(this,"Сохранить файл","","*.txt");
    if(file=="")
        return;
    QFile file1(file);
    if (!file1.open(QIODevice::WriteOnly))
    {
        return;
    }
    //QTextCodec::setCodecForLocale( QTextCodec::codecForName( "UTF-8" ) );
    QTextStream writeStream(&file1);
    writeStream.setCodec("UTF-8");
    writeStream << tr("Целевая функция:")<<"\n"<<ui->lineEdit_20->text()<<"\n\n"<<tr("Ограничения первого рода:");
    writeStream <<"\n"<<"Min T1: "<<ui->lineEdit_22->text()<<" "<<"Max T1 "<<ui->lineEdit_21->text()<<"\n";
    writeStream <<"Min T2: "<<ui->lineEdit_23->text()<<" "<<"Max T2 "<<ui->lineEdit_24->text()<<"\n";
    writeStream <<"\n"<<tr("Ограничения второго рода:\n")<<ui->lineEdit_25->text()<<"\n\n";
    writeStream <<tr("Точность нахождения решения: ")<<ui->lineEdit_26->text()<<"\n\n";
     writeStream <<tr("Метод поиска экстремума: ")<<ui->comboBox_2->currentText()<<"\n\n";
      writeStream <<tr("Результаты:")<<"\n\n";
      writeStream <<tr("Максимальный экстремум: ")<<"\n";
      writeStream <<tr("Целевая функция: ")<<ui->lineEdit_29->text()<<tr(" При Т1: ")<<ui->lineEdit_28->text()<<" T2: "<<ui->lineEdit_30->text()<<"\n\n";
      writeStream <<tr("Минимальный экстремум: ")<<"\n";
      writeStream <<tr("Целевая функция: ")<<ui->lineEdit_31->text()<<tr(" При Т1: ")<<ui->lineEdit_32->text()<<" T2: "<<ui->lineEdit_33->text()<<"\n\n";
      writeStream <<tr("Прибыль от реализации целевого компонента составляет: ")<<QString("%1").arg(ui->lineEdit_29->text().toDouble()*100, 0, 'f', 0)+" y.e.";
    file1.close();
}

void MainWindow::on_pushButton_6_clicked()
{
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setStyleSheet("QMessageBox {"
                          "background-color: rgb(250, 250, 210);"
                           "}"
                          "QMessageBox QPushButton {"
                           "background-color: rgb(249, 250, 150);"
                           "font:bold;"
                           "border: 1px solid #000000;"
                          "border-radius: 20px;"
                            "}"
                          "QMessageBox QPushButton:pressed {"
                           "border: 3px solid #000000;"
                            "}"
                          "QMessageBox QPushButton:hover {"
                           "background-color: rgb(227, 227, 136);"
                            "}");
     msgBox->setWindowTitle("Справка");
     msgBox->setText("Курсовой проект\nТема: Разработка программного комплекса для решения задачи оптимизации заданного объекта исследования");
     msgBox->setInformativeText("Разработанные методы поиска экстремума целевой функции:\n- Метод Сканирования,\n- Метод Бокса.");
     msgBox->setIcon(QMessageBox::Information);
     QPushButton *edit = msgBox->addButton("Закрыть", QMessageBox::ActionRole);
     edit->setMinimumWidth(110);
     edit->setMinimumHeight(40);
     msgBox->setDefaultButton(edit);
     if(msgBox->clickedButton()== edit){
         msgBox->close();
     }
     msgBox->exec();
}
