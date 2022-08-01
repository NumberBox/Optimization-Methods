#include "dialog_question.h"
#include "ui_dialog_question.h"


dialog_question::dialog_question(QStringList list_h,QStringList list_present,QStringList list_bd,QSqlDatabase db,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialog_question)
{
    ui->setupUi(this);

    list_BD = list_bd;
    db2 = db;
        if(list_present.size()>0){
            id_delete = list_present[0].toInt();
            ui->lineEdit->setText(list_present[0]);
            ui->lineEdit_2->setText(list_present[1]);
            ui->textEdit->setText(list_present[2]);
            ui->pushButton_3->setEnabled(true);
        }
        else{
            db2.open();
            QSqlQuery query;
            query.exec("SELECT id FROM QUESTION ORDER BY ID DESC LIMIT 1");
            while (query.next()){
                ui->lineEdit->setText(QString::number(query.value(0).toInt()+1));
            }
            db2.close();
            ui->pushButton_3->setEnabled(false);
            ui->pushButton_2->setText("Добавить");
        }
}

dialog_question::~dialog_question()
{
    delete ui;
}

void dialog_question::on_pushButton_clicked()//отмена
{
    this->close();
}

void dialog_question::on_pushButton_2_clicked()//сохр //доб
{
    db2.open();
    QSqlQuery a_query;

        if(ui->pushButton_2->text()!="Добавить"){
            a_query.exec("DELETE FROM "+list_BD[0]+" WHERE "+list_BD[1]+" = "+QString::number(id_delete)+"");
        }

        QString str = " (";
        for (int i = 1;i<list_BD.size();i++) {
            if(i==list_BD.size()-1){
              str+=list_BD[i]+")";
            }
            else
                str+=list_BD[i]+", ";
        }
        str+="VALUES (";
        for (int i = 1;i<list_BD.size();i++) {
            if(i==list_BD.size()-1){
              str+="?)";
            }
            else
                str+="?, ";
        }

        a_query.prepare("INSERT INTO "+list_BD[0]+str);
            a_query.addBindValue(ui->lineEdit->text());//
              a_query.addBindValue(ui->lineEdit_2->text());//
            a_query.addBindValue(ui->textEdit->toPlainText());
    a_query.exec();

    db2.close();
    emit close_dialog();
    this->close();
}

void dialog_question::on_pushButton_3_clicked()//дел
{
    db2.open();
    QSqlQuery a_query;
    a_query.exec("DELETE FROM "+list_BD[0]+" WHERE "+list_BD[1]+" = "+QString::number(id_delete)+"");
    db2.close();
    emit close_dialog();
    this->close();
}
