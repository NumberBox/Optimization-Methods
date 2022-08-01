#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include "QtSql/QSqlDatabase"
#include "QtSql/QSqlQuery"
#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QHeightMapSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtDataVisualization/QValue3DAxis>
#include <QtDataVisualization/QCustom3DItem>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QPercentBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLegend>
#include <QtCharts/QValueAxis>
#include <QtWidgets/QSlider>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QStringList list;
    QSqlDatabase db;
    QSqlTableModel *model;
    QStringList list_bd;

    QTimer *time1;
    int time_i = 0;
    bool d3_or_2d = true;

    QVector<double> t1;
    QVector<double> t2;
    QVector<double> c;

    QVector<double> t1_;
    QVector<double> t2_;
    QVector<double> c_;

    QVector<double> t1__;
    QVector<double> t2__;
    QVector<double> c__;

    QWidget *container;
    QtDataVisualization::Q3DSurface *graph = new QtDataVisualization::Q3DSurface();
    QtDataVisualization::QSurfaceDataProxy *m_sqrtSinProxy;
    QtDataVisualization::QSurface3DSeries *m_sqrtSinSeries;

    QtCharts::QChartView *chartView;
    QtCharts::QChart *chart_;
    QtCharts::QValueAxis *axisX;
    QtCharts::QValueAxis *axisY;
    QtCharts::QScatterSeries *seriesA = new QtCharts::QScatterSeries();
    QtCharts::QScatterSeries *seriesB = new QtCharts::QScatterSeries();
    QVector<double> vec_t1_2d;
    QVector<double> vec_t2_2d;
    QtDataVisualization::QCustom3DItem *m_sun;


private slots:

    void update_model();
    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_12_clicked();
     void slotEditRecord(QModelIndex index);
         void select_buttons();

         void on_comboBox_activated(const QString &arg1);

         void table();

         void on_pushButton_5_clicked();

         void on_pushButton_4_clicked();

         bool on_lineEdit_22_textChanged(const QString &arg1);

         bool on_lineEdit_21_textChanged(const QString &arg1);

         bool on_lineEdit_23_textChanged(const QString &arg1);

         bool on_lineEdit_24_textChanged(const QString &arg1);

         bool on_lineEdit_26_textChanged(const QString &arg1);

         bool on_lineEdit_27_textChanged(const QString &arg1);

         bool on_lineEdit_textChanged(const QString &arg1);

         bool on_lineEdit_2_textChanged(const QString &arg1);

         void on_pushButton_clicked();

         void box_max();
         void box_min();

         void scan_max();

         void on_pushButton_3_clicked();

         void on_pushButton_6_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
