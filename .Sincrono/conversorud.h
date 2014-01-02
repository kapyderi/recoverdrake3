#ifndef CONVERSORUD_H
#define CONVERSORUD_H

#include <QDialog>
#include <QSqlDatabase>
#include "ayuda.h"
#include "calculadora.h"

namespace Ui {
class ConversorUD;
}

class ConversorUD : public QDialog
{
    Q_OBJECT
    
public:
    explicit ConversorUD(QWidget *parent = 0);
    ~ConversorUD();
    void Valor(QString valor, QString Logs);

protected:
    virtual bool eventFilter(QObject *, QEvent *);
    void closeEvent(QCloseEvent *event);

signals:
   void Cerrar();
    
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_lineEdit_textChanged(const QString &arg1);
    void on_comboBox_activated(const QString &arg1);
    void on_comboBox_2_activated(const QString &arg1);
    void on_lineEdit_returnPressed();
    void on_comboBox_3_activated(const QString &arg1);
    void Reprobar(QString Tipo);
    void Unidad(double Value,double Value1,double Cantidad,QString Nombre, QString Base);
    void Unidad1(double Value,double Value1,double Cantidad, QString Nombre, QString Base);
    void on_lineEdit_14_textChanged(const QString &arg1);
    void on_lineEdit_14_returnPressed();
    void on_lineEdit_5_textChanged(const QString &arg1);
    void on_lineEdit_19_returnPressed();
    void on_lineEdit_21_returnPressed();
    void on_lineEdit_6_returnPressed();
    void on_lineEdit_5_returnPressed();
    void on_lineEdit_21_textChanged(const QString &arg1);
    void on_lineEdit_6_textChanged(const QString &arg1);
    void Comprobar();
    void Comprobar1();
    void on_lineEdit_25_returnPressed();
    void on_lineEdit_22_returnPressed();
    void on_lineEdit_26_returnPressed();
    void on_lineEdit_23_returnPressed();
    void on_lineEdit_22_textChanged(const QString &arg1);
    void on_lineEdit_26_textChanged(const QString &arg1);
    void on_lineEdit_23_textChanged(const QString &arg1);
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_10_clicked();
    void on_pushButton_11_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_12_clicked();
    void on_lineEdit_19_textChanged(const QString &arg1);
    void on_lineEdit_25_textChanged(const QString &arg1);
    void on_pushButton_14_clicked();
    void on_lineEdit_27_textChanged(const QString &arg1);
    void on_lineEdit_27_returnPressed();
    void on_lineEdit_29_textChanged(const QString &arg1);
    void on_lineEdit_29_returnPressed();
    void on_lineEdit_31_textChanged(const QString &arg1);
    void on_lineEdit_31_returnPressed();
    void on_comboBox_5_currentIndexChanged(int index);
    void on_comboBox_7_currentIndexChanged(int index);
    void Tabla();
    void on_pushButton_13_clicked();
    void Comprobar2();
    void on_checkBox_clicked();
    void Comprobar3();
    void on_comboBox_10_currentIndexChanged(int index);
    void on_lineEdit_15_returnPressed();
    void on_lineEdit_15_textChanged(const QString &arg1);
    void on_lineEdit_18_returnPressed();
    void on_lineEdit_17_textChanged(const QString &arg1);
    void on_lineEdit_17_returnPressed();
    void on_pushButton_20_clicked();
    void on_lineEdit_47_returnPressed();
    void on_lineEdit_47_textChanged(const QString &arg1);
    void on_comboBox_26_activated(const QString &arg1);
    void on_comboBox_27_activated(const QString &arg1);
    void on_comboBox_25_activated(const QString &arg1);
    void on_pushButton_17_clicked();
    void on_lineEdit_40_returnPressed();

    void on_lineEdit_40_textChanged(const QString &arg1);

    void on_comboBox_19_activated(const QString &arg1);

    void on_comboBox_20_activated(const QString &arg1);

private:
    Ui::ConversorUD *ui;
    double Referencia(QString Value,QString Tipo);    
    QSqlDatabase db;
    QString cantidadDefecto;
    QString cantidad47;
    QString cantidad48;
    QString cantidad49;
    QString DatoTalla;
    QString DatoNegro;
    QString cantidad50;
    QString cantidad51;
    QString Stilo;
    Ayuda *ayuda;
    Calculadora *Calcula;
    int Decimales;
    int CierreTotal;
    QString Log;
    QString Idioma;
    QString CalculoDNI(int Numero);
    QString VerNum(QString Valor, int Tipo);
    int Periodo(QString Tipo);
    QString ValorCierre;
    int AmortizacionP;
    QString PonerMil(QString Valor);
    QString QuitarMil(QString Valor);
    QString CuotaCarencia;
    int CalculoDC(int Numero, int Digit);

};

#endif // CONVERSORUD_H