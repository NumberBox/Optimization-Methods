#ifndef DIALOG_QUESTION_H
#define DIALOG_QUESTION_H
#include <QDialog>
#include "QtSql/QSqlDatabase"
#include "QtSql/QSqlQuery"


namespace Ui {
class dialog_question;
}

class dialog_question : public QDialog
{
    Q_OBJECT

public:
    explicit dialog_question(QStringList list_h, QStringList list_present,QStringList list_bd,QSqlDatabase db, QWidget *parent = nullptr);
    ~dialog_question();

    QStringList list_BD;
    QSqlDatabase db2;
    int id_delete;

signals:
    void close_dialog();
private slots:
    void on_pushButton_clicked();


    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::dialog_question *ui;
};

#endif // DIALOG_QUESTION_H
