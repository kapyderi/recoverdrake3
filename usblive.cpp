#include "usblive.h"
#include "ui_usblive.h"
#include "drakesistema.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QProcess>
#include <QTest>
#include <QProgressBar>
#include "drakeprocesos.h"
#include <QFile>
#include <QChar>
#include <QSqlQuery>
#include <QProgressDialog>
#include <QtConcurrentRun>

UsbLive::UsbLive(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UsbLive)
{
    ui->setupUi(this);
    db=QSqlDatabase::database("PRINCIPAL");
    drakeSistema drake;
    user = drake.getUser();    
    Duplicado = 0;
    Control = 0;
    TamanoTotal = 0;
    mib=0;
    Ctrl = 0;
    Marcado = 0;
    Stilo = "B";
    Deshabilitar = "No";
    ui->progressBar->hide();
    ui->label_6->hide();
    connect(ui->tableWidget, SIGNAL(cellPressed(int,int)), this, SLOT(tableClicked(int,int)));
    connect(ui->radioButton_12,SIGNAL(clicked()),this,SLOT(Disabled()));
    connect(ui->radioButton_4,SIGNAL(clicked()),this,SLOT(Disabled()));
    QSqlQuery queryDefecto(db);
    queryDefecto.exec("SELECT Defecto FROM Miscelanea WHERE id=2");
    queryDefecto.first();
    if (queryDefecto.isSelect())
        cantidadDefecto=queryDefecto.value(0).toString();
    if (cantidadDefecto == "0")
    {
        QSqlQuery query47(db);
        query47.exec("SELECT Fuente FROM Font WHERE id=2");
        query47.first();
        if (query47.isSelect())
            cantidad47=query47.value(0).toString();
        QSqlQuery query48(db);
        query48.exec("SELECT Estilo FROM Font WHERE id=2");
        query48.first();
        if (query48.isSelect())
            cantidad48=query48.value(0).toString();
        QSqlQuery query49(db);
        query49.exec("SELECT Talla FROM Font WHERE id=2");
        query49.first();
        if (query49.isSelect())
            cantidad49=query49.value(0).toString();
        if (cantidad48 == "Regular")
        {
            DatoTalla = "normal";
            DatoNegro = "normal";
        }
        if (cantidad48 == "Italic")
        {
            DatoTalla = "italic";
            DatoNegro = "normal";
        }
        if (cantidad48 == "Bold Italic")
        {
            DatoTalla = "italic";
            DatoNegro = "bold";
        }
        if (cantidad48 == "Bold")
        {
            DatoTalla = "normal";
            DatoNegro = "bold";
        }
        QSqlQuery query50(db);
        query50.exec("SELECT ColorTexto FROM Color WHERE id=2");
        query50.first();
        if (query50.isSelect())
            cantidad50=query50.value(0).toString();
        QSqlQuery query51(db);
        query51.exec("SELECT ColorFondo FROM Color WHERE id=2");
        query51.first();
        if (query51.isSelect())
            cantidad51=query51.value(0).toString();
        Stilo = "A";
    }
    this->Arranque();
    this->installEventFilter(this);
    ui->label_55->setText("");
    ui->label_57->setText("");
    ui->label_59->setText("");
    ui->label_61->setText("");
    ui->label_63->setText("");
    ui->label_65->setText("");
    ui->label_67->setText("");
    ui->label_69->setText("");
    ui->label_71->setText("");
    ui->label_73->setText("");
    ui->label_74->setText("");
    ui->label_84->setText("");
    ui->label_87->setText("");
    ui->label_78->setText("");
    ui->label_76->setText("");
    ui->label_18->setText("");
    connect(ui->radioButton_20,SIGNAL(clicked()),this,SLOT(Comprobar()));
    connect(ui->radioButton_19,SIGNAL(clicked()),this,SLOT(Comprobar()));
    connect(ui->radioButton_21,SIGNAL(clicked()),this,SLOT(Comprobar()));
    ui->radioButton_20->setChecked(true);
    ui->stackedWidget->setCurrentIndex(0);
    this->EstadoLinea();
    TipoGrub = "NADA";
    ui->radioButton_12->setEnabled(true);
    QFuture<void> f1 = QtConcurrent::run(this, &UsbLive::Revisar);
    Q_UNUSED(f1);
}

void UsbLive::Revisar()
{
    drakeSistema drake;
    QString Valor20 = "find /run/media -maxdepth 1";
    QString valor = drake.getStart(Valor20);
    QStringList Comparar = valor.split("\n");
    QString value = Comparar.value(0);
    if (value == "")
        ruta = "/media/";
    else
        ruta = "/run/media/"+user+"/";
    QString GrubI1 = getPack("grub-efi");
    QString GrubI2 = getPack("grub");
    QString GrubI3 = getPack("grub2");
    if (GrubI1 != "")
    {
        ui->radioButton_12->setChecked(false);
        ui->radioButton_12->setEnabled(false);
        ui->radioButton_4->setChecked(true);
        ui->pushButton_35->show();
        GrubInfo = "GRUB2";
        Deshabilitar = "Si";
    }
    else if (GrubI2 != "")
    {
        ui->radioButton_12->setChecked(true);
        ui->radioButton_4->setChecked(false);
        ui->pushButton_35->hide();
        GrubInfo = "GRUB";
    }
    else if (GrubI3 != "")
    {
        ui->radioButton_12->setChecked(false);
        ui->radioButton_4->setChecked(true);
        ui->pushButton_35->hide();
        GrubInfo = "GRUB2";
    }
}

void UsbLive::Comprobar()
{
    if (ui->radioButton_20->isChecked())
        ui->stackedWidget->setCurrentIndex(0);
    else if (ui->radioButton_19->isChecked())
    {
        ui->stackedWidget->setCurrentIndex(1);
        ui->groupBox_92->hide();
        ui->groupBox_15->show();
        ui->tableWidget_2->showColumn(4);
        ui->radioButton_15->setEnabled(false);
        ui->radioButton_16->setEnabled(false);
        this->Rastreo();
    }
    else if (ui->radioButton_21->isChecked())
    {
        ui->stackedWidget->setCurrentIndex(1);
        ui->groupBox_92->show();
        ui->groupBox_15->hide();
        ui->tableWidget_2->hideColumn(4);
        ui->radioButton_15->setEnabled(true);
        ui->radioButton_16->setEnabled(true);
        this->Rastreo();
    }
}

void UsbLive::Rastreo()
{
    int Borrado, x;
    Borrado = ui->tableWidget_2->rowCount();
    for(x=0;x<Borrado;x++)
    {
         ui->tableWidget_2->removeRow(x);
         x=x-1;
         Borrado=Borrado-1;
    }
    int iFilas;
    drakeSistema drake;
    QString Valor20 = "find /run/media -maxdepth 1";
    QString valor = drake.getStart(Valor20);
    QStringList Comparar = valor.split("\n");
    QString value = Comparar.value(0);
    if (value == "")
        ruta = "/media/";
    else
        ruta = "/run/media/"+user+"/";
    QString Valor21 = "find "+ruta+"DRAKLIVE/ -maxdepth 0";
    QString valor1 = drake.getStart(Valor21);
    if (valor1 == "")
    {
        QMessageBox m;
        if (Stilo == "A")
            m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
        m.setText(tr("No has insertado o montado ningun pendrive creado con RecoverDrake. Por favor inserta o monta DRAKLIVE para poder continuar."));
        m.exec();
        ui->stackedWidget->setCurrentIndex(0);
        ui->radioButton_20->setChecked(true);
        return;
    }
    else
    {
        QString Grub = "find "+ruta+"DRAKLIVE/boot -maxdepth 1";
        QString Gruby = drake.getStart(Grub);
        if (Gruby.contains("grub2"))
            ui->radioButton_16->setChecked(true);
        else
            ui->radioButton_15->setChecked(true);
        QString MediaInfo = getMediaInfo(""+ruta+"DRAKLIVE");
        QStringList ListaMedia = MediaInfo.split(" ");
        QString Resto = ListaMedia.value(0);
        Resto.chop(1);
        ui->label_18->setText(Resto);
        QString Valor = "find "+ruta+"DRAKLIVE/iso -maxdepth 1";
        QString valor = drake.getStart(Valor);
        QStringList Comparar = valor.split("\n");
        for(int i=1;i<Comparar.count()-1;i++)
        {
            QString Valor1, Valor2, Valor3, Valor4;
            QString Value = Comparar.value(i);
            QStringList Parcial = Value.split("/");
            Value = Parcial.value(Parcial.count()-1);
            if (Value.contains("Mandriva"))
            {
                Valor1 = "Mandriva";
                if (Value.contains("32"))
                    Valor2 = "32bit";
                else
                    Valor2 = "64bit";
                QStringList Arch = Value.split("_");
                Valor3 = Arch.value(1);
            }
            if (Value.contains("PCLinuxOS"))
            {
                Valor1 = "PCLinuxOS";
                if (Value.contains("32"))
                    Valor2 = "32bit";
                else
                    Valor2 = "64bit";
                QStringList Arch = Value.split("_");
                Valor3 = Arch.value(1);
            }
            if (Value.contains("Fedora"))
            {
                Valor1 = "Fedora";
                if (Value.contains("32"))
                    Valor2 = "32bit";
                else
                    Valor2 = "64bit";
                QStringList Arch = Value.split("_");
                Valor3 = Arch.value(1);
            }
            if (Value.contains("Ubuntu"))
            {
                Valor1 = "Ubuntu";
                if (Value.contains("32"))
                    Valor2 = "32bit";
                else
                    Valor2 = "64bit";
                QStringList Arch = Value.split("_");
                Valor3 = Arch.value(1);
            }
            if (Value.contains("ArchLinux"))
            {
                Valor1 = "ArchLinux";
                Valor2 = "Dual";
                QStringList Arch = Value.split("_");
                Valor3 = Arch.value(1);
            }
            if (Value.contains("CentOS"))
            {
                Valor1 = "CentOS";
                if (Value.contains("32"))
                    Valor2 = "32bit";
                else
                    Valor2 = "64bit";
                QStringList Arch = Value.split("_");
                Valor3 = Arch.value(1);
            }
            if (Value.contains("Debian"))
            {
                Valor1 = "Debian";
                if (Value.contains("32"))
                    Valor2 = "32bit";
                else
                    Valor2 = "64bit";
                QStringList Arch = Value.split("_");
                Valor3 = Arch.value(1);
            }
            if (Value.contains("Knoppix"))
            {
                Valor1 = "Knoppix";
                if (Value.contains("32"))
                    Valor2 = "32bit";
                else
                    Valor2 = "64bit";
                QStringList Arch = Value.split("_");
                Valor3 = Arch.value(1);
            }
            if (Value.contains("OpenSUSE"))
            {
                Valor1 = "OpenSUSE";
                if (Value.contains("32"))
                    Valor2 = "32bit";
                else
                    Valor2 = "64bit";
                QStringList Arch = Value.split("_");
                Valor3 = Arch.value(1);
            }
            if (Value.contains("LinuxMint"))
            {
                Valor1 = "LinuxMint";
                if (Value.contains("32"))
                    Valor2 = "32bit";
                else
                    Valor2 = "64bit";
                QStringList Arch = Value.split("_");
                Valor3 = Arch.value(1);
            }
            if (Value.contains("BootRepair"))
            {
                Valor1 = "BootRepair";
                if (Value.contains("32"))
                    Valor2 = "32bit";
                else
                    Valor2 = "64bit";
                QStringList Arch = Value.split("_");
                Valor3 = Arch.value(1);
            }
            if (Value.contains("Clonezilla"))
            {
                Valor1 = "Clonezilla";
                if (Value.contains("32"))
                    Valor2 = "32bit";
                else
                    Valor2 = "64bit";
                QStringList Arch = Value.split("_");
                Valor3 = Arch.value(1);
            }
            if (Value.contains("GParted"))
            {
                Valor1 = "GParted";
                if (Value.contains("32"))
                    Valor2 = "32bit";
                else
                    Valor2 = "64bit";
                QStringList Arch = Value.split("_");
                Valor3 = Arch.value(1);
            }
            if (Value.contains("SystemRescue"))
            {
                Valor1 = "SystemRescue";
                if (Value.contains("32"))
                    Valor2 = "32bit";
                else
                    Valor2 = "64bit";
                QStringList Arch = Value.split("_");
                Valor3 = Arch.value(1);
            }
            if (Value.contains("OpenMandriva"))
            {
                Valor1 = "OpenMandriva";
                if (Value.contains("32"))
                    Valor2 = "32bit";
                else
                    Valor2 = "64bit";
                QStringList Arch = Value.split("_");
                Valor3 = Arch.value(1);
            }
            Valor4 = Value;
            QTableWidgetItem *item1, *item2, *item3, *item4, *item5;
            item1=new QTableWidgetItem;
            item2=new QTableWidgetItem;
            item3=new QTableWidgetItem;
            item4=new QTableWidgetItem;
            item5=new QTableWidgetItem;
            item1->setText(""+Valor1+"");
            item2->setText(""+Valor2+"");
            item3->setText(""+Valor3+"");
            item4->setText(""+Valor4+"");
            item5->setText("No");
            iFilas=ui->tableWidget_2->rowCount();
            ui->tableWidget_2->insertRow(iFilas);
            ui->tableWidget_2->setItem(iFilas,0,item1);
            ui->tableWidget_2->setItem(iFilas,1,item2);
            ui->tableWidget_2->setItem(iFilas,2,item3);
            ui->tableWidget_2->setItem(iFilas,3,item4);
            ui->tableWidget_2->setItem(iFilas,4,item5);
            ui->tableWidget_2->item(iFilas,4)->setIcon(QIcon(":/Imagenes/bad.png"));
        }
        ui->tableWidget_2->resizeColumnsToContents();
        ui->tableWidget_2->resizeRowsToContents();
    }
}


QString UsbLive::getMediaInfo(QString Dato)
{
    QProcess *procesoMount, *procesoGrep;
    QStringList argumentosGrep;
    QByteArray IMedia;
    procesoMount=new QProcess(this);
    procesoGrep=new QProcess(this);
    argumentosGrep << Dato;
    procesoMount->setStandardOutputProcess(procesoGrep);
    procesoMount->start("mount");
    procesoGrep->start("grep", argumentosGrep);
    if (! procesoGrep->waitForStarted())
        return QString("");
    procesoMount->waitForFinished();
    procesoGrep->waitForFinished();
    IMedia = procesoGrep->readAllStandardOutput();
    delete procesoMount;
    delete procesoGrep;
    QString res = QString(IMedia);
    res.chop(1);
    return res;
}

void UsbLive::EstadoLinea()
{
    QString Comprobacion;
    QSqlQuery Variable(db);
    Variable.exec("SELECT Version FROM Distro WHERE NomDistro LIKE 'Mandriva'");
    while(Variable.next())
    {
        Comprobacion = Variable.value(0).toString();
        if (ui->comboBox_11->findText(Comprobacion) == -1)
            ui->comboBox_11->addItem(Variable.value(0).toString());
    }
    ui->comboBox_11->setCurrentIndex(0);
    this->TipoArch("Mandriva");
    QSqlQuery Variable1(db);
    Variable1.exec("SELECT Version FROM Distro WHERE NomDistro LIKE 'PCLinuxOS'");
    while(Variable1.next())
    {
        Comprobacion = Variable1.value(0).toString();
        if (ui->comboBox_12->findText(Comprobacion) == -1)
            ui->comboBox_12->addItem(Variable1.value(0).toString());
    }
    ui->comboBox_12->setCurrentIndex(0);
    this->TipoArch("PCLinuxOS");
    QSqlQuery Variable2(db);
    Variable2.exec("SELECT Version FROM Distro WHERE NomDistro LIKE 'Fedora'");
    while(Variable2.next())
    {
        Comprobacion = Variable2.value(0).toString();
        if (ui->comboBox_13->findText(Comprobacion)  == -1)
            ui->comboBox_13->addItem(Variable2.value(0).toString());
    }
    ui->comboBox_13->setCurrentIndex(0);
    this->TipoArch("Fedora");
    QSqlQuery Variable3(db);
    Variable3.exec("SELECT Version FROM Distro WHERE NomDistro LIKE 'Ubuntu'");
    while(Variable3.next())
    {
        Comprobacion = Variable3.value(0).toString();
        if (ui->comboBox_14->findText(Comprobacion)  == -1)
            ui->comboBox_14->addItem(Variable3.value(0).toString());
    }
    ui->comboBox_14->setCurrentIndex(0);
    this->TipoArch("Ubuntu");
    QSqlQuery Variable4(db);
    Variable4.exec("SELECT Version FROM Distro WHERE NomDistro LIKE 'ArchLinux'");
    while(Variable4.next())
    {
        Comprobacion = Variable4.value(0).toString();
        if (ui->comboBox_15->findText(Comprobacion)  == -1)
            ui->comboBox_15->addItem(Variable4.value(0).toString());
    }
    ui->comboBox_15->setCurrentIndex(0);
    this->TipoArch("ArchLinux");
    QSqlQuery Variable5(db);
    Variable5.exec("SELECT Version FROM Distro WHERE NomDistro LIKE 'CentOS'");
    while(Variable5.next())
    {
        Comprobacion = Variable5.value(0).toString();
        if (ui->comboBox_16->findText(Comprobacion)  == -1)
            ui->comboBox_16->addItem(Variable5.value(0).toString());
    }
    ui->comboBox_16->setCurrentIndex(0);
    this->TipoArch("CentOS");
    QSqlQuery Variable6(db);
    Variable6.exec("SELECT Version FROM Distro WHERE NomDistro LIKE 'Debian'");
    while(Variable6.next())
    {
        Comprobacion = Variable6.value(0).toString();
        if (ui->comboBox_17->findText(Comprobacion)  == -1)
            ui->comboBox_17->addItem(Variable6.value(0).toString());
    }
    ui->comboBox_17->setCurrentIndex(0);
    this->TipoArch("Debian");
    QSqlQuery Variable7(db);
    Variable7.exec("SELECT Version FROM Distro WHERE NomDistro LIKE 'KNOPPIX'");
    while(Variable7.next())
    {
        Comprobacion = Variable7.value(0).toString();
        if (ui->comboBox_18->findText(Comprobacion)  == -1)
            ui->comboBox_18->addItem(Variable7.value(0).toString());
    }
    ui->comboBox_18->setCurrentIndex(0);
    this->TipoArch("KNOPPIX");
    QSqlQuery Variable8(db);
    Variable8.exec("SELECT Version FROM Distro WHERE NomDistro LIKE 'OpenSUSE'");
    while(Variable8.next())
    {
        Comprobacion = Variable8.value(0).toString();
        if (ui->comboBox_19->findText(Comprobacion)  == -1)
            ui->comboBox_19->addItem(Variable8.value(0).toString());
    }
    ui->comboBox_19->setCurrentIndex(0);
    this->TipoArch("OpenSUSE");
    QSqlQuery Variable9(db);
    Variable9.exec("SELECT Version FROM Distro WHERE NomDistro LIKE 'LinuxMint'");
    while(Variable9.next())
    {
        Comprobacion = Variable9.value(0).toString();
        if (ui->comboBox_20->findText(Comprobacion)  == -1)
            ui->comboBox_20->addItem(Variable9.value(0).toString());
    }
    ui->comboBox_20->setCurrentIndex(0);
    this->TipoArch("LinuxMint");
    QSqlQuery Variable10(db);
    Variable10.exec("SELECT Version FROM Distro WHERE NomDistro LIKE 'Boot Repair Disk'");
    while(Variable10.next())
    {
        Comprobacion = Variable10.value(0).toString();
        if (ui->comboBox_28->findText(Comprobacion)  == -1)
            ui->comboBox_28->addItem(Variable10.value(0).toString());
    }
    ui->comboBox_28->setCurrentIndex(0);
    this->TipoArch("Boot Repair Disk");
    QSqlQuery Variable11(db);
    Variable11.exec("SELECT Version FROM Distro WHERE NomDistro LIKE 'Clonezilla'");
    while(Variable11.next())
    {
        Comprobacion = Variable11.value(0).toString();
        if (ui->comboBox_30->findText(Comprobacion)  == -1)
            ui->comboBox_30->addItem(Variable11.value(0).toString());
    }
    ui->comboBox_30->setCurrentIndex(0);
    this->TipoArch("Clonezilla");
    QSqlQuery Variable12(db);
    Variable12.exec("SELECT Version FROM Distro WHERE NomDistro LIKE 'GParted'");
    while(Variable12.next())
    {
        Comprobacion = Variable12.value(0).toString();
        if (ui->comboBox_32->findText(Comprobacion)  == -1)
            ui->comboBox_32->addItem(Variable12.value(0).toString());
    }
    ui->comboBox_32->setCurrentIndex(0);
    this->TipoArch("GParted");
    QSqlQuery Variable13(db);
    Variable13.exec("SELECT Version FROM Distro WHERE NomDistro LIKE 'System Rescue CD'");
    while(Variable13.next())
    {
        Comprobacion = Variable13.value(0).toString();
        if (ui->comboBox_36->findText(Comprobacion)  == -1)
            ui->comboBox_36->addItem(Variable13.value(0).toString());
    }
    ui->comboBox_36->setCurrentIndex(0);
    this->TipoArch("System Rescue CD");
    QSqlQuery Variable14(db);
    Variable14.exec("SELECT Version FROM Distro WHERE NomDistro LIKE 'OpenMandriva'");
    while(Variable14.next())
    {
        Comprobacion = Variable14.value(0).toString();
        if (ui->comboBox_22->findText(Comprobacion)  == -1)
            ui->comboBox_22->addItem(Variable14.value(0).toString());
    }
    ui->comboBox_22->setCurrentIndex(0);
    this->TipoArch("OpenMandriva");
}

void UsbLive::TipoArch(QString Valor)
{
    QString Validacion;
    if (Valor == "Mandriva")
    {
        ui->comboBox->clear();
        QSqlQuery Variable(db);
        Variable.exec("SELECT Arquitectura FROM Distro WHERE NomDistro LIKE 'Mandriva' AND Version LIKE '"+ui->comboBox_11->currentText()+"'");
        while(Variable.next())
        {
            Validacion = Variable.value(0).toString();
            if (ui->comboBox->findText(Validacion) == -1)
                ui->comboBox->addItem(Variable.value(0).toString());
        }
        ui->comboBox->setCurrentIndex(0);
    }
    if (Valor == "PCLinuxOS")
    {
        ui->comboBox_2->clear();
        QSqlQuery Variable1(db);
        Variable1.exec("SELECT Arquitectura FROM Distro WHERE NomDistro LIKE 'PCLinuxOS' AND Version LIKE '"+ui->comboBox_12->currentText()+"'");
        while(Variable1.next())
        {
            Validacion = Variable1.value(0).toString();
            if (ui->comboBox_2->findText(Validacion) == -1)
                ui->comboBox_2->addItem(Variable1.value(0).toString());
        }
        ui->comboBox_2->setCurrentIndex(0);
    }
    if (Valor == "Fedora")
    {
        ui->comboBox_3->clear();
        QSqlQuery Variable2(db);
        Variable2.exec("SELECT Arquitectura FROM Distro WHERE NomDistro LIKE 'Fedora' AND Version LIKE '"+ui->comboBox_13->currentText()+"'");
        while(Variable2.next())
        {
            Validacion = Variable2.value(0).toString();
            if (ui->comboBox_3->findText(Validacion) == -1)
                ui->comboBox_3->addItem(Variable2.value(0).toString());
        }
        ui->comboBox_3->setCurrentIndex(0);
    }
    if (Valor == "Ubuntu")
    {
        ui->comboBox_4->clear();
        QSqlQuery Variable3(db);
        Variable3.exec("SELECT Arquitectura FROM Distro WHERE NomDistro LIKE 'Ubuntu' AND Version LIKE '"+ui->comboBox_14->currentText()+"'");
        while(Variable3.next())
        {
            Validacion = Variable3.value(0).toString();
            if (ui->comboBox_4->findText(Validacion) == -1)
                ui->comboBox_4->addItem(Variable3.value(0).toString());
        }
        ui->comboBox_4->setCurrentIndex(0);
    }
    if (Valor == "ArchLinux")
    {
        ui->comboBox_5->clear();
        QSqlQuery Variable4(db);
        Variable4.exec("SELECT Arquitectura FROM Distro WHERE NomDistro LIKE 'ArchLinux' AND Version LIKE '"+ui->comboBox_15->currentText()+"'");
        while(Variable4.next())
        {
            Validacion = Variable4.value(0).toString();
            if (ui->comboBox_5->findText(Validacion) == -1)
                ui->comboBox_5->addItem(Variable4.value(0).toString());
        }
        ui->comboBox_5->setCurrentIndex(0);
    }
    if (Valor == "CentOS")
    {
        ui->comboBox_6->clear();
        QSqlQuery Variable5(db);
        Variable5.exec("SELECT Arquitectura FROM Distro WHERE NomDistro LIKE 'CentOS' AND Version LIKE '"+ui->comboBox_16->currentText()+"'");
        while(Variable5.next())
        {
            Validacion = Variable5.value(0).toString();
            if (ui->comboBox_6->findText(Validacion) == -1)
                ui->comboBox_6->addItem(Variable5.value(0).toString());
        }
        ui->comboBox_6->setCurrentIndex(0);
    }
    if (Valor == "Debian")
    {
        ui->comboBox_7->clear();
        QSqlQuery Variable6(db);
        Variable6.exec("SELECT Arquitectura FROM Distro WHERE NomDistro LIKE 'Debian' AND Version LIKE '"+ui->comboBox_17->currentText()+"'");
        while(Variable6.next())
        {
            Validacion = Variable6.value(0).toString();
            if (ui->comboBox_7->findText(Validacion) == -1)
                ui->comboBox_7->addItem(Variable6.value(0).toString());
        }
        ui->comboBox_7->setCurrentIndex(0);
    }
    if (Valor == "KNOPPIX")
    {
        ui->comboBox_8->clear();
        QSqlQuery Variable7(db);
        Variable7.exec("SELECT Arquitectura FROM Distro WHERE NomDistro LIKE 'KNOPPIX' AND Version LIKE '"+ui->comboBox_18->currentText()+"'");
        while(Variable7.next())
        {
            Validacion = Variable7.value(0).toString();
            if (ui->comboBox_8->findText(Validacion) == -1)
                ui->comboBox_8->addItem(Variable7.value(0).toString());
        }
        ui->comboBox_8->setCurrentIndex(0);
    }
    if (Valor == "OpenSUSE")
    {
        ui->comboBox_9->clear();
        QSqlQuery Variable8(db);
        Variable8.exec("SELECT Arquitectura FROM Distro WHERE NomDistro LIKE 'OpenSUSE' AND Version LIKE '"+ui->comboBox_19->currentText()+"'");
        while(Variable8.next())
        {
            Validacion = Variable8.value(0).toString();
            if (ui->comboBox_9->findText(Validacion) == -1)
                ui->comboBox_9->addItem(Variable8.value(0).toString());
        }
        ui->comboBox_9->setCurrentIndex(0);
    }
    if (Valor == "LinuxMint")
    {
        ui->comboBox_10->clear();
        QSqlQuery Variable9(db);
        Variable9.exec("SELECT Arquitectura FROM Distro WHERE NomDistro LIKE 'LinuxMint' AND Version LIKE '"+ui->comboBox_20->currentText()+"'");
        while(Variable9.next())
        {
            Validacion = Variable9.value(0).toString();
            if (ui->comboBox_10->findText(Validacion) == -1)
                ui->comboBox_10->addItem(Variable9.value(0).toString());
        }
        ui->comboBox_10->setCurrentIndex(0);
    }    
    if (Valor == "Boot Repair Disk")
    {
        ui->comboBox_27->clear();
        QSqlQuery Variable10(db);
        Variable10.exec("SELECT Arquitectura FROM Distro WHERE NomDistro LIKE 'Boot Repair Disk' AND Version LIKE '"+ui->comboBox_28->currentText()+"'");
        while(Variable10.next())
        {
            Validacion = Variable10.value(0).toString();
            if (ui->comboBox_27->findText(Validacion) == -1)
                ui->comboBox_27->addItem(Variable10.value(0).toString());
        }
        ui->comboBox_27->setCurrentIndex(0);
    }
    if (Valor == "Clonezilla")
    {
        ui->comboBox_29->clear();
        QSqlQuery Variable11(db);
        Variable11.exec("SELECT Arquitectura FROM Distro WHERE NomDistro LIKE 'Clonezilla' AND Version LIKE '"+ui->comboBox_30->currentText()+"'");
        while(Variable11.next())
        {
            Validacion = Variable11.value(0).toString();
            if (ui->comboBox_29->findText(Validacion) == -1)
                ui->comboBox_29->addItem(Variable11.value(0).toString());
        }
        ui->comboBox_29->setCurrentIndex(0);
    }
    if (Valor == "GParted")
    {
        ui->comboBox_31->clear();
        QSqlQuery Variable12(db);
        Variable12.exec("SELECT Arquitectura FROM Distro WHERE NomDistro LIKE 'GParted' AND Version LIKE '"+ui->comboBox_32->currentText()+"'");
        while(Variable12.next())
        {
            Validacion = Variable12.value(0).toString();
            if (ui->comboBox_31->findText(Validacion) == -1)
                ui->comboBox_31->addItem(Variable12.value(0).toString());
        }
        ui->comboBox_31->setCurrentIndex(0);
    }
    if (Valor == "System Rescue CD")
    {
        ui->comboBox_35->clear();
        QSqlQuery Variable13(db);
        Variable13.exec("SELECT Arquitectura FROM Distro WHERE NomDistro LIKE 'System Rescue CD' AND Version LIKE '"+ui->comboBox_36->currentText()+"'");
        while(Variable13.next())
        {
            Validacion = Variable13.value(0).toString();
            if (ui->comboBox_35->findText(Validacion) == -1)
                ui->comboBox_35->addItem(Variable13.value(0).toString());
        }
        ui->comboBox_35->setCurrentIndex(0);
    }
    if (Valor == "OpenMandriva")
    {
        ui->comboBox_21->clear();
        QSqlQuery Variable14(db);
        Variable14.exec("SELECT Arquitectura FROM Distro WHERE NomDistro LIKE 'OpenMandriva' AND Version LIKE '"+ui->comboBox_22->currentText()+"'");
        while(Variable14.next())
        {
            Validacion = Variable14.value(0).toString();
            if (ui->comboBox_21->findText(Validacion) == -1)
                ui->comboBox_21->addItem(Variable14.value(0).toString());
        }
        ui->comboBox_21->setCurrentIndex(0);
    }
}

void UsbLive::Disponibilidad(QString Valor)
{
    QString RastreoW, RastreoP, Visto, Verificado;
    if (Valor == "Mandriva")
    {
        ui->label_55->setText("");
        RastreoW = ui->comboBox_11->currentText();
        RastreoP = ui->comboBox->currentText();
        QSqlQuery LocalizarW(db);
        LocalizarW.exec("SELECT Direccion FROM Distro WHERE NomDistro LIKE '"+Valor+"' AND Version LIKE '"+RastreoW+"' AND Arquitectura LIKE '"+RastreoP+"'");
        LocalizarW.first();
        Verificado = LocalizarW.value(0).toString();
        Visto = getWget(Verificado);
        Visto = Reload();
        if (Visto.contains("404 Not Found", Qt::CaseInsensitive))
            ui->radioButton->setIcon(QIcon(QString::fromUtf8(":/Imagenes/bad.png")));
        else
        {
            ui->radioButton->setIcon(QIcon(QString::fromUtf8(":/Imagenes/good.png")));
            QStringList Buscador = Visto.split("\n");
            for(int a=0;a<Buscador.count();a++)
            {
                QString Rastreo = Buscador.value(a);
                if (Rastreo.contains("Content-Length:"))
                {
                    Rastreo.remove("Content-Length: ");
                    int Total = Rastreo.toInt();
                    Total = (Total/1024)/1024;
                    ui->label_55->setText(QString::number(Total)+"Mb");
                    break;
                }
            }
        }
    }
    if (Valor == "PCLinuxOS")
    {
        ui->label_57->setText("");
        RastreoW = ui->comboBox_12->currentText();
        RastreoP = ui->comboBox_2->currentText();
        QSqlQuery LocalizarW(db);
        LocalizarW.exec("SELECT Direccion FROM Distro WHERE NomDistro LIKE '"+Valor+"' AND Version LIKE '"+RastreoW+"' AND Arquitectura LIKE '"+RastreoP+"'");
        LocalizarW.first();
        Verificado = LocalizarW.value(0).toString();
        Visto = getWget(Verificado);
        Visto = Reload();
        if (Visto.contains("404 Not Found", Qt::CaseInsensitive))
            ui->radioButton_5->setIcon(QIcon(QString::fromUtf8(":/Imagenes/bad.png")));
        else
        {
            ui->radioButton_5->setIcon(QIcon(QString::fromUtf8(":/Imagenes/good.png")));            
            QStringList Buscador = Visto.split("\n");
            for(int a=0;a<Buscador.count();a++)
            {
                QString Rastreo = Buscador.value(a);
                if (Rastreo.contains("Content-Length:"))
                {
                    Rastreo = Rastreo.remove("Content-Length: ");
                    int Total = Rastreo.toInt();
                    Total = (Total/1024)/1024;
                    ui->label_57->setText(QString::number(Total)+"Mb");
                    break;
                }
            }
        }
    }
    if (Valor == "Fedora")
    {
        ui->label_59->setText("");
        RastreoW = ui->comboBox_13->currentText();
        RastreoP = ui->comboBox_3->currentText();
        QSqlQuery LocalizarW(db);
        LocalizarW.exec("SELECT Direccion FROM Distro WHERE NomDistro LIKE '"+Valor+"' AND Version LIKE '"+RastreoW+"' AND Arquitectura LIKE '"+RastreoP+"'");
        LocalizarW.first();
        Verificado = LocalizarW.value(0).toString();
        Visto = getWget(Verificado);
        Visto = Reload();
        if (Visto.contains("404 Not Found", Qt::CaseInsensitive))
            ui->radioButton_9->setIcon(QIcon(QString::fromUtf8(":/Imagenes/bad.png")));
        else
        {
            ui->radioButton_9->setIcon(QIcon(QString::fromUtf8(":/Imagenes/good.png")));
            QStringList Buscador = Visto.split("\n");
            for(int a=0;a<Buscador.count();a++)
            {
                QString Rastreo = Buscador.value(a);
                if (Rastreo.contains("Content-Length:"))
                {
                    Rastreo.remove("Content-Length: ");
                    int Total = Rastreo.toInt();
                    Total = (Total/1024)/1024;
                    ui->label_59->setText(QString::number(Total)+"Mb");
                    break;
                }
            }
        }
    }
    if (Valor == "Ubuntu")
    {
        ui->label_61->setText("");
        RastreoW = ui->comboBox_14->currentText();
        RastreoP = ui->comboBox_4->currentText();
        QSqlQuery LocalizarW(db);
        LocalizarW.exec("SELECT Direccion FROM Distro WHERE NomDistro LIKE '"+Valor+"' AND Version LIKE '"+RastreoW+"' AND Arquitectura LIKE '"+RastreoP+"'");
        LocalizarW.first();
        Verificado = LocalizarW.value(0).toString();
        Visto = getWget(Verificado);
        Visto = Reload();
        if (Visto.contains("404 Not Found", Qt::CaseInsensitive))
            ui->radioButton_13->setIcon(QIcon(QString::fromUtf8(":/Imagenes/bad.png")));
        else
        {
            ui->radioButton_13->setIcon(QIcon(QString::fromUtf8(":/Imagenes/good.png")));
            QStringList Buscador = Visto.split("\n");
            for(int a=0;a<Buscador.count();a++)
            {
                QString Rastreo = Buscador.value(a);
                if (Rastreo.contains("Content-Length:"))
                {
                    Rastreo.remove("Content-Length: ");
                    int Total = Rastreo.toInt();
                    Total = (Total/1024)/1024;
                    ui->label_61->setText(QString::number(Total)+"Mb");
                    break;
                }
            }
        }
    }
    if (Valor == "ArchLinux")
    {
        ui->label_63->setText("");
        RastreoW = ui->comboBox_15->currentText();
        RastreoP = ui->comboBox_5->currentText();
        QSqlQuery LocalizarW(db);
        LocalizarW.exec("SELECT Direccion FROM Distro WHERE NomDistro LIKE '"+Valor+"' AND Version LIKE '"+RastreoW+"' AND Arquitectura LIKE '"+RastreoP+"'");
        LocalizarW.first();
        Verificado = LocalizarW.value(0).toString();
        Visto = getWget(Verificado);
        Visto = Reload();
        if (Visto.contains("404 Not Found", Qt::CaseInsensitive))
            ui->radioButton_17->setIcon(QIcon(QString::fromUtf8(":/Imagenes/bad.png")));
        else
        {
            ui->radioButton_17->setIcon(QIcon(QString::fromUtf8(":/Imagenes/good.png")));
            QStringList Buscador = Visto.split("\n");
            for(int a=0;a<Buscador.count();a++)
            {
                QString Rastreo = Buscador.value(a);
                if (Rastreo.contains("Content-Length:"))
                {
                    Rastreo.remove("Content-Length: ");
                    int Total = Rastreo.toInt();
                    Total = (Total/1024)/1024;
                    ui->label_63->setText(QString::number(Total)+"Mb");
                    break;
                }
            }
        }
    }
    if (Valor == "CentOS")
    {
        ui->label_65->setText("");
        RastreoW = ui->comboBox_16->currentText();
        RastreoP = ui->comboBox_6->currentText();
        QSqlQuery LocalizarW(db);
        LocalizarW.exec("SELECT Direccion FROM Distro WHERE NomDistro LIKE '"+Valor+"' AND Version LIKE '"+RastreoW+"' AND Arquitectura LIKE '"+RastreoP+"'");
        LocalizarW.first();
        Verificado = LocalizarW.value(0).toString();
        Visto = getWget(Verificado);
        Visto = Reload();
        if (Visto.contains("404 Not Found", Qt::CaseInsensitive))
            ui->radioButton_39->setIcon(QIcon(QString::fromUtf8(":/Imagenes/bad.png")));
        else
        {
            ui->radioButton_39->setIcon(QIcon(QString::fromUtf8(":/Imagenes/good.png")));
            QStringList Buscador = Visto.split("\n");
            for(int a=0;a<Buscador.count();a++)
            {
                QString Rastreo = Buscador.value(a);
                if (Rastreo.contains("Content-Length:"))
                {
                    Rastreo.remove("Content-Length: ");
                    int Total = Rastreo.toInt();
                    Total = (Total/1024)/1024;
                    ui->label_65->setText(QString::number(Total)+"Mb");
                    break;
                }
            }
        }
    }
    if (Valor == "Debian")
    {
        ui->label_67->setText("");
        RastreoW = ui->comboBox_17->currentText();
        RastreoP = ui->comboBox_7->currentText();
        QSqlQuery LocalizarW(db);
        LocalizarW.exec("SELECT Direccion FROM Distro WHERE NomDistro LIKE '"+Valor+"' AND Version LIKE '"+RastreoW+"' AND Arquitectura LIKE '"+RastreoP+"'");
        LocalizarW.first();
        Verificado = LocalizarW.value(0).toString();
        Visto = getWget(Verificado);
        Visto = Reload();
        if (Visto.contains("404 Not Found", Qt::CaseInsensitive))
            ui->radioButton_43->setIcon(QIcon(QString::fromUtf8(":/Imagenes/bad.png")));
        else
        {
            ui->radioButton_43->setIcon(QIcon(QString::fromUtf8(":/Imagenes/good.png")));
            QStringList Buscador = Visto.split("\n");
            for(int a=0;a<Buscador.count();a++)
            {
                QString Rastreo = Buscador.value(a);
                if (Rastreo.contains("Content-Length:"))
                {
                    Rastreo.remove("Content-Length: ");
                    int Total = Rastreo.toInt();
                    Total = (Total/1024)/1024;
                    ui->label_67->setText(QString::number(Total)+"Mb");
                    break;
                }
            }
        }
    }
    if (Valor == "KNOPPIX")
    {
        ui->label_69->setText("");
        RastreoW = ui->comboBox_18->currentText();
        RastreoP = ui->comboBox_8->currentText();
        QSqlQuery LocalizarW(db);
        LocalizarW.exec("SELECT Direccion FROM Distro WHERE NomDistro LIKE '"+Valor+"' AND Version LIKE '"+RastreoW+"' AND Arquitectura LIKE '"+RastreoP+"'");
        LocalizarW.first();
        Verificado = LocalizarW.value(0).toString();
        Visto = getWget(Verificado);
        Visto = Reload();
        if (Visto.contains("404 Not Found", Qt::CaseInsensitive))
            ui->radioButton_55->setIcon(QIcon(QString::fromUtf8(":/Imagenes/bad.png")));
        else
        {
            ui->radioButton_55->setIcon(QIcon(QString::fromUtf8(":/Imagenes/good.png")));
            QStringList Buscador = Visto.split("\n");
            for(int a=0;a<Buscador.count();a++)
            {
                QString Rastreo = Buscador.value(a);
                if (Rastreo.contains("Content-Length:"))
                {
                    Rastreo.remove("Content-Length: ");
                    int Total = Rastreo.toInt();
                    Total = (Total/1024)/1024;
                    ui->label_69->setText(QString::number(Total)+"Mb");
                    break;
                }
            }
        }
    }
    if (Valor == "OpenSUSE")
    {
        ui->label_71->setText("");
        RastreoW = ui->comboBox_19->currentText();
        RastreoP = ui->comboBox_9->currentText();
        QSqlQuery LocalizarW(db);
        LocalizarW.exec("SELECT Direccion FROM Distro WHERE NomDistro LIKE '"+Valor+"' AND Version LIKE '"+RastreoW+"' AND Arquitectura LIKE '"+RastreoP+"'");
        LocalizarW.first();
        Verificado = LocalizarW.value(0).toString();
        Visto = getWget(Verificado);
        Visto = Reload();
        if (Visto.contains("404 Not Found", Qt::CaseInsensitive))
            ui->radioButton_51->setIcon(QIcon(QString::fromUtf8(":/Imagenes/bad.png")));
        else
        {
            ui->radioButton_51->setIcon(QIcon(QString::fromUtf8(":/Imagenes/good.png")));
            QStringList Buscador = Visto.split("\n");
            for(int a=0;a<Buscador.count();a++)
            {
                QString Rastreo = Buscador.value(a);
                if (Rastreo.contains("Content-Length:"))
                {
                    Rastreo.remove("Content-Length: ");
                    int Total = Rastreo.toInt();
                    Total = (Total/1024)/1024;
                    ui->label_71->setText(QString::number(Total)+"Mb");
                    break;
                }
            }
        }
    }
    if (Valor == "LinuxMint")
    {
        ui->label_73->setText("");
        RastreoW = ui->comboBox_20->currentText();
        RastreoP = ui->comboBox_10->currentText();
        QSqlQuery LocalizarW(db);
        LocalizarW.exec("SELECT Direccion FROM Distro WHERE NomDistro LIKE '"+Valor+"' AND Version LIKE '"+RastreoW+"' AND Arquitectura LIKE '"+RastreoP+"'");
        LocalizarW.first();
        Verificado = LocalizarW.value(0).toString();
        Visto = getWget(Verificado);
        Visto = Reload();
        if (Visto.contains("404 Not Found", Qt::CaseInsensitive))
            ui->radioButton_47->setIcon(QIcon(QString::fromUtf8(":/Imagenes/bad.png")));
        else
        {
            ui->radioButton_47->setIcon(QIcon(QString::fromUtf8(":/Imagenes/good.png")));
            QStringList Buscador = Visto.split("\n");
            for(int a=0;a<Buscador.count();a++)
            {
                QString Rastreo = Buscador.value(a);
                if (Rastreo.contains("Content-Length:"))
                {
                    Rastreo.remove("Content-Length: ");
                    int Total = Rastreo.toDouble();
                    Total = (Total/1024)/1024;
                    ui->label_73->setText(QString::number(Total)+"Mb");
                    break;
                }
            }
        }
    }
    if (Valor == "Boot Repair Disk")
    {
        ui->label_74->setText("");
        RastreoW = ui->comboBox_28->currentText();
        RastreoP = ui->comboBox_27->currentText();
        QSqlQuery LocalizarW(db);
        LocalizarW.exec("SELECT Direccion FROM Distro WHERE NomDistro LIKE '"+Valor+"' AND Version LIKE '"+RastreoW+"' AND Arquitectura LIKE '"+RastreoP+"'");
        LocalizarW.first();
        Verificado = LocalizarW.value(0).toString();
        Visto = getWget(Verificado);
        Visto = Reload();
        if (Visto.contains("404 Not Found", Qt::CaseInsensitive))
            ui->radioButton_23->setIcon(QIcon(QString::fromUtf8(":/Imagenes/bad.png")));
        else
        {
            ui->radioButton_23->setIcon(QIcon(QString::fromUtf8(":/Imagenes/good.png")));
            QStringList Buscador = Visto.split("\n");
            for(int a=0;a<Buscador.count();a++)
            {
                QString Rastreo = Buscador.value(a);
                if (Rastreo.contains("Content-Length:"))
                {
                    Rastreo.remove("Content-Length: ");
                    int Total = Rastreo.toDouble();
                    Total = (Total/1024)/1024;
                    ui->label_74->setText(QString::number(Total)+"Mb");
                    break;
                }
            }
        }
    }
    if (Valor == "Clonezilla")
    {
        ui->label_84->setText("");
        RastreoW = ui->comboBox_30->currentText();
        RastreoP = ui->comboBox_29->currentText();
        QSqlQuery LocalizarW(db);
        LocalizarW.exec("SELECT Direccion FROM Distro WHERE NomDistro LIKE '"+Valor+"' AND Version LIKE '"+RastreoW+"' AND Arquitectura LIKE '"+RastreoP+"'");
        LocalizarW.first();
        Verificado = LocalizarW.value(0).toString();
        Visto = getWget(Verificado);
        Visto = Reload();
        if (Visto.contains("404 Not Found", Qt::CaseInsensitive))
            ui->radioButton_25->setIcon(QIcon(QString::fromUtf8(":/Imagenes/bad.png")));
        else
        {
            ui->radioButton_25->setIcon(QIcon(QString::fromUtf8(":/Imagenes/good.png")));
            QStringList Buscador = Visto.split("\n");
            for(int a=0;a<Buscador.count();a++)
            {
                QString Rastreo = Buscador.value(a);
                if (Rastreo.contains("Content-Length:"))
                {
                    Rastreo.remove("Content-Length: ");
                    int Total = Rastreo.toDouble();
                    Total = (Total/1024)/1024;
                    ui->label_84->setText(QString::number(Total)+"Mb");
                    break;
                }
            }
        }
    }
    if (Valor == "GParted")
    {
        ui->label_87->setText("");
        RastreoW = ui->comboBox_32->currentText();
        RastreoP = ui->comboBox_31->currentText();
        QSqlQuery LocalizarW(db);
        LocalizarW.exec("SELECT Direccion FROM Distro WHERE NomDistro LIKE '"+Valor+"' AND Version LIKE '"+RastreoW+"' AND Arquitectura LIKE '"+RastreoP+"'");
        LocalizarW.first();
        Verificado = LocalizarW.value(0).toString();
        Visto = getWget(Verificado);
        Visto = Reload();
        if (Visto.contains("404 Not Found", Qt::CaseInsensitive))
            ui->radioButton_27->setIcon(QIcon(QString::fromUtf8(":/Imagenes/bad.png")));
        else
        {
            ui->radioButton_27->setIcon(QIcon(QString::fromUtf8(":/Imagenes/good.png")));
            QStringList Buscador = Visto.split("\n");
            for(int a=0;a<Buscador.count();a++)
            {
                QString Rastreo = Buscador.value(a);
                if (Rastreo.contains("Content-Length:"))
                {
                    Rastreo.remove("Content-Length: ");
                    int Total = Rastreo.toDouble();
                    Total = (Total/1024)/1024;
                    ui->label_87->setText(QString::number(Total)+"Mb");
                    break;
                }
            }
        }
    }
    if (Valor == "System Rescue CD")
    {
        ui->label_78->setText("");
        RastreoW = ui->comboBox_36->currentText();
        RastreoP = ui->comboBox_35->currentText();
        QSqlQuery LocalizarW(db);
        LocalizarW.exec("SELECT Direccion FROM Distro WHERE NomDistro LIKE '"+Valor+"' AND Version LIKE '"+RastreoW+"' AND Arquitectura LIKE '"+RastreoP+"'");
        LocalizarW.first();
        Verificado = LocalizarW.value(0).toString();
        Visto = getWget(Verificado);
        Visto = Reload();
        if (Visto.contains("404 Not Found", Qt::CaseInsensitive))
            ui->radioButton_31->setIcon(QIcon(QString::fromUtf8(":/Imagenes/bad.png")));
        else
        {
            ui->radioButton_31->setIcon(QIcon(QString::fromUtf8(":/Imagenes/good.png")));
            QStringList Buscador = Visto.split("\n");
            for(int a=0;a<Buscador.count();a++)
            {
                QString Rastreo = Buscador.value(a);
                if (Rastreo.contains("Content-Length:"))
                {
                    Rastreo.remove("Content-Length: ");
                    int Total = Rastreo.toDouble();
                    Total = (Total/1024)/1024;
                    ui->label_78->setText(QString::number(Total)+"Mb");
                    break;
                }
            }
        }
    }
    if (Valor == "OpenMandriva")
    {
        ui->label_76->setText("");
        RastreoW = ui->comboBox_22->currentText();
        RastreoP = ui->comboBox_21->currentText();
        QSqlQuery LocalizarW(db);
        LocalizarW.exec("SELECT Direccion FROM Distro WHERE NomDistro LIKE '"+Valor+"' AND Version LIKE '"+RastreoW+"' AND Arquitectura LIKE '"+RastreoP+"'");
        LocalizarW.first();
        Verificado = LocalizarW.value(0).toString();
        Visto = getWget(Verificado);
        Visto = Reload();
        if (Visto.contains("404 Not Found", Qt::CaseInsensitive))
            ui->radioButton_7->setIcon(QIcon(QString::fromUtf8(":/Imagenes/bad.png")));
        else
        {
            ui->radioButton_7->setIcon(QIcon(QString::fromUtf8(":/Imagenes/good.png")));
            QStringList Buscador = Visto.split("\n");
            for(int a=0;a<Buscador.count();a++)
            {
                QString Rastreo = Buscador.value(a);
                if (Rastreo.contains("Content-Length:"))
                {
                    Rastreo.remove("Content-Length: ");
                    int Total = Rastreo.toDouble();
                    Total = (Total/1024)/1024;
                    ui->label_76->setText(QString::number(Total)+"Mb");
                    break;
                }
            }
        }
    }
}

QString UsbLive::getWget(QString Verificado)
{
    QProcess *procesoFind;
    QStringList argumentosFind;
    QByteArray TipVirtual;
    procesoFind=new QProcess(this);
    argumentosFind << "-S" << "--spider" << Verificado << "-o" << "/usr/share/RecoverDrake/Aracnido.inf";
    procesoFind->start("wget",argumentosFind);
    if (! procesoFind->waitForStarted())
        return QString("");
    procesoFind->waitForFinished();
    TipVirtual = QString(procesoFind->readAllStandardOutput());
    delete procesoFind;
    QString res = QString(TipVirtual);
    res.chop(1);
    return res;
}

QString UsbLive::Reload()
{
    QFile file1("/usr/share/RecoverDrake/Aracnido.inf");
    file1.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in1(&file1);
    QString ValorReg = in1.readAll();
    file1.close();
    system("rm -f /usr/share/RecoverDrake/Aracnido.inf");
    return ValorReg;
}

UsbLive::~UsbLive()
{
    delete ui;
    if (mib != 0)
        delete mib;
}

void UsbLive::Arranque()
{
    ui->comboBox->setEnabled(false);
    ui->radioButton->setEnabled(false);
    ui->radioButton_2->setEnabled(false);
    ui->pushButton->setEnabled(false);
    ui->label_8->setEnabled(false);
    ui->comboBox_2->setEnabled(false);
    ui->radioButton_5->setEnabled(false);
    ui->radioButton_6->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->label_31->setEnabled(false);
    ui->comboBox_3->setEnabled(false);
    ui->radioButton_9->setEnabled(false);
    ui->radioButton_10->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->label_39->setEnabled(false);
    ui->comboBox_4->setEnabled(false);
    ui->radioButton_13->setEnabled(false);
    ui->radioButton_14->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->label_46->setEnabled(false);
    ui->comboBox_5->setEnabled(false);
    ui->radioButton_17->setEnabled(false);
    ui->radioButton_18->setEnabled(false);
    ui->pushButton_5->setEnabled(false);
    ui->label_91->setEnabled(false);
    ui->comboBox_6->setEnabled(false);
    ui->radioButton_39->setEnabled(false);
    ui->radioButton_40->setEnabled(false);
    ui->pushButton_8->setEnabled(false);
    ui->label_101->setEnabled(false);
    ui->comboBox_7->setEnabled(false);
    ui->radioButton_43->setEnabled(false);
    ui->radioButton_44->setEnabled(false);
    ui->pushButton_10->setEnabled(false);
    ui->label_110->setEnabled(false);
    ui->comboBox_8->setEnabled(false);
    ui->radioButton_55->setEnabled(false);
    ui->radioButton_56->setEnabled(false);
    ui->pushButton_14->setEnabled(false);
    ui->label_136->setEnabled(false);
    ui->comboBox_9->setEnabled(false);
    ui->radioButton_51->setEnabled(false);
    ui->radioButton_52->setEnabled(false);
    ui->pushButton_13->setEnabled(false);
    ui->label_127->setEnabled(false);
    ui->comboBox_10->setEnabled(false);
    ui->radioButton_47->setEnabled(false);
    ui->radioButton_48->setEnabled(false);
    ui->pushButton_12->setEnabled(false);
    ui->label_118->setEnabled(false);
    ui->radioButton->setChecked(true);
    ui->radioButton_5->setChecked(true);
    ui->radioButton_9->setChecked(true);
    ui->radioButton_13->setChecked(true);
    ui->radioButton_17->setChecked(true);
    ui->radioButton_39->setChecked(true);
    ui->radioButton_43->setChecked(true);
    ui->radioButton_55->setChecked(true);
    ui->radioButton_51->setChecked(true);
    ui->radioButton_47->setChecked(true);
    ui->tabWidget->setCurrentPage(0);
    if (Deshabilitar == "No")
        ui->radioButton_12->setChecked(true);
    ui->radioButton_4->setChecked(false);
    ui->radioButton_8->setChecked(true);
    ui->radioButton_11->setChecked(false);
    ui->label_55->setEnabled(false);
    ui->label_57->setEnabled(false);
    ui->label_59->setEnabled(false);
    ui->label_61->setEnabled(false);
    ui->label_63->setEnabled(false);
    ui->label_65->setEnabled(false);
    ui->label_67->setEnabled(false);
    ui->label_69->setEnabled(false);
    ui->label_71->setEnabled(false);
    ui->label_73->setEnabled(false);
    ui->label_7->setEnabled(false);
    ui->label_56->setEnabled(false);
    ui->label_58->setEnabled(false);
    ui->label_60->setEnabled(false);
    ui->label_62->setEnabled(false);
    ui->label_64->setEnabled(false);
    ui->label_66->setEnabled(false);
    ui->label_68->setEnabled(false);
    ui->label_70->setEnabled(false);
    ui->label_72->setEnabled(false);
    ui->label_15->setEnabled(false);
    ui->label_74->setEnabled(false);
    ui->label_16->setEnabled(false);
    ui->label_84->setEnabled(false);
    ui->label_17->setEnabled(false);
    ui->label_87->setEnabled(false);
    ui->label_20->setEnabled(false);
    ui->label_78->setEnabled(false);
    ui->label_23->setEnabled(false);
    ui->comboBox_27->setEnabled(false);
    ui->radioButton_23->setEnabled(false);
    ui->radioButton_22->setEnabled(false);
    ui->pushButton_27->setEnabled(false);
    ui->label_75->setEnabled(false);
    ui->radioButton_23->setChecked(true);
    ui->radioButton_22->setChecked(false);
    ui->label_24->setEnabled(false);
    ui->comboBox_29->setEnabled(false);
    ui->radioButton_25->setEnabled(false);
    ui->radioButton_24->setEnabled(false);
    ui->pushButton_28->setEnabled(false);
    ui->label_85->setEnabled(false);
    ui->radioButton_25->setChecked(true);
    ui->radioButton_24->setChecked(false);
    ui->label_25->setEnabled(false);
    ui->comboBox_31->setEnabled(false);
    ui->radioButton_27->setEnabled(false);
    ui->radioButton_26->setEnabled(false);
    ui->pushButton_29->setEnabled(false);
    ui->label_88->setEnabled(false);
    ui->radioButton_27->setChecked(true);
    ui->radioButton_26->setChecked(false);
    ui->label_29->setEnabled(false);
    ui->comboBox_35->setEnabled(false);
    ui->radioButton_31->setEnabled(false);
    ui->radioButton_30->setEnabled(false);
    ui->pushButton_31->setEnabled(false);
    ui->label_79->setEnabled(false);
    ui->radioButton_31->setChecked(true);
    ui->radioButton_30->setChecked(false);
    ui->label_29->setEnabled(false);
    ui->label_25->setEnabled(false);
    ui->label_24->setEnabled(false);
    ui->label_134->setEnabled(false);
    ui->label_125->setEnabled(false);
    ui->label_116->setEnabled(false);
    ui->label_12->setEnabled(false);
    ui->label_14->setEnabled(false);
    ui->comboBox_21->setEnabled(false);
    ui->radioButton_7->setEnabled(false);
    ui->radioButton_3->setEnabled(false);
    ui->pushButton_24->setEnabled(false);
    ui->label_76->setEnabled(false);
    ui->radioButton_7->setChecked(true);
    ui->radioButton_3->setChecked(false);
}

void UsbLive::on_pushButton_6_clicked()
{
    ui->tabWidget->setCurrentPage(1);
}

void UsbLive::on_pushButton_9_clicked()
{
    ui->tabWidget->setCurrentPage(0);
}

void UsbLive::Value(QString valor)
{
    if (valor == "Quitar")
        CierreTotal = 1;
}

void UsbLive::on_pushButton_7_clicked()
{
    if (CierreTotal == 1)
        emit Cerrar();
    else
        close();
}

void UsbLive::closeEvent ( QCloseEvent * event )
{
    if (CierreTotal == 1)
    {
        emit Cerrar();
        event->accept();
    }
    else
    {
        close();
        event->accept();
    }
}

bool UsbLive::eventFilter(QObject* obj, QEvent *event)
{
    if (obj == this)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Escape)
            {
                if (CierreTotal == 0)
                    close();
                else if (CierreTotal == 1)
                    return true;
            }
        }
        if (event->type() == QEvent::KeyRelease)
        {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_F1)
            {
                ayuda = new Ayuda(this);
                ayuda->show();
                ayuda->Valor(tr("Crear MultiBoot"));
                return true;
            }
        }
        return false;
    }
    return QDialog::eventFilter(obj, event);
}

void UsbLive::on_checkBox_clicked()
{
    if (ui->checkBox->isChecked() == true)
    {
        ui->comboBox->setEnabled(true);
        ui->radioButton->setEnabled(true);
        ui->radioButton_2->setEnabled(true);
        ui->label_7->setEnabled(true);
        ui->label_55->setEnabled(true);
    }
    else
    {
        ui->comboBox->setEnabled(false);
        ui->radioButton->setEnabled(false);
        ui->radioButton_2->setEnabled(false);
        ui->pushButton->setEnabled(false);
        ui->label_7->setEnabled(false);
        ui->label_55->setEnabled(false);
    }
}

void UsbLive::on_checkBox_2_clicked()
{
    if (ui->checkBox_2->isChecked() == true)
    {
        ui->comboBox_2->setEnabled(true);
        ui->radioButton_5->setEnabled(true);
        ui->radioButton_6->setEnabled(true);
        ui->label_56->setEnabled(true);
        ui->label_57->setEnabled(true);
    }
    else
    {
        ui->comboBox_2->setEnabled(false);
        ui->radioButton_5->setEnabled(false);
        ui->radioButton_6->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
        ui->label_56->setEnabled(false);
        ui->label_57->setEnabled(false);
    }
}

void UsbLive::on_checkBox_3_clicked()
{
    if (ui->checkBox_3->isChecked() == true)
    {
        ui->comboBox_3->setEnabled(true);
        ui->radioButton_9->setEnabled(true);
        ui->radioButton_10->setEnabled(true);
        ui->label_58->setEnabled(true);
        ui->label_59->setEnabled(true);
    }
    else
    {
        ui->comboBox_3->setEnabled(false);
        ui->radioButton_9->setEnabled(false);
        ui->radioButton_10->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        ui->label_58->setEnabled(false);
        ui->label_59->setEnabled(false);
    }
}

void UsbLive::on_checkBox_4_clicked()
{
    if (ui->checkBox_4->isChecked() == true)
    {
        ui->comboBox_4->setEnabled(true);
        ui->radioButton_13->setEnabled(true);
        ui->radioButton_14->setEnabled(true);
        ui->label_60->setEnabled(true);
        ui->label_61->setEnabled(true);
    }
    else
    {
        ui->comboBox_4->setEnabled(false);
        ui->radioButton_13->setEnabled(false);
        ui->radioButton_14->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
        ui->label_60->setEnabled(false);
        ui->label_61->setEnabled(false);
    }
}

void UsbLive::on_checkBox_5_clicked()
{
    if (ui->checkBox_5->isChecked() == true)
    {
        ui->comboBox_5->setEnabled(true);
        ui->radioButton_17->setEnabled(true);
        ui->radioButton_18->setEnabled(true);
        ui->label_62->setEnabled(true);
        ui->label_63->setEnabled(true);
    }
    else
    {
        ui->comboBox_5->setEnabled(false);
        ui->radioButton_17->setEnabled(false);
        ui->radioButton_18->setEnabled(false);
        ui->pushButton_5->setEnabled(false);
        ui->label_62->setEnabled(false);
        ui->label_63->setEnabled(false);
    }
}

void UsbLive::on_checkBox_6_clicked()
{
    if (ui->checkBox_6->isChecked() == true)
    {
        ui->comboBox_6->setEnabled(true);
        ui->radioButton_39->setEnabled(true);
        ui->radioButton_40->setEnabled(true);
        ui->label_64->setEnabled(true);
        ui->label_65->setEnabled(true);
    }
    else
    {
        ui->comboBox_6->setEnabled(false);
        ui->radioButton_39->setEnabled(false);
        ui->radioButton_40->setEnabled(false);
        ui->pushButton_8->setEnabled(false);
        ui->label_64->setEnabled(false);
        ui->label_65->setEnabled(false);
    }
}

void UsbLive::on_checkBox_7_clicked()
{
    if (ui->checkBox_7->isChecked() == true)
    {
        ui->comboBox_7->setEnabled(true);
        ui->radioButton_43->setEnabled(true);
        ui->radioButton_44->setEnabled(true);
        ui->label_66->setEnabled(true);
        ui->label_67->setEnabled(true);
    }
    else
    {
        ui->comboBox_7->setEnabled(false);
        ui->radioButton_43->setEnabled(false);
        ui->radioButton_44->setEnabled(false);
        ui->pushButton_10->setEnabled(false);
        ui->label_66->setEnabled(false);
        ui->label_67->setEnabled(false);
    }
}

void UsbLive::on_checkBox_19_clicked()
{
    if (ui->checkBox_19->isChecked() == true)
    {
        ui->comboBox_8->setEnabled(true);
        ui->radioButton_55->setEnabled(true);
        ui->radioButton_56->setEnabled(true);
        ui->label_68->setEnabled(true);
        ui->label_69->setEnabled(true);
    }
    else
    {
        ui->comboBox_8->setEnabled(false);
        ui->radioButton_55->setEnabled(false);
        ui->radioButton_56->setEnabled(false);
        ui->pushButton_14->setEnabled(false);
        ui->label_68->setEnabled(false);
        ui->label_69->setEnabled(false);
    }
}

void UsbLive::on_checkBox_9_clicked()
{
    if (ui->checkBox_9->isChecked() == true)
    {
        ui->comboBox_9->setEnabled(true);
        ui->radioButton_51->setEnabled(true);
        ui->radioButton_52->setEnabled(true);
        ui->label_71->setEnabled(true);
        ui->label_70->setEnabled(true);
    }
    else
    {
        ui->comboBox_9->setEnabled(false);
        ui->radioButton_51->setEnabled(false);
        ui->radioButton_52->setEnabled(false);
        ui->pushButton_13->setEnabled(false);
        ui->label_71->setEnabled(false);
        ui->label_70->setEnabled(false);
    }
}

void UsbLive::on_checkBox_8_clicked()
{
    if (ui->checkBox_8->isChecked() == true)
    {
        ui->comboBox_10->setEnabled(true);
        ui->radioButton_47->setEnabled(true);
        ui->radioButton_48->setEnabled(true);
        ui->label_73->setEnabled(true);
        ui->label_72->setEnabled(true);
    }
    else
    {
        ui->comboBox_10->setEnabled(false);
        ui->radioButton_47->setEnabled(false);
        ui->radioButton_48->setEnabled(false);
        ui->pushButton_12->setEnabled(false);
        ui->label_73->setEnabled(false);
        ui->label_72->setEnabled(false);
    }
}

void UsbLive::on_radioButton_clicked()
{
    if (ui->radioButton->isChecked() == true)
    {
        ui->pushButton->setEnabled(false);
        ui->label_7->setEnabled(true);
        ui->label_55->setEnabled(true);
        ui->label_8->setEnabled(false);
        ui->label_47->setEnabled(false);
    }
}

void UsbLive::on_radioButton_2_clicked()
{
    if (ui->radioButton_2->isChecked() == true)
    {
        ui->pushButton->setEnabled(true);
        ui->label_7->setEnabled(false);
        ui->label_55->setEnabled(false);
        ui->label_8->setEnabled(true);
        ui->label_47->setEnabled(true);
    }
}

void UsbLive::on_radioButton_5_clicked()
{
    if (ui->radioButton_5->isChecked() == true)
    {
        ui->pushButton_2->setEnabled(false);
        ui->label_56->setEnabled(true);
        ui->label_57->setEnabled(true);
        ui->label_31->setEnabled(false);
        ui->label_80->setEnabled(false);
    }
}

void UsbLive::on_radioButton_6_clicked()
{
    if (ui->radioButton_6->isChecked() == true)
    {
        ui->pushButton_2->setEnabled(true);
        ui->label_56->setEnabled(false);
        ui->label_57->setEnabled(false);
        ui->label_31->setEnabled(true);
        ui->label_80->setEnabled(true);
    }
}

void UsbLive::on_radioButton_9_clicked()
{
    if (ui->radioButton_9->isChecked() == true)
    {
        ui->pushButton_3->setEnabled(false);
        ui->label_58->setEnabled(true);
        ui->label_59->setEnabled(true);
        ui->label_39->setEnabled(false);
        ui->label_81->setEnabled(false);
    }
}

void UsbLive::on_radioButton_10_clicked()
{
    if (ui->radioButton_10->isChecked() == true)
    {
        ui->pushButton_3->setEnabled(true);
        ui->label_58->setEnabled(false);
        ui->label_59->setEnabled(false);
        ui->label_39->setEnabled(true);
        ui->label_81->setEnabled(true);
    }
}

void UsbLive::on_radioButton_13_clicked()
{
    if (ui->radioButton_13->isChecked() == true)
    {
        ui->pushButton_4->setEnabled(false);
        ui->label_60->setEnabled(true);
        ui->label_61->setEnabled(true);
        ui->label_46->setEnabled(false);
        ui->label_82->setEnabled(false);
    }
}

void UsbLive::on_radioButton_14_clicked()
{
    if (ui->radioButton_14->isChecked() == true)
    {
        ui->pushButton_4->setEnabled(true);
        ui->label_60->setEnabled(false);
        ui->label_61->setEnabled(false);
        ui->label_46->setEnabled(true);
        ui->label_82->setEnabled(true);
    }
}

void UsbLive::on_radioButton_17_clicked()
{
    if (ui->radioButton_17->isChecked() == true)
    {
        ui->pushButton_5->setEnabled(false);
        ui->label_62->setEnabled(true);
        ui->label_63->setEnabled(true);
        ui->label_91->setEnabled(false);
        ui->label_89->setEnabled(false);
    }
}

void UsbLive::on_radioButton_18_clicked()
{
    if (ui->radioButton_18->isChecked() == true)
    {
        ui->pushButton_5->setEnabled(true);
        ui->label_62->setEnabled(false);
        ui->label_63->setEnabled(false);
        ui->label_91->setEnabled(true);
        ui->label_89->setEnabled(true);
    }
}

void UsbLive::on_radioButton_39_clicked()
{
    if (ui->radioButton_39->isChecked() == true)
    {
        ui->pushButton_8->setEnabled(false);
        ui->label_64->setEnabled(true);
        ui->label_65->setEnabled(true);
        ui->label_101->setEnabled(false);
        ui->label_99->setEnabled(false);
    }
}

void UsbLive::on_radioButton_40_clicked()
{
    if (ui->radioButton_40->isChecked() == true)
    {
        ui->pushButton_8->setEnabled(true);
        ui->label_64->setEnabled(false);
        ui->label_65->setEnabled(false);
        ui->label_101->setEnabled(true);
        ui->label_99->setEnabled(true);
    }
}

void UsbLive::on_radioButton_43_clicked()
{
    if (ui->radioButton_43->isChecked() == true)
    {
        ui->pushButton_10->setEnabled(false);
        ui->label_66->setEnabled(true);
        ui->label_67->setEnabled(true);
        ui->label_110->setEnabled(false);
        ui->label_108->setEnabled(false);
    }
}

void UsbLive::on_radioButton_44_clicked()
{
    if (ui->radioButton_44->isChecked() == true)
    {
        ui->pushButton_10->setEnabled(true);
        ui->label_66->setEnabled(false);
        ui->label_67->setEnabled(false);
        ui->label_110->setEnabled(true);
        ui->label_108->setEnabled(true);
    }
}

void UsbLive::on_radioButton_55_clicked()
{
    if (ui->radioButton_55->isChecked() == true)
    {
        ui->pushButton_14->setEnabled(false);
        ui->label_68->setEnabled(true);
        ui->label_69->setEnabled(true);
        ui->label_134->setEnabled(false);
        ui->label_136->setEnabled(false);
    }
}

void UsbLive::on_radioButton_56_clicked()
{
    if (ui->radioButton_56->isChecked() == true)
    {
        ui->pushButton_14->setEnabled(true);
        ui->label_68->setEnabled(false);
        ui->label_69->setEnabled(false);
        ui->label_134->setEnabled(true);
        ui->label_136->setEnabled(true);
    }
}

void UsbLive::on_radioButton_51_clicked()
{
    if (ui->radioButton_51->isChecked() == true)
    {
        ui->pushButton_13->setEnabled(false);
        ui->label_71->setEnabled(true);
        ui->label_70->setEnabled(true);
        ui->label_125->setEnabled(false);
        ui->label_127->setEnabled(false);
    }
}

void UsbLive::on_radioButton_52_clicked()
{
    if (ui->radioButton_52->isChecked() == true)
    {
        ui->pushButton_13->setEnabled(true);
        ui->label_71->setEnabled(false);
        ui->label_70->setEnabled(false);
        ui->label_125->setEnabled(true);
        ui->label_127->setEnabled(true);
    }
}

void UsbLive::on_radioButton_47_clicked()
{
    if (ui->radioButton_47->isChecked() == true)
    {
        ui->pushButton_12->setEnabled(false);
        ui->label_73->setEnabled(true);
        ui->label_72->setEnabled(true);
        ui->label_116->setEnabled(false);
        ui->label_118->setEnabled(false);
    }
}

void UsbLive::on_radioButton_48_clicked()
{
    if (ui->radioButton_48->isChecked() == true)
    {
        ui->pushButton_12->setEnabled(true);
        ui->label_73->setEnabled(false);
        ui->label_72->setEnabled(false);
        ui->label_116->setEnabled(true);
        ui->label_118->setEnabled(true);
    }
}

void UsbLive::on_pushButton_clicked()
{
    QString path=tr("/home/%1/Documentos/");
    path=path.arg(user);
    QString fileNameOrigen = QFileDialog::getOpenFileName(this,QString::fromUtf8(tr("Abrir archivo de imagen de Mandriva .iso")),
                        path,trUtf8(tr("Imagen .iso (*.iso)")));
    if (fileNameOrigen.isEmpty())
         return;
    fileName = fileNameOrigen.replace(" ", "\\ ").replace("&","\\&").replace("'","\\'").replace("(","\\(").replace(")","\\)");    
    QString Version = ui->comboBox_11->currentText();
    Version = Version.split(".").value(0);
    QString Distribucion = ui->groupBox->title();
    QString Arquitectura;
    if (ui->comboBox->currentText() == "32bit")
        Arquitectura = "i586";
    else if (ui->comboBox->currentText() == "64bit")
        Arquitectura = "x86_64";
    if (fileNameOrigen.contains(Distribucion, Qt::CaseInsensitive) != true || fileNameOrigen.contains(Version, Qt::CaseInsensitive) != true || fileNameOrigen.contains(Arquitectura, Qt::CaseInsensitive) != true)
    {
        QMessageBox m;
        if (Stilo == "A")
            m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
        m.setText(tr("El nombre de la imagen debe contener "+Distribucion+", "+Arquitectura+" y "+Version+" para poder continuar."));
        m.exec();
        return;
    }
    ui->label_47->setText(fileNameOrigen);
}

void UsbLive::on_pushButton_2_clicked()
{
    QString path=tr("/home/%1/Documentos/");
    path=path.arg(user);
    QString fileNameOrigen = QFileDialog::getOpenFileName(this,QString::fromUtf8(tr("Abrir archivo de imagen de PCLinuxOS .iso")),
                        path,trUtf8(tr("Imagen .iso (*.iso)")));
    if (fileNameOrigen.isEmpty())
         return;
    fileName1 = fileNameOrigen.replace(" ", "\\ ").replace("&","\\&").replace("'","\\'").replace("(","\\(").replace(")","\\)");
    QString Version = ui->comboBox_12->currentText();
    QString Distribucion = ui->groupBox_2->title();
    QString Arquitectura;
    if (ui->comboBox_2->currentText() == "32bit")
    {
        if (fileNameOrigen.contains(Distribucion, Qt::CaseInsensitive) != true || fileNameOrigen.contains(Version, Qt::CaseInsensitive) != true)
        {
            QMessageBox m;
            if (Stilo == "A")
                m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
            m.setText(tr("El nombre de la imagen debe contener "+Distribucion+" y "+Version+" para poder continuar."));
            m.exec();
            return;
        }
    }
    else if (ui->comboBox_2->currentText() == "64bit")
    {
        Arquitectura = "64";
        if (fileNameOrigen.contains(Distribucion, Qt::CaseInsensitive) != true || fileNameOrigen.contains(Version, Qt::CaseInsensitive) != true || fileNameOrigen.contains(Arquitectura, Qt::CaseInsensitive) != true)
        {
            QMessageBox m;
            if (Stilo == "A")
                m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
            m.setText(tr("El nombre de la imagen debe contener "+Distribucion+", "+Arquitectura+" y "+Version+" para poder continuar."));
            m.exec();
            return;
        }
    }
    ui->label_80->setText(fileNameOrigen);
}

void UsbLive::on_pushButton_3_clicked()
{
    QString path=tr("/home/%1/Documentos/");
    path=path.arg(user);
    QString fileNameOrigen = QFileDialog::getOpenFileName(this,QString::fromUtf8(tr("Abrir archivo de imagen de Fedora .iso")),
                        path,trUtf8(tr("Imagen .iso (*.iso)")));
    if (fileNameOrigen.isEmpty())
         return;
    fileName2 = fileNameOrigen.replace(" ", "\\ ").replace("&","\\&").replace("'","\\'").replace("(","\\(").replace(")","\\)");
    QString Version = ui->comboBox_13->currentText();
    Version = Version.split(".").value(0);
    QString Distribucion = ui->groupBox_3->title();
    QString Arquitectura;
    if (ui->comboBox_3->currentText() == "32bit")
        Arquitectura = "i686";
    else if (ui->comboBox_3->currentText() == "64bit")
        Arquitectura = "x86_64";
    if (fileNameOrigen.contains(Distribucion, Qt::CaseInsensitive) != true || fileNameOrigen.contains(Version, Qt::CaseInsensitive) != true || fileNameOrigen.contains(Arquitectura, Qt::CaseInsensitive) != true)
    {
        QMessageBox m;
        if (Stilo == "A")
            m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
        m.setText(tr("El nombre de la imagen debe contener "+Distribucion+", "+Arquitectura+" y "+Version+" para poder continuar."));
        m.exec();
        return;
    }
    ui->label_81->setText(fileNameOrigen);
}

void UsbLive::on_pushButton_4_clicked()
{
    QString path=tr("/home/%1/Documentos/");
    path=path.arg(user);
    QString fileNameOrigen = QFileDialog::getOpenFileName(this,QString::fromUtf8(tr("Abrir archivo de imagen de Ubuntu .iso")),
                        path,trUtf8(tr("Imagen .iso (*.iso)")));
    if (fileNameOrigen.isEmpty())
         return;
    fileName3 = fileNameOrigen.replace(" ", "\\ ").replace("&","\\&").replace("'","\\'").replace("(","\\(").replace(")","\\)");
    QString Version = ui->comboBox_14->currentText();
    QString Distribucion = ui->groupBox_4->title();
    QString Arquitectura;
    if (ui->comboBox_4->currentText() == "32bit")
        Arquitectura = "i386";
    else if (ui->comboBox_4->currentText() == "64bit")
        Arquitectura = "amd64";
    if (fileNameOrigen.contains(Distribucion, Qt::CaseInsensitive) != true || fileNameOrigen.contains(Version, Qt::CaseInsensitive) != true || fileNameOrigen.contains(Arquitectura, Qt::CaseInsensitive) != true)
    {
        QMessageBox m;
        if (Stilo == "A")
            m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
        m.setText(tr("El nombre de la imagen debe contener "+Distribucion+", "+Arquitectura+" y "+Version+" para poder continuar."));
        m.exec();
        return;
    }
    ui->label_81->setText(fileNameOrigen);
}

void UsbLive::on_pushButton_5_clicked()
{
    QString path=tr("/home/%1/Documentos/");
    path=path.arg(user);
    QString fileNameOrigen = QFileDialog::getOpenFileName(this,QString::fromUtf8(tr("Abrir archivo de imagen de ArchLinux .iso")),
                        path,trUtf8(tr("Imagen .iso (*.iso)")));
    if (fileNameOrigen.isEmpty())
         return;
    fileName4 = fileNameOrigen.replace(" ", "\\ ").replace("&","\\&").replace("'","\\'").replace("(","\\(").replace(")","\\)");
    QString Version = ui->comboBox_15->currentText();
    QString Distribucion = ui->groupBox_5->title();
    QString Arquitectura = ui->comboBox_4->currentText();
    if (fileNameOrigen.contains(Distribucion, Qt::CaseInsensitive) != true || fileNameOrigen.contains(Version, Qt::CaseInsensitive) != true || fileNameOrigen.contains(Arquitectura, Qt::CaseInsensitive) != true)
    {
        QMessageBox m;
        if (Stilo == "A")
            m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
        m.setText(tr("El nombre de la imagen debe contener "+Distribucion+", "+Arquitectura+" y "+Version+" para poder continuar."));
        m.exec();
        return;
    }
    ui->label_89->setText(fileNameOrigen);
}

void UsbLive::on_pushButton_8_clicked()
{
    QString path=tr("/home/%1/Documentos/");
    path=path.arg(user);
    QString fileNameOrigen = QFileDialog::getOpenFileName(this,QString::fromUtf8(tr("Abrir archivo de imagen de CentOS .iso")),
                        path,trUtf8(tr("Imagen .iso (*.iso)")));
    if (fileNameOrigen.isEmpty())
         return;
    fileName5 = fileNameOrigen.replace(" ", "\\ ").replace("&","\\&").replace("'","\\'").replace("(","\\(").replace(")","\\)");
    QString Version = ui->comboBox_16->currentText();
    QString Distribucion = ui->groupBox_6->title();
    QString Arquitectura;
    if (ui->comboBox_6->currentText() == "32bit")
        Arquitectura = "i386";
    else if (ui->comboBox_6->currentText() == "64bit")
        Arquitectura = "x86_64";
    if (fileNameOrigen.contains(Distribucion, Qt::CaseInsensitive) != true || fileNameOrigen.contains(Version, Qt::CaseInsensitive) != true || fileNameOrigen.contains(Arquitectura, Qt::CaseInsensitive) != true)
    {
        QMessageBox m;
        if (Stilo == "A")
            m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
        m.setText(tr("El nombre de la imagen debe contener "+Distribucion+", "+Arquitectura+" y "+Version+" para poder continuar."));
        m.exec();
        return;
    }
    ui->label_99->setText(fileNameOrigen);
}

void UsbLive::on_pushButton_10_clicked()
{
    QString path=tr("/home/%1/Documentos/");
    path=path.arg(user);
    QString fileNameOrigen = QFileDialog::getOpenFileName(this,QString::fromUtf8(tr("Abrir archivo de imagen de Debian .iso")),
                        path,trUtf8(tr("Imagen .iso (*.iso)")));
    if (fileNameOrigen.isEmpty())
         return;
    fileName6 = fileNameOrigen.replace(" ", "\\ ").replace("&","\\&").replace("'","\\'").replace("(","\\(").replace(")","\\)");
    QString Version = ui->comboBox_17->currentText();
    QString Distribucion = ui->groupBox_7->title();
    QString Arquitectura;
    if (ui->comboBox_7->currentText() == "32bit")
        Arquitectura = "i386";
    else if (ui->comboBox_7->currentText() == "64bit")
        Arquitectura = "amd64";
    if (fileNameOrigen.contains(Distribucion, Qt::CaseInsensitive) != true || fileNameOrigen.contains(Version, Qt::CaseInsensitive) != true || fileNameOrigen.contains(Arquitectura, Qt::CaseInsensitive) != true)
    {
        QMessageBox m;
        if (Stilo == "A")
            m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
        m.setText(tr("El nombre de la imagen debe contener "+Distribucion+", "+Arquitectura+" y "+Version+" para poder continuar."));
        m.exec();
        return;
    }
    ui->label_108->setText(fileNameOrigen);
}

void UsbLive::on_pushButton_14_clicked()
{
    QString path=tr("/home/%1/Documentos/");
    path=path.arg(user);
    QString fileNameOrigen = QFileDialog::getOpenFileName(this,QString::fromUtf8(tr("Abrir archivo de imagen de KNOPPIX .iso")),
                        path,trUtf8(tr("Imagen .iso (*.iso)")));
    if (fileNameOrigen.isEmpty())
         return;
    fileName7 = fileNameOrigen.replace(" ", "\\ ").replace("&","\\&").replace("'","\\'").replace("(","\\(").replace(")","\\)");
    QString Version = ui->comboBox_18->currentText();
    QString Distribucion = ui->groupBox_10->title();
    if (fileNameOrigen.contains(Distribucion, Qt::CaseInsensitive) != true || fileNameOrigen.contains(Version, Qt::CaseInsensitive) != true)
    {
        QMessageBox m;
        if (Stilo == "A")
            m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
        m.setText(tr("El nombre de la imagen debe contener "+Distribucion+" y "+Version+" para poder continuar."));
        m.exec();
        return;
    }
    ui->label_134->setText(fileNameOrigen);
}

void UsbLive::on_pushButton_13_clicked()
{
    QString path=tr("/home/%1/Documentos/");
    path=path.arg(user);
    QString fileNameOrigen = QFileDialog::getOpenFileName(this,QString::fromUtf8(tr("Abrir archivo de imagen de OpenSUSE .iso")),
                        path,trUtf8(tr("Imagen .iso (*.iso)")));
    if (fileNameOrigen.isEmpty())
         return;
    fileName8 = fileNameOrigen.replace(" ", "\\ ").replace("&","\\&").replace("'","\\'").replace("(","\\(").replace(")","\\)");
    QString Version = ui->comboBox_19->currentText();
    QString Distribucion = ui->groupBox_29->title();
    QString Arquitectura;
    if (ui->comboBox_9->currentText() == "32bit")
        Arquitectura = "i686";
    else if (ui->comboBox_9->currentText() == "64bit")
        Arquitectura = "x86_64";
    if (fileNameOrigen.contains(Distribucion, Qt::CaseInsensitive) != true || fileNameOrigen.contains(Version, Qt::CaseInsensitive) != true || fileNameOrigen.contains(Arquitectura, Qt::CaseInsensitive) != true)
    {
        QMessageBox m;
        if (Stilo == "A")
            m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
        m.setText(tr("El nombre de la imagen debe contener "+Distribucion+", "+Arquitectura+" y "+Version+" para poder continuar."));
        m.exec();
        return;
    }
    ui->label_125->setText(fileNameOrigen);
}

void UsbLive::on_pushButton_12_clicked()
{
    QString path=tr("/home/%1/Documentos/");
    path=path.arg(user);
    QString fileNameOrigen = QFileDialog::getOpenFileName(this,QString::fromUtf8(tr("Abrir archivo de imagen de LinuxMint .iso")),
                        path,trUtf8(tr("Imagen .iso (*.iso)")));
    if (fileNameOrigen.isEmpty())
         return;
    fileName9 = fileNameOrigen.replace(" ", "\\ ").replace("&","\\&").replace("'","\\'").replace("(","\\(").replace(")","\\)");
    QString Version = ui->comboBox_20->currentText();
    QString Distribucion = ui->groupBox_8->title();
    QString Arquitectura;
    if (ui->comboBox_10->currentText() == "32bit")
        Arquitectura = "32bit";
    else if (ui->comboBox_10->currentText() == "64bit")
        Arquitectura = "64bit";
    if (fileNameOrigen.contains(Distribucion, Qt::CaseInsensitive) != true || fileNameOrigen.contains(Version, Qt::CaseInsensitive) != true || fileNameOrigen.contains(Arquitectura, Qt::CaseInsensitive) != true)
    {
        QMessageBox m;
        if (Stilo == "A")
            m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
        m.setText(tr("El nombre de la imagen debe contener "+Distribucion+", "+Arquitectura+" y "+Version+" para poder continuar."));
        m.exec();
        return;
    }
    ui->label_116->setText(fileNameOrigen);
}

void UsbLive::on_pushButton_11_clicked()
{
    if (Deshabilitar == "Si")
        ui->radioButton_4->setChecked(true);
    ui->tabWidget->setCurrentPage(2);
}

void UsbLive::UsbInfo()
{    
    ui->tabWidget->setCurrentPage(2);
    drakeSistema drake;
    Media = drake.getMedia();    
    if (Duplicado == 1)
        return;
    int Borrado, x;
    Borrado = ui->tableWidget->rowCount();
    for(x=0;x<Borrado;x++)
    {
         ui->tableWidget->removeRow(x);
         x=x-1;
         Borrado=Borrado-1;
    }
    int iFilas;
    Lista = Media.split("\n");    
    if (Lista.count() == 1)
    {
        if (Lista.value(0) == "")
        {
            QMessageBox m;
            if (Stilo == "A")
                m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
            m.setText(tr("Nada que procesar.<p>No hay nada en la ruta "+ruta+" para procesar. Introduce un pendrive valido para continuar."));
            m.exec();
            return;
        }
        else
        {
            Value0 = Lista.value(0);
            Valor = Value0.split(" ");
            if (Valor.value(4) != "vfat")
            {
                QMessageBox m;
                if (Stilo == "A")
                    m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
                m.setText(tr("Nada que procesar.<p>No hay nada valido en la ruta "+ruta+" para procesar. Introduce un pendrive valido para continuar."));
                m.exec();
                return;
            }
        }
    }
    for(int i=0;i<Lista.count();i++)
    {
        Value0 = Lista.value(i);
        if (Value0 != "")
        {
            Valor = Value0.split(" ");
            for(int a=0;a<Valor.count();a++)
            {
                if (a == 0)
                {
                    Parte1 = Valor.value(a);
                    Value1 = Parte1.left(8);
                    Peso = getTalla(Parte1);
                    Peso.replace(QRegExp("\\s+"), " ");
                    Peso.trimmed();
                    QStringList Final = Peso.split(" ");
                    Value4 = Parte1;
                    Value5 = Final.value(1);
                    LabelOld = drake.getLabelOld(Value4);
                    LabelOld=LabelOld.remove("Volume label is ").remove(" ");
                    Value6 = LabelOld;
                }
                if (a == 2)
                {
                    Parte2 = Valor.value(a);
                    Value2 = Parte2.remove(ruta);
                }
                if (a == 4)
                {
                    Parte3 = Valor.value(a);
                    Value3 = Parte3;
                }
            }
        }
        if (Value3 == "vfat" && Value5 !="")
        {
            QTableWidgetItem *item1, *item2, *item3, *item4, *item5, *item6, *item7;
            item1=new QTableWidgetItem;
            item2=new QTableWidgetItem;
            item3=new QTableWidgetItem;
            item4=new QTableWidgetItem;
            item5=new QTableWidgetItem;
            item6=new QTableWidgetItem;
            item7=new QTableWidgetItem;
            item1->setText(""+Value1+"");
            item2->setText(""+Value2+"");
            item3->setText(""+Value3+"");
            item4->setText(""+Value4+"");
            item5->setText(""+Value5+"");
            item6->setText("");
            item7->setText(""+Value6+"");
            iFilas=ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(iFilas);
            ui->tableWidget->setItem(iFilas,0,item1);
            ui->tableWidget->setItem(iFilas,1,item2);
            ui->tableWidget->setItem(iFilas,2,item3);
            ui->tableWidget->setItem(iFilas,3,item4);
            ui->tableWidget->setItem(iFilas,4,item5);
            ui->tableWidget->setItem(iFilas,5,item6);
            ui->tableWidget->setItem(iFilas,6,item7);
            ui->tableWidget->item(iFilas,5)->setIcon(QIcon(":/Imagenes/bad.png"));
        }
    }
    Duplicado=1;
}

void UsbLive::on_tabWidget_currentChanged(int /*index*/)
{
    if (Deshabilitar == "Si")
        ui->radioButton_4->setChecked(true);
    if (ui->tabWidget->currentIndex() == 2)
        UsbInfo();
    else if (ui->tabWidget->currentIndex() == 3)
        Multiboot();
}

QString UsbLive::getTalla(QString Peso)
{
    QProcess *procesoFree, *procesoGrep;
    QStringList argumentosFree;
    QStringList argumentosGrep;
    QByteArray Total;
    procesoFree=new QProcess(this);
    procesoGrep=new QProcess(this);
    argumentosFree << "--block-size=G" << ""+Peso+"";
    argumentosGrep << ""+Peso+"";
    procesoFree->setStandardOutputProcess(procesoGrep);
    procesoFree->start("df", argumentosFree);
    procesoGrep->start("grep", argumentosGrep);
    if (! procesoGrep->waitForStarted())
        return QString("");
    procesoFree->waitForFinished();
    procesoGrep->waitForFinished();
    Total = procesoGrep->readAllStandardOutput();
    delete procesoFree;
    delete procesoGrep;    
    QString res = QString(Total);
    res.chop(0);
    return res;
}

void UsbLive::on_tableWidget_itemSelectionChanged()
{
    ui->textEdit->setText("");
    int Row = ui->tableWidget->currentRow();
    if (Row >= 0)
    {
        QTableWidgetItem *item1, *item2;
        item1=new QTableWidgetItem;
        item2=new QTableWidgetItem;
        item1=ui->tableWidget->item(Row,1);
        item2=ui->tableWidget->item(Row,2);
        QString valor1=item1->text();
        QString valor2= item2->text();
        if (valor2 == "vfat")
        {
            QString comando;
            comando= ruta+"/%1";
            comando = comando.arg(valor1);
            QString Recibir = getTree(comando);
            ui->textEdit->setText(Recibir);
        }
    }
}

QString UsbLive::getTree(QString Tree)
{
    QProcess *procesoTree;
    QStringList argumentosFree;
    QByteArray Total;
    procesoTree=new QProcess(this);
    argumentosFree << ""+Tree+"";
    procesoTree->start("tree", argumentosFree);
    if (! procesoTree->waitForStarted())
        return QString("");
    procesoTree->waitForFinished();
    Total = procesoTree->readAllStandardOutput();
    delete procesoTree;
    QString res = QString(Total);
    res.chop(0);
    return res;
}

void UsbLive::on_pushButton_18_clicked()
{
    ui->tabWidget->setCurrentPage(1);
}

void UsbLive::on_pushButton_19_clicked()
{    
    if (Marcado > 0)
    {
        QMessageBox m;
        if (Stilo == "A")
            m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
        m.setText(tr("Solo puede haber un USB marcado. Desactiva el marcado y vuelve a intentarlo."));
        m.exec();
        return;
    }
    else if (Marcado == 0)
    {
        int iFilas;
        QString valor;
        iFilas=ui->tableWidget->currentRow();
        if (iFilas == -1)
        {
            QMessageBox m;
            if (Stilo == "A")
                m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
            m.setText(tr("No has seleccionado ningun registro, por lo que no puedes procesar la solicitud."));
            m.exec();
            return;
        }
        else
        {
            QTableWidgetItem *item;
            item=new QTableWidgetItem;
            item=ui->tableWidget->item(iFilas,5);
            valor=item->text();
            if(valor != "Ok")
            {
                ui->tableWidget->item(iFilas,5)->setIcon(QIcon(":/Imagenes/good.png"));
                item->setText("Ok");
                ui->tableWidget->setItem(iFilas,5,item);
                Marcado++;
            }
        }
    }
}

void UsbLive::on_pushButton_20_clicked()
{
    if (Marcado == 0)
    {
        QMessageBox m;
        if (Stilo == "A")
            m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
        m.setText(tr("Nada que desmarcar."));
        m.exec();
        return;
    }
    else if (Marcado > 0)
    {
        int iFilas;
        QString valor;
        iFilas=ui->tableWidget->currentRow();
        if (iFilas == -1)
        {
            QMessageBox m;
            if (Stilo == "A")
                m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
            m.setText(tr("No has seleccionado ningun registro, por lo que no puedes procesar la solicitud."));
            m.exec();
            return;
        }
        else
        {
            QTableWidgetItem *item;
            item=new QTableWidgetItem;
            item=ui->tableWidget->item(iFilas,5);
            valor=item->text();
            if(valor == "Ok")
            {
                ui->tableWidget->item(iFilas,5)->setIcon(QIcon(":/Imagenes/bad.png"));
                item->setText("");
                ui->tableWidget->setItem(iFilas,5,item);
                Marcado--;
            }
        }
    }
}

void UsbLive::on_pushButton_15_clicked()
{
    ui->label_4->hide();
    ui->textEdit->setText("");
    QTableWidgetItem *item, *item1, *item2;
    QString activo, activo1, activo2;
    QStringList comandos;
    int itemCount = ui->tableWidget->currentRow();
    if (itemCount == -1)
    {
        QMessageBox m;
        if (Stilo == "A")
            m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
        m.setText(tr("No has seleccionado ningun registro, por lo que no puedes procesar la solicitud."));
        m.exec();
        return;
    }
    else
    {       
        item = ui->tableWidget->item(itemCount,3);
        item1 = ui->tableWidget->item(itemCount,5);
        item2 = ui->tableWidget->item(itemCount,1);
        activo = item->text();
        activo1 = item1->text();
        activo2 = item2->text();
        if (activo1 == "Ok")
        {
            int respuesta = 0;
            {
                respuesta = QMessageBox::question(this, QString::fromUtf8(tr("Formatear USB")),
                                   QString::fromUtf8(tr("<center><b>Formatear USB<b></center><p>"
                                    "Una forma de saber si tu pendrive es el correcto, es viendo la informacion que se detalla en el visor "
                                    "de contenido de la unidad y tambien es interesante ver el peso de la unidad para asegurarte.<p>"
                                   "<b>Procede con mucha precaucion.<p>"
                                   "&iquest;Estas seguro de querer formatear la unidad "+activo+" denominada "+activo2+"?")), QMessageBox::Ok, QMessageBox::No);
            }
            if (respuesta == QMessageBox::Ok)
            {
                Ctrl = 1;
                QString cm = QString::fromUtf8(tr("echo Desmontando USB..."));
                QString cmd1 = "umount %1";
                cmd1=cmd1.arg(activo);
                QString cm1= QString::fromUtf8(tr("echo Formateando USB..."));
                QString cmd2 = "mkfs.vfat -F 32 -n DRAKLIVE %2";
                cmd2=cmd2.arg(activo);
                QString cm2= QString::fromUtf8(tr("echo Montando USB..."));
                QString cmd4, cmd3;
                cmd4 = "mkdir "+ruta+"/DRAKLIVE";
                cmd3 = "mount %1 "+ruta+"/DRAKLIVE";
                cmd3=cmd3.arg(activo);
                comandos << cm << cmd1 << cm1 << cmd2 << cm2 << cmd4 << cmd3;
                if (mib != 0)
                    delete mib;
                mib = new DrakeProcesos(comandos, this);
                connect(mib, SIGNAL(publicarDatos(QString)), this, SLOT(mibEscribir(QString)));
                connect(mib, SIGNAL(finProceso()), this, SLOT(mibFin()));
                int valor= comandos.count();
                mib->Valor(valor,3);
                mib->Mascara(cantidad51,cantidad50,cantidad49,DatoTalla,cantidad47,DatoNegro);
                mib->iniciarProceso();
            }
            return;
        }
        else
        {
            QMessageBox m;
            if (Stilo == "A")
                m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
            m.setText(tr("El USB seleccionado no esta activado, por lo que no puedes procesar la solicitud."));
            m.exec();
            return;
        }
    }
}

void UsbLive::on_pushButton_16_clicked()
{
    Duplicado=0;
    Marcado = 0;
    UsbInfo();
}

QString UsbLive::getPack(QString activo)
{
    QProcess *procesoRPM;
    QStringList argumentosRPM;
    QByteArray Paquete;
    procesoRPM=new QProcess(this);
    argumentosRPM << "-q" << "--queryformat=%{NAME}" << activo;
    procesoRPM->start("rpm",argumentosRPM);
    if (! procesoRPM->waitForStarted())
        return QString("");
    procesoRPM->waitForFinished();
    Paquete = QString(procesoRPM->readAllStandardOutput());
    procesoRPM->waitForFinished();
    delete procesoRPM;
    QString res = QString(Paquete);
    QStringList Total = res.split(" ");
    if (Total.count() > 1)
        res = "";
    else
        res = Total.value(0);
    return res;
}

void UsbLive::on_pushButton_17_clicked()
{
    ui->tabWidget->setCurrentPage(3);
}

void UsbLive::Multiboot()
{    
    TamanoTotal = 0;
    Control = 0;
    ui->textEdit_3->setText("");
    ui->textEdit_2->setText("");
    comandos.clear();
    grub.clear();
    QTableWidgetItem *item1, *item2, *item3, *item4, *item5, *item6;
    int itemCount;
    itemCount = ui->tableWidget->rowCount();
    if (itemCount == -1 || itemCount == 0)
    {
        QMessageBox m;
        if (Stilo == "A")
            m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
        m.setText(tr("No has seleccionado ningun USB, por lo que no puedes procesar la solicitud."));
        m.exec();
        return ui->tabWidget->setCurrentPage(2);
    }
    else
    {
        int ContOk = 0;
        for(int a=0;a<itemCount;a++)
        {
            item1 = ui->tableWidget->item(a,5);
            activo1 = item1->text();
            if (activo1 == "Ok")
                ContOk++;
        }
        if (ContOk == 0)
        {
            QMessageBox m;
            if (Stilo == "A")
                m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
            m.setText(tr("No has seleccionado ningun USB, por lo que no puedes procesar la solicitud."));
            m.exec();
            return ui->tabWidget->setCurrentPage(2);
        }
        for(int i=0;i<itemCount;i++)
        {            
            item1 = ui->tableWidget->item(i,5);
            item2 = ui->tableWidget->item(i,1);
            item3 = ui->tableWidget->item(i,0);
            item4 = ui->tableWidget->item(i,3);
            item5 = ui->tableWidget->item(i,4);
            item6 = ui->tableWidget->item(i,6);
            activo1 = item1->text();
            activo2 = item2->text();
            activo3 = item3->text();
            activo4 = item4->text();
            activo5 = item5->text();
            activo6 = item6->text();
            if (activo1 == "Ok")
            {
                Valor1 = activo3;
                Valor2 = activo6;
                Valor2.chop(1);
                if (ui->radioButton_12->isChecked())
                    ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">Menu de arranque en pendrive: GRUB (GRand Unifier Bootloader)"));
                else if(ui->radioButton_4->isChecked())
                    ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">Menu de arranque en pendrive: GRUB2 (GRand Unifier Bootloader 2)"));
                ui->textEdit_3->append("");                                
                if (ui->checkBox->isChecked() == true)
                {
                    ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">1) Instalar Mandriva: Si"));
                    if (ui->radioButton_2->isChecked() == true)
                    {
                        if (ui->label_47->text() == "")
                        {
                            Control++;
                            ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">- ERROR(")+QString::number(Control)+tr("): Ruta iso/Mandriva en equipo: NO SE HA DEFINIDO NINGUNA RUTA y NO SE PUEDE CONTINUAR."));
                        }
                        else if(ui->label_47->text() != "")
                        {
                            QString TipoArc;
                            if (ui->comboBox->currentText() ==  "32bit")
                                TipoArc = "Mandriva32_";
                            else
                                TipoArc = "Mandriva64_";
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", ""+TipoArc+""+ui->comboBox_11->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == ""+TipoArc+""+ui->comboBox_11->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira."));
                            else
                            {
                                TipMandriva = TipoArc;
                                QFile file(ui->label_47->text());
                                qint64 size = QFileInfo(file).size();
                                float f;
                                f=(size + 1023) / 1024;
                                f=(f/1024)/1024;
                                TamanoTotal = TamanoTotal + f;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Ruta iso/Mandriva en equipo: ")+ui->label_47->text()+"");
                                QString md11 = tr("echo Creando directorio "+TipoArc+""+ui->comboBox_11->currentText()+"...");
                                QString mdd9, md12, mdd10;
                                mdd9 = "mkdir -pv "+ruta+"%1/iso/";
                                mdd9 = mdd9.arg(Valor2);
                                md12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                mdd10 = "file-roller --force --extract-to="+ruta+"%1/iso/%4%5/ %3";
                                mdd10 = mdd10.arg(Valor2).arg(ui->label_47->text()).arg(TipMandriva).arg(ui->comboBox_11->currentText());
                                comandos << md11 << mdd9 << md12 << mdd10;
                            }
                        }
                    }
                    else if (ui->radioButton_2->isChecked() == false)
                    {
                        ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Descargar iso/Mandriva del servidor: Si"));
                        if (ui->comboBox->currentText() ==  "32bit")
                        {
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", "Mandriva32_"+ui->comboBox_11->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == "Mandriva32_"+ui->comboBox_11->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira."));
                            else
                            {
                                QString Mandriva;
                                TipMandriva = "Mandriva32_";
                                QString Mandriva1 = "Mandriva";
                                QSqlQuery query(db);
                                query.exec("SELECT Direccion FROM Distro WHERE NomDistro='"+Mandriva1+"' AND Version='"+ui->comboBox_11->currentText()+"' AND Arquitectura='"+ui->comboBox->currentText()+"'");
                                query.first();
                                if (query.isValid())
                                    Mandriva=query.value(0).toString();
                                QStringList ParcIso = Mandriva.split("/");
                                int TamIso = ParcIso.count();
                                QString Iso = ParcIso.value(TamIso-1);
                                Tamano = ui->label_55->text()+" Mb";
                                QString TamanoMb = Tamano.remove(" ").remove("M").remove("b");
                                TamanoTotal = (TamanoMb.toFloat()/1024)+TamanoTotal;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Arquitectura: ")+ui->comboBox->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Version: ")+ui->comboBox_11->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Direccion de descarga: ")+Mandriva+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Peso de la descarga: ")+Tamano+" Mb");
                                QString md3 = tr("echo Descargando del servidor...");
                                QString mdd1 = "wget -c -P/home/%1/tmp/ %2";
                                mdd1=mdd1.arg(user).arg(Mandriva);
                                QString md15 = tr("echo Creando directorio Mandriva32_"+ui->comboBox_11->currentText()+"...");
                                QString mdd13, md16, mdd14;
                                mdd13 = "mkdir -pv "+ruta+"%1/iso/";
                                mdd13 = mdd13.arg(Valor2);
                                md16 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                mdd14 = "file-roller --force --extract-to="+ruta+"%1/iso/%3%5/ /home/%2/tmp/%4";
                                mdd14 = mdd14.arg(Valor2).arg(user).arg(TipMandriva).arg(Iso).arg(ui->comboBox_11->currentText());
                                comandos << md3 << mdd1 << md15 << mdd13 << md16 << mdd14;
                            }
                        }
                        else if(ui->comboBox->currentText() == "64bit")
                        {
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", "Mandriva64_"+ui->comboBox_11->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == "Mandriva64_"+ui->comboBox_11->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                QString Mandriva;
                                TipMandriva = "Mandriva64_";
                                QString Mandriva1 = "Mandriva";
                                QSqlQuery query(db);
                                query.exec("SELECT direccion FROM Distro WHERE NomDistro='"+Mandriva1+"' AND Version='"+ui->comboBox_11->currentText()+"' AND Arquitectura='"+ui->comboBox->currentText()+"'");
                                query.first();
                                if (query.isValid())
                                        Mandriva=query.value(0).toString();
                                QStringList ParcIso = Mandriva.split("/");
                                int TamIso = ParcIso.count();
                                QString Iso = ParcIso.value(TamIso-1);
                                Tamano = ui->label_55->text()+" Mb";
                                QString TamanoMb = Tamano.remove(" ").remove("M").remove("b");
                                TamanoTotal = (TamanoMb.toFloat()/1024)+TamanoTotal;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Arquitectura: ")+ui->comboBox->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Version: ")+ui->comboBox_11->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Direccion de descarga: ")+Mandriva+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Peso de la descarga: ")+Tamano+" Mb");
                                QString md3 = tr("echo Descargando del servidor...");
                                QString mdd1 = "wget -c -P/home/%1/tmp/ %2";
                                mdd1=mdd1.arg(user).arg(Mandriva);
                                QString md15 = tr("echo Creando directorio Mandriva64_"+ui->comboBox_11->currentText()+"...");
                                QString mdd13, md16, mdd14;
                                mdd13 = "mkdir -pv "+ruta+"%1/iso/";
                                mdd13 = mdd13.arg(Valor2);
                                md16 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                mdd14 = "file-roller --force --extract-to="+ruta+"%1/iso/%3%5/ /home/%2/tmp/%4";
                                mdd14 = mdd14.arg(Valor2).arg(user).arg(TipMandriva).arg(Iso).arg(ui->comboBox_11->currentText());
                                comandos << md3 << mdd1 << md15 << mdd13 << md16 << mdd14;
                            }
                        }
                    }
                }
                else if (ui->checkBox->isChecked() == false)
                {
                    TipMandriva = "";
                    ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">1) Instalar Mandriva: No"));
                }
                ui->textEdit_3->append("");
                if (ui->checkBox_2->isChecked() == true)
                {
                    ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">2) Instalar PCLinuxOS: Si"));
                    if (ui->radioButton_6->isChecked() == true)
                    {
                        if (ui->label_80->text() == "")
                        {
                            Control++;
                            ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">- ERROR(")+QString::number(Control)+tr("): Ruta iso/PCLinuxOS en equipo: NO SE HA DEFINIDO NINGUNA RUTA y NO SE PUEDE CONTINUAR"));
                        }
                        else if(ui->label_80->text() != "")
                        {
                            QString TipoArc;
                            if (ui->comboBox_2->currentText() ==  "32bit")
                                TipoArc = "PCLinuxOS32_";
                            else
                                TipoArc = "PCLinuxOS64_";
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", ""+TipoArc+""+ui->comboBox_12->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == ""+TipoArc+""+ui->comboBox_12->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                TipPCLinuxOS = TipoArc;
                                QFile file(ui->label_80->text());
                                qint64 size = QFileInfo(file).size();
                                float f;
                                f=(size + 1023) / 1024;
                                f=(f/1024)/1024;
                                TamanoTotal = TamanoTotal + f;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Ruta iso/PCLinuxOS en equipo: ")+ui->label_80->text()+"");
                                QString pc11 = tr("echo Creando directorio "+TipoArc+""+ui->comboBox_12->currentText()+"...");
                                QString pcd9, pc12, pcd10;
                                pcd9 = "mkdir -pv "+ruta+"%1/iso/";
                                pcd9 = pcd9.arg(Valor2);
                                pc12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                pcd10 = "file-roller --force --extract-to="+ruta+"%1/iso/%4%5/ %3";
                                pcd10 = pcd10.arg(Valor2).arg(ui->label_80->text()).arg(TipPCLinuxOS).arg(ui->comboBox_12->currentText());
                                comandos << pc11 << pcd9 << pc12 << pcd10;
                            }
                        }
                    }
                    else if (ui->radioButton_6->isChecked() == false)
                    {
                        ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Descargar iso/PCLinuxOS del servidor: Si"));
                        if (ui->comboBox_2->currentText() ==  "32bit")
                        {
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", "PCLinuxOS32_"+ui->comboBox_12->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == "PCLinuxOS_"+ui->comboBox_12->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                QString PCLinuxOS;
                                TipPCLinuxOS = "PCLinuxOS32_";
                                QString PCLinuxOS1 = "PCLinuxOS";
                                QSqlQuery query(db);
                                query.exec("SELECT direccion FROM Distro WHERE NomDistro='"+PCLinuxOS1+"' AND Version='"+ui->comboBox_12->currentText()+"' AND Arquitectura='"+ui->comboBox_2->currentText()+"'");
                                query.first();
                                if (query.isValid())
                                    PCLinuxOS=query.value(0).toString();
                                QStringList ParcIso = PCLinuxOS.split("/");
                                int TamIso = ParcIso.count();
                                QString Iso = ParcIso.value(TamIso-1);
                                Tamano = ui->label_57->text()+" Mb";
                                QString TamanoMb = Tamano.remove(" ").remove("M").remove("b");
                                TamanoTotal = (TamanoMb.toFloat()/1024)+TamanoTotal;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Arquitectura: ")+ui->comboBox_2->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Version: ")+ui->comboBox_12->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Direccion de descarga: ")+PCLinuxOS+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Peso de la descarga: ")+Tamano+" Mb");
                                QString pc3 = tr("echo Descargando del servidor...");
                                QString pcd1 = "wget -c -P/home/%1/tmp/ %2";
                                pcd1=pcd1.arg(user).arg(PCLinuxOS);
                                QString pc15 = tr("echo Creando directorio PCLinuxOS32_"+ui->comboBox_12->currentText()+"...");
                                QString pcd13, pc16, pcd14;
                                pcd13 = "mkdir -pv "+ruta+"%1/iso/";
                                pcd13 = pcd13.arg(Valor2);
                                pc16 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                pcd14 = "file-roller --force --extract-to="+ruta+"%1/iso/%3%5/ /home/%2/tmp/%4";
                                pcd14 = pcd14.arg(Valor2).arg(user).arg(TipPCLinuxOS).arg(Iso).arg(ui->comboBox_12->currentText());
                                comandos << pc3 << pcd1 << pc15 << pcd13 << pc16 << pcd14;
                            }
                        }
                        if (ui->comboBox_2->currentText() ==  "64bit")
                        {
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", "PCLinuxOS64_"+ui->comboBox_12->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == "PCLinuxOS_"+ui->comboBox_12->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                QString PCLinuxOS;
                                TipPCLinuxOS = "PCLinuxOS64_";
                                QString PCLinuxOS1 = "PCLinuxOS";
                                QSqlQuery query(db);
                                query.exec("SELECT direccion FROM Distro WHERE NomDistro='"+PCLinuxOS1+"' AND Version='"+ui->comboBox_12->currentText()+"' AND Arquitectura='"+ui->comboBox_2->currentText()+"'");
                                query.first();
                                if (query.isValid())
                                    PCLinuxOS=query.value(0).toString();
                                QStringList ParcIso = PCLinuxOS.split("/");
                                int TamIso = ParcIso.count();
                                QString Iso = ParcIso.value(TamIso-1);
                                Tamano = ui->label_57->text()+" Mb";
                                QString TamanoMb = Tamano.remove(" ").remove("M").remove("b");
                                TamanoTotal = (TamanoMb.toFloat()/1024)+TamanoTotal;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Arquitectura: ")+ui->comboBox_2->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Version: ")+ui->comboBox_12->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Direccion de descarga: ")+PCLinuxOS+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Peso de la descarga: ")+Tamano+" Mb");
                                QString pc3 = tr("echo Descargando del servidor...");
                                QString pcd1 = "wget -c -P/home/%1/tmp/ %2";
                                pcd1=pcd1.arg(user).arg(PCLinuxOS);
                                QString pc15 = tr("echo Creando directorio PCLinuxOS64_"+ui->comboBox_12->currentText()+"...");
                                QString pcd13, pc16, pcd14;
                                pcd13 = "mkdir -pv "+ruta+"%1/iso/";
                                pcd13 = pcd13.arg(Valor2);
                                pc16 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                pcd14 = "file-roller --force --extract-to="+ruta+"%1/iso/%3%5/ /home/%2/tmp/%4";
                                pcd14 = pcd14.arg(Valor2).arg(user).arg(TipPCLinuxOS).arg(Iso).arg(ui->comboBox_12->currentText());
                                comandos << pc3 << pcd1 << pc15 << pcd13 << pc16 << pcd14;
                            }
                        }
                    }
                }
                else if (ui->checkBox_2->isChecked() == false)
                {
                    TipPCLinuxOS = "";
                    ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">2) Instalar PCLinuxOS: No"));
                }
                ui->textEdit_3->append("");
                if (ui->checkBox_3->isChecked() == true)
                {
                    ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">3) Instalar Fedora: Si"));
                    if (ui->radioButton_10->isChecked() == true)
                    {
                        if (ui->label_81->text() == "")
                        {
                            Control++;
                            ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">- ERROR(")+QString::number(Control)+tr("): Ruta iso/Fedora en equipo: NO SE HA DEFINIDO NINGUNA RUTA y NO SE PUEDE CONTINUAR."));
                        }
                        else if(ui->label_81->text() != "")
                        {
                            QString TipoArc;
                            if (ui->comboBox_3->currentText() ==  "32bit")
                                TipoArc = "Fedora32_";
                            else
                                TipoArc = "Fedora64_";
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", ""+TipoArc+""+ui->comboBox_13->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == ""+TipoArc+""+ui->comboBox_13->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                TipFedora = TipoArc;
                                QFile file(ui->label_81->text());
                                qint64 size = QFileInfo(file).size();
                                float f;
                                f=(size + 1023) / 1024;
                                f=(f/1024)/1024;
                                TamanoTotal = TamanoTotal + f;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Ruta iso/Fedora en equipo: ")+ui->label_81->text()+"");
                                QString fd11 = tr("echo Creando directorio "+TipoArc+""+ui->comboBox_13->currentText()+"...");
                                QString fdd9, fd12, fdd10;
                                fdd9 = "mkdir -pv "+ruta+"%1/iso/";
                                fdd9 = fdd9.arg(Valor2);
                                fd12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                fdd10 = "file-roller --force --extract-to="+ruta+"%1/iso/%4%5/ %3";
                                fdd10 = fdd10.arg(Valor2).arg(ui->label_81->text()).arg(TipFedora).arg(ui->comboBox_13->currentText());
                                comandos << fd11 << fdd9 << fd12 << fdd10;
                            }
                        }
                    }
                    else if (ui->radioButton_10->isChecked() == false)
                    {
                        ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Descargar iso/Fedora del servidor: Si"));
                        if (ui->comboBox_2->currentText() ==  "32bit")
                        {
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", "Fedora32_"+ui->comboBox_13->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == "Fedora32_"+ui->comboBox_13->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                QString Fedora;
                                TipFedora = "Fedora32_";
                                QString Fedora1 = "Fedora";
                                QSqlQuery query(db);
                                query.exec("SELECT direccion FROM Distro WHERE NomDistro='"+Fedora1+"' AND Version='"+ui->comboBox_13->currentText()+"' AND Arquitectura='"+ui->comboBox_3->currentText()+"'");
                                query.first();
                                if (query.isValid())
                                    Fedora=query.value(0).toString();
                                QStringList ParcIso = Fedora.split("/");
                                int TamIso = ParcIso.count();
                                QString Iso = ParcIso.value(TamIso-1);
                                Tamano = ui->label_59->text()+" Mb";
                                QString TamanoMb = Tamano.remove(" ").remove("M").remove("b");
                                TamanoTotal = (TamanoMb.toFloat()/1024)+TamanoTotal;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Arquitectura: ")+ui->comboBox_3->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Version: ")+ui->comboBox_13->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Direccion de descarga: ")+Fedora+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Peso de la descarga: ")+Tamano+" Mb");
                                QString fe3 = tr("echo Descargando del servidor...");
                                QString fed1 = "wget -c -P/home/%1/tmp/ %2";
                                fed1=fed1.arg(user).arg(Fedora);
                                QString fe15 = tr("echo Creando directorio Fedora32_"+ui->comboBox_13->currentText()+"...");
                                QString fdd9, fd12, fdd10;
                                fdd9 = "mkdir -pv "+ruta+"%1/iso/";
                                fdd9 = fdd9.arg(Valor2);
                                fd12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                fdd10 = "file-roller --force --extract-to="+ruta+"%1/iso/%3%5/ /home/%2/tmp/%4";
                                fdd10 = fdd10.arg(Valor2).arg(user).arg(TipFedora).arg(Iso).arg(ui->comboBox_13->currentText());
                                comandos << fe3 << fed1 << fe15 << fdd9 << fd12 << fdd10;
                            }
                        }
                        else if(ui->comboBox_2->currentText() == "64bit")
                        {
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", "Fedora64_"+ui->comboBox_13->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == "Fedora64_"+ui->comboBox_13->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                QString Fedora;
                                TipFedora = "Fedora64_";
                                QString Fedora1 = "Fedora";
                                QSqlQuery query(db);
                                query.exec("SELECT direccion FROM Distro WHERE NomDistro='"+Fedora1+"' AND Version='"+ui->comboBox_13->currentText()+"' AND Arquitectura='"+ui->comboBox_3->currentText()+"'");
                                query.first();
                                if (query.isValid())
                                    Fedora=query.value(0).toString();
                                QStringList ParcIso = Fedora.split("/");
                                int TamIso = ParcIso.count();
                                QString Iso = ParcIso.value(TamIso-1);
                                Tamano = ui->label_59->text()+" Mb";
                                QString TamanoMb = Tamano.remove(" ").remove("M").remove("b");
                                TamanoTotal = (TamanoMb.toFloat()/1024)+TamanoTotal;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Arquitectura: ")+ui->comboBox_3->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Version: ")+ui->comboBox_13->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Direccion de descarga: ")+Fedora+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Peso de la descarga: ")+Tamano+" Mb");
                                QString fe3 = tr("echo Descargando del servidor...");
                                QString fed1 = "wget -c -P/home/%1/tmp/ %2";
                                fed1=fed1.arg(user).arg(Fedora);
                                QString fe15 = tr("echo Creando directorio Fedora64_"+ui->comboBox_13->currentText()+"...");
                                QString fdd9, fd12, fdd10;
                                fdd9 = "mkdir -pv "+ruta+"%1/iso/";
                                fdd9 = fdd9.arg(Valor2);
                                fd12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                fdd10 = "file-roller --force --extract-to="+ruta+"%1/iso/%3%5/ /home/%2/tmp/%4";
                                fdd10 = fdd10.arg(Valor2).arg(user).arg(TipFedora).arg(Iso).arg(ui->comboBox_13->currentText());
                                comandos << fe3 << fed1 << fe15 << fdd9 << fd12 << fdd10;
                            }
                        }
                    }
                }
                else if (ui->checkBox_3->isChecked() == false)
                {
                    ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">3) Instalar Fedora: No"));
                }
                ui->textEdit_3->append("");
                if (ui->checkBox_4->isChecked() == true)
                {
                    ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">4) Instalar Ubuntu: Si"));
                    if (ui->radioButton_14->isChecked() == true)
                    {
                        if (ui->label_82->text() == "")
                        {
                            Control++;
                            ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">- ERROR(")+QString::number(Control)+tr("): Ruta iso/Ubuntu en equipo: NO SE HA DEFINIDO NINGUNA RUTA y NO SE PUEDE CONTINUAR."));
                        }
                        else if(ui->label_82->text() != "")
                        {
                            QString TipoArc;
                            if (ui->comboBox_4->currentText() ==  "32bit")
                                TipoArc = "Ubuntu32_";
                            else
                                TipoArc = "Ubuntu64_";
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", ""+TipoArc+""+ui->comboBox_14->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == ""+TipoArc+""+ui->comboBox_14->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                TipUbuntu = TipoArc;
                                QFile file(ui->label_82->text());
                                qint64 size = QFileInfo(file).size();
                                float f;
                                f=(size + 1023) / 1024;
                                f=(f/1024)/1024;
                                TamanoTotal = TamanoTotal + f;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Ruta iso/Ubuntu en equipo: ")+ui->label_82->text()+"");
                                QString ub11 = tr("echo Creando directorio "+TipoArc+""+ui->comboBox_14->currentText()+"...");
                                QString ubd9, ub12, ubd10;
                                ubd9 = "mkdir -pv "+ruta+"%1/iso/";
                                ubd9 = ubd9.arg(Valor2);
                                ub12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                ubd10 = "file-roller --force --extract-to="+ruta+"%1/iso/%4%5/ %3";
                                ubd10 = ubd10.arg(Valor2).arg(ui->label_82->text()).arg(TipUbuntu).arg(ui->comboBox_14->currentText());
                                comandos << ub11 << ubd9 << ub12 << ubd10;
                            }
                        }
                    }
                    else if (ui->radioButton_14->isChecked() == false)
                    {
                        ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Descargar iso/Ubuntu del servidor: Si"));
                        if (ui->comboBox_4->currentText() ==  "32bit")
                        {
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", "Ubuntu32_"+ui->comboBox_14->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == "Ubuntu32_"+ui->comboBox_14->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                QString Ubuntu;
                                TipUbuntu = "Ubuntu32_";
                                QString Ubuntu1 = "Ubuntu";
                                QSqlQuery query(db);
                                query.exec("SELECT direccion FROM Distro WHERE NomDistro='"+Ubuntu1+"' AND Version='"+ui->comboBox_14->currentText()+"' AND Arquitectura='"+ui->comboBox_4->currentText()+"'");
                                query.first();
                                if (query.isValid())
                                    Ubuntu=query.value(0).toString();
                                QStringList ParcIso = Ubuntu.split("/");
                                int TamIso = ParcIso.count();
                                QString Iso = ParcIso.value(TamIso-1);
                                Tamano = ui->label_61->text()+" Mb";
                                QString TamanoMb = Tamano.remove(" ").remove("M").remove("b");
                                TamanoTotal = (TamanoMb.toFloat()/1024)+TamanoTotal;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Arquitectura: ")+ui->comboBox_4->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Version: ")+ui->comboBox_14->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Direccion de descarga: ")+Ubuntu+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Peso de la descarga: ")+Tamano+" Mb");
                                QString ub3 = tr("echo Descargando del servidor...");
                                QString ubd1 = "wget -c -P/home/%1/tmp/ %2";
                                ubd1=ubd1.arg(user).arg(Ubuntu);
                                QString ub15 = tr("echo Creando directorio Ubuntu32_"+ui->comboBox_14->currentText()+"...");
                                QString ubd9, ub12, ubd10;
                                ubd9 = "mkdir -pv "+ruta+"%1/iso/";
                                ubd9 = ubd9.arg(Valor2);
                                ub12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                ubd10 = "file-roller --force --extract-to="+ruta+"%1/iso/%3%5/ /home/%2/tmp/%4";
                                ubd10 = ubd10.arg(Valor2).arg(user).arg(TipUbuntu).arg(Iso).arg(ui->comboBox_14->currentText());
                                comandos << ub3 << ubd1 << ub15 << ubd9 << ub12 << ubd10;
                            }
                        }
                        else if(ui->comboBox_4->currentText() == "64bit")
                        {
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", "Ubuntu64_"+ui->comboBox_14->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == "Ubuntu64_"+ui->comboBox_14->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                QString Ubuntu;
                                TipUbuntu = "Ubuntu64_";
                                QString Ubuntu1 = "Ubuntu";
                                QSqlQuery query(db);
                                query.exec("SELECT direccion FROM Distro WHERE NomDistro='"+Ubuntu1+"' AND Version='"+ui->comboBox_14->currentText()+"' AND Arquitectura='"+ui->comboBox_4->currentText()+"'");
                                query.first();
                                if (query.isValid())
                                    Ubuntu=query.value(0).toString();
                                QStringList ParcIso = Ubuntu.split("/");
                                int TamIso = ParcIso.count();
                                QString Iso = ParcIso.value(TamIso-1);
                                Tamano = ui->label_61->text()+" Mb";
                                QString TamanoMb = Tamano.remove(" ").remove("M").remove("b");
                                TamanoTotal = (TamanoMb.toFloat()/1024)+TamanoTotal;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Arquitectura: ")+ui->comboBox_4->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Version: ")+ui->comboBox_14->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Direccion de descarga: ")+Ubuntu+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Peso de la descarga: ")+Tamano+" Mb");
                                QString ub3 = tr("echo Descargando del servidor...");
                                QString ubd1 = "wget -c -P/home/%1/tmp/ %2";
                                ubd1=ubd1.arg(user).arg(Ubuntu);
                                QString ub15 = tr("echo Creando directorio Ubuntu64_"+ui->comboBox_14->currentText()+"...");
                                QString ubd9, ub12, ubd10;
                                ubd9 = "mkdir -pv "+ruta+"%1/iso/";
                                ubd9 = ubd9.arg(Valor2);
                                ub12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                ubd10 = "file-roller --force --extract-to="+ruta+"%1/iso/%3%5/ /home/%2/tmp/%4";
                                ubd10 = ubd10.arg(Valor2).arg(user).arg(TipUbuntu).arg(Iso).arg(ui->comboBox_14->currentText());
                                comandos << ub3 << ubd1 << ub15 << ubd9 << ub12 << ubd10;
                            }
                        }
                    }
                }
                else if (ui->checkBox_4->isChecked() == false)
                {
                    TipUbuntu = "";
                    ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">4) Instalar Ubuntu: No"));
                }
                ui->textEdit_3->append("");
                if (ui->checkBox_5->isChecked() == true)
                {
                    ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">5) Instalar ArchLinux: Si"));
                    if (ui->radioButton_18->isChecked() == true)
                    {
                        if (ui->label_89->text() == "")
                        {
                            Control++;
                            ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">- ERROR(")+QString::number(Control)+tr("): Ruta iso/ArchLinux en equipo: NO SE HA DEFINIDO NINGUNA RUTA y NO SE PUEDE CONTINUAR"));
                        }
                        else if(ui->label_89->text() != "")
                        {                            
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", "ArchLinux_"+ui->comboBox_15->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == "ArchLinux_"+ui->comboBox_15->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                TipArchLinux = "ArchLinux_";
                                QFile file(ui->label_89->text());
                                qint64 size = QFileInfo(file).size();
                                float f;
                                f=(size + 1023) / 1024;
                                f=(f/1024)/1024;
                                TamanoTotal = TamanoTotal + f;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Ruta iso/ArchLinux en equipo: ")+ui->label_89->text()+"");
                                QString ar11 = tr("echo Creando directorio ArchLinux_"+ui->comboBox_15->currentText()+"...");
                                QString ard9, ar12, ard10;
                                ard9 = "mkdir -pv "+ruta+"%1/iso/";
                                ard9 = ard9.arg(Valor2);
                                ar12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                ard10 = "file-roller --force --extract-to="+ruta+"%1/iso/%4%5/ %3";
                                ard10 = ard10.arg(Valor2).arg(ui->label_89->text()).arg(TipArchLinux).arg(ui->comboBox_15->currentText());
                                comandos << ar11 << ard9 << ar12 << ard10;
                            }
                        }
                    }
                    else if (ui->radioButton_18->isChecked() == false)
                    {
                        ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Descargar iso/ArchLinux del servidor: Si"));
                        if (ui->comboBox_5->currentText() ==  "Dual")
                        {
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", "ArchLinux_"+ui->comboBox_15->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == "ArchLinux_"+ui->comboBox_15->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                QString ArchLinux;
                                TipArchLinux = "ArchLinux_";
                                QString ArchLinux1 = "ArchLinux";
                                QSqlQuery query(db);
                                query.exec("SELECT direccion FROM Distro WHERE NomDistro='"+ArchLinux1+"' AND Version='"+ui->comboBox_15->currentText()+"' AND Arquitectura='"+ui->comboBox_5->currentText()+"'");
                                query.first();
                                if (query.isValid())
                                    ArchLinux=query.value(0).toString();
                                QStringList ParcIso = ArchLinux.split("/");
                                int TamIso = ParcIso.count();
                                QString Iso = ParcIso.value(TamIso-1);
                                Tamano = ui->label_63->text()+" Mb";
                                QString TamanoMb = Tamano.remove(" ").remove("M").remove("b");
                                TamanoTotal = (TamanoMb.toFloat()/1024)+TamanoTotal;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Arquitectura: ")+ui->comboBox_5->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Version: ")+ui->comboBox_15->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Direccion de descarga: ")+ArchLinux+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Peso de la descarga: ")+Tamano+" Mb");
                                QString ar3 = tr("echo Descargando del servidor...");
                                QString ard1 = "wget -c -P/home/%1/tmp/ %2";
                                ard1=ard1.arg(user).arg(ArchLinux);
                                QString ar15 = tr("echo Creando directorio ArchLinux_"+ui->comboBox_15->currentText()+"...");
                                QString ard9, ar12, ard10;
                                ard9 = "mkdir -pv "+ruta+"%1/iso/";
                                ard9 = ard9.arg(Valor2);
                                ar12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                ard10 = "file-roller --force --extract-to="+ruta+"%1/iso/%3%5/ /home/%2/tmp/%4";
                                ard10 = ard10.arg(Valor2).arg(user).arg(TipArchLinux).arg(Iso).arg(ui->comboBox_15->currentText());
                                comandos << ar3 << ard1 << ar15 << ard9 << ar12 << ard10;
                            }
                        }
                    }
                }
                else if (ui->checkBox_5->isChecked() == false)
                {
                    TipArchLinux = "";
                    ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">5) Instalar ArchLinux: No"));
                }
                ui->textEdit_3->append("");
                if (ui->checkBox_6->isChecked() == true)
                {
                    ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">6) Instalar CentOS: Si"));
                    if (ui->radioButton_40->isChecked() == true)
                    {
                        if (ui->label_99->text() == "")
                        {
                            Control++;
                            ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">- ERROR(")+QString::number(Control)+tr("): Ruta iso/CentOS en equipo: NO SE HA DEFINIDO NINGUNA RUTA y NO SE PUEDE CONTINUAR."));
                        }
                        else if(ui->label_99->text() != "")
                        {
                            QString TipoArc;
                            if (ui->comboBox_6->currentText() ==  "32bit")
                                TipoArc = "CentOS32_";
                            else
                                TipoArc = "CentOS64_";
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", ""+TipoArc+""+ui->comboBox_16->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == ""+TipoArc+""+ui->comboBox_16->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                TipCentOS = TipoArc;
                                QFile file(ui->label_99->text());
                                qint64 size = QFileInfo(file).size();
                                float f;
                                f=(size + 1023) / 1024;
                                f=(f/1024)/1024;
                                TamanoTotal = TamanoTotal + f;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Ruta iso/CentOS en equipo: ")+ui->label_99->text()+"");
                                QString ct11 = tr("echo Creando directorio "+TipoArc+""+ui->comboBox_16->currentText()+"...");
                                QString ctd9, ct12, ctd10;
                                ctd9 = "mkdir -pv "+ruta+"%1/iso/";
                                ctd9 = ctd9.arg(Valor2);
                                ct12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                ctd10 = "file-roller --force --extract-to="+ruta+"%1/iso/%4%5/ %3";
                                ctd10 = ctd10.arg(Valor2).arg(ui->label_99->text()).arg(TipCentOS).arg(ui->comboBox_16->currentText());
                                comandos << ct11 << ctd9 << ct12 << ctd10;
                            }
                        }
                    }
                    else if (ui->radioButton_40->isChecked() == false)
                    {
                        ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Descargar iso/CentOS del servidor: Si"));
                        if (ui->comboBox_6->currentText() ==  "32bit")
                        {
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", "CentOS32_"+ui->comboBox_16->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == "CentOS32_"+ui->comboBox_16->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                QString CentOS;
                                TipCentOS = "CentOS32_";
                                QString CentOS1 = "CentOS";
                                QSqlQuery query(db);
                                query.exec("SELECT direccion FROM Distro WHERE NomDistro='"+CentOS1+"' AND Version='"+ui->comboBox_16->currentText()+"' AND Arquitectura='"+ui->comboBox_6->currentText()+"'");
                                query.first();
                                if (query.isValid())
                                    CentOS=query.value(0).toString();
                                QStringList ParcIso = CentOS.split("/");
                                int TamIso = ParcIso.count();
                                QString Iso = ParcIso.value(TamIso-1);
                                Tamano = ui->label_65->text()+" Mb";
                                QString TamanoMb = Tamano.remove(" ").remove("M").remove("b");
                                TamanoTotal = (TamanoMb.toFloat()/1024)+TamanoTotal;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Arquitectura: ")+ui->comboBox_6->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Version: ")+ui->comboBox_16->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Direccion de descarga: ")+CentOS+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Peso de la descarga: ")+Tamano+" Mb");
                                QString ct3 = tr("echo Descargando del servidor...");
                                QString ctd1 = "wget -c -P/home/%1/tmp/ %2";
                                ctd1=ctd1.arg(user).arg(CentOS);
                                QString ct15 = tr("echo Creando directorio CentOS32_"+ui->comboBox_16->currentText()+"...");
                                QString ctd9, ct12, ctd10;
                                ctd9 = "mkdir -pv "+ruta+"%1/iso/";
                                ctd9 = ctd9.arg(Valor2);
                                ct12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                ctd10 = "file-roller --force --extract-to="+ruta+"%1/iso/%3%5/ /home/%2/tmp/%4";
                                ctd10 = ctd10.arg(Valor2).arg(user).arg(TipCentOS).arg(Iso).arg(ui->comboBox_16->currentText());
                                comandos << ct3 << ctd1 << ct15 << ctd9 << ct12 << ctd10;
                            }
                        }
                        else if(ui->comboBox_6->currentText() == "64bit")
                        {
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", "CentOS64_"+ui->comboBox_16->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == "CentOS64_"+ui->comboBox_16->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                QString CentOS;
                                TipCentOS = "CentOS64_";
                                QString CentOS1 = "CentOS";
                                QSqlQuery query(db);
                                query.exec("SELECT direccion FROM Distro WHERE NomDistro='"+CentOS1+"' AND Version='"+ui->comboBox_16->currentText()+"' AND Arquitectura='"+ui->comboBox_6->currentText()+"'");
                                query.first();
                                if (query.isValid())
                                    CentOS=query.value(0).toString();
                                QStringList ParcIso = CentOS.split("/");
                                int TamIso = ParcIso.count();
                                QString Iso = ParcIso.value(TamIso-1);
                                Tamano = ui->label_65->text()+" Mb";
                                QString TamanoMb = Tamano.remove(" ").remove("M").remove("b");
                                TamanoTotal = (TamanoMb.toFloat()/1024)+TamanoTotal;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Arquitectura: ")+ui->comboBox_6->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Version: ")+ui->comboBox_16->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Direccion de descarga: ")+CentOS+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Peso de la descarga: ")+Tamano+" Mb");
                                QString ct3 = tr("echo Descargando del servidor...");
                                QString ctd1 = "wget -c -P/home/%1/tmp/ %2";
                                ctd1=ctd1.arg(user).arg(CentOS);
                                QString ct15 = tr("echo Creando directorio CentOS64_"+ui->comboBox_16->currentText()+"...");
                                QString ctd9, ct12, ctd10;
                                ctd9 = "mkdir -pv "+ruta+"%1/iso/";
                                ctd9 = ctd9.arg(Valor2);
                                ct12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                ctd10 = "file-roller --force --extract-to="+ruta+"%1/iso/%3%5/ /home/%2/tmp/%4";
                                ctd10 = ctd10.arg(Valor2).arg(user).arg(TipCentOS).arg(Iso).arg(ui->comboBox_16->currentText());
                                comandos << ct3 << ctd1 << ct15 << ctd9 << ct12 << ctd10;
                            }
                        }
                    }
                }
                else if (ui->checkBox_6->isChecked() == false)
                {
                    TipCentOS = "";
                    ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">6) Instalar CentOS: No"));
                }
                ui->textEdit_3->append("");
                if (ui->checkBox_7->isChecked() == true)
                {
                    ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">7) Instalar Debian: Si"));
                    if (ui->radioButton_44->isChecked() == true)
                    {
                        if (ui->label_108->text() == "")
                        {
                            Control++;
                            ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">- ERROR(")+QString::number(Control)+tr("): Ruta iso/Debian en equipo: NO SE HA DEFINIDO NINGUNA RUTA y NO SE PUEDE CONTINUAR."));
                        }
                        else if(ui->label_108->text() != "")
                        {
                            QString TipoArc;
                            if (ui->comboBox_7->currentText() ==  "32bit")
                                TipoArc = "Debian32_";
                            else
                                TipoArc = "Debian64_";
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", ""+TipoArc+""+ui->comboBox_17->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == ""+TipoArc+""+ui->comboBox_17->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                TipDebian = TipoArc;
                                QFile file(ui->label_108->text());
                                qint64 size = QFileInfo(file).size();
                                float f;
                                f=(size + 1023) / 1024;
                                f=(f/1024)/1024;
                                TamanoTotal = TamanoTotal + f;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Ruta iso/Debian en equipo: ")+ui->label_108->text()+"");
                                QString db11 = tr("echo Creando directorio "+TipoArc+""+ui->comboBox_17->currentText()+"...");
                                QString dbd9, db12, dbd10;
                                dbd9 = "mkdir -pv "+ruta+"%1/iso/";
                                dbd9 = dbd9.arg(Valor2);
                                db12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                dbd10 = "file-roller --force --extract-to="+ruta+"%1/iso/%4%5/ %3";
                                dbd10 = dbd10.arg(Valor2).arg(ui->label_108->text()).arg(TipDebian).arg(ui->comboBox_17->currentText());
                                comandos << db11 << dbd9 << db12 << dbd10;
                            }
                        }
                    }
                    else if (ui->radioButton_44->isChecked() == false)
                    {
                        ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Descargar iso/Debian del servidor: Si"));
                        if (ui->comboBox_7->currentText() ==  "32bit")
                        {
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", "Debian32_"+ui->comboBox_17->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == "Debian32_"+ui->comboBox_17->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                QString Debian;
                                TipDebian = "Debian32_";
                                QString Debian1 = "Debian";
                                QSqlQuery query(db);
                                query.exec("SELECT direccion FROM Distro WHERE NomDistro='"+Debian1+"' AND Version='"+ui->comboBox_17->currentText()+"' AND Arquitectura='"+ui->comboBox_7->currentText()+"'");
                                query.first();
                                if (query.isValid())
                                    Debian=query.value(0).toString();
                                QStringList ParcIso = Debian.split("/");
                                int TamIso = ParcIso.count();
                                QString Iso = ParcIso.value(TamIso-1);
                                Tamano = ui->label_67->text()+" Mb";
                                QString TamanoMb = Tamano.remove(" ").remove("M").remove("b");
                                TamanoTotal = (TamanoMb.toFloat()/1024)+TamanoTotal;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Arquitectura: ")+ui->comboBox_7->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Version: ")+ui->comboBox_17->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Direccion de descarga: ")+Debian+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Peso de la descarga: ")+Tamano+" Mb");
                                QString db3 = tr("echo Descargando del servidor...");
                                QString dbd1 = "wget -c -P/home/%1/tmp/ %2";
                                dbd1=dbd1.arg(user).arg(Debian);
                                QString db15 = tr("echo Creando directorio Debian32_"+ui->comboBox_17->currentText()+"...");
                                QString dbd9, db12, dbd10;
                                dbd9 = "mkdir -pv "+ruta+"%1/iso/";
                                dbd9 = dbd9.arg(Valor2);
                                db12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                dbd10 = "file-roller --force --extract-to="+ruta+"%1/iso/%3%5/ /home/%2/tmp/%4";
                                dbd10 = dbd10.arg(Valor2).arg(user).arg(TipDebian).arg(Iso).arg(ui->comboBox_17->currentText());
                                comandos << db3 << dbd1 << db15 << dbd9 << db12 << dbd10;
                            }
                        }
                        else if(ui->comboBox_7->currentText() == "64bit")
                        {
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", "Debian64_"+ui->comboBox_17->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == "Debian64_"+ui->comboBox_17->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                QString Debian;
                                TipDebian = "Debian64_";
                                QString Debian1 = "Debian";
                                QSqlQuery query(db);
                                query.exec("SELECT direccion FROM Distro WHERE NomDistro='"+Debian1+"' AND Version='"+ui->comboBox_17->currentText()+"' AND Arquitectura='"+ui->comboBox_7->currentText()+"'");
                                query.first();
                                if (query.isValid())
                                    Debian=query.value(0).toString();
                                QStringList ParcIso = Debian.split("/");
                                int TamIso = ParcIso.count();
                                QString Iso = ParcIso.value(TamIso-1);
                                Tamano = ui->label_67->text()+" Mb";
                                QString TamanoMb = Tamano.remove(" ").remove("M").remove("b");
                                TamanoTotal = (TamanoMb.toFloat()/1024)+TamanoTotal;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Arquitectura: ")+ui->comboBox_7->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Version: ")+ui->comboBox_17->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Direccion de descarga: ")+Debian+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Peso de la descarga: ")+Tamano+" Mb");
                                QString db3 = tr("echo Descargando del servidor...");
                                QString dbd1 = "wget -c -P/home/%1/tmp/ %2";
                                dbd1=dbd1.arg(user).arg(Debian);
                                QString db15 = tr("echo Creando directorio Debian64_"+ui->comboBox_17->currentText()+"...");
                                QString dbd9, db12, dbd10;
                                dbd9 = "mkdir -pv "+ruta+"%1/iso/";
                                dbd9 = dbd9.arg(Valor2);
                                db12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                dbd10 = "file-roller --force --extract-to="+ruta+"%1/iso/%3%5/ /home/%2/tmp/%4";
                                dbd10 = dbd10.arg(Valor2).arg(user).arg(TipDebian).arg(Iso).arg(ui->comboBox_17->currentText());
                                comandos << db3 << dbd1 << db15 << dbd9 << db12 << dbd10;
                            }
                        }
                    }
                }
                else if (ui->checkBox_7->isChecked() == false)
                {
                    TipDebian = "";
                    ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">7) Instalar Debian: No"));
                }
                ui->textEdit_3->append("");
                if (ui->checkBox_19->isChecked() == true)
                {
                    ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">8) Instalar Knoppix: Si"));
                    if (ui->radioButton_56->isChecked() == true)
                    {
                        if (ui->label_136->text() == "")
                        {
                            Control++;
                            ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">- ERROR(")+QString::number(Control)+tr("): Ruta iso/Knoppix en equipo: NO SE HA DEFINIDO NINGUNA RUTA y NO SE PUEDE CONTINUAR"));
                        }
                        else if(ui->label_136->text() != "")
                        {
                            QString TipoArc;
                            if (ui->comboBox_8->currentText() ==  "32bit")
                                TipoArc = "Knoppix32_";
                            else
                                TipoArc = "Knoppix64_";
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", ""+TipoArc+""+ui->comboBox_18->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == ""+TipoArc+""+ui->comboBox_18->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                TipKnoppix = TipoArc;
                                QFile file(ui->label_136->text());
                                qint64 size = QFileInfo(file).size();
                                float f;
                                f=(size + 1023) / 1024;
                                f=(f/1024)/1024;
                                TamanoTotal = TamanoTotal + f;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Ruta iso/Knoppix en equipo: ")+ui->label_136->text()+"");
                                QString kn11 = tr("echo Creando directorio "+TipoArc+""+ui->comboBox_18->currentText()+"...");
                                QString knd9, kn12, knd10;
                                knd9 = "mkdir -pv "+ruta+"%1/iso/";
                                knd9 = knd9.arg(Valor2);
                                kn12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                knd10 = "file-roller --force --extract-to="+ruta+"%1/iso/%4%5/ %3";
                                knd10 = knd10.arg(Valor2).arg(ui->label_136->text()).arg(TipKnoppix).arg(ui->comboBox_18->currentText());
                                comandos << kn11 << knd9 << kn12 << knd10;
                            }
                        }
                    }
                    else if (ui->radioButton_56->isChecked() == false)
                    {
                        ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Descargar iso/Knoppix del servidor: Si"));
                        if (ui->comboBox_8->currentText() ==  "32bit")
                        {
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", "Knoppix_"+ui->comboBox_18->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == "Knoppix_"+ui->comboBox_18->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                QString Knoppix;
                                TipKnoppix = "Knoppix32_";
                                QString Knoppix1 = "Knoppix";
                                QSqlQuery query(db);
                                query.exec("SELECT direccion FROM Distro WHERE NomDistro='"+Knoppix1+"' AND Version='"+ui->comboBox_18->currentText()+"' AND Arquitectura='"+ui->comboBox_8->currentText()+"'");
                                query.first();
                                if (query.isValid())
                                    Knoppix=query.value(0).toString();
                                QStringList ParcIso = Knoppix.split("/");
                                int TamIso = ParcIso.count();
                                QString Iso = ParcIso.value(TamIso-1);
                                Tamano = ui->label_69->text()+" Mb";
                                QString TamanoMb = Tamano.remove(" ").remove("M").remove("b");
                                TamanoTotal = (TamanoMb.toFloat()/1024)+TamanoTotal;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Arquitectura: ")+ui->comboBox_8->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Version: ")+ui->comboBox_18->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Direccion de descarga: ")+Knoppix+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Peso de la descarga: ")+Tamano+" Mb");
                                QString kn3 = tr("echo Descargando del servidor...");
                                QString knd1 = "wget -c -P/home/%1/tmp/ %2";
                                knd1=knd1.arg(user).arg(Knoppix);
                                QString kn15 = tr("echo Creando directorio Knoppix32_"+ui->comboBox_18->currentText()+"...");
                                QString knd9, kn12, knd10;
                                knd9 = "mkdir -pv "+ruta+"%1/iso/";
                                knd9 = knd9.arg(Valor2);
                                kn12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                knd10 = "file-roller --force --extract-to="+ruta+"%1/iso/%3%5/ /home/%2/tmp/%4";
                                knd10 = knd10.arg(Valor2).arg(user).arg(TipKnoppix).arg(Iso).arg(ui->comboBox_18->currentText());
                                comandos << kn3 << knd1 << kn15 << knd9 << kn12 << knd10;
                            }
                        }
                    }
                }
                else if (ui->checkBox_19->isChecked() == false)
                {
                    TipKnoppix = "";
                    ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">8) Instalar Knoppix: No"));
                }
                ui->textEdit_3->append("");
                if (ui->checkBox_9->isChecked() == true)
                {
                    ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">9) Instalar OpenSUSE: Si"));
                    if (ui->radioButton_52->isChecked() == true)
                    {
                        if (ui->label_127->text() == "")
                        {
                            Control++;
                            ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">- ERROR(")+QString::number(Control)+tr("): Ruta iso/OpenSUSE en equipo: NO SE HA DEFINIDO NINGUNA RUTA y NO SE PUEDE CONTINUAR."));
                        }
                        else if(ui->label_127->text() != "")
                        {
                            QString TipoArc;
                            if (ui->comboBox_9->currentText() ==  "32bit")
                                TipoArc = "OpenSUSE32_";
                            else
                                TipoArc = "OpenSUSE64_";
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", ""+TipoArc+""+ui->comboBox_19->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == ""+TipoArc+""+ui->comboBox_19->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                TipOpenSUSE = TipoArc;
                                QFile file(ui->label_127->text());
                                qint64 size = QFileInfo(file).size();
                                float f;
                                f=(size + 1023) / 1024;
                                f=(f/1024)/1024;
                                TamanoTotal = TamanoTotal + f;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Ruta iso/OpenSUSE en equipo: ")+ui->label_127->text()+"");
                                QString op11 = tr("echo Creando directorio "+TipoArc+""+ui->comboBox_19->currentText()+"...");
                                QString opd9, op12, opd10;
                                opd9 = "mkdir -pv "+ruta+"%1/iso/";
                                opd9 = opd9.arg(Valor2);
                                op12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                opd10 = "file-roller --force --extract-to="+ruta+"%1/iso/%4%5/ %3";
                                opd10 = opd10.arg(Valor2).arg(ui->label_127->text()).arg(TipOpenSUSE).arg(ui->comboBox_19->currentText());
                                comandos << op11 << opd9 << op12 << opd10;
                            }
                        }
                    }
                    else if (ui->radioButton_52->isChecked() == false)
                    {
                        ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Descargar iso/OpenSUSE del servidor: Si"));
                        if (ui->comboBox_9->currentText() ==  "32bit")
                        {
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", "OpenSUSE32_"+ui->comboBox_19->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == "OpenSUSE32_"+ui->comboBox_19->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                QString OpenSUSE;
                                TipOpenSUSE = "OpenSUSE32_";
                                QString OpenSUSE1 = "OpenSUSE";
                                QSqlQuery query(db);
                                query.exec("SELECT direccion FROM Distro WHERE NomDistro='"+OpenSUSE1+"' AND Version='"+ui->comboBox_19->currentText()+"' AND Arquitectura='"+ui->comboBox_9->currentText()+"'");
                                query.first();
                                if (query.isValid())
                                    OpenSUSE=query.value(0).toString();
                                QStringList ParcIso = OpenSUSE.split("/");
                                int TamIso = ParcIso.count();
                                QString Iso = ParcIso.value(TamIso-1);
                                Tamano = ui->label_71->text()+" Mb";
                                QString TamanoMb = Tamano.remove(" ").remove("M").remove("b");
                                TamanoTotal = (TamanoMb.toFloat()/1024)+TamanoTotal;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Arquitectura: ")+ui->comboBox_9->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Version: ")+ui->comboBox_19->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Direccion de descarga: ")+OpenSUSE+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Peso de la descarga: ")+Tamano+" Mb");
                                QString op3 = tr("echo Descargando del servidor...");
                                QString opd1 = "wget -c -P/home/%1/tmp/ %2";
                                opd1=opd1.arg(user).arg(OpenSUSE);
                                QString op15 = tr("echo Creando directorio OpenSUSE32_"+ui->comboBox_19->currentText()+"...");
                                QString opd9, op12, opd10;
                                opd9 = "mkdir -pv "+ruta+"%1/iso/";
                                opd9 = opd9.arg(Valor2);
                                op12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                opd10 = "file-roller --force --extract-to="+ruta+"%1/iso/%3%5/ /home/%2/tmp/%4";
                                opd10 = opd10.arg(Valor2).arg(user).arg(TipOpenSUSE).arg(Iso).arg(ui->comboBox_19->currentText());
                                comandos << op3 << opd1 << op15 << opd9 << op12 << opd10;
                            }
                        }
                        else if(ui->comboBox_9->currentText() == "64bit")
                        {
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", "OpenSUSE64_"+ui->comboBox_19->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == "OpenSUSE64_"+ui->comboBox_19->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                QString OpenSUSE;
                                TipOpenSUSE = "OpenSUSE64_";
                                QString OpenSUSE1 = "OpenSUSE";
                                QSqlQuery query(db);
                                query.exec("SELECT direccion FROM Distro WHERE NomDistro='"+OpenSUSE1+"' AND Version='"+ui->comboBox_19->currentText()+"' AND Arquitectura='"+ui->comboBox_9->currentText()+"'");
                                query.first();
                                if (query.isValid())
                                    OpenSUSE=query.value(0).toString();
                                QStringList ParcIso = OpenSUSE.split("/");
                                int TamIso = ParcIso.count();
                                QString Iso = ParcIso.value(TamIso-1);
                                Tamano = ui->label_71->text()+" Mb";
                                QString TamanoMb = Tamano.remove(" ").remove("M").remove("b");
                                TamanoTotal = (TamanoMb.toFloat()/1024)+TamanoTotal;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Arquitectura: ")+ui->comboBox_9->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Version: ")+ui->comboBox_19->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Direccion de descarga: ")+OpenSUSE+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Peso de la descarga: ")+Tamano+" Mb");
                                QString op3 = tr("echo Descargando del servidor...");
                                QString opd1 = "wget -c -P/home/%1/tmp/ %2";
                                opd1=opd1.arg(user).arg(OpenSUSE);
                                QString op15 = tr("echo Creando directorio OpenSUSE64_"+ui->comboBox_19->currentText()+"...");
                                QString opd9, op12, opd10;
                                opd9 = "mkdir -pv "+ruta+"%1/iso/";
                                opd9 = opd9.arg(Valor2);
                                op12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                opd10 = "file-roller --force --extract-to="+ruta+"%1/iso/%3%5/ /home/%2/tmp/%4";
                                opd10 = opd10.arg(Valor2).arg(user).arg(TipOpenSUSE).arg(Iso).arg(ui->comboBox_19->currentText());
                                comandos << op3 << opd1 << op15 << opd9 << op12 << opd10;
                            }
                        }
                    }
                }
                else if (ui->checkBox_9->isChecked() == false)
                {
                    TipOpenSUSE = "";
                    ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">9) Instalar OpenSUSE: No"));
                }
                ui->textEdit_3->append("");
                if (ui->checkBox_8->isChecked() == true)
                {
                    ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">10) Instalar LinuxMint: Si"));
                    if (ui->radioButton_48->isChecked() == true)
                    {
                        if (ui->label_118->text() == "")
                        {
                            Control++;
                            ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">- ERROR(")+QString::number(Control)+tr("): Ruta iso/LinuxMint en equipo: NO SE HA DEFINIDO NINGUNA RUTA y NO SE PUEDE CONTINUAR."));
                        }
                        else if(ui->label_118->text() != "")
                        {
                            QString TipoArc;
                            if (ui->comboBox_10->currentText() ==  "32bit")
                                TipoArc = "LinuxMint32_";
                            else
                                TipoArc = "LinuxMint64_";
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", ""+TipoArc+""+ui->comboBox_20->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == ""+TipoArc+""+ui->comboBox_20->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                TipLinuxMint = TipoArc;
                                QFile file(ui->label_118->text());
                                qint64 size = QFileInfo(file).size();
                                float f;
                                f=(size + 1023) / 1024;
                                f=(f/1024)/1024;
                                TamanoTotal = TamanoTotal + f;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Ruta iso/LinuxMint en equipo: ")+ui->label_118->text()+"");
                                QString lm11 = tr("echo Creando directorio "+TipoArc+""+ui->comboBox_20->currentText()+"...");
                                QString lmd9, lm12, lmd10;
                                lmd9 = "mkdir -pv "+ruta+"%1/iso/";
                                lmd9 = lmd9.arg(Valor2);
                                lm12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                lmd10 = "file-roller --force --extract-to="+ruta+"%1/iso/%4%5/ %3";
                                lmd10 = lmd10.arg(Valor2).arg(ui->label_118->text()).arg(TipLinuxMint).arg(ui->comboBox_20->currentText());
                                comandos << lm11 << lmd9 << lm12 << lmd10;
                            }
                        }
                    }
                    else if (ui->radioButton_48->isChecked() == false)
                    {
                        ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Descargar iso/LinuxMint del servidor: Si"));
                        if (ui->comboBox_10->currentText() ==  "32bit")
                        {
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", "LinuxMint32_"+ui->comboBox_20->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == "LinuxMint32_"+ui->comboBox_20->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                QString LinuxMint;
                                TipLinuxMint = "LinuxMint32_";
                                QString LinuxMint1 = "LinuxMint";
                                QSqlQuery query(db);
                                query.exec("SELECT direccion FROM Distro WHERE NomDistro='"+LinuxMint1+"' AND Version='"+ui->comboBox_20->currentText()+"' AND Arquitectura='"+ui->comboBox_10->currentText()+"'");
                                query.first();
                                if (query.isValid())
                                    LinuxMint=query.value(0).toString();
                                QStringList ParcIso = LinuxMint.split("/");
                                int TamIso = ParcIso.count();
                                QString Iso = ParcIso.value(TamIso-1);
                                Tamano = ui->label_73->text()+" Mb";
                                QString TamanoMb = Tamano.remove(" ").remove("M").remove("b");
                                TamanoTotal = (TamanoMb.toFloat()/1024)+TamanoTotal;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Arquitectura: ")+ui->comboBox_10->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Version: ")+ui->comboBox_20->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Direccion de descarga: ")+LinuxMint+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Peso de la descarga: ")+Tamano+" Mb");
                                QString lm3 = tr("echo Descargando del servidor...");
                                QString lmd1 = "wget -c -P/home/%1/tmp/ %2";
                                lmd1=lmd1.arg(user).arg(LinuxMint);
                                QString lm15 = tr("echo Creando directorio LinuxMint32_"+ui->comboBox_20->currentText()+"...");
                                QString lmd9, lm12, lmd10;
                                lmd9 = "mkdir -pv "+ruta+"%1/iso/";
                                lmd9 = lmd9.arg(Valor2);
                                lm12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                lmd10 = "file-roller --force --extract-to="+ruta+"%1/iso/%3%5/ /home/%2/tmp/%4";
                                lmd10 = lmd10.arg(Valor2).arg(user).arg(TipLinuxMint).arg(Iso).arg(ui->comboBox_20->currentText());
                                comandos << lm3 << lmd1 << lm15 << lmd9 << lm12 << lmd10;
                            }
                        }
                        else if(ui->comboBox_10->currentText() == "64bit")
                        {
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", "LinuxMint64_"+ui->comboBox_20->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == "LinuxMint64_"+ui->comboBox_20->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                QString LinuxMint;
                                TipLinuxMint = "LinuxMint64_";
                                QString LinuxMint1 = "LinuxMint";
                                QSqlQuery query(db);
                                query.exec("SELECT direccion FROM Distro WHERE NomDistro='"+LinuxMint1+"' AND Version='"+ui->comboBox_20->currentText()+"' AND Arquitectura='"+ui->comboBox_10->currentText()+"'");
                                query.first();
                                if (query.isValid())
                                    LinuxMint=query.value(0).toString();
                                QStringList ParcIso = LinuxMint.split("/");
                                int TamIso = ParcIso.count();
                                QString Iso = ParcIso.value(TamIso-1);
                                Tamano = ui->label_73->text()+" Mb";
                                QString TamanoMb = Tamano.remove(" ").remove("M").remove("b");
                                TamanoTotal = (TamanoMb.toFloat()/1024)+TamanoTotal;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Arquitectura: ")+ui->comboBox_10->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Version: ")+ui->comboBox_20->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Direccion de descarga: ")+LinuxMint+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Peso de la descarga: ")+Tamano+" Mb");
                                QString lm3 = tr("echo Descargando del servidor...");
                                QString lmd1 = "wget -c -P/home/%1/tmp/ %2";
                                lmd1=lmd1.arg(user).arg(LinuxMint);
                                QString lm15 = tr("echo Creando directorio LinuxMint64_"+ui->comboBox_20->currentText()+"...");
                                QString lmd9, lm12, lmd10;
                                lmd9 = "mkdir -pv "+ruta+"%1/iso/";
                                lmd9 = lmd9.arg(Valor2);
                                lm12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                lmd10 = "file-roller --force --extract-to="+ruta+"%1/iso/%3%5/ /home/%2/tmp/%4";
                                lmd10 = lmd10.arg(Valor2).arg(user).arg(TipLinuxMint).arg(Iso).arg(ui->comboBox_20->currentText());
                                comandos << lm3 << lmd1 << lm15 << lmd9 << lm12 << lmd10;
                            }
                        }
                    }
                }
                else if (ui->checkBox_8->isChecked() == false)
                {
                    TipLinuxMint = "";
                    ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">10) Instalar LinuxMint: No"));
                }
                ui->textEdit_3->append("");
                if (ui->checkBox_13->isChecked() == true)
                {
                    ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">11) Instalar Boot Repair Disk: Si"));
                    if (ui->radioButton_22->isChecked() == true)
                    {
                        if (ui->label_75->text() == "")
                        {
                            Control++;
                            ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">- ERROR(")+QString::number(Control)+tr("): Ruta iso/Boot Repair Disk en equipo: NO SE HA DEFINIDO NINGUNA RUTA y NO SE PUEDE CONTINUAR."));
                        }
                        else if(ui->label_75->text() != "")
                        {
                            QString TipoArc;
                            if (ui->comboBox_27->currentText() ==  "32bit")
                                TipoArc = "BootRepair32_";
                            else
                                TipoArc = "BootRepair64_";
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", ""+TipoArc+""+ui->comboBox_28->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == ""+TipoArc+""+ui->comboBox_28->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                TipBootRepair = TipoArc;
                                QFile file(ui->label_75->text());
                                qint64 size = QFileInfo(file).size();
                                float f;
                                f=(size + 1023) / 1024;
                                f=(f/1024)/1024;
                                TamanoTotal = TamanoTotal + f;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Ruta iso/Boot Repair Disk en equipo: ")+ui->label_75->text()+"");
                                QString lm11 = tr("echo Creando directorio "+TipoArc+""+ui->comboBox_28->currentText()+"...");
                                QString lmd9, lm12, lmd10;
                                lmd9 = "mkdir -pv "+ruta+"%1/iso/";
                                lmd9 = lmd9.arg(Valor2);
                                lm12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                lmd10 = "file-roller --force --extract-to="+ruta+"%1/iso/%4%5/ %3";
                                lmd10 = lmd10.arg(Valor2).arg(ui->label_75->text()).arg(TipBootRepair).arg(ui->comboBox_28->currentText());
                                comandos << lm11 << lmd9 << lm12 << lmd10;
                            }
                        }
                    }
                    else if (ui->radioButton_22->isChecked() == false)
                    {
                        ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Descargar iso/Boot Repair Disk del servidor: Si"));
                        if (ui->comboBox_27->currentText() ==  "32bit")
                        {
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", "BootRepair32_"+ui->comboBox_28->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == "BootRepair32_"+ui->comboBox_28->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                QString BootRepair;
                                TipBootRepair = "BootRepair32_";
                                QString BootRepair1 = "Boot Repair Disk";
                                QSqlQuery query(db);
                                query.exec("SELECT direccion FROM Distro WHERE NomDistro='"+BootRepair1+"' AND Version='"+ui->comboBox_28->currentText()+"' AND Arquitectura='"+ui->comboBox_27->currentText()+"'");
                                query.first();
                                if (query.isValid())
                                    BootRepair=query.value(0).toString();
                                QStringList ParcIso = BootRepair.split("/");
                                int TamIso = ParcIso.count();
                                QString Iso = ParcIso.value(TamIso-1);
                                Tamano = ui->label_74->text()+" Mb";
                                QString TamanoMb = Tamano.remove(" ").remove("M").remove("b");
                                TamanoTotal = (TamanoMb.toFloat()/1024)+TamanoTotal;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Arquitectura: ")+ui->comboBox_27->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Version: ")+ui->comboBox_28->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Direccion de descarga: ")+BootRepair+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Peso de la descarga: ")+Tamano+" Mb");
                                QString lm3 = tr("echo Descargando del servidor...");
                                QString lmd1 = "wget -c -P/home/%1/tmp/ %2";
                                lmd1=lmd1.arg(user).arg(BootRepair);
                                QString lm15 = tr("echo Creando directorio BootRepair32_"+ui->comboBox_28->currentText()+"...");
                                QString lmd9, lm12, lmd10;
                                lmd9 = "mkdir -pv "+ruta+"%1/iso/";
                                lmd9 = lmd9.arg(Valor2);
                                lm12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                lmd10 = "file-roller --force --extract-to="+ruta+"%1/iso/%3%5/ /home/%2/tmp/%4";
                                lmd10 = lmd10.arg(Valor2).arg(user).arg(TipBootRepair).arg(Iso).arg(ui->comboBox_28->currentText());
                                comandos << lm3 << lmd1 << lm15 << lmd9 << lm12 << lmd10;
                            }
                        }
                        else if(ui->comboBox_27->currentText() == "64bit")
                        {
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", "BootRepair64_"+ui->comboBox_28->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == "BootRepair64_"+ui->comboBox_28->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                QString BootRepair;
                                TipBootRepair = "BootRepair64_";
                                QString BootRepair1 = "Boot Repair Disk";
                                QSqlQuery query(db);
                                query.exec("SELECT direccion FROM Distro WHERE NomDistro='"+BootRepair1+"' AND Version='"+ui->comboBox_28->currentText()+"' AND Arquitectura='"+ui->comboBox_27->currentText()+"'");
                                query.first();
                                if (query.isValid())
                                    BootRepair=query.value(0).toString();
                                QStringList ParcIso = BootRepair.split("/");
                                int TamIso = ParcIso.count();
                                QString Iso = ParcIso.value(TamIso-1);
                                Tamano = ui->label_74->text()+" Mb";
                                QString TamanoMb = Tamano.remove(" ").remove("M").remove("b");
                                TamanoTotal = (TamanoMb.toFloat()/1024)+TamanoTotal;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Arquitectura: ")+ui->comboBox_27->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Version: ")+ui->comboBox_28->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Direccion de descarga: ")+BootRepair+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Peso de la descarga: ")+Tamano+" Mb");
                                QString lm3 = tr("echo Descargando del servidor...");
                                QString lmd1 = "wget -c -P/home/%1/tmp/ %2";
                                lmd1=lmd1.arg(user).arg(BootRepair);
                                QString lm15 = tr("echo Creando directorio BootRepair64_"+ui->comboBox_28->currentText()+"...");
                                QString lmd9, lm12, lmd10;
                                lmd9 = "mkdir -pv "+ruta+"%1/iso/";
                                lmd9 = lmd9.arg(Valor2);
                                lm12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                lmd10 = "file-roller --force --extract-to="+ruta+"%1/iso/%3%5/ /home/%2/tmp/%4";
                                lmd10 = lmd10.arg(Valor2).arg(user).arg(TipBootRepair).arg(Iso).arg(ui->comboBox_28->currentText());
                                comandos << lm3 << lmd1 << lm15 << lmd9 << lm12 << lmd10;
                            }
                        }
                    }
                }
                else if (ui->checkBox_13->isChecked() == false)
                {
                    TipBootRepair = "";
                    ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">11) Instalar Boot Repair Disk: No"));
                }
                ui->textEdit_3->append("");
                if (ui->checkBox_14->isChecked() == true)
                {
                    ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">12) Instalar Clonezilla: Si"));
                    if (ui->radioButton_24->isChecked() == true)
                    {
                        if (ui->label_85->text() == "")
                        {
                            Control++;
                            ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">- ERROR(")+QString::number(Control)+tr("): Ruta iso/Clonezilla en equipo: NO SE HA DEFINIDO NINGUNA RUTA y NO SE PUEDE CONTINUAR."));
                        }
                        else if(ui->label_85->text() != "")
                        {
                            QString TipoArc;
                            if (ui->comboBox_29->currentText() ==  "32bit")
                                TipoArc = "Clonezilla32_";
                            else
                                TipoArc = "Clonezilla64_";
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", ""+TipoArc+""+ui->comboBox_30->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == ""+TipoArc+""+ui->comboBox_30->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                TipClonezilla = TipoArc;
                                QFile file(ui->label_85->text());
                                qint64 size = QFileInfo(file).size();
                                float f;
                                f=(size + 1023) / 1024;
                                f=(f/1024)/1024;
                                TamanoTotal = TamanoTotal + f;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Ruta iso/Clonezilla en equipo: ")+ui->label_85->text()+"");
                                QString lm11 = tr("echo Creando directorio "+TipoArc+""+ui->comboBox_30->currentText()+"...");
                                QString lmd9, lm12, lmd10;
                                lmd9 = "mkdir -pv "+ruta+"%1/iso/";
                                lmd9 = lmd9.arg(Valor2);
                                lm12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                lmd10 = "file-roller --force --extract-to="+ruta+"%1/iso/%4%5/ %3";
                                lmd10 = lmd10.arg(Valor2).arg(ui->label_85->text()).arg(TipClonezilla).arg(ui->comboBox_30->currentText());
                                comandos << lm11 << lmd9 << lm12 << lmd10;
                            }
                        }
                    }
                    else if (ui->radioButton_24->isChecked() == false)
                    {
                        ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Descargar iso/Clonezilla del servidor: Si"));
                        if (ui->comboBox_29->currentText() ==  "32bit")
                        {
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", "Clonezilla32_"+ui->comboBox_30->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == "Clonezilla32_"+ui->comboBox_30->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                QString Clonezilla;
                                TipBootRepair = "Clonezilla32_";
                                QString Clonezilla1 = "Clonezilla";
                                QSqlQuery query(db);
                                query.exec("SELECT direccion FROM Distro WHERE NomDistro='"+Clonezilla1+"' AND Version='"+ui->comboBox_30->currentText()+"' AND Arquitectura='"+ui->comboBox_29->currentText()+"'");
                                query.first();
                                if (query.isValid())
                                    Clonezilla=query.value(0).toString();
                                QStringList ParcIso = Clonezilla.split("/");
                                int TamIso = ParcIso.count();
                                QString Iso = ParcIso.value(TamIso-1);
                                Tamano = ui->label_84->text()+" Mb";
                                QString TamanoMb = Tamano.remove(" ").remove("M").remove("b");
                                TamanoTotal = (TamanoMb.toFloat()/1024)+TamanoTotal;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Arquitectura: ")+ui->comboBox_29->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Version: ")+ui->comboBox_30->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Direccion de descarga: ")+Clonezilla+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Peso de la descarga: ")+Tamano+" Mb");
                                QString lm3 = tr("echo Descargando del servidor...");
                                QString lmd1 = "wget -c -P/home/%1/tmp/ %2";
                                lmd1=lmd1.arg(user).arg(Clonezilla);
                                QString lm15 = tr("echo Creando directorio Clonezilla32_"+ui->comboBox_30->currentText()+"...");
                                QString lmd9, lm12, lmd10;
                                lmd9 = "mkdir -pv "+ruta+"%1/iso/";
                                lmd9 = lmd9.arg(Valor2);
                                lm12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                lmd10 = "file-roller --force --extract-to="+ruta+"%1/iso/%3%5/ /home/%2/tmp/%4";
                                lmd10 = lmd10.arg(Valor2).arg(user).arg(TipClonezilla).arg(Iso).arg(ui->comboBox_30->currentText());
                                comandos << lm3 << lmd1 << lm15 << lmd9 << lm12 << lmd10;
                            }
                        }
                        else if(ui->comboBox_29->currentText() == "64bit")
                        {
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", "Clonezilla64_"+ui->comboBox_30->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == "Clonezilla64_"+ui->comboBox_30->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                QString Clonezilla;
                                TipBootRepair = "Clonezilla64_";
                                QString Clonezilla1 = "Clonezilla";
                                QSqlQuery query(db);
                                query.exec("SELECT direccion FROM Distro WHERE NomDistro='"+Clonezilla1+"' AND Version='"+ui->comboBox_30->currentText()+"' AND Arquitectura='"+ui->comboBox_29->currentText()+"'");
                                query.first();
                                if (query.isValid())
                                    Clonezilla=query.value(0).toString();
                                QStringList ParcIso = Clonezilla.split("/");
                                int TamIso = ParcIso.count();
                                QString Iso = ParcIso.value(TamIso-1);
                                Tamano = ui->label_84->text()+" Mb";
                                QString TamanoMb = Tamano.remove(" ").remove("M").remove("b");
                                TamanoTotal = (TamanoMb.toFloat()/1024)+TamanoTotal;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Arquitectura: ")+ui->comboBox_29->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Version: ")+ui->comboBox_30->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Direccion de descarga: ")+Clonezilla+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Peso de la descarga: ")+Tamano+" Mb");
                                QString lm3 = tr("echo Descargando del servidor...");
                                QString lmd1 = "wget -c -P/home/%1/tmp/ %2";
                                lmd1=lmd1.arg(user).arg(Clonezilla);
                                QString lm15 = tr("echo Creando directorio Clonezilla64_"+ui->comboBox_30->currentText()+"...");
                                QString lmd9, lm12, lmd10;
                                lmd9 = "mkdir -pv "+ruta+"%1/iso/";
                                lmd9 = lmd9.arg(Valor2);
                                lm12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                lmd10 = "file-roller --force --extract-to="+ruta+"%1/iso/%3%5/ /home/%2/tmp/%4";
                                lmd10 = lmd10.arg(Valor2).arg(user).arg(TipClonezilla).arg(Iso).arg(ui->comboBox_30->currentText());
                                comandos << lm3 << lmd1 << lm15 << lmd9 << lm12 << lmd10;
                            }
                        }
                    }
                }
                else if (ui->checkBox_14->isChecked() == false)
                {
                    TipClonezilla = "";
                    ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">12) Instalar Clonezilla: No"));
                }
                ui->textEdit_3->append("");
                if (ui->checkBox_15->isChecked() == true)
                {
                    ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">13) Instalar GParted: Si"));
                    if (ui->radioButton_26->isChecked() == true)
                    {
                        if (ui->label_88->text() == "")
                        {
                            Control++;
                            ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">- ERROR(")+QString::number(Control)+tr("): Ruta iso/GParted en equipo: NO SE HA DEFINIDO NINGUNA RUTA y NO SE PUEDE CONTINUAR."));
                        }
                        else if(ui->label_88->text() != "")
                        {
                            QString TipoArc;
                            if (ui->comboBox_31->currentText() ==  "32bit")
                                TipoArc = "GParted32_";
                            else
                                TipoArc = "GParted64_";
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", ""+TipoArc+""+ui->comboBox_32->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == ""+TipoArc+""+ui->comboBox_32->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                TipGParted = TipoArc;
                                QFile file(ui->label_88->text());
                                qint64 size = QFileInfo(file).size();
                                float f;
                                f=(size + 1023) / 1024;
                                f=(f/1024)/1024;
                                TamanoTotal = TamanoTotal + f;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Ruta iso/GParted en equipo: ")+ui->label_88->text()+"");
                                QString lm11 = tr("echo Creando directorio "+TipoArc+""+ui->comboBox_32->currentText()+"...");
                                QString lmd9, lm12, lmd10;
                                lmd9 = "mkdir -pv "+ruta+"%1/iso/";
                                lmd9 = lmd9.arg(Valor2);
                                lm12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                lmd10 = "file-roller --force --extract-to="+ruta+"%1/iso/%4%5/ %3";
                                lmd10 = lmd10.arg(Valor2).arg(ui->label_88->text()).arg(TipGParted).arg(ui->comboBox_32->currentText());
                                comandos << lm11 << lmd9 << lm12 << lmd10;
                            }
                        }
                    }
                    else if (ui->radioButton_26->isChecked() == false)
                    {
                        ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Descargar iso/GParted del servidor: Si"));
                        if (ui->comboBox_31->currentText() ==  "32bit")
                        {
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", "GParted32_"+ui->comboBox_32->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == "GParted32_"+ui->comboBox_32->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                QString GParted;
                                TipGParted = "GParted32_";
                                QString GParted1 = "GParted";
                                QSqlQuery query(db);
                                query.exec("SELECT direccion FROM Distro WHERE NomDistro='"+GParted1+"' AND Version='"+ui->comboBox_32->currentText()+"' AND Arquitectura='"+ui->comboBox_31->currentText()+"'");
                                query.first();
                                if (query.isValid())
                                    GParted=query.value(0).toString();
                                QStringList ParcIso = GParted.split("/");
                                int TamIso = ParcIso.count();
                                QString Iso = ParcIso.value(TamIso-1);
                                Tamano = ui->label_87->text()+" Mb";
                                QString TamanoMb = Tamano.remove(" ").remove("M").remove("b");
                                TamanoTotal = (TamanoMb.toFloat()/1024)+TamanoTotal;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Arquitectura: ")+ui->comboBox_31->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Version: ")+ui->comboBox_32->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Direccion de descarga: ")+GParted+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Peso de la descarga: ")+Tamano+" Mb");
                                QString lm3 = tr("echo Descargando del servidor...");
                                QString lmd1 = "wget -c -P/home/%1/tmp/ %2";
                                lmd1=lmd1.arg(user).arg(GParted);
                                QString lm15 = tr("echo Creando directorio GParted32_"+ui->comboBox_32->currentText()+"...");
                                QString lmd9, lm12, lmd10;
                                lmd9 = "mkdir -pv "+ruta+"%1/iso/";
                                lmd9 = lmd9.arg(Valor2);
                                lm12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                lmd10 = "file-roller --force --extract-to="+ruta+"%1/iso/%3%5/ /home/%2/tmp/%4";
                                lmd10 = lmd10.arg(Valor2).arg(user).arg(TipGParted).arg(Iso).arg(ui->comboBox_32->currentText());
                                comandos << lm3 << lmd1 << lm15 << lmd9 << lm12 << lmd10;
                            }
                        }
                        else if(ui->comboBox_31->currentText() == "64bit")
                        {
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", "GParted64_"+ui->comboBox_32->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == "GParted64_"+ui->comboBox_32->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                QString GParted;
                                TipGParted = "GParted64_";
                                QString GParted1 = "GParted";
                                QSqlQuery query(db);
                                query.exec("SELECT direccion FROM Distro WHERE NomDistro='"+GParted1+"' AND Version='"+ui->comboBox_32->currentText()+"' AND Arquitectura='"+ui->comboBox_31->currentText()+"'");
                                query.first();
                                if (query.isValid())
                                    GParted=query.value(0).toString();
                                QStringList ParcIso = GParted.split("/");
                                int TamIso = ParcIso.count();
                                QString Iso = ParcIso.value(TamIso-1);
                                Tamano = ui->label_87->text()+" Mb";
                                QString TamanoMb = Tamano.remove(" ").remove("M").remove("b");
                                TamanoTotal = (TamanoMb.toFloat()/1024)+TamanoTotal;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Arquitectura: ")+ui->comboBox_31->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Version: ")+ui->comboBox_32->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Direccion de descarga: ")+GParted+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Peso de la descarga: ")+Tamano+" Mb");
                                QString lm3 = tr("echo Descargando del servidor...");
                                QString lmd1 = "wget -c -P/home/%1/tmp/ %2";
                                lmd1=lmd1.arg(user).arg(GParted);
                                QString lm15 = tr("echo Creando directorio GParted64_"+ui->comboBox_32->currentText()+"...");
                                QString lmd9, lm12, lmd10;
                                lmd9 = "mkdir -pv "+ruta+"%1/iso/";
                                lmd9 = lmd9.arg(Valor2);
                                lm12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                lmd10 = "file-roller --force --extract-to="+ruta+"%1/iso/%3%5/ /home/%2/tmp/%4";
                                lmd10 = lmd10.arg(Valor2).arg(user).arg(TipGParted).arg(Iso).arg(ui->comboBox_32->currentText());
                                comandos << lm3 << lmd1 << lm15 << lmd9 << lm12 << lmd10;
                            }
                        }
                    }
                }
                else if (ui->checkBox_15->isChecked() == false)
                {
                    TipGParted = "";
                    ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">13) Instalar GParted: No"));
                }
                ui->textEdit_3->append("");
                if (ui->checkBox_17->isChecked() == true)
                {
                    ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">14) Instalar System Rescue CD: Si"));
                    if (ui->radioButton_30->isChecked() == true)
                    {
                        if (ui->label_79->text() == "")
                        {
                            Control++;
                            ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">- ERROR(")+QString::number(Control)+tr("): Ruta iso/System Rescue CD en equipo: NO SE HA DEFINIDO NINGUNA RUTA y NO SE PUEDE CONTINUAR."));
                        }
                        else if(ui->label_79->text() != "")
                        {
                            QString TipoArc;
                            if (ui->comboBox_35->currentText() ==  "32bit")
                                TipoArc = "SystemRescue32_";
                            else
                                TipoArc = "SystemRescue64_";
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", ""+TipoArc+""+ui->comboBox_36->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == ""+TipoArc+""+ui->comboBox_36->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                TipSystemRescue = TipoArc;
                                QFile file(ui->label_79->text());
                                qint64 size = QFileInfo(file).size();
                                float f;
                                f=(size + 1023) / 1024;
                                f=(f/1024)/1024;
                                TamanoTotal = TamanoTotal + f;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Ruta iso/System Rescue CD en equipo: ")+ui->label_79->text()+"");
                                QString lm11 = tr("echo Creando directorio "+TipoArc+""+ui->comboBox_36->currentText()+"...");
                                QString lmd9, lm12, lmd10;
                                lmd9 = "mkdir -pv "+ruta+"%1/iso/";
                                lmd9 = lmd9.arg(Valor2);
                                lm12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                lmd10 = "file-roller --force --extract-to="+ruta+"%1/iso/%4%5/ %3";
                                lmd10 = lmd10.arg(Valor2).arg(ui->label_79->text()).arg(TipSystemRescue).arg(ui->comboBox_36->currentText());
                                comandos << lm11 << lmd9 << lm12 << lmd10;
                            }
                        }
                    }
                    else if (ui->radioButton_30->isChecked() == false)
                    {
                        ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Descargar iso/System Rescue CD del servidor: Si"));
                        if (ui->comboBox_35->currentText() == "64bit")
                        {
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", "SystemRescue64_"+ui->comboBox_36->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == "SystemRescue64_"+ui->comboBox_36->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                QString SystemRescue;
                                TipSystemRescue = "SystemRescue64_";
                                QString SystemRescue1 = "System Rescue CD";
                                QSqlQuery query(db);
                                query.exec("SELECT direccion FROM Distro WHERE NomDistro='"+SystemRescue1+"' AND Version='"+ui->comboBox_36->currentText()+"' AND Arquitectura='"+ui->comboBox_35->currentText()+"'");
                                query.first();
                                if (query.isValid())
                                    SystemRescue=query.value(0).toString();
                                QStringList ParcIso = SystemRescue.split("/");
                                int TamIso = ParcIso.count();
                                QString Iso = ParcIso.value(TamIso-1);
                                Tamano = ui->label_78->text()+" Mb";
                                QString TamanoMb = Tamano.remove(" ").remove("M").remove("b");
                                TamanoTotal = (TamanoMb.toFloat()/1024)+TamanoTotal;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Arquitectura: ")+ui->comboBox_35->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Version: ")+ui->comboBox_36->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Direccion de descarga: ")+SystemRescue+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Peso de la descarga: ")+Tamano+" Mb");
                                QString lm3 = tr("echo Descargando del servidor...");
                                QString lmd1 = "wget -c -P/home/%1/tmp/ %2";
                                lmd1=lmd1.arg(user).arg(SystemRescue);
                                QString lm15 = tr("echo Creando directorio SystemRescue64_"+ui->comboBox_36->currentText()+"...");
                                QString lmd9, lm12, lmd10;
                                lmd9 = "mkdir -pv "+ruta+"%1/iso/";
                                lmd9 = lmd9.arg(Valor2);
                                lm12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                lmd10 = "file-roller --force --extract-to="+ruta+"%1/iso/%3%5/ /home/%2/tmp/%4";
                                lmd10 = lmd10.arg(Valor2).arg(user).arg(TipSystemRescue).arg(Iso).arg(ui->comboBox_36->currentText());
                                comandos << lm3 << lmd1 << lm15 << lmd9 << lm12 << lmd10;
                            }
                        }
                    }
                }
                else if (ui->checkBox_17->isChecked() == false)
                {
                    TipSystemRescue = "";
                    ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">14) Instalar System Rescue CD: No"));
                }
                ui->textEdit_3->append("");
                if (ui->checkBox_10->isChecked() == true)
                {
                    ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">15) Instalar OpenMandriva: Si"));
                    if (ui->radioButton_3->isChecked() == true)
                    {
                        if (ui->label_48->text() == "")
                        {
                            Control++;
                            ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">- ERROR(")+QString::number(Control)+tr("): Ruta iso/OpenMandriva en equipo: NO SE HA DEFINIDO NINGUNA RUTA y NO SE PUEDE CONTINUAR."));
                        }
                        else if(ui->label_48->text() != "")
                        {
                            QString TipoArc;
                            if (ui->comboBox_21->currentText() ==  "32bit")
                                TipoArc = "OpenMandriva32_";
                            else
                                TipoArc = "OpenMandriva64_";
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", ""+TipoArc+""+ui->comboBox_22->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == ""+TipoArc+""+ui->comboBox_22->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira."));
                            else
                            {
                                TipOpenMandriva = TipoArc;
                                QFile file(ui->label_48->text());
                                qint64 size = QFileInfo(file).size();
                                float f;
                                f=(size + 1023) / 1024;
                                f=(f/1024)/1024;
                                TamanoTotal = TamanoTotal + f;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Ruta iso/OpenMandriva en equipo: ")+ui->label_48->text()+"");
                                QString md11 = tr("echo Creando directorio "+TipoArc+""+ui->comboBox_22->currentText()+"...");
                                QString mdd9, md12, mdd10;
                                mdd9 = "mkdir -pv "+ruta+"%1/iso/";
                                mdd9 = mdd9.arg(Valor2);
                                md12 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                mdd10 = "file-roller --force --extract-to="+ruta+"%1/iso/%4%5/ %3";
                                mdd10 = mdd10.arg(Valor2).arg(ui->label_48->text()).arg(TipOpenMandriva).arg(ui->comboBox_22->currentText());
                                comandos << md11 << mdd9 << md12 << mdd10;
                            }
                        }
                    }
                    else if (ui->radioButton_3->isChecked() == false)
                    {
                        ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Descargar iso/OpenMandriva del servidor: Si"));
                        if (ui->comboBox_21->currentText() ==  "32bit")
                        {
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", "OpenMandriva32_"+ui->comboBox_22->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == "OpenMandriva32_"+ui->comboBox_22->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira."));
                            else
                            {
                                QString OpenMandriva;
                                TipOpenMandriva = "OpenMandriva32_";
                                QString OpenMandriva1 = "OpenMandriva";
                                QSqlQuery query(db);
                                query.exec("SELECT Direccion FROM Distro WHERE NomDistro='"+OpenMandriva1+"' AND Version='"+ui->comboBox_22->currentText()+"' AND Arquitectura='"+ui->comboBox_21->currentText()+"'");
                                query.first();
                                if (query.isValid())
                                    OpenMandriva=query.value(0).toString();
                                QStringList ParcIso = OpenMandriva.split("/");
                                int TamIso = ParcIso.count();
                                QString Iso = ParcIso.value(TamIso-1);
                                Tamano = ui->label_55->text()+" Mb";
                                QString TamanoMb = Tamano.remove(" ").remove("M").remove("b");
                                TamanoTotal = (TamanoMb.toFloat()/1024)+TamanoTotal;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Arquitectura: ")+ui->comboBox_21->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Version: ")+ui->comboBox_22->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Direccion de descarga: ")+OpenMandriva+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Peso de la descarga: ")+Tamano+" Mb");
                                QString md3 = tr("echo Descargando del servidor...");
                                QString mdd1 = "wget -c -P/home/%1/tmp/ %2";
                                mdd1=mdd1.arg(user).arg(OpenMandriva);
                                QString md15 = tr("echo Creando directorio OpenMandriva32_"+ui->comboBox_22->currentText()+"...");
                                QString mdd13, md16, mdd14;
                                mdd13 = "mkdir -pv "+ruta+"%1/iso/";
                                mdd13 = mdd13.arg(Valor2);
                                md16 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                mdd14 = "file-roller --force --extract-to="+ruta+"%1/iso/%3%5/ /home/%2/tmp/%4";
                                mdd14 = mdd14.arg(Valor2).arg(user).arg(TipOpenMandriva).arg(Iso).arg(ui->comboBox_22->currentText());
                                comandos << md3 << mdd1 << md15 << mdd13 << md16 << mdd14;
                            }
                        }
                        else if(ui->comboBox_21->currentText() == "64bit")
                        {
                            QString Localizar = getDistroIns(""+ruta+""+Valor2+"/iso/", "OpenMandriva64_"+ui->comboBox_22->currentText()+"");
                            Localizar=Localizar.remove("/");
                            if (Localizar == "OpenMandriva64_"+ui->comboBox_22->currentText()+"")
                                 ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">Ya esta instalada, se omitira"));
                            else
                            {
                                QString OpenMandriva;
                                TipOpenMandriva = "OpenMandriva64_";
                                QString OpenMandriva1 = "OpenMandriva";
                                QSqlQuery query(db);
                                query.exec("SELECT direccion FROM Distro WHERE NomDistro='"+OpenMandriva1+"' AND Version='"+ui->comboBox_22->currentText()+"' AND Arquitectura='"+ui->comboBox_21->currentText()+"'");
                                query.first();
                                if (query.isValid())
                                        OpenMandriva=query.value(0).toString();
                                QStringList ParcIso = OpenMandriva.split("/");
                                int TamIso = ParcIso.count();
                                QString Iso = ParcIso.value(TamIso-1);
                                Tamano = ui->label_55->text()+" Mb";
                                QString TamanoMb = Tamano.remove(" ").remove("M").remove("b");
                                TamanoTotal = (TamanoMb.toFloat()/1024)+TamanoTotal;
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Arquitectura: ")+ui->comboBox_21->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Version: ")+ui->comboBox_22->currentText()+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Direccion de descarga: ")+OpenMandriva+"");
                                ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">- Peso de la descarga: ")+Tamano+" Mb");
                                QString md3 = tr("echo Descargando del servidor...");
                                QString mdd1 = "wget -c -P/home/%1/tmp/ %2";
                                mdd1=mdd1.arg(user).arg(OpenMandriva);
                                QString md15 = tr("echo Creando directorio OpenMandriva64_"+ui->comboBox_22->currentText()+"...");
                                QString mdd13, md16, mdd14;
                                mdd13 = "mkdir -pv "+ruta+"%1/iso/";
                                mdd13 = mdd13.arg(Valor2);
                                md16 = tr("echo Copiando ficheros al pendrive. Puede tardar un tiempo, segun capacidad de la iso...");
                                mdd14 = "file-roller --force --extract-to="+ruta+"%1/iso/%3%5/ /home/%2/tmp/%4";
                                mdd14 = mdd14.arg(Valor2).arg(user).arg(TipOpenMandriva).arg(Iso).arg(ui->comboBox_22->currentText());
                                comandos << md3 << mdd1 << md15 << mdd13 << md16 << mdd14;
                            }
                        }
                    }
                }
                else if (ui->checkBox_10->isChecked() == false)
                {
                    TipOpenMandriva = "";
                    ui->textEdit_3->append(tr("<FONT COLOR=\"RED\">15) Instalar OpenMandriva: No"));
                }
                ui->textEdit_3->append("");
                float Total;
                activo5 = activo5.remove("G");
                Total = activo5.toFloat();
                QString Ocupado = getSize(ruta+"DRAKLIVE");
                if (Ocupado != "")
                    TamanoTotal = TamanoTotal + Ocupado.toDouble();
                if (TamanoTotal > Total)
                {
                    Control++;
                    ui->textEdit_3->append("<FONT COLOR=\"RED\">ERROR("+QString::number(Control)+tr("): Total Gigas a grabar e instaladas (")+QString::number(TamanoTotal,'f',3)+tr(" Gb) superior a la capacidad del pendrive (")+activo5+" Gb)");
                    ui->textEdit_3->append("");
                }
                else if (TamanoTotal < Total)
                {
                    ui->textEdit_3->append(tr("<FONT COLOR=\"BLUE\">Total Gigas a grabar en Pendrive (incluido lo instalado): ")+QString::number(TamanoTotal,'f',3)+tr(" Gb de (")+activo5+" Gb)");
                    ui->textEdit_3->append("");
                }
                if (TamanoTotal == 0)
                {
                    Control++;
                    ui->textEdit_3->append("<FONT COLOR=\"RED\">ERROR("+QString::number(Control)+tr("): Ninguna Distro que instalar o ya estan instaladas, revisa las distros marcadas. No se puede continuar."));
                    ui->textEdit_3->append("");
                }
                if (Control > 0)
                    ui->label_5->setText(tr("<FONT COLOR=\"RED\">Datos de Configuracion (Hay ")+QString::number(Control)+tr(" errores sin solucionar):"));
                if (ui->radioButton_12->isChecked() == true)
                {
                    QString TipGrub1 = getGrub("/sbin/grub-install");
                    if (ui->radioButton_8->isChecked() == false)
                    {
                        if (TipGrub1 == "")
                        {
                            QMessageBox m;
                            if (Stilo == "A")
                                m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
                            m.setText(tr("No tienes instalado grub en tu sistema y es necesario para este proposito. Por favor instalalo si quieres continuar."));
                            m.exec();
                            return;
                        }
                        else if (TipGrub1 == "/sbin/grub-install")
                        {
                            QString gru1 = "echo instalando grub en el USB...";
                            QString grub1 = "/sbin/grub-install --root-directory="+ruta+"%1 %2 --no-floppy";
                            grub1 = grub1.arg(Valor2).arg(Valor1);
                            comandos << gru1 << grub1;
                        }
                        QString Kill = "echo Creando fichero de mapeo de unidades y menu.lst...";
                        QString Kill2 = "touch "+ruta+"%1/boot/grub/menu.lst";
                        Kill2 = Kill2.arg(Valor2);
                        QString device0 = "echo Introduciendo datos de mapeo y configuracion de inicio de distros para grub...";
                        comandos << Kill << Kill2 << device0;
                    }
                    else if (ui->radioButton_8->isChecked() == true)
                    {
                        QString Kill1 = "mv "+ruta+"%1/boot/grub/menu.lst "+ruta+"%1";
                        Kill1 = Kill1.arg(Valor2);
                        QString Kill0 = "rm -vR -f "+ruta+"%1/boot/";
                        Kill0 = Kill0.arg(Valor2);
                        comandos << Kill1 << Kill0;
                        QString gru2 = "echo Reinstalando grub en el USB...";
                        QString grub2 = "/sbin/grub-install --root-directory="+ruta+"%1 %2 --no-floppy";
                        grub2 = grub2.arg(Valor2).arg(Valor1);
                        QString grub3 = "mv "+ruta+"%1/menu.lst "+ruta+"%1/boot/grub/";
                        grub3 = grub3.arg(Valor2);
                        comandos << gru2 << grub2 << grub3;
                        QString device0 = "echo Actualizando datos de mapeo de grub...";
                        QString reelido = "echo Realizando proceso de reinstalado en device.map...";
                        comandos << device0 << reelido;
                    }
                    QString procesando = "echo Realizando proceso de insercion de datos de distros en menu.lst...";
                    comandos << procesando;
                    if (ui->radioButton_8->isChecked() == false)
                       grub << "timeout 10" << "#color black/cyan yellow/cyan" << "default 0" << "";
                    if (TipMandriva !="")
                    {
                        QString Mandriva;
                        QString Mandriva1 = "Mandriva";
                        TipMandriva = ""+TipMandriva+""+ui->comboBox_11->currentText()+"";
                        QSqlQuery query(db);
                        query.exec("SELECT grub FROM Distro WHERE NomDistro='"+Mandriva1+"' AND Version='"+ui->comboBox_11->currentText()+"' AND Arquitectura='"+ui->comboBox->currentText()+"'");
                        query.first();
                        if (query.isValid())
                            Mandriva=query.value(0).toString();
                        QStringList MandrivaDat = Mandriva.split("\n");
                        for(int i=0;i<MandrivaDat.count();i++)
                        {
                            QString Value = MandrivaDat.value(i);
                            Value.replace(QString("MANDRIVA"), QString(TipMandriva));
                            Value.replace(QString("ETIQUETA"), QString(Valor2));
                            Value.replace("\n","");
                            grub << ""+Value+"";
                        }
                        grub << "#";
                    }
                    if (TipPCLinuxOS != "")
                    {
                        QString PCLinuxOS;
                        QString PCLinuxOS1 = "PCLinuxOS";
                        TipPCLinuxOS = ""+TipPCLinuxOS+""+ui->comboBox_12->currentText()+"";
                        QSqlQuery query(db);
                        query.exec("SELECT grub FROM Distro WHERE NomDistro='"+PCLinuxOS1+"' AND Version='"+ui->comboBox_12->currentText()+"' AND Arquitectura='"+ui->comboBox_2->currentText()+"'");
                        query.first();
                        if (query.isValid())
                            PCLinuxOS=query.value(0).toString();                        
                        QStringList PCLinuxOSDat  = PCLinuxOS.split("\n");
                        for(int i=0;i<PCLinuxOSDat.count();i++)
                        {
                            QString Value1 = PCLinuxOSDat.value(i);
                            Value1.replace( QString("PCLINUXOS"), QString(TipPCLinuxOS));
                            Value1.replace( QString("ETIQUETA"), QString(Valor2));
                            Value1.replace("\n","");
                            grub << ""+Value1+"";
                        }
                        grub << "#";
                    }
                    if (TipFedora !="")
                    {
                        QString Fedora;
                        QString Fedora1 = "Fedora";
                        TipFedora = ""+TipFedora+""+ui->comboBox_13->currentText()+"";
                        QSqlQuery query(db);
                        query.exec("SELECT grub FROM Distro WHERE NomDistro='"+Fedora1+"' AND Version='"+ui->comboBox_13->currentText()+"' AND Arquitectura='"+ui->comboBox_3->currentText()+"'");
                        query.first();
                        if (query.isValid())
                            Fedora=query.value(0).toString();                        
                        QStringList FedoraDat  = Fedora.split("\n");
                        for(int i=0;i<FedoraDat.count();i++)
                        {
                            QString Value1 = FedoraDat.value(i);
                            Value1.replace( QString("FEDORA"), QString(TipFedora));
                            Value1.replace( QString("ETIQUETA"), QString(Valor2));
                            Value1.replace("\n","");
                            grub << ""+Value1+"";
                        }
                        grub << "#";
                    }
                    if (TipUbuntu !="")
                    {
                        QString Ubuntu;
                        QString Ubuntu1 = "Ubuntu";
                        TipUbuntu = ""+TipUbuntu+""+ui->comboBox_14->currentText()+"";
                        QSqlQuery query(db);
                        query.exec("SELECT grub FROM Distro WHERE NomDistro='"+Ubuntu1+"' AND Version='"+ui->comboBox_14->currentText()+"' AND Arquitectura='"+ui->comboBox_4->currentText()+"'");
                        query.first();
                        if (query.isValid())
                            Ubuntu=query.value(0).toString();                        
                        QStringList UbuntuDat  = Ubuntu.split("\n");
                        for(int i=0;i<UbuntuDat.count();i++)
                        {
                            QString Value1 = UbuntuDat.value(i);
                            Value1.replace( QString("UBUNTU"), QString(TipUbuntu));
                            Value1.replace( QString("ETIQUETA"), QString(Valor2));
                            Value1.replace("\n","");
                            grub << ""+Value1+"";
                        }
                        grub << "#";
                    }
                    if (TipArchLinux !="")
                    {
                        QString ArchLinux;
                        QString ArchLinux1 = "ArchLinux";
                        TipArchLinux = ""+TipArchLinux+""+ui->comboBox_15->currentText()+"";
                        QSqlQuery query(db);
                        query.exec("SELECT grub FROM Distro WHERE NomDistro='"+ArchLinux1+"' AND Version='"+ui->comboBox_15->currentText()+"' AND Arquitectura='"+ui->comboBox_5->currentText()+"'");
                        query.first();
                        if (query.isValid())
                            ArchLinux=query.value(0).toString();                        
                        QStringList ArchLinuxDat  = ArchLinux.split("\n");
                        for(int i=0;i<ArchLinuxDat.count();i++)
                        {
                            QString Value1 = ArchLinuxDat.value(i);
                            Value1.replace( QString("ARCHLINUX"), QString(TipArchLinux));
                            Value1.replace( QString("ETIQUETA"), QString(Valor2));
                            Value1.replace("\n","");
                            grub << ""+Value1+"";
                        }
                        grub << "#";
                    }
                    if (TipCentOS !="")
                    {
                        QString CentOS;
                        QString CentOS1 = "CentOS";
                        TipCentOS = ""+TipCentOS+""+ui->comboBox_16->currentText()+"";
                        QSqlQuery query(db);
                        query.exec("SELECT grub FROM Distro WHERE NomDistro='"+CentOS1+"' AND Version='"+ui->comboBox_16->currentText()+"' AND Arquitectura='"+ui->comboBox_6->currentText()+"'");
                        query.first();
                        if (query.isValid())
                            CentOS=query.value(0).toString();                        
                        QStringList CentOSDat  = CentOS.split("\n");
                        for(int i=0;i<CentOSDat.count();i++)
                        {
                            QString Value1 = CentOSDat.value(i);
                            Value1.replace( QString("CENTOS"), QString(TipCentOS));
                            Value1.replace( QString("ETIQUETA"), QString(Valor2));
                            Value1.replace("\n","");
                            grub << ""+Value1+"";
                        }
                        grub << "#";
                    }
                    if (TipDebian !="")
                    {
                        QString Debian;
                        QString Debian1 = "Debian";
                        TipDebian = ""+TipDebian+""+ui->comboBox_17->currentText()+"";
                        QSqlQuery query(db);
                        query.exec("SELECT grub FROM Distro WHERE NomDistro='"+Debian1+"' AND Version='"+ui->comboBox_17->currentText()+"' AND Arquitectura='"+ui->comboBox_7->currentText()+"'");
                        query.first();
                        if (query.isValid())
                            Debian=query.value(0).toString();                        
                        QStringList DebianDat  = Debian.split("\n");
                        for(int i=0;i<DebianDat.count();i++)
                        {
                            QString Value1 = DebianDat.value(i);
                            Value1.replace( QString("DEBIAN"), QString(TipDebian));
                            Value1.replace( QString("ETIQUETA"), QString(Valor2));
                            Value1.replace("\n","");
                            grub << ""+Value1+"";
                        }
                        grub << "#";
                    }
                    if (TipKnoppix !="")
                    {
                        QString Knoppix;
                        QString Knoppix1 = "Knoppix";
                        TipKnoppix = ""+TipKnoppix+""+ui->comboBox_18->currentText()+"";
                        QSqlQuery query(db);
                        query.exec("SELECT grub FROM Distro WHERE NomDistro='"+Knoppix1+"' AND Version='"+ui->comboBox_18->currentText()+"' AND Arquitectura='"+ui->comboBox_8->currentText()+"'");
                        query.first();
                        if (query.isValid())
                            Knoppix=query.value(0).toString();                        
                        QStringList KnoppixDat  = Knoppix.split("\n");
                        for(int i=0;i<KnoppixDat.count();i++)
                        {
                            QString Value1 = KnoppixDat.value(i);
                            Value1.replace( QString("KNOPPIX"), QString(TipKnoppix));
                            Value1.replace( QString("ETIQUETA"), QString(Valor2));
                            Value1.replace("\n","");
                            grub << ""+Value1+"";
                        }
                        grub << "#";
                    }
                    if (TipOpenSUSE !="")
                    {
                        QString OpenSUSE;
                        QString OpenSUSE1 = "OpenSUSE";
                        TipOpenSUSE = ""+TipOpenSUSE+""+ui->comboBox_19->currentText()+"";
                        QSqlQuery query(db);
                        query.exec("SELECT grub FROM Distro WHERE NomDistro='"+OpenSUSE1+"' AND Version='"+ui->comboBox_19->currentText()+"' AND Arquitectura='"+ui->comboBox_9->currentText()+"'");
                        query.first();
                        if (query.isValid())
                            OpenSUSE=query.value(0).toString();                        
                        QStringList OpenSUSEDat  = OpenSUSE.split("\n");
                        for(int i=0;i<OpenSUSEDat.count();i++)
                        {
                            QString Value1 = OpenSUSEDat.value(i);
                            Value1.replace( QString("OPENSUSE"), QString(TipOpenSUSE));
                            Value1.replace( QString("ETIQUETA"), QString(Valor2));
                            Value1.replace("\n","");
                            grub << ""+Value1+"";
                        }
                        grub << "#";
                    }
                    if (TipLinuxMint !="")
                    {
                        QString LinuxMint;
                        QString LinuxMint1 = "LinuxMint";
                        TipLinuxMint = ""+TipLinuxMint+""+ui->comboBox_20->currentText()+"";
                        QSqlQuery query(db);
                        query.exec("SELECT grub FROM Distro WHERE NomDistro='"+LinuxMint1+"' AND Version='"+ui->comboBox_20->currentText()+"' AND Arquitectura='"+ui->comboBox_10->currentText()+"'");
                        query.first();
                        if (query.isValid())
                            LinuxMint=query.value(0).toString();                        
                        QStringList LinuxMintDat  = LinuxMint.split("\n");
                        for(int i=0;i<LinuxMintDat.count();i++)
                        {
                            QString Value1 = LinuxMintDat.value(i);
                            Value1.replace( QString("LINUXMINT"), QString(TipLinuxMint));
                            Value1.replace( QString("ETIQUETA"), QString(Valor2));
                            Value1.replace("\n","");
                            grub << ""+Value1+"";
                        }
                        grub << "#";
                    }
                    if (TipBootRepair !="")
                    {
                        QString BootRepair;
                        QString BootRepair1 = "Boot Repair Disk";
                        TipBootRepair = ""+TipBootRepair+""+ui->comboBox_28->currentText()+"";
                        QSqlQuery query(db);
                        query.exec("SELECT grub FROM Distro WHERE NomDistro='"+BootRepair1+"' AND Version='"+ui->comboBox_28->currentText()+"' AND Arquitectura='"+ui->comboBox_27->currentText()+"'");
                        query.first();
                        if (query.isValid())
                            BootRepair=query.value(0).toString();                        
                        QStringList BootRepairDat  = BootRepair.split("\n");
                        for(int i=0;i<BootRepairDat.count();i++)
                        {
                            QString Value1 = BootRepairDat.value(i);
                            Value1.replace( QString("BootRepair"), QString(TipBootRepair));
                            Value1.replace( QString("ETIQUETA"), QString(Valor2));
                            Value1.replace("\n","");
                            grub << ""+Value1+"";
                        }
                        grub << "#";
                    }
                    if (TipClonezilla !="")
                    {
                        QString Clonezilla;
                        QString Clonezilla1 = "Clonezilla";
                        TipClonezilla = ""+TipClonezilla+""+ui->comboBox_30->currentText()+"";
                        QSqlQuery query(db);
                        query.exec("SELECT grub FROM Distro WHERE NomDistro='"+Clonezilla1+"' AND Version='"+ui->comboBox_30->currentText()+"' AND Arquitectura='"+ui->comboBox_29->currentText()+"'");
                        query.first();
                        if (query.isValid())
                            Clonezilla=query.value(0).toString();                        
                        QStringList ClonezillaDat  = Clonezilla.split("\n");
                        for(int i=0;i<ClonezillaDat.count();i++)
                        {
                            QString Value1 = ClonezillaDat.value(i);
                            Value1.replace( QString("Clonezilla"), QString(TipClonezilla));
                            Value1.replace( QString("ETIQUETA"), QString(Valor2));
                            Value1.replace("\n","");
                            grub << ""+Value1+"";
                        }
                        grub << "#";
                    }
                    if (TipGParted !="")
                    {
                        QString GParted;
                        QString GParted1 = "GParted";
                        TipGParted = ""+TipGParted+""+ui->comboBox_32->currentText()+"";
                        QSqlQuery query(db);
                        query.exec("SELECT grub FROM Distro WHERE NomDistro='"+GParted1+"' AND Version='"+ui->comboBox_32->currentText()+"' AND Arquitectura='"+ui->comboBox_31->currentText()+"'");
                        query.first();
                        if (query.isValid())
                            GParted=query.value(0).toString();                        
                        QStringList GPartedDat  = GParted.split("\n");
                        for(int i=0;i<GPartedDat.count();i++)
                        {
                            QString Value1 = GPartedDat.value(i);
                            Value1.replace( QString("GParted"), QString(TipGParted));
                            Value1.replace( QString("ETIQUETA"), QString(Valor2));
                            Value1.replace("\n","");
                            grub << ""+Value1+"";
                        }
                        grub << "#";
                    }
                    if (TipSystemRescue !="")
                    {
                        QString SystemRescue;
                        QString SystemRescue1 = "System Rescue CD";
                        TipSystemRescue = ""+TipSystemRescue+""+ui->comboBox_36->currentText()+"";
                        QSqlQuery query(db);
                        query.exec("SELECT grub FROM Distro WHERE NomDistro='"+SystemRescue1+"' AND Version='"+ui->comboBox_36->currentText()+"' AND Arquitectura='"+ui->comboBox_35->currentText()+"'");
                        query.first();
                        if (query.isValid())
                            SystemRescue=query.value(0).toString();                        
                        QStringList SystemRescueDat  = SystemRescue.split("\n");
                        for(int i=0;i<SystemRescueDat.count();i++)
                        {
                            QString Value1 = SystemRescueDat.value(i);
                            Value1.replace( QString("SystemRescue"), QString(TipSystemRescue));
                            Value1.replace( QString("ETIQUETA"), QString(Valor2));
                            Value1.replace("\n","");
                            grub << ""+Value1+"";
                        }
                        grub << "#";
                    }
                    if (TipOpenMandriva !="")
                    {
                        QString OpenMandriva;
                        QString OpenMandriva1 = "OpenMandriva";
                        TipOpenMandriva = ""+TipOpenMandriva+""+ui->comboBox_22->currentText()+"";
                        QSqlQuery query(db);
                        query.exec("SELECT grub FROM Distro WHERE NomDistro='"+OpenMandriva1+"' AND Version='"+ui->comboBox_22->currentText()+"' AND Arquitectura='"+ui->comboBox_21->currentText()+"'");
                        query.first();
                        if (query.isValid())
                            OpenMandriva=query.value(0).toString();                        
                        QStringList OpenMandrivaDat  = OpenMandriva.split("\n");
                        for(int i=0;i<OpenMandrivaDat.count();i++)
                        {
                            QString Value = OpenMandrivaDat.value(i);
                            Value1.replace( QString("OPENMANDRIVA"), QString(TipOpenMandriva));
                            Value1.replace( QString("ETIQUETA"), QString(Valor2));
                            Value1.replace("\n","");
                            grub << ""+Value+"";
                        }
                        grub << "#";
                    }
                }
                if (ui->radioButton_4->isChecked() == true)
                {
                    QString TipGrub1 = getGrub("/usr/sbin/grub-install");
                    QString TipGrub2 = getGrub("/usr/sbin/grub2-install");
                    QString TipGrub3 = getIso(""+ruta+""+Valor2+"/boot/");
                    TipGrub3.remove("/");
                    if (ui->radioButton_8->isChecked() == false)
                    {
                        if (TipGrub2 == "")
                        {
                            if (TipGrub1 == "")
                            {
                                QMessageBox m;
                                if (Stilo == "A")
                                    m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
                                m.setText(tr("No tienes instalado grub2 en tu sistema y es necesario para este proposito. Por favor instalalo si quieres continuar."));
                                m.exec();
                                return;
                            }
                            else if (TipGrub1 == "/usr/sbin/grub-install")
                            {
                                QString gru1 = "echo instalando grub2 en el USB...";
                                QString grub1 = "/usr/sbin/grub-install --root-directory="+ruta+"%1 %2 --no-floppy";
                                grub1 = grub1.arg(Valor2).arg(Valor1);
                                comandos << gru1 << grub1;
                            }
                        }
                        else if (TipGrub2 == "/usr/sbin/grub2-install")
                        {
                            QString gru1 = "echo instalando grub2 en el USB...";
                            QString grub1 = "/usr/sbin/grub2-install --root-directory="+ruta+"%1 %2 --no-floppy";
                            grub1 = grub1.arg(Valor2).arg(Valor1);
                            comandos << gru1 << grub1;
                        }
                        if (TipGrub3 == "grub2")
                        {
                            QString Kill = "echo Creando fichero de mapeo de unidades y grub.cfg...";
                            QString Kill0 = "rm -vR -f "+ruta+"%1/boot/grub2/device.map";
                            Kill0 = Kill0.arg(Valor2);
                            QString Kill1 = "touch "+ruta+"%1/boot/grub2/device.map";
                            Kill1 = Kill1.arg(Valor2);
                            QString Kill2 = "touch "+ruta+"%1/boot/grub2/grub.cfg";
                            Kill2 = Kill2.arg(Valor2);
                            QString device0 = "echo Introduciendo datos de mapeo y configuracion de inicio de distros...";
                            comandos << Kill << Kill0 << Kill1 << Kill2 << device0;
                            QString gru2 = "echo Reinstalando grub en el USB...";
                            QString grub2 = "/usr/sbin/grub2-install --root-directory="+ruta+"%1 %2 --no-floppy";
                            grub2 = grub2.arg(Valor2).arg(Valor1);
                            comandos << gru2 << grub2;
                        }
                        else if (TipGrub3 != "grub2")
                        {
                            QString Kill = "echo Creando fichero de mapeo de unidades y grub.cfg...";
                            QString Kill0 = "rm -vR -f "+ruta+"%1/boot/grub/device.map";
                            Kill0 = Kill0.arg(Valor2);
                            QString Kill1 = "touch "+ruta+"%1/boot/grub/device.map";
                            Kill1 = Kill1.arg(Valor2);
                            QString Kill2 = "touch "+ruta+"%1/boot/grub/grub.cfg";
                            Kill2 = Kill2.arg(Valor2);
                            QString device0 = "echo Introduciendo datos de mapeo y configuracion de inicio de distros...";
                            comandos << Kill << Kill0 << Kill1 << Kill2 << device0;
                            QString gru2 = "echo Reinstalando grub en el USB...";
                            QString grub2 = "/usr/sbin/grub-install --root-directory="+ruta+"%1 %2 --no-floppy";
                            grub2 = grub2.arg(Valor2).arg(Valor1);
                            comandos << gru2 << grub2;
                        }
                    }
                    else if (ui->radioButton_8->isChecked() == true)
                    {
                        if (TipGrub3 == "grub2")
                        {
                            QString kill = "rm -vR -f "+ruta+"%1/boot/grub2/device.map";
                            kill = kill.arg(Valor2);
                            QString killa = "touch "+ruta+"%1/boot/grub2/device.map";
                            killa = killa.arg(Valor2);
                            comandos << kill << killa;
                            QString gru2 = "echo Reinstalando grub en el USB...";
                            QString grub2 = "/usr/sbin/grub2-install --root-directory="+ruta+"%1 %2 --no-floppy";
                            grub2 = grub2.arg(Valor2).arg(Valor1);
                            comandos << gru2 << grub2;
                        }
                        else if (TipGrub3 != "grub2")
                        {
                            QString Kill0 = "rm -vR -f "+ruta+"%1/boot/grub/device.map";
                            Kill0 = Kill0.arg(Valor2);
                            QString Kill1 = "touch "+ruta+"%1/boot/grub/device.map";
                            Kill1 = Kill1.arg(Valor2);
                            comandos << Kill0 << Kill1;
                            QString gru2 = "echo Reinstalando grub en el USB...";
                            QString grub2 = "/usr/sbin/grub-install --root-directory="+ruta+"%1 %2 --no-floppy";
                            grub2 = grub2.arg(Valor2).arg(Valor1);
                            comandos << gru2 << grub2;
                        }
                        QString device0 = "echo Actualizando datos de mapeo...";
                        QString reelido = "echo Realizando proceso de reinstalado en device.map...";
                        comandos << device0 << reelido;
                    }
                    QString procesando = "echo Realizando proceso de insercion de datos de distros en grub.cfg...";
                    comandos << procesando;
                    if (ui->radioButton_8->isChecked() == false)
                       grub << "set default=0" << "set timeout=10" << "set root=(hd0,1)" << "terminal console" << "";
                    if (TipMandriva !="")
                    {
                        QString Mandriva;
                        QString Mandriva1 = "Mandriva";
                        TipMandriva = ""+TipMandriva+""+ui->comboBox_11->currentText()+"";
                        QSqlQuery query(db);
                        query.exec("SELECT grub2 FROM Distro WHERE NomDistro='"+Mandriva1+"' AND Version='"+ui->comboBox_11->currentText()+"' AND Arquitectura='"+ui->comboBox->currentText()+"'");
                        query.first();
                        if (query.isValid())
                            Mandriva=query.value(0).toString();
                        QStringList MandrivaDat = Mandriva.split("\n");
                        for(int i=0;i<MandrivaDat.count();i++)
                        {
                            QString Value = MandrivaDat.value(i);
                            Value.replace(QString("MANDRIVA"), QString(TipMandriva));
                            Value.replace(QString("ETIQUETA"), QString(Valor2));
                            Value.replace("\n","");
                            grub << ""+Value+"";
                        }
                        grub << "#";
                    }
                    if (TipPCLinuxOS != "")
                    {
                        QString PCLinuxOS;
                        QString PCLinuxOS1 = "PCLinuxOS";
                        TipPCLinuxOS = ""+TipPCLinuxOS+""+ui->comboBox_12->currentText()+"";
                        QSqlQuery query(db);
                        query.exec("SELECT grub2 FROM Distro WHERE NomDistro='"+PCLinuxOS1+"' AND Version='"+ui->comboBox_12->currentText()+"' AND Arquitectura='"+ui->comboBox_2->currentText()+"'");
                        query.first();
                        if (query.isValid())
                            PCLinuxOS=query.value(0).toString();
                        QStringList PCLinuxOSDat  = PCLinuxOS.split("\n");
                        for(int i=0;i<PCLinuxOSDat.count();i++)
                        {
                            QString Value1 = PCLinuxOSDat.value(i);
                            Value1.replace( QString("PCLINUXOS"), QString(TipPCLinuxOS));
                            Value1.replace( QString("ETIQUETA"), QString(Valor2));
                            Value1.replace("\n","");
                            grub << ""+Value1+"";
                        }
                        grub << "#";
                    }
                    if (TipFedora !="")
                    {
                        QString Fedora;
                        QString Fedora1 = "Fedora";
                        TipFedora = ""+TipFedora+""+ui->comboBox_13->currentText()+"";
                        QSqlQuery query(db);
                        query.exec("SELECT grub2 FROM Distro WHERE NomDistro='"+Fedora1+"' AND Version='"+ui->comboBox_13->currentText()+"' AND Arquitectura='"+ui->comboBox_3->currentText()+"'");
                        query.first();
                        if (query.isValid())
                            Fedora=query.value(0).toString();
                        QStringList FedoraDat  = Fedora.split("\n");
                        for(int i=0;i<FedoraDat.count();i++)
                        {
                            QString Value1 = FedoraDat.value(i);
                            Value1.replace( QString("FEDORA"), QString(TipFedora));
                            Value1.replace( QString("ETIQUETA"), QString(Valor2));
                            Value1.replace("\n","");
                            grub << ""+Value1+"";
                        }
                        grub << "#";
                    }
                    if (TipUbuntu !="")
                    {
                        QString Ubuntu;
                        QString Ubuntu1 = "Ubuntu";
                        TipUbuntu = ""+TipUbuntu+""+ui->comboBox_14->currentText()+"";
                        QSqlQuery query(db);
                        query.exec("SELECT grub2 FROM Distro WHERE NomDistro='"+Ubuntu1+"' AND Version='"+ui->comboBox_14->currentText()+"' AND Arquitectura='"+ui->comboBox_4->currentText()+"'");
                        query.first();
                        if (query.isValid())
                            Ubuntu=query.value(0).toString();
                        QStringList UbuntuDat  = Ubuntu.split("\n");
                        for(int i=0;i<UbuntuDat.count();i++)
                        {
                            QString Value1 = UbuntuDat.value(i);
                            Value1.replace( QString("UBUNTU"), QString(TipUbuntu));
                            Value1.replace( QString("ETIQUETA"), QString(Valor2));
                            Value1.replace("\n","");
                            grub << ""+Value1+"";
                        }
                        grub << "#";
                    }
                    if (TipArchLinux !="")
                    {
                        QString ArchLinux;
                        QString ArchLinux1 = "ArchLinux";
                        TipArchLinux = ""+TipArchLinux+""+ui->comboBox_15->currentText()+"";
                        QSqlQuery query(db);
                        query.exec("SELECT grub2 FROM Distro WHERE NomDistro='"+ArchLinux1+"' AND Version='"+ui->comboBox_15->currentText()+"' AND Arquitectura='"+ui->comboBox_5->currentText()+"'");
                        query.first();
                        if (query.isValid())
                            ArchLinux=query.value(0).toString();
                        QStringList ArchLinuxDat  = ArchLinux.split("\n");
                        for(int i=0;i<ArchLinuxDat.count();i++)
                        {
                            QString Value1 = ArchLinuxDat.value(i);
                            Value1.replace( QString("ARCHLINUX"), QString(TipArchLinux));
                            Value1.replace( QString("ETIQUETA"), QString(Valor2));
                            Value1.replace("\n","");
                            grub << ""+Value1+"";
                        }
                        grub << "#";
                    }
                    if (TipCentOS !="")
                    {
                        QString CentOS;
                        QString CentOS1 = "CentOS";
                        TipCentOS = ""+TipCentOS+""+ui->comboBox_16->currentText()+"";
                        QSqlQuery query(db);
                        query.exec("SELECT grub2 FROM Distro WHERE NomDistro='"+CentOS1+"' AND Version='"+ui->comboBox_16->currentText()+"' AND Arquitectura='"+ui->comboBox_6->currentText()+"'");
                        query.first();
                        if (query.isValid())
                            CentOS=query.value(0).toString();
                        QStringList CentOSDat  = CentOS.split("\n");
                        for(int i=0;i<CentOSDat.count();i++)
                        {
                            QString Value1 = CentOSDat.value(i);
                            Value1.replace( QString("CENTOS"), QString(TipCentOS));
                            Value1.replace( QString("ETIQUETA"), QString(Valor2));
                            Value1.replace("\n","");
                            grub << ""+Value1+"";
                        }
                        grub << "#";
                    }
                    if (TipDebian !="")
                    {
                        QString Debian;
                        QString Debian1 = "Debian";
                        TipDebian = ""+TipDebian+""+ui->comboBox_17->currentText()+"";
                        QSqlQuery query(db);
                        query.exec("SELECT grub2 FROM Distro WHERE NomDistro='"+Debian1+"' AND Version='"+ui->comboBox_17->currentText()+"' AND Arquitectura='"+ui->comboBox_7->currentText()+"'");
                        query.first();
                        if (query.isValid())
                            Debian=query.value(0).toString();
                        QStringList DebianDat  = Debian.split("\n");
                        for(int i=0;i<DebianDat.count();i++)
                        {
                            QString Value1 = DebianDat.value(i);
                            Value1.replace( QString("DEBIAN"), QString(TipDebian));
                            Value1.replace( QString("ETIQUETA"), QString(Valor2));
                            Value1.replace("\n","");
                            grub << ""+Value1+"";
                        }
                        grub << "#";
                    }
                    if (TipKnoppix !="")
                    {
                        QString Knoppix;
                        QString Knoppix1 = "Knoppix";
                        TipKnoppix = ""+TipKnoppix+""+ui->comboBox_18->currentText()+"";
                        QSqlQuery query(db);
                        query.exec("SELECT grub2 FROM Distro WHERE NomDistro='"+Knoppix1+"' AND Version='"+ui->comboBox_18->currentText()+"' AND Arquitectura='"+ui->comboBox_8->currentText()+"'");
                        query.first();
                        if (query.isValid())
                            Knoppix=query.value(0).toString();
                        QStringList KnoppixDat  = Knoppix.split("\n");
                        for(int i=0;i<KnoppixDat.count();i++)
                        {
                            QString Value1 = KnoppixDat.value(i);
                            Value1.replace( QString("KNOPPIX"), QString(TipKnoppix));
                            Value1.replace( QString("ETIQUETA"), QString(Valor2));
                            Value1.replace("\n","");
                            grub << ""+Value1+"";
                        }
                        grub << "#";
                    }
                    if (TipOpenSUSE !="")
                    {
                        QString OpenSUSE;
                        QString OpenSUSE1 = "OpenSUSE";
                        TipOpenSUSE = ""+TipOpenSUSE+""+ui->comboBox_19->currentText()+"";
                        QSqlQuery query(db);
                        query.exec("SELECT grub2 FROM Distro WHERE NomDistro='"+OpenSUSE1+"' AND Version='"+ui->comboBox_19->currentText()+"' AND Arquitectura='"+ui->comboBox_9->currentText()+"'");
                        query.first();
                        if (query.isValid())
                            OpenSUSE=query.value(0).toString();
                        QStringList OpenSUSEDat  = OpenSUSE.split("\n");
                        for(int i=0;i<OpenSUSEDat.count();i++)
                        {
                            QString Value1 = OpenSUSEDat.value(i);
                            Value1.replace( QString("OPENSUSE"), QString(TipOpenSUSE));
                            Value1.replace( QString("ETIQUETA"), QString(Valor2));
                            Value1.replace("\n","");
                            grub << ""+Value1+"";
                        }
                        grub << "#";
                    }
                    if (TipLinuxMint !="")
                    {
                        QString LinuxMint;
                        QString LinuxMint1 = "LinuxMint";
                        TipLinuxMint = ""+TipLinuxMint+""+ui->comboBox_20->currentText()+"";
                        QSqlQuery query(db);
                        query.exec("SELECT grub2 FROM Distro WHERE NomDistro='"+LinuxMint1+"' AND Version='"+ui->comboBox_20->currentText()+"' AND Arquitectura='"+ui->comboBox_10->currentText()+"'");
                        query.first();
                        if (query.isValid())
                            LinuxMint=query.value(0).toString();
                        QStringList LinuxMintDat  = LinuxMint.split("\n");
                        for(int i=0;i<LinuxMintDat.count();i++)
                        {
                            QString Value1 = LinuxMintDat.value(i);
                            Value1.replace( QString("LINUXMINT"), QString(TipLinuxMint));
                            Value1.replace( QString("ETIQUETA"), QString(Valor2));
                            Value1.replace("\n","");
                            grub << ""+Value1+"";
                        }
                        grub << "#";
                    }
                    if (TipBootRepair !="")
                    {
                        QString BootRepair;
                        QString BootRepair1 = "Boot Repair Disk";
                        TipBootRepair = ""+TipBootRepair+""+ui->comboBox_28->currentText()+"";
                        QSqlQuery query(db);
                        query.exec("SELECT grub2 FROM Distro WHERE NomDistro='"+BootRepair1+"' AND Version='"+ui->comboBox_28->currentText()+"' AND Arquitectura='"+ui->comboBox_27->currentText()+"'");
                        query.first();
                        if (query.isValid())
                            BootRepair=query.value(0).toString();
                        QStringList BootRepairDat  = BootRepair.split("\n");
                        for(int i=0;i<BootRepairDat.count();i++)
                        {
                            QString Value1 = BootRepairDat.value(i);
                            Value1.replace( QString("BootRepair"), QString(TipBootRepair));
                            Value1.replace( QString("ETIQUETA"), QString(Valor2));
                            Value1.replace("\n","");
                            grub << ""+Value1+"";
                        }
                        grub << "#";
                    }
                    if (TipClonezilla !="")
                    {
                        QString Clonezilla;
                        QString Clonezilla1 = "Clonezilla";
                        TipClonezilla = ""+TipClonezilla+""+ui->comboBox_30->currentText()+"";
                        QSqlQuery query(db);
                        query.exec("SELECT grub2 FROM Distro WHERE NomDistro='"+Clonezilla1+"' AND Version='"+ui->comboBox_30->currentText()+"' AND Arquitectura='"+ui->comboBox_29->currentText()+"'");
                        query.first();
                        if (query.isValid())
                            Clonezilla=query.value(0).toString();
                        QStringList ClonezillaDat  = Clonezilla.split("\n");
                        for(int i=0;i<ClonezillaDat.count();i++)
                        {
                            QString Value1 = ClonezillaDat.value(i);
                            Value1.replace( QString("Clonezilla"), QString(TipClonezilla));
                            Value1.replace( QString("ETIQUETA"), QString(Valor2));
                            Value1.replace("\n","");
                            grub << ""+Value1+"";
                        }
                        grub << "#";
                    }
                    if (TipGParted !="")
                    {
                        QString GParted;
                        QString GParted1 = "GParted";
                        TipGParted = ""+TipGParted+""+ui->comboBox_32->currentText()+"";
                        QSqlQuery query(db);
                        query.exec("SELECT grub2 FROM Distro WHERE NomDistro='"+GParted1+"' AND Version='"+ui->comboBox_32->currentText()+"' AND Arquitectura='"+ui->comboBox_31->currentText()+"'");
                        query.first();
                        if (query.isValid())
                            GParted=query.value(0).toString();
                        QStringList GPartedDat  = GParted.split("\n");
                        for(int i=0;i<GPartedDat.count();i++)
                        {
                            QString Value1 = GPartedDat.value(i);
                            Value1.replace( QString("GParted"), QString(TipGParted));
                            Value1.replace( QString("ETIQUETA"), QString(Valor2));
                            Value1.replace("\n","");
                            grub << ""+Value1+"";
                        }
                        grub << "#";
                    }
                    if (TipSystemRescue !="")
                    {
                        QString SystemRescue;
                        QString SystemRescue1 = "System Rescue CD";
                        TipSystemRescue = ""+TipSystemRescue+""+ui->comboBox_36->currentText()+"";
                        QSqlQuery query(db);
                        query.exec("SELECT grub2 FROM Distro WHERE NomDistro='"+SystemRescue1+"' AND Version='"+ui->comboBox_36->currentText()+"' AND Arquitectura='"+ui->comboBox_35->currentText()+"'");
                        query.first();
                        if (query.isValid())
                            SystemRescue=query.value(0).toString();
                        QStringList SystemRescueDat  = SystemRescue.split("\n");
                        for(int i=0;i<SystemRescueDat.count();i++)
                        {
                            QString Value1 = SystemRescueDat.value(i);
                            Value1.replace( QString("SystemRescue"), QString(TipSystemRescue));
                            Value1.replace( QString("ETIQUETA"), QString(Valor2));
                            Value1.replace("\n","");
                            grub << ""+Value1+"";
                        }
                        grub << "#";
                    }
                    if (TipOpenMandriva !="")
                    {
                        QString OpenMandriva;
                        QString OpenMandriva1 = "OpenMandriva";
                        TipOpenMandriva = ""+TipOpenMandriva+""+ui->comboBox_22->currentText()+"";
                        QSqlQuery query(db);
                        query.exec("SELECT grub2 FROM Distro WHERE NomDistro='"+OpenMandriva1+"' AND Version='"+ui->comboBox_22->currentText()+"' AND Arquitectura='"+ui->comboBox_21->currentText()+"'");
                        query.first();
                        if (query.isValid())
                            OpenMandriva=query.value(0).toString();
                        QStringList OpenMandrivaDat  = OpenMandriva.split("\n");
                        for(int i=0;i<OpenMandrivaDat.count();i++)
                        {
                            QString Value = OpenMandrivaDat.value(i);
                            Value1.replace( QString("OPENMANDRIVA"), QString(TipOpenMandriva));
                            Value1.replace( QString("ETIQUETA"), QString(Valor2));
                            Value1.replace("\n","");
                            grub << ""+Value+"";
                        }
                        grub << "#";
                    }
                }                
                if (ui->radioButton_12->isChecked() == true)
                {
                    QString gru2 = "echo Actualizando grub en el USB...";
                    QString grub2 = "/sbin/grub-install --root-directory="+ruta+"%1 %2 --no-floppy";
                    grub2 = grub2.arg(Valor2).arg(Valor1);
                    comandos << gru2 << grub2;
                }
                else if (ui->radioButton_4->isChecked() == true)
                {
                    QString TipGrub3 = getIso(""+ruta+""+Valor2+"/boot/");
                    TipGrub3.remove("/");
                    if (TipGrub3 == "grub2")
                    {
                        QString gru2 = "echo Actualizando grub en el USB...";
                        QString grub2 = "/usr/sbin/grub2-install --root-directory="+ruta+"%1 %2 --no-floppy";
                        grub2 = grub2.arg(Valor2).arg(Valor1);
                        comandos << gru2 << grub2;
                    }
                    else if (TipGrub3 != "grub2")
                    {
                        QString gru2 = "echo Actualizando grub en el USB...";
                        QString grub2 = "/usr/sbin/grub-install --root-directory="+ruta+"%1 %2 --no-floppy";
                        grub2 = grub2.arg(Valor2).arg(Valor1);
                        comandos << gru2 << grub2;
                    }
                }
            }            
        }
    }
}

QString UsbLive::getSize(QString Dir)
{
    QProcess *procesoDu, *procesoCut;
    QStringList argumentosDu;
    QStringList argumentosCut;
    QByteArray Size;
    procesoDu=new QProcess(this);
    procesoCut=new QProcess(this);
    argumentosDu << "-s" << "--block-size=G" << ""+Dir+"";
    argumentosCut << "-d" << "G" << "-f1";
    procesoDu->setStandardOutputProcess(procesoCut);
    procesoDu->start("du",argumentosDu);
    procesoCut->start("cut",argumentosCut);
    if (! procesoCut->waitForStarted())
        return QString("");
    procesoDu->waitForFinished();
    procesoCut->waitForFinished();
    Size = QString(procesoCut->readAllStandardOutput());
    procesoCut->waitForFinished();
    delete procesoDu;
    delete procesoCut;
    QString res =  QString(Size);
    res.chop(1);
    return res;
}

void UsbLive::on_pushButton_21_clicked()
{
    ui->textEdit_2->setText("");
    if (ui->pushButton_21->text() == tr("Comenzar"))
    {
        ui->pushButton_21->setText(tr("Cancelar"));
        if (Control > 0)
        {
            QMessageBox m;
            if (Stilo == "A")
                m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
            m.setText(tr("Hay errores que corregir. Debes corregirlos para poder continuar."));
            m.exec();
            ui->pushButton_21->setText(tr("Comenzar"));
            return;
        }
        if (comandos.count() <= 0)
        {
            QMessageBox m;
            if (Stilo == "A")
                m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
            m.setText(tr("No hay procesos para realizar. Comprueba que este todo correcto."));
            m.exec();
            ui->pushButton_21->setText(tr("Comenzar"));
            return;
        }
        ui->progressBar->show();
        ui->label_6->show();
        int respuesta = 0;
        respuesta = QMessageBox::question(this, QString::fromUtf8(tr("Crear USB Live Multiboot")),
                    QString::fromUtf8(tr("<center><b>Crear USB Live Multiboot<b></center><p>"
                            "A partir de dar a aceptar ya no se podrá invertir el proceso<p>"
                            "&iquest;Estas seguro de querer continuar?")), QMessageBox ::Ok, QMessageBox::No);
        if (respuesta == QMessageBox::Ok)
        {
            if (mib != 0)
                delete mib;
            mib = new DrakeProcesos(comandos, this);
            connect(mib, SIGNAL(publicarDatos(QString)), this, SLOT(mibEscribir(QString)));
            connect(mib, SIGNAL(DatoComand(QString)), this, SLOT(mibReferencia(QString)));
            connect(mib, SIGNAL(progreso(QString)), this, SLOT(mibprogreso(QString)));
            connect(mib, SIGNAL(finProceso()), this, SLOT(mibFin()));
            connect(mib, SIGNAL(finProceso()), this, SLOT(ProbarUSB()));
            int valor= comandos.count();
            mib->Valor(valor,3);
            mib->Mascara(cantidad51,cantidad50,cantidad49,DatoTalla,cantidad47,DatoNegro);
            mib->iniciarProceso();
        }
        else if (respuesta == QMessageBox::No)
        {
            ui->pushButton_21->setText(tr("Comenzar"));
            ui->label_4->show();
            ui->progressBar->hide();
            ui->label_6->show();
        }
        return;
    }
    else if (ui->pushButton_21->text() == "Cancelar")
    {
        int respuesta = 0;
        respuesta = QMessageBox::question(this, QString::fromUtf8(tr("Cancelar procesos")),
                    QString::fromUtf8(tr("<center><b>Cancelar procesos<b></center><p>"
                            "Debes confirmar para verificar que no ha sido un error.<p>"
                            "&iquest;Estas seguro de querer cancelar?")), QMessageBox::Ok, QMessageBox::No);
        if (respuesta == QMessageBox::Ok)
        {
            comandos.clear();
            QString cmd1 = "killall wget";
            comandos << cmd1;
            if (mib != 0)
                delete mib;
            mib = new DrakeProcesos(comandos, this);
            connect(mib, SIGNAL(publicarDatos(QString)), this, SLOT(mibEscribir(QString)));
            connect(mib, SIGNAL(progreso(QString)), this, SLOT(mibprogreso(QString)));
            connect(mib, SIGNAL(DatoComand(QString)), this, SLOT(mibReferencia(QString)));
            connect(mib, SIGNAL(finProceso()), this, SLOT(mibFin()));
            int valor= comandos.count();
            mib->Valor(valor,3);
            mib->Mascara(cantidad51,cantidad50,cantidad49,DatoTalla,cantidad47,DatoNegro);
            mib->iniciarProceso();
        }
        else if (respuesta == QMessageBox::No)
        {
            ui->pushButton_21->setText(tr("Comenzar"));
            ui->label_4->show();
            ui->progressBar->hide();
            ui->label_6->show();
        }
        return;
    }
}

void UsbLive::on_pushButton_22_clicked()
{
    ui->tabWidget->setCurrentPage(2);
}

void UsbLive::mibEscribir(QString valor)
{
    valor = valor.remove("# ");
    if (DatoComand == "wget")
    {
        valor.remove(".");
        QString Dato = valor.right(1);
        if (Dato != "")
        {
            if (Dato == " ")
            {
                valor.remove("K");
                int Megas = valor.toInt();
                Megas = Megas/1024;
                if (Megas != 0)
                   ui->label_6->setText(tr("Descargando ")+QString::number(Megas)+tr(" Mb de ")+Tamano+" Mb");
            }
        }
    }
    else
    {
        if (Ctrl == 1)
            ui->textEdit->append(valor);
        else if (Ctrl == 0)
            ui->textEdit_2->append(valor);
        else if (Ctrl == 2)
            ui->textEdit_4->append(valor);
    }
}

void UsbLive::mibprogreso(QString Dat)
{
    ui->progressBar->setValue(Dat.toInt());
}

void UsbLive::mibReferencia(QString comando)
{
    QString ValueGrub;
    if (TipoGrub == "NADA")
    {
        if (ui->radioButton_12->isChecked())
            ValueGrub = "grub";
        else if (ui->radioButton_4->isChecked())
        {
            QString TipGrub2 = getGrub("/usr/sbin/grub2-install");
            if (TipGrub2 == "")
                ValueGrub = "grub";
            else
                ValueGrub = "grub2";
        }
    }
    else
        ValueGrub = TipoGrub;
    if (comando == "echo Introduciendo datos de mapeo y configuracion de inicio de distros..." || comando == "echo Actualizando datos de mapeo...")
    {
        QString device = "(hd0) %1";
        device = device.arg(activo3);
        QFile file(""+ruta+""+Valor2+"/boot/"+ValueGrub+"/device.map");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream in(&file);
        in << ""+device+"";
        file.close();
    }
    if (comando == "echo Introduciendo datos de mapeo y configuracion de inicio de distros para grub..." || comando == "echo Actualizando datos de mapeo de grub...")
    {        
        QString device = "(hd0) %1";
        device = device.arg(activo3);
        QString Ruta;
        Ruta = ""+ruta+""+Valor2+"/boot/grub/device.map";
        QFile file(Ruta);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream in(&file);
        in << ""+device+"";
        file.close();
    }
    if (comando == "echo Reconstruyendo datos de mapeo y configuracion de inicio de distros para grub...")
    {
        QString device = "(hd0) %1";
        device = device.arg(ui->label_18->text());
        QString Ruta;
        Ruta = ""+ruta+"DRAKLIVE/boot/grub/device.map";
        QFile file(Ruta);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream in(&file);
        in << ""+device+"";
        file.close();
    }
    if (comando == "echo Realizando proceso de insercion de datos de distros en grub.cfg...")
    {        
        QString Value0;
        QString Ruta;
        QString Compara;
        Ruta = ""+ruta+""+Valor2+"/boot/"+ValueGrub+"/grub.cfg";
        QFile file(Ruta);
        file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
        QTextStream in2(&file);
        for(int i=0;i<grub.count();i++)
        {
            Value0 = grub.value(i);
            if (Value0 != " ")
            {
                Compara = getCompara(Value0, Ruta);
                if (Value0 != Compara)
                    in2 << ""+Value0+"" << "\n";
            }
        }
        grub.clear();
        file.close();
    }
    if (comando == "echo Realizando proceso de insercion de datos de distros en menu.lst...")
    {        
        QString Value0;
        QString Ruta;
        QString Compara;
        Ruta = ""+ruta+""+Valor2+"/boot/grub/menu.lst";
        QFile file(Ruta);
        file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
        QTextStream in2(&file);
        for(int i=0;i<grub.count();i++)
        {
            Value0 = grub.value(i);
            if (Value0 != " ")
            {
                Compara = getCompara(Value0, Ruta);
                if (Value0 != Compara)
                    in2 << ""+Value0+"" << "\n";
            }
        }
        grub.clear();
        file.close();
    }
    if (comando == "echo Realizando nuevo proceso de insercion de datos de distros en menu.lst...")
    {
        QString Value0;
        QString Ruta;
        QString Compara;
        Ruta = ""+ruta+"DRAKLIVE/boot/grub/menu.lst";
        QFile file(Ruta);
        file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
        QTextStream in2(&file);
        for(int i=0;i<ValorGrub.count();i++)
        {
            Value0 = ValorGrub.value(i);
            if (Value0 != " ")
            {
                Compara = getCompara(Value0, Ruta);
                if (Value0 != Compara)
                    in2 << ""+Value0+"" << "\n";
            }
        }
        ValorGrub.clear();
        file.close();
    }
    if (comando == "echo Realizando nuevo proceso de insercion de datos de distros en grub.cfg...")
    {
        QString Value0;
        QString Ruta;
        QString Compara;
        Ruta = ""+ruta+"DRAKLIVE/boot/"+ValueGrub+"/grub.cfg";
        QFile file(Ruta);
        file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
        QTextStream in2(&file);
        for(int i=0;i<ValorGrub.count();i++)
        {
            Value0 = ValorGrub.value(i);
            if (Value0 != " ")
            {
                Compara = getCompara(Value0, Ruta);
                if (Value0 != Compara)
                    in2 << ""+Value0+"" << "\n";
            }
        }
        ValorGrub.clear();
        file.close();
    }
    else
        DatoComand = comando.left(4);
}

QString UsbLive::getCompara (QString Valor, QString Ruta)
{
    QProcess *procesoFind, *procesoCut;
    QStringList argumentosFind;
    QStringList argumentosCut;
    QByteArray TipVirtual;
    procesoFind=new QProcess(this);
    procesoCut=new QProcess(this);
    argumentosFind << ""+Ruta+"";
    argumentosCut << ""+Valor+"";
    procesoFind->setStandardOutputProcess(procesoCut);
    procesoFind->start("cat",argumentosFind);
    procesoCut->start("grep",argumentosCut);
    if (! procesoCut->waitForStarted())
        return QString("");
    procesoFind->waitForFinished();
    procesoCut->waitForFinished();
    TipVirtual = QString(procesoCut->readAllStandardOutput());
    delete procesoFind;
    delete procesoCut;
    QString res =  QString(TipVirtual);
    res.chop(1);
    return res;
}

void UsbLive::mibFin()
{
    disconnect(mib, SIGNAL(publicarDatos(QString)), this, SLOT(mibEscribir(QString)));
    disconnect(mib, SIGNAL(progreso(QString)), this, SLOT(mibprogreso(QString)));
    disconnect(mib, SIGNAL(DatoComand(QString)), this, SLOT(mibReferencia(QString)));
    disconnect(mib, SIGNAL(finProceso()), this, SLOT(mibFin()));
    ui->label_4->show();
    ui->progressBar->setValue(0);
    ui->progressBar->hide();
    ui->label_6->hide();
    ui->pushButton_21->setText(tr("Comenzar"));
    Ctrl = 0;
    TipoGrub = "NADA";
    comandos.clear();
}

QString UsbLive::getGrub(QString grub)
{
    QProcess *procesoRPM;
    QStringList argumentosRPM;
    QByteArray Paquete;
    procesoRPM=new QProcess(this);
    argumentosRPM << ""+grub+"";
    procesoRPM->start("find",argumentosRPM);
    if (! procesoRPM->waitForStarted())
        return QString("");
    procesoRPM->waitForFinished();
    Paquete = QString(procesoRPM->readAllStandardOutput());
    procesoRPM->waitForFinished();
    delete procesoRPM;
    QString res =  QString(Paquete);
    res.chop(1);
    return res;
}

QString UsbLive::getIso(QString iso)
{
    QProcess *procesoRPM;
    QStringList argumentosRPM;
    QByteArray Paquete;
    procesoRPM=new QProcess(this);
    argumentosRPM << ""+iso+"";
    procesoRPM->start("ls",argumentosRPM);
    if (! procesoRPM->waitForStarted())
        return QString("");
    procesoRPM->waitForFinished();
    Paquete = QString(procesoRPM->readAllStandardOutput());
    procesoRPM->waitForFinished();
    delete procesoRPM;
    QString res =  QString(Paquete);
    res.chop(1);
    return res;
}

void UsbLive::on_pushButton_23_clicked()
{           
    QTableWidgetItem *item, *item1, *item2;
    QString activo, activo1, activo2;
    QStringList comandos;
    int itemCount = ui->tableWidget->currentRow();
    if (itemCount == -1)
    {
        QMessageBox m;
        if (Stilo == "A")
            m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
        m.setText(tr("No has seleccionado ningun registro, por lo que no puedes procesar la solicitud."));
        m.exec();
        return;
    }
    else
    {
        item = ui->tableWidget->item(itemCount,3);
        item1 = ui->tableWidget->item(itemCount,5);
        item2 = ui->tableWidget->item(itemCount,1);
        activo = item->text();
        activo1 = item1->text();
        activo2 = item2->text();
        if (activo1 == "Ok")
        {
            int respuesta = 0;
            {
                respuesta = QMessageBox::question(this, QString::fromUtf8(tr("Vaciar el contenido del USB")),
                                   QString::fromUtf8(tr("<center><b>Vaciar USB<b></center><p>"
                                   "En el caso de instalar/utilizar GRUB/GRUB2, no es necesario formatear el USB, ya que con "
                                   "simple hecho de borrar su contenido es posible utilizarlo correctamente.<p>"
                                   "Tambien es aconsejable si quieres vaciarlo por algun motivo, como para cambiar las ISO's "
                                   "instaladas sin tener que formatear, siempre que se utilice GRUB/GRUB2.<p>"
                                   "<b>Procede con mucha precaucion.<p>"
                                   "&iquest;Estas seguro de querer vaciar la unidad "+activo+" denominada "+activo2+"?")), QMessageBox::Ok, QMessageBox::No);
            }
            if (respuesta == QMessageBox::Ok)
            {
                Ctrl = 1;
                ui->label_4->hide();
                ui->textEdit->setText("");
                QString cm1= QString::fromUtf8(tr("echo Borrado el contenido del USB"));
                system("rm -vR -f "+ruta+""+activo2+"/*");
                this->on_pushButton_16_clicked();
                ui->radioButton_8->setChecked(false);
                comandos << cm1;
                if (mib != 0)
                    delete mib;
                mib = new DrakeProcesos(comandos, this);
                connect(mib, SIGNAL(publicarDatos(QString)), this, SLOT(mibEscribir(QString)));
                connect(mib, SIGNAL(finProceso()), this, SLOT(mibFin()));
                int valor= comandos.count();
                mib->Valor(valor,3);
                mib->Mascara(cantidad51,cantidad50,cantidad49,DatoTalla,cantidad47,DatoNegro);
                mib->iniciarProceso();
            }
            return;
        }
        else
        {
            QMessageBox m;
            if (Stilo == "A")
                m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
            m.setText(tr("El USB seleccionado no esta activado, por lo que no puedes procesar la solicitud."));
            m.exec();
            return;
        }
    }
}

QString UsbLive::getDistroIns(QString dev, QString distro)
{
    QProcess *procesoCat, *procesoGrep;
    QStringList argumentosCat, argumentosGrep;
    QByteArray Label;
    procesoCat=new QProcess(this);
    procesoGrep=new QProcess(this);
    argumentosCat << ""+dev+"";
    argumentosGrep << ""+distro+"";
    procesoCat->setStandardOutputProcess(procesoGrep);
    procesoCat->start("ls", argumentosCat);
    procesoGrep->start("grep", argumentosGrep);
    if (! procesoGrep->waitForStarted())
        return QString("");
    procesoCat->waitForFinished();
    procesoGrep->waitForFinished();
    Label = procesoGrep->readAllStandardOutput();
    delete procesoCat;
    delete procesoGrep;
    QString res = QString(Label);
    res.chop(1);
    return res;
}

void UsbLive::ProbarUSB()
{
    comandos.clear();
    int respuesta = 0;
    {
        respuesta = QMessageBox::question(this, QString::fromUtf8(tr("Probar USB")),
                    QString::fromUtf8(tr("<center><b>Probar USB<b></center><p>"
                    "Puedes probar la distros instaladas en el USB mediante emulacion para ver si "
                    "todo ha ido correcto.<p>"
                    "&iquest;Probar la instalacion realizada en el USB "+Valor1+"?")), QMessageBox::Ok, QMessageBox::No);
    }
    if (respuesta == QMessageBox::Ok)
    {
        QString cm1= QString::fromUtf8(tr("echo Probando la instalacion realizada..."));
        QString cm2= "qemu-system-x86_64 %2";
        cm2=cm2.arg(Valor1);
        comandos << cm1 << cm2;
        if (mib != 0)
            delete mib;
        mib = new DrakeProcesos(comandos, this);
        connect(mib, SIGNAL(publicarDatos(QString)), this, SLOT(mibEscribir(QString)));
        connect(mib, SIGNAL(finProceso()), this, SLOT(mibFin()));
        int valor= comandos.count();
        mib->Valor(valor,3);
        mib->Mascara(cantidad51,cantidad50,cantidad49,DatoTalla,cantidad47,DatoNegro);
        mib->iniciarProceso();
    }
    return;
}

void UsbLive::tableClicked(int row, int /*column*/)
{
    QTableWidgetItem *item1, *item2, *item3, *item4, *item5;
    row = ui->tableWidget->currentRow();
    item1=new QTableWidgetItem;
    item2=new QTableWidgetItem;
    item3=new QTableWidgetItem;
    item4=new QTableWidgetItem;
    item5=new QTableWidgetItem;
    item1 = ui->tableWidget->item(row,5);
    item2 = ui->tableWidget->item(row,1);
    item3 = ui->tableWidget->item(row,0);
    item4 = ui->tableWidget->item(row,3);
    item5 = ui->tableWidget->item(row,4);
    activo1 = item1->text();
    activo2 = item2->text();
    activo3 = item3->text();
    activo4 = item4->text();
    activo5 = item5->text();
    QString Localizar = getDistroIns(""+ruta+""+activo2+"/boot/grub/", "menu.lst");
    QString TipGrub3 = getIso(""+ruta+""+activo2+"/boot/");
    TipGrub3.remove("/");
    QString Localizar1;
    if (TipGrub3 == "grub")
        Localizar1 = getDistroIns(""+ruta+""+activo2+"/boot/grub/", "grub.cfg");
    else if (TipGrub3 == "grub2")
        Localizar1 = getDistroIns(""+ruta+""+activo2+"/boot/grub2/", "grub.cfg");
    if (Localizar == "menu.lst")
    {
        ui->radioButton_12->setChecked(true);
        ui->radioButton_4->setChecked(false);
    }
    else if (Localizar1 == "grub.cfg")
    {
        ui->radioButton_12->setChecked(false);
        ui->radioButton_4->setChecked(true);
    }
    QString Localizar3 = getDistroIns(""+ruta+""+activo2+"/", "boot");
    Localizar3=Localizar3.remove("/");
    QString Localizar4 = getDistroIns(""+ruta+""+activo2+"/", "draklive");
    Localizar4=Localizar4.remove("/");
    if (Localizar4 == "draklive" || Localizar3 == "boot")
    {
        ui->radioButton_8->setChecked(true);
        ui->radioButton_11->setChecked(false);
    }
    else
    {
        ui->radioButton_8->setChecked(false);
        ui->radioButton_11->setChecked(true);
    }
}

QString UsbLive::getLs (QString Ruta, QString Dato)
{
    QProcess *procesoFind, *procesoCut;
    QStringList argumentosFind;
    QStringList argumentosCut;
    QByteArray TipVirtual;
    procesoFind=new QProcess(this);
    procesoCut=new QProcess(this);
    argumentosFind << ""+Ruta+"";
    argumentosCut << ""+Dato+"";
    procesoFind->setStandardOutputProcess(procesoCut);
    procesoFind->start("ls",argumentosFind);
    procesoCut->start("grep",argumentosCut);
    if (! procesoCut->waitForStarted())
        return QString("");
    procesoFind->waitForFinished();
    procesoCut->waitForFinished();
    TipVirtual = QString(procesoCut->readAllStandardOutput());
    delete procesoFind;
    delete procesoCut;
    QString res =  QString(TipVirtual);
    res.chop(1);
    return res;
}

void UsbLive::Enable()
{
    ui->groupBox_12->setEnabled(false);
    ui->pushButton_19->setEnabled(false);
    ui->pushButton_20->setEnabled(false);
    ui->pushButton_23->setEnabled(false);
    ui->pushButton_16->setEnabled(false);
    ui->pushButton_15->setEnabled(false);
    ui->tableWidget->setEnabled(false);
    ui->label_9->hide();
}

void UsbLive::Disabled()
{
    ui->groupBox_12->setEnabled(true);
    ui->pushButton_19->setEnabled(true);
    ui->pushButton_20->setEnabled(true);
    ui->pushButton_23->setEnabled(true);
    ui->pushButton_16->setEnabled(true);
    ui->pushButton_15->setEnabled(true);
    ui->tableWidget->setEnabled(true);
    ui->label_9->show();
}

void UsbLive::on_comboBox_11_currentIndexChanged(const QString &arg1)
{    
    if (ui->checkBox->isChecked())
    {
        Q_UNUSED(arg1);
        this->TipoArch("Mandriva");
        this->Disponibilidad("Mandriva");
    }
}

void UsbLive::on_comboBox_12_currentIndexChanged(const QString &arg1)
{
    if (ui->checkBox_2->isChecked())
    {
        Q_UNUSED(arg1);
        this->TipoArch("PCLinuxOS");
        this->Disponibilidad("PCLinuxOS");
    }
}

void UsbLive::on_comboBox_13_currentIndexChanged(const QString &arg1)
{
    if (ui->checkBox_3->isChecked())
    {
        Q_UNUSED(arg1);
        this->TipoArch("Fedora");
        this->Disponibilidad("Fedora");
    }
}

void UsbLive::on_comboBox_14_currentIndexChanged(const QString &arg1)
{
    if (ui->checkBox_4->isChecked())
    {
        Q_UNUSED(arg1);
        this->TipoArch("Ubuntu");
        this->Disponibilidad("Ubuntu");
    }
}

void UsbLive::on_comboBox_15_currentIndexChanged(const QString &arg1)
{
    if (ui->checkBox_5->isChecked())
    {
        Q_UNUSED(arg1);
        this->TipoArch("ArchLinux");
        this->Disponibilidad("ArchLinux");
    }
}

void UsbLive::on_comboBox_16_currentIndexChanged(const QString &arg1)
{
    if (ui->checkBox_6->isChecked())
    {
        Q_UNUSED(arg1);
        this->TipoArch("CentOS");
        this->Disponibilidad("CentOS");
    }
}

void UsbLive::on_comboBox_17_currentIndexChanged(const QString &arg1)
{
    if (ui->checkBox_7->isChecked())
    {
        Q_UNUSED(arg1);
        this->TipoArch("Debian");
        this->Disponibilidad("Debian");
    }
}

void UsbLive::on_comboBox_18_currentIndexChanged(const QString &arg1)
{
    if (ui->checkBox_19->isChecked())
    {
        Q_UNUSED(arg1);
        this->TipoArch("KNOPPIX");
        this->Disponibilidad("KNOPPIX");
    }
}

void UsbLive::on_comboBox_19_currentIndexChanged(const QString &arg1)
{
    if (ui->checkBox_9->isChecked())
    {
        Q_UNUSED(arg1);
        this->TipoArch("OpenSUSE");
        this->Disponibilidad("OpenSUSE");
    }
}

void UsbLive::on_comboBox_20_currentIndexChanged(const QString &arg1)
{
    if (ui->checkBox_8->isChecked())
    {
        Q_UNUSED(arg1);
        this->TipoArch("LinuxMint");
        this->Disponibilidad("LinuxMint");
    }
}

void UsbLive::on_checkBox_clicked(bool checked)
{
    if (checked == true)
        this->Disponibilidad("Mandriva");
}

void UsbLive::on_checkBox_2_clicked(bool checked)
{
    if (checked == true)
        this->Disponibilidad("PCLinuxOS");
}

void UsbLive::on_checkBox_3_clicked(bool checked)
{
    if (checked == true)
        this->Disponibilidad("Fedora");
}

void UsbLive::on_checkBox_4_clicked(bool checked)
{
    if (checked == true)
        this->Disponibilidad("Ubuntu");
}

void UsbLive::on_checkBox_5_clicked(bool checked)
{
    if (checked == true)
        this->Disponibilidad("ArchLinux");
}

void UsbLive::on_checkBox_6_clicked(bool checked)
{
    if (checked == true)
        this->Disponibilidad("CentOS");
}

void UsbLive::on_checkBox_7_clicked(bool checked)
{
    if (checked == true)
        this->Disponibilidad("Debian");
}

void UsbLive::on_checkBox_19_clicked(bool checked)
{
    if (checked == true)
        this->Disponibilidad("KNOPPIX");
}

void UsbLive::on_checkBox_9_clicked(bool checked)
{
    if (checked == true)
        this->Disponibilidad("OpenSUSE");
}

void UsbLive::on_checkBox_8_clicked(bool checked)
{
    if (checked == true)
        this->Disponibilidad("LinuxMint");
}

void UsbLive::on_comboBox_currentIndexChanged(const QString &arg1)
{
    if (ui->checkBox->isChecked())
    {
        Q_UNUSED(arg1);
        if (ui->radioButton->isChecked())
            this->Disponibilidad("Mandriva");
        else
            ui->label_47->setText("");
    }
}

void UsbLive::on_comboBox_2_currentIndexChanged(const QString &arg1)
{
    if (ui->checkBox_2->isChecked())
    {
        Q_UNUSED(arg1);
        if (ui->radioButton_5->isChecked())
            this->Disponibilidad("PCLinuxOS");
        else
            ui->label_80->setText("");
    }
}

void UsbLive::on_comboBox_3_currentIndexChanged(const QString &arg1)
{
    if (ui->checkBox_3->isChecked())
    {
        Q_UNUSED(arg1);
        if (ui->radioButton_9->isChecked())
            this->Disponibilidad("Fedora");
        else
            ui->label_81->setText("");
    }
}

void UsbLive::on_comboBox_4_currentIndexChanged(const QString &arg1)
{
    if (ui->checkBox_4->isChecked())
    {
        Q_UNUSED(arg1);
        if (ui->radioButton_13->isChecked())
            this->Disponibilidad("Ubuntu");
        else
            ui->label_82->setText("");
    }
}

void UsbLive::on_comboBox_5_currentIndexChanged(const QString &arg1)
{
    if (ui->checkBox_5->isChecked())
    {
        Q_UNUSED(arg1);
        if (ui->radioButton_17->isChecked())
            this->Disponibilidad("ArchLinux");
        else
            ui->label_89->setText("");
    }
}

void UsbLive::on_comboBox_6_currentIndexChanged(const QString &arg1)
{
    if (ui->checkBox_6->isChecked())
    {
        Q_UNUSED(arg1);
        if (ui->radioButton_39->isChecked())
            this->Disponibilidad("CentOS");
        else
            ui->label_99->setText("");
    }
}

void UsbLive::on_comboBox_7_currentIndexChanged(const QString &arg1)
{
    if (ui->checkBox_7->isChecked())
    {
        Q_UNUSED(arg1);
        if (ui->radioButton_43->isChecked())
            this->Disponibilidad("Debian");
        else
            ui->label_108->setText("");
    }
}

void UsbLive::on_comboBox_8_currentIndexChanged(const QString &arg1)
{
    if (ui->checkBox_19->isChecked())
    {
        Q_UNUSED(arg1);
        if (ui->radioButton_55->isChecked())
            this->Disponibilidad("KNOPPIX");
        else
            ui->label_136->setText("");
    }
}

void UsbLive::on_comboBox_9_currentIndexChanged(const QString &arg1)
{
    if (ui->checkBox_9->isChecked())
    {
        Q_UNUSED(arg1);
        if (ui->radioButton_51->isChecked())
            this->Disponibilidad("OpenSUSE");
        else
            ui->label_127->setText("");
    }
}

void UsbLive::on_comboBox_10_currentIndexChanged(const QString &arg1)
{
    if (ui->checkBox_8->isChecked())
    {
        Q_UNUSED(arg1);
        if (ui->radioButton_47->isChecked())
            this->Disponibilidad("LinuxMint");
        else
            ui->label_118->setText("");
    }
}

void UsbLive::on_checkBox_13_clicked()
{
    if (ui->checkBox_13->isChecked() == true)
    {
        ui->comboBox_27->setEnabled(true);
        ui->radioButton_23->setEnabled(true);
        ui->radioButton_22->setEnabled(true);
        ui->label_15->setEnabled(true);
        ui->label_74->setEnabled(true);
    }
    else
    {
        ui->comboBox_27->setEnabled(false);
        ui->radioButton_23->setEnabled(false);
        ui->radioButton_22->setEnabled(false);
        ui->pushButton_27->setEnabled(false);
        ui->label_15->setEnabled(false);
        ui->label_74->setEnabled(false);
    }
}

void UsbLive::on_checkBox_14_clicked()
{
    if (ui->checkBox_14->isChecked() == true)
    {
        ui->comboBox_29->setEnabled(true);
        ui->radioButton_25->setEnabled(true);
        ui->radioButton_24->setEnabled(true);
        ui->label_16->setEnabled(true);
        ui->label_84->setEnabled(true);
    }
    else
    {
        ui->comboBox_29->setEnabled(false);
        ui->radioButton_25->setEnabled(false);
        ui->radioButton_24->setEnabled(false);
        ui->pushButton_28->setEnabled(false);
        ui->label_16->setEnabled(false);
        ui->label_84->setEnabled(false);
    }
}

void UsbLive::on_checkBox_15_clicked()
{
    if (ui->checkBox_15->isChecked() == true)
    {
        ui->comboBox_31->setEnabled(true);
        ui->radioButton_27->setEnabled(true);
        ui->radioButton_26->setEnabled(true);
        ui->label_17->setEnabled(true);
        ui->label_87->setEnabled(true);
    }
    else
    {
        ui->comboBox_31->setEnabled(false);
        ui->radioButton_27->setEnabled(false);
        ui->radioButton_26->setEnabled(false);
        ui->pushButton_29->setEnabled(false);
        ui->label_17->setEnabled(false);
        ui->label_87->setEnabled(false);
    }
}

void UsbLive::on_checkBox_17_clicked()
{
    if (ui->checkBox_17->isChecked() == true)
    {
        ui->comboBox_35->setEnabled(true);
        ui->radioButton_31->setEnabled(true);
        ui->radioButton_30->setEnabled(true);
        ui->label_20->setEnabled(true);
        ui->label_78->setEnabled(true);
    }
    else
    {
        ui->comboBox_35->setEnabled(false);
        ui->radioButton_31->setEnabled(false);
        ui->radioButton_30->setEnabled(false);
        ui->pushButton_31->setEnabled(false);
        ui->label_20->setEnabled(false);
        ui->label_78->setEnabled(false);
    }
}

void UsbLive::on_radioButton_23_clicked()
{
    if (ui->radioButton_23->isChecked() == true)
    {
        ui->pushButton_27->setEnabled(false);
        ui->label_15->setEnabled(true);
        ui->label_74->setEnabled(true);
        ui->label_23->setEnabled(false);
        ui->label_75->setEnabled(false);
    }
}

void UsbLive::on_radioButton_22_clicked()
{
    if (ui->radioButton_22->isChecked() == true)
    {
        ui->pushButton_27->setEnabled(true);
        ui->label_15->setEnabled(false);
        ui->label_74->setEnabled(false);
        ui->label_23->setEnabled(true);
        ui->label_75->setEnabled(true);
    }
}

void UsbLive::on_radioButton_25_clicked()
{
    if (ui->radioButton_25->isChecked() == true)
    {
        ui->pushButton_28->setEnabled(false);
        ui->label_16->setEnabled(true);
        ui->label_84->setEnabled(true);
        ui->label_24->setEnabled(false);
        ui->label_85->setEnabled(false);
    }
}

void UsbLive::on_radioButton_24_clicked()
{
    if (ui->radioButton_24->isChecked() == true)
    {
        ui->pushButton_28->setEnabled(true);
        ui->label_16->setEnabled(false);
        ui->label_84->setEnabled(false);
        ui->label_24->setEnabled(true);
        ui->label_85->setEnabled(true);
    }
}

void UsbLive::on_radioButton_27_clicked()
{
    if (ui->radioButton_27->isChecked() == true)
    {
        ui->pushButton_29->setEnabled(false);
        ui->label_17->setEnabled(true);
        ui->label_87->setEnabled(true);
        ui->label_25->setEnabled(false);
        ui->label_88->setEnabled(false);
    }
}

void UsbLive::on_radioButton_26_clicked()
{
    if (ui->radioButton_26->isChecked() == true)
    {
        ui->pushButton_29->setEnabled(true);
        ui->label_17->setEnabled(false);
        ui->label_87->setEnabled(false);
        ui->label_25->setEnabled(true);
        ui->label_88->setEnabled(true);
    }
}

void UsbLive::on_radioButton_31_clicked()
{
    if (ui->radioButton_31->isChecked() == true)
    {
        ui->pushButton_31->setEnabled(false);
        ui->label_20->setEnabled(true);
        ui->label_78->setEnabled(true);
        ui->label_29->setEnabled(false);
        ui->label_79->setEnabled(false);
    }
}

void UsbLive::on_radioButton_30_clicked()
{
    if (ui->radioButton_30->isChecked() == true)
    {
        ui->pushButton_31->setEnabled(true);
        ui->label_20->setEnabled(false);
        ui->label_78->setEnabled(false);
        ui->label_29->setEnabled(true);
        ui->label_79->setEnabled(true);
    }
}

void UsbLive::on_comboBox_28_currentIndexChanged(const QString &arg1)
{
    if (ui->checkBox_13->isChecked())
    {
        Q_UNUSED(arg1);
        this->TipoArch("Boot Repair Disk");
        this->Disponibilidad("Boot Repair Disk");
    }
}

void UsbLive::on_comboBox_30_currentIndexChanged(const QString &arg1)
{
    if (ui->checkBox_14->isChecked())
    {
        Q_UNUSED(arg1);
        this->TipoArch("Clonezilla");
        this->Disponibilidad("Clonezilla");
    }
}

void UsbLive::on_comboBox_32_currentIndexChanged(const QString &arg1)
{
    if (ui->checkBox_15->isChecked())
    {
        Q_UNUSED(arg1);
        this->TipoArch("GParted");
        this->Disponibilidad("GParted");
    }
}

void UsbLive::on_comboBox_36_currentIndexChanged(const QString &arg1)
{
    if (ui->checkBox_17->isChecked())
    {
        Q_UNUSED(arg1);
        this->TipoArch("System Rescue CD");
        this->Disponibilidad("System Rescue CD");
    }
}

void UsbLive::on_checkBox_13_clicked(bool checked)
{
    if (checked == true)
        this->Disponibilidad("Boot Repair Disk");
}

void UsbLive::on_checkBox_14_clicked(bool checked)
{
    if (checked == true)
        this->Disponibilidad("Clonezilla");
}

void UsbLive::on_checkBox_15_clicked(bool checked)
{
    if (checked == true)
        this->Disponibilidad("GParted");
}

void UsbLive::on_checkBox_17_clicked(bool checked)
{
    if (checked == true)
        this->Disponibilidad("System Rescue CD");
}

void UsbLive::on_comboBox_27_currentIndexChanged(const QString &arg1)
{
    if (ui->checkBox_13->isChecked())
    {
        Q_UNUSED(arg1);
        if (ui->radioButton_23->isChecked())
            this->Disponibilidad("Boot Repair Disk");
        else
            ui->label_75->setText("");
    }
}

void UsbLive::on_comboBox_29_currentIndexChanged(const QString &arg1)
{
    if (ui->checkBox_14->isChecked())
    {
        Q_UNUSED(arg1);
        if (ui->radioButton_25->isChecked())
            this->Disponibilidad("Clonezilla");
        else
            ui->label_85->setText("");
    }
}

void UsbLive::on_comboBox_31_currentIndexChanged(const QString &arg1)
{
    if (ui->checkBox_15->isChecked())
    {
        Q_UNUSED(arg1);
        if (ui->radioButton_27->isChecked())
            this->Disponibilidad("GParted");
        else
            ui->label_88->setText("");
    }
}

void UsbLive::on_comboBox_35_currentIndexChanged(const QString &arg1)
{
    if (ui->checkBox_17->isChecked())
    {
        Q_UNUSED(arg1);
        if (ui->radioButton_31->isChecked())
            this->Disponibilidad("System Rescue CD");
        else
            ui->label_79->setText("");
    }
}

void UsbLive::on_pushButton_27_clicked()
{
    QString path=tr("/home/%1/Documentos/");
    path=path.arg(user);
    QString fileNameOrigen = QFileDialog::getOpenFileName(this,QString::fromUtf8(tr("Abrir archivo de imagen de Boot Repair Disk .iso")),
                        path,trUtf8(tr("Imagen .iso (*.iso)")));
    if (fileNameOrigen.isEmpty())
         return;
    fileName2 = fileNameOrigen.replace(" ", "\\ ").replace("&","\\&").replace("'","\\'").replace("(","\\(").replace(")","\\)");
    QString Version = ui->comboBox_28->currentText();
    Version = Version.split(".").value(0);
    QString Distribucion = ui->groupBox_70->title();
    QString Arquitectura;
    if (ui->comboBox_27->currentText() == "32bit")
        Arquitectura = "32bit";
    else if (ui->comboBox_27->currentText() == "64bit")
        Arquitectura = "64bit";
    if (fileNameOrigen.contains(Distribucion, Qt::CaseInsensitive) != true || fileNameOrigen.contains(Version, Qt::CaseInsensitive) != true || fileNameOrigen.contains(Arquitectura, Qt::CaseInsensitive) != true)
    {
        QMessageBox m;
        if (Stilo == "A")
            m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
        m.setText(tr("El nombre de la imagen debe contener "+Distribucion+", "+Arquitectura+" y "+Version+" para poder continuar."));
        m.exec();
        return;
    }
    ui->label_75->setText(fileNameOrigen);
}

void UsbLive::on_pushButton_28_clicked()
{
    QString path=tr("/home/%1/Documentos/");
    path=path.arg(user);
    QString fileNameOrigen = QFileDialog::getOpenFileName(this,QString::fromUtf8(tr("Abrir archivo de imagen de Clonezilla .iso")),
                        path,trUtf8(tr("Imagen .iso (*.iso)")));
    if (fileNameOrigen.isEmpty())
         return;
    fileName2 = fileNameOrigen.replace(" ", "\\ ").replace("&","\\&").replace("'","\\'").replace("(","\\(").replace(")","\\)");
    QString Version = ui->comboBox_30->currentText();
    Version = Version.split(".").value(0);
    QString Distribucion = ui->groupBox_76->title();
    QString Arquitectura;
    if (ui->comboBox_29->currentText() == "32bit")
        Arquitectura = "i486";
    else if (ui->comboBox_29->currentText() == "64bit")
        Arquitectura = "amd64";
    if (fileNameOrigen.contains(Distribucion, Qt::CaseInsensitive) != true || fileNameOrigen.contains(Version, Qt::CaseInsensitive) != true || fileNameOrigen.contains(Arquitectura, Qt::CaseInsensitive) != true)
    {
        QMessageBox m;
        if (Stilo == "A")
            m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
        m.setText(tr("El nombre de la imagen debe contener "+Distribucion+", "+Arquitectura+" y "+Version+" para poder continuar."));
        m.exec();
        return;
    }
    ui->label_85->setText(fileNameOrigen);
}

void UsbLive::on_pushButton_29_clicked()
{
    QString path=tr("/home/%1/Documentos/");
    path=path.arg(user);
    QString fileNameOrigen = QFileDialog::getOpenFileName(this,QString::fromUtf8(tr("Abrir archivo de imagen de GParted .iso")),
                        path,trUtf8(tr("Imagen .iso (*.iso)")));
    if (fileNameOrigen.isEmpty())
         return;
    fileName2 = fileNameOrigen.replace(" ", "\\ ").replace("&","\\&").replace("'","\\'").replace("(","\\(").replace(")","\\)");
    QString Version = ui->comboBox_32->currentText();
    Version = Version.split(".").value(0);
    QString Distribucion = ui->groupBox_82->title();
    QString Arquitectura;
    if (ui->comboBox_31->currentText() == "32bit")
        Arquitectura = "i486";
    else if (ui->comboBox_31->currentText() == "64bit")
        Arquitectura = "amd64";
    if (fileNameOrigen.contains(Distribucion, Qt::CaseInsensitive) != true || fileNameOrigen.contains(Version, Qt::CaseInsensitive) != true || fileNameOrigen.contains(Arquitectura, Qt::CaseInsensitive) != true)
    {
        QMessageBox m;
        if (Stilo == "A")
            m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
        m.setText(tr("El nombre de la imagen debe contener "+Distribucion+", "+Arquitectura+" y "+Version+" para poder continuar."));
        m.exec();
        return;
    }
    ui->label_88->setText(fileNameOrigen);
}

void UsbLive::on_pushButton_31_clicked()
{
    QString path=tr("/home/%1/Documentos/");
    path=path.arg(user);
    QString fileNameOrigen = QFileDialog::getOpenFileName(this,QString::fromUtf8(tr("Abrir archivo de imagen de System Rescue CD .iso")),
                        path,trUtf8(tr("Imagen .iso (*.iso)")));
    if (fileNameOrigen.isEmpty())
         return;
    fileName2 = fileNameOrigen.replace(" ", "\\ ").replace("&","\\&").replace("'","\\'").replace("(","\\(").replace(")","\\)");
    QString Version = ui->comboBox_36->currentText();
    Version = Version.split(".").value(0);
    QString Distribucion = ui->groupBox_100->title();
    QString Arquitectura;
    if (ui->comboBox_35->currentText() == "64bit")
        Arquitectura = "x86";
    if (fileNameOrigen.contains(Distribucion, Qt::CaseInsensitive) != true || fileNameOrigen.contains(Version, Qt::CaseInsensitive) != true || fileNameOrigen.contains(Arquitectura, Qt::CaseInsensitive) != true)
    {
        QMessageBox m;
        if (Stilo == "A")
            m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
        m.setText(tr("El nombre de la imagen debe contener "+Distribucion+", "+Arquitectura+" y "+Version+" para poder continuar."));
        m.exec();
        return;
    }
    ui->label_79->setText(fileNameOrigen);
}

void UsbLive::on_comboBox_22_currentIndexChanged(const QString &arg1)
{
    if (ui->checkBox_10->isChecked())
    {
        Q_UNUSED(arg1);
        this->TipoArch("OpenMandriva");
        this->Disponibilidad("OpenMandriva");
    }
}

void UsbLive::on_comboBox_21_currentIndexChanged(const QString &arg1)
{
    if (ui->checkBox_10->isChecked())
    {
        Q_UNUSED(arg1);
        if (ui->radioButton_7->isChecked())
            this->Disponibilidad("OpenMandriva");
        else
            ui->label_48->setText("");
    }
}

void UsbLive::on_radioButton_7_clicked()
{
    if (ui->radioButton_7->isChecked() == true)
    {
        ui->pushButton_24->setEnabled(false);
        ui->label_12->setEnabled(true);
        ui->label_76->setEnabled(true);
        ui->label_14->setEnabled(false);
        ui->label_48->setEnabled(false);
    }
}

void UsbLive::on_radioButton_3_clicked()
{
    if (ui->radioButton_3->isChecked() == true)
    {
        ui->pushButton_24->setEnabled(true);
        ui->label_12->setEnabled(false);
        ui->label_76->setEnabled(false);
        ui->label_14->setEnabled(true);
        ui->label_48->setEnabled(true);
    }
}

void UsbLive::on_pushButton_24_clicked()
{
    QString path=tr("/home/%1/Documentos/");
    path=path.arg(user);
    QString fileNameOrigen = QFileDialog::getOpenFileName(this,QString::fromUtf8(tr("Abrir archivo de imagen de OpenMandriva .iso")),
                        path,trUtf8(tr("Imagen .iso (*.iso)")));
    if (fileNameOrigen.isEmpty())
         return;
    fileName2 = fileNameOrigen.replace(" ", "\\ ").replace("&","\\&").replace("'","\\'").replace("(","\\(").replace(")","\\)");
    QString Version = ui->comboBox_22->currentText();
    Version = Version.split(".").value(0);
    QString Distribucion = ui->groupBox_9->title();
    QString Arquitectura;
    if (ui->comboBox_21->currentText() == "32bit")
        Arquitectura = "i586";
    else if (ui->comboBox_21->currentText() == "64bit")
        Arquitectura = "x86_64";
    if (fileNameOrigen.contains(Distribucion, Qt::CaseInsensitive) != true || fileNameOrigen.contains(Version, Qt::CaseInsensitive) != true || fileNameOrigen.contains(Arquitectura, Qt::CaseInsensitive) != true)
    {
        QMessageBox m;
        if (Stilo == "A")
            m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
        m.setText(tr("El nombre de la imagen debe contener "+Distribucion+", "+Arquitectura+" y "+Version+" para poder continuar."));
        m.exec();
        return;
    }
    ui->label_48->setText(fileNameOrigen);
}

void UsbLive::on_checkBox_10_clicked(bool checked)
{
    if (checked == true)
        this->Disponibilidad("OpenMandriva");
}

void UsbLive::on_checkBox_10_clicked()
{
    if (ui->checkBox_10->isChecked() == true)
    {
        ui->comboBox_21->setEnabled(true);
        ui->radioButton_7->setEnabled(true);
        ui->radioButton_3->setEnabled(true);
        ui->label_12->setEnabled(true);
        ui->label_76->setEnabled(true);
    }
    else
    {
        ui->comboBox_21->setEnabled(false);
        ui->radioButton_7->setEnabled(false);
        ui->radioButton_3->setEnabled(false);
        ui->pushButton_24->setEnabled(false);
        ui->label_12->setEnabled(false);
        ui->label_76->setEnabled(false);
    }
}

void UsbLive::on_pushButton_25_clicked()
{
    QTableWidgetItem *item1, *item2;
    int itemCount;
    QString activo, resultado;
    itemCount = ui->tableWidget->rowCount();
    if (itemCount == -1 || itemCount == 0)
    {
        QMessageBox m;
        if (Stilo == "A")
            m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
        m.setText(tr("No has seleccionado ningun USB, por lo que no puedes realizar la emulacion."));
        m.exec();
        return;
    }
    else
    {
        int ContOk = 0;
        for(int a=0;a<itemCount;a++)
        {
            item1 = ui->tableWidget->item(a,5);
            activo = item1->text();
            if (activo == "Ok")
            {
                ContOk++;
                item2 = ui->tableWidget->item(a,0);
                resultado = item2->text();
                break;
            }
        }
        if (ContOk == 0)
        {
            QMessageBox m;
            if (Stilo == "A")
                m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
            m.setText(tr("No has seleccionado ningun USB, por lo que no puedes realizar la emulacion."));
            m.exec();
            return;
        }
    }
    QString cm1= QString::fromUtf8(tr("echo Probando la instalacion realizada..."));
    QString cm2= "qemu-system-x86_64 %2";
    cm2=cm2.arg(resultado);
    comandos << cm1 << cm2;
    if (mib != 0)
        delete mib;
    mib = new DrakeProcesos(comandos, this);
    connect(mib, SIGNAL(finProceso()), this, SLOT(mibFin()));
    int valor= comandos.count();
    mib->Valor(valor,5);
    mib->Mascara(cantidad51,cantidad50,cantidad49,DatoTalla,cantidad47,DatoNegro);
    mib->iniciarProceso();
}

void UsbLive::on_pushButton_30_clicked()
{
    int iFilas;
    QString valor;
    iFilas=ui->tableWidget_2->currentRow();
    if (iFilas == -1)
    {
        QMessageBox m;
        if (Stilo == "A")
            m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
        m.setText(tr("No has seleccionado ningun registro, por lo que no puedes procesar la solicitud."));
        m.exec();
        return;
    }
    else
    {
        QTableWidgetItem *item;
        item=new QTableWidgetItem;
        item=ui->tableWidget_2->item(iFilas,4);
        valor=item->text();
        if(valor != "Ok")
        {
            ui->tableWidget_2->item(iFilas,4)->setIcon(QIcon(":/Imagenes/good.png"));
            item->setText("Ok");
            ui->tableWidget_2->setItem(iFilas,4,item);
        }
    }
}

void UsbLive::on_pushButton_32_clicked()
{
    int iFilas;
    QString valor;
    iFilas=ui->tableWidget_2->currentRow();
    if (iFilas == -1)
    {
        QMessageBox m;
        if (Stilo == "A")
            m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
        m.setText(tr("No has seleccionado ningun registro, por lo que no puedes procesar la solicitud."));
        m.exec();
        return;
    }
    else
    {
        QTableWidgetItem *item;
        item=new QTableWidgetItem;
        item=ui->tableWidget_2->item(iFilas,4);
        valor=item->text();
        if(valor == "Ok")
        {
            ui->tableWidget_2->item(iFilas,4)->setIcon(QIcon(":/Imagenes/bad.png"));
            item->setText("No");
            ui->tableWidget_2->setItem(iFilas,4,item);
        }
    }
}

void UsbLive::on_pushButton_36_clicked()
{
    if (ui->radioButton_16->isChecked())
            TipoGrub = "grub2";
    else if( ui->radioButton_15->isChecked())
            TipoGrub = "grub";
    int Activo = 0;
    ui->textEdit_4->setText("");
    QTableWidgetItem *item, *item1, *item2, *item3, *item4;
    QString activo, activo1, Nombre, Version, Arquitectura, Carpeta;
    int itemCount = ui->tableWidget_2->rowCount();
    if (itemCount == -1)
    {
        QMessageBox m;
        if (Stilo == "A")
            m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
        m.setText(tr("No has seleccionado ningun registro, por lo que no puedes procesar la solicitud."));
        m.exec();
        return;
    }
    else
    {
        int respuesta = 0;
        {
            respuesta = QMessageBox::question(this, QString::fromUtf8(tr("Borrado de Distros/Utilidades")),
                               QString::fromUtf8(tr("<center><b>Borrado de Distros/Utilidades<b></center><p>"
                                "Se va a proceder al borrado de las distribuciones y utilidades seleccionadas.<p>"
                               "<b>No se puede volver a atras, excepto que las vuelvas a instalar en el pendrive.<p>"
                               "&iquest;Estas seguro de querer eliminar las distros/utilidades seleccionadas?")), QMessageBox::Ok, QMessageBox::No);
        }
        if (respuesta == QMessageBox::Ok)
        {
            for (int a=0;a<itemCount;a++)
            {
                item1 = ui->tableWidget_2->item(a,4);
                activo1 = item1->text();
                if (activo1 == "Ok")
                    Activo++;
            }
            if (Activo > 0)
            {
                Ctrl = 2;
                for (int a=0;a<itemCount;a++)
                {
                    item = ui->tableWidget_2->item(a,3);
                    item1 = ui->tableWidget_2->item(a,4);
                    activo = item->text();
                    activo1 = item1->text();
                    if (activo1 == "Ok")
                    {
                        QString cm = QString::fromUtf8(tr("echo Eliminando %1..."));
                        cm = cm.arg(activo);
                        QString cmd1 = "rm -vR -f "+ruta+"DRAKLIVE/iso/%1/*";
                        cmd1=cmd1.arg(activo);
                        RecoUSB << cm << cmd1;
                    }
                }
                if (ui->radioButton_15->isChecked())
                {
                    QString Kill0 = "rm -vR -f "+ruta+"%1/boot/";
                    Kill0 = Kill0.arg("DRAKLIVE");
                    RecoUSB << Kill0;
                    QString gru1 = "echo Reinstalando grub en el USB...";
                    QString grub1 = "/sbin/grub-install --root-directory="+ruta+"%1 %2 --no-floppy";
                    grub1 = grub1.arg("DRAKLIVE").arg(ui->label_18->text());
                    RecoUSB << gru1 << grub1;
                    QString Kill = "echo Creando fichero de mapeo de unidades y menu.lst...";
                    QString Kill2 = "touch "+ruta+"%1/boot/grub/menu.lst";
                    Kill2 = Kill2.arg("DRAKLIVE");
                    QString device = "echo Reconstruyendo datos de mapeo y configuracion de inicio de distros para grub...";
                    RecoUSB << Kill << Kill2 << device;
                    QString procesando = "echo Realizando nuevo proceso de insercion de datos de distros en menu.lst...";
                    RecoUSB << procesando;
                    ValorGrub << "timeout 10" << "#color black/cyan yellow/cyan" << "default 0" << "";
                }
                else
                {
                    QString Kill0 = "rm -vR -f "+ruta+"%1/boot/";
                    Kill0 = Kill0.arg("DRAKLIVE");
                    RecoUSB << Kill0;
                    QString gru1 = "echo Reinstalando grub en el USB...";
                    QString grub1 = "/sbin/grub2-install --root-directory="+ruta+"%1 %2 --no-floppy";
                    grub1 = grub1.arg("DRAKLIVE").arg(ui->label_18->text());
                    RecoUSB << gru1 << grub1;
                    QString Kill = "echo Creando fichero de mapeo de unidades y grub.cfg...";
                    QString Kill1 = "touch "+ruta+"%1/boot/grub2/device.map";
                    Kill1 = Kill1.arg("DRAKLIVE");
                    QString Kill2 = "touch "+ruta+"%1/boot/grub2/grub.cfg";
                    Kill2 = Kill2.arg("DRAKLIVE");
                    QString device0 = "echo Introduciendo datos de mapeo y configuracion de inicio de distros...";
                    RecoUSB << Kill << Kill1 << Kill2 << device0;
                    QString gru2 = "echo Reinstalando grub en el USB...";
                    QString grub2 = "/usr/sbin/grub2-install --root-directory="+ruta+"%1 %2 --no-floppy";
                    grub2 = grub2.arg("DRAKLIVE").arg(ui->label_18->text());
                    RecoUSB << gru2 << grub2;
                    QString procesando = "echo Realizando nuevo proceso de insercion de datos de distros en grub.cfg...";
                    RecoUSB << procesando;
                    ValorGrub << "set default=0" << "set timeout=10" << "set root=(hd0,1)" << "terminal console" << "";
                }
                for (int b=0;b<itemCount;b++)
                {
                    item1 = ui->tableWidget_2->item(b,4);
                    item2 = ui->tableWidget_2->item(b,0);
                    item3 = ui->tableWidget_2->item(b,1);
                    item4 = ui->tableWidget_2->item(b,2);
                    item = ui->tableWidget_2->item(b,3);
                    activo1 = item1->text();
                    Nombre = item2->text();
                    Arquitectura = item3->text();
                    Version = item4->text();
                    Carpeta = item->text();
                    if (activo1 != "Ok")
                        this->Reconstruir(Nombre,Version,Arquitectura,Carpeta);
                }
                if (mib != 0)
                    delete mib;
                mib = new DrakeProcesos(RecoUSB, this);
                connect(mib, SIGNAL(publicarDatos(QString)), this, SLOT(mibEscribir(QString)));
                connect(mib, SIGNAL(DatoComand(QString)), this, SLOT(mibReferencia(QString)));
                connect(mib, SIGNAL(finProceso()), this, SLOT(mibFin()));
                int valor = RecoUSB.count();
                mib->Valor(valor,3);
                mib->Mascara(cantidad51,cantidad50,cantidad49,DatoTalla,cantidad47,DatoNegro);
                mib->iniciarProceso();
            }
            else
            {
                QMessageBox m;
                if (Stilo == "A")
                    m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
                m.setText(tr("No hay nada seleccionado para ser eliminado.<p>Comprueba que haya al menos un registro marcado como eliminado"));
                m.exec();
                return;
            }
        }
    }
}

void UsbLive::Reconstruir(QString Nombre, QString Version, QString Arquitectura, QString Carpeta)
{
    if (ui->radioButton_15->isChecked())
    {
        if (Nombre == "Mandriva")
        {
            QString Mandriva;
            QSqlQuery query(db);
            query.exec("SELECT grub FROM Distro WHERE NomDistro='"+Nombre+"' AND Version='"+Version+"' AND Arquitectura='"+Arquitectura+"'");
            query.first();
            if (query.isValid())
                Mandriva=query.value(0).toString();
            QStringList MandrivaDat = Mandriva.split("\n");
            for(int i=0;i<MandrivaDat.count();i++)
            {
                QString Value = MandrivaDat.value(i);
                Value.replace(QString("MANDRIVA"), QString(Carpeta));
                Value.replace(QString("ETIQUETA"), QString("DRAKLIVE"));
                Value.replace("\n","");
                ValorGrub << ""+Value+"";
            }
            ValorGrub << "#";
        }
        if (Nombre == "PCLinuxOS")
        {
            QString PCLinuxOS;
            QSqlQuery query(db);
            query.exec("SELECT grub FROM Distro WHERE NomDistro='"+Nombre+"' AND Version='"+Version+"' AND Arquitectura='"+Arquitectura+"'");
            query.first();
            if (query.isValid())
                PCLinuxOS=query.value(0).toString();
            QStringList PCLinuxOSDat  = PCLinuxOS.split("\n");
            for(int i=0;i<PCLinuxOSDat.count();i++)
            {
                QString Value1 = PCLinuxOSDat.value(i);
                Value1.replace( QString("PCLINUXOS"), QString(Carpeta));
                Value1.replace( QString("ETIQUETA"), QString("DRAKLIVE"));
                Value1.replace("\n","");
                ValorGrub << ""+Value1+"";
            }
            ValorGrub << "#";
        }
        if (Nombre == "Fedora")
        {
            QString Fedora;
            QSqlQuery query(db);
            query.exec("SELECT grub FROM Distro WHERE NomDistro='"+Nombre+"' AND Version='"+Version+"' AND Arquitectura='"+Arquitectura+"'");
            query.first();
            if (query.isValid())
                Fedora=query.value(0).toString();
            QStringList FedoraDat  = Fedora.split("\n");
            for(int i=0;i<FedoraDat.count();i++)
            {
                QString Value1 = FedoraDat.value(i);
                Value1.replace( QString("FEDORA"), QString(Carpeta));
                Value1.replace( QString("ETIQUETA"), QString("DRAKLIVE"));
                Value1.replace("\n","");
                ValorGrub << ""+Value1+"";
            }
            ValorGrub << "#";
        }
        if (Nombre == "Ubuntu")
        {
            QString Ubuntu;
            QSqlQuery query(db);
            query.exec("SELECT grub FROM Distro WHERE NomDistro='"+Nombre+"' AND Version='"+Version+"' AND Arquitectura='"+Arquitectura+"'");
            query.first();
            if (query.isValid())
                Ubuntu=query.value(0).toString();
            QStringList UbuntuDat  = Ubuntu.split("\n");
            for(int i=0;i<UbuntuDat.count();i++)
            {
                QString Value1 = UbuntuDat.value(i);
                Value1.replace( QString("UBUNTU"), QString(Carpeta));
                Value1.replace( QString("ETIQUETA"), QString("DRAKLIVE"));
                Value1.replace("\n","");
                ValorGrub << ""+Value1+"";
            }
            ValorGrub << "#";
        }
        if (Nombre == "ArchLinux")
        {
            QString ArchLinux;
            QSqlQuery query(db);
            query.exec("SELECT grub FROM Distro WHERE NomDistro='"+Nombre+"' AND Version='"+Version+"' AND Arquitectura='"+Arquitectura+"'");
            query.first();
            if (query.isValid())
                ArchLinux=query.value(0).toString();
            QStringList ArchLinuxDat  = ArchLinux.split("\n");
            for(int i=0;i<ArchLinuxDat.count();i++)
            {
                QString Value1 = ArchLinuxDat.value(i);
                Value1.replace( QString("ARCHLINUX"), QString(Carpeta));
                Value1.replace( QString("ETIQUETA"), QString("DRAKLIVE"));
                Value1.replace("\n","");
                ValorGrub << ""+Value1+"";
            }
            ValorGrub << "#";
        }
        if (Nombre == "CentOS")
        {
            QString CentOS;
            QSqlQuery query(db);
            query.exec("SELECT grub FROM Distro WHERE NomDistro='"+Nombre+"' AND Version='"+Version+"' AND Arquitectura='"+Arquitectura+"'");
            query.first();
            if (query.isValid())
                CentOS=query.value(0).toString();
            QStringList CentOSDat  = CentOS.split("\n");
            for(int i=0;i<CentOSDat.count();i++)
            {
                QString Value1 = CentOSDat.value(i);
                Value1.replace( QString("CENTOS"), QString(Carpeta));
                Value1.replace( QString("ETIQUETA"), QString("DRAKLIVE"));
                Value1.replace("\n","");
                ValorGrub << ""+Value1+"";
            }
            ValorGrub << "#";
        }
        if (Nombre == "Debian")
        {
            QString Debian;
            QSqlQuery query(db);
            query.exec("SELECT grub FROM Distro WHERE NomDistro='"+Nombre+"' AND Version='"+Version+"' AND Arquitectura='"+Arquitectura+"'");
            query.first();
            if (query.isValid())
                Debian=query.value(0).toString();
            QStringList DebianDat  = Debian.split("\n");
            for(int i=0;i<DebianDat.count();i++)
            {
                QString Value1 = DebianDat.value(i);
                Value1.replace( QString("DEBIAN"), QString(Carpeta));
                Value1.replace( QString("ETIQUETA"), QString("DRAKLIVE"));
                Value1.replace("\n","");
                ValorGrub << ""+Value1+"";
            }
            ValorGrub << "#";
        }
        if (Nombre == "Knoppix")
        {
            QString Knoppix;
            QSqlQuery query(db);
            query.exec("SELECT grub FROM Distro WHERE NomDistro='"+Nombre+"' AND Version='"+Version+"' AND Arquitectura='"+Arquitectura+"'");
            query.first();
            if (query.isValid())
                Knoppix=query.value(0).toString();
            QStringList KnoppixDat  = Knoppix.split("\n");
            for(int i=0;i<KnoppixDat.count();i++)
            {
                QString Value1 = KnoppixDat.value(i);
                Value1.replace( QString("KNOPPIX"), QString(Carpeta));
                Value1.replace( QString("ETIQUETA"), QString("DRAKLIVE"));
                Value1.replace("\n","");
                ValorGrub << ""+Value1+"";
            }
            ValorGrub << "#";
        }
        if (Nombre == "OpenSUSE")
        {
            QString OpenSUSE;
            QSqlQuery query(db);
            query.exec("SELECT grub FROM Distro WHERE NomDistro='"+Nombre+"' AND Version='"+Version+"' AND Arquitectura='"+Arquitectura+"'");
            query.first();
            if (query.isValid())
                OpenSUSE=query.value(0).toString();
            QStringList OpenSUSEDat  = OpenSUSE.split("\n");
            for(int i=0;i<OpenSUSEDat.count();i++)
            {
                QString Value1 = OpenSUSEDat.value(i);
                Value1.replace( QString("OPENSUSE"), QString(Carpeta));
                Value1.replace( QString("ETIQUETA"), QString("DRAKLIVE"));
                Value1.replace("\n","");
                ValorGrub << ""+Value1+"";
            }
            ValorGrub << "#";
        }
        if (Nombre == "LinuxMint")
        {
            QString LinuxMint;
            QSqlQuery query(db);
            query.exec("SELECT grub FROM Distro WHERE NomDistro='"+Nombre+"' AND Version='"+Version+"' AND Arquitectura='"+Arquitectura+"'");
            query.first();
            if (query.isValid())
                LinuxMint=query.value(0).toString();
            QStringList LinuxMintDat  = LinuxMint.split("\n");
            for(int i=0;i<LinuxMintDat.count();i++)
            {
                QString Value1 = LinuxMintDat.value(i);
                Value1.replace( QString("LINUXMINT"), QString(Carpeta));
                Value1.replace( QString("ETIQUETA"), QString("DRAKLIVE"));
                Value1.replace("\n","");
                ValorGrub << ""+Value1+"";
            }
            ValorGrub << "#";
        }
        if (Nombre == "BootRepair")
        {
            QString BootRepair;
            QSqlQuery query(db);
            query.exec("SELECT grub FROM Distro WHERE NomDistro='"+Nombre+"' AND Version='"+Version+"' AND Arquitectura='"+Arquitectura+"'");
            query.first();
            if (query.isValid())
                BootRepair=query.value(0).toString();
            QStringList BootRepairDat  = BootRepair.split("\n");
            for(int i=0;i<BootRepairDat.count();i++)
            {
                QString Value1 = BootRepairDat.value(i);
                Value1.replace( QString("BootRepair"), QString(Carpeta));
                Value1.replace( QString("ETIQUETA"), QString("DRAKLIVE"));
                Value1.replace("\n","");
                ValorGrub << ""+Value1+"";
            }
            ValorGrub << "#";
        }
        if (Nombre == "Clonezilla")
        {
            QString Clonezilla;
            QSqlQuery query(db);
            query.exec("SELECT grub FROM Distro WHERE NomDistro='"+Nombre+"' AND Version='"+Version+"' AND Arquitectura='"+Arquitectura+"'");
            query.first();
            if (query.isValid())
                Clonezilla=query.value(0).toString();
            QStringList ClonezillaDat  = Clonezilla.split("\n");
            for(int i=0;i<ClonezillaDat.count();i++)
            {
                QString Value1 = ClonezillaDat.value(i);
                Value1.replace( QString("Clonezilla"), QString(Carpeta));
                Value1.replace( QString("ETIQUETA"), QString("DRAKLIVE"));
                Value1.replace("\n","");
                ValorGrub << ""+Value1+"";
            }
            ValorGrub << "#";
        }
        if (Nombre == "GParted")
        {
            QString GParted;
            QSqlQuery query(db);
            query.exec("SELECT grub FROM Distro WHERE NomDistro='"+Nombre+"' AND Version='"+Version+"' AND Arquitectura='"+Arquitectura+"'");
            query.first();
            if (query.isValid())
                GParted=query.value(0).toString();
            QStringList GPartedDat  = GParted.split("\n");
            for(int i=0;i<GPartedDat.count();i++)
            {
                QString Value1 = GPartedDat.value(i);
                Value1.replace( QString("GParted"), QString(Carpeta));
                Value1.replace( QString("ETIQUETA"), QString("DRAKLIVE"));
                Value1.replace("\n","");
                ValorGrub << ""+Value1+"";
            }
            ValorGrub << "#";
        }
        if (Nombre == "SystemRescue")
        {
            QString SystemRescue;
            QSqlQuery query(db);
            query.exec("SELECT grub FROM Distro WHERE NomDistro='"+Nombre+"' AND Version='"+Version+"' AND Arquitectura='"+Arquitectura+"'");
            query.first();
            if (query.isValid())
                SystemRescue=query.value(0).toString();
            QStringList SystemRescueDat  = SystemRescue.split("\n");
            for(int i=0;i<SystemRescueDat.count();i++)
            {
                QString Value1 = SystemRescueDat.value(i);
                Value1.replace( QString("SystemRescue"), QString(Carpeta));
                Value1.replace( QString("ETIQUETA"), QString("DRAKLIVE"));
                Value1.replace("\n","");
                ValorGrub << ""+Value1+"";
            }
            ValorGrub << "#";
        }
        if (Nombre == "OpenMandriva")
        {
            QString OpenMandriva;
            QSqlQuery query(db);
            query.exec("SELECT grub FROM Distro WHERE NomDistro='"+Nombre+"' AND Version='"+Version+"' AND Arquitectura='"+Arquitectura+"'");
            query.first();
            if (query.isValid())
                OpenMandriva=query.value(0).toString();
            QStringList OpenMandrivaDat  = OpenMandriva.split("\n");
            for(int i=0;i<OpenMandrivaDat.count();i++)
            {
                QString Value = OpenMandrivaDat.value(i);
                Value1.replace( QString("OPENMANDRIVA"), QString(Carpeta));
                Value1.replace( QString("ETIQUETA"), QString("DRAKLIVE"));
                Value1.replace("\n","");
                ValorGrub << ""+Value+"";
            }
            ValorGrub << "#";
        }
    }
    else if (ui->radioButton_16->isChecked())
    {
        if (Nombre == "Mandriva")
        {
            QString Mandriva;
            QSqlQuery query(db);
            query.exec("SELECT grub2 FROM Distro WHERE NomDistro='"+Nombre+"' AND Version='"+Version+"' AND Arquitectura='"+Arquitectura+"'");
            query.first();
            if (query.isValid())
                Mandriva=query.value(0).toString();
            QStringList MandrivaDat = Mandriva.split("\n");
            for(int i=0;i<MandrivaDat.count();i++)
            {
                QString Value = MandrivaDat.value(i);
                Value.replace(QString("MANDRIVA"), QString(Carpeta));
                Value.replace(QString("ETIQUETA"), QString("DRAKLIVE"));
                Value.replace("\n","");
                ValorGrub << ""+Value+"";
            }
            ValorGrub << "#";
        }
        if (Nombre == "PCLinuxOS")
        {
            QString PCLinuxOS;
            QSqlQuery query(db);
            query.exec("SELECT grub2 FROM Distro WHERE NomDistro='"+Nombre+"' AND Version='"+Version+"' AND Arquitectura='"+Arquitectura+"'");
            query.first();
            if (query.isValid())
                PCLinuxOS=query.value(0).toString();
            QStringList PCLinuxOSDat  = PCLinuxOS.split("\n");
            for(int i=0;i<PCLinuxOSDat.count();i++)
            {
                QString Value1 = PCLinuxOSDat.value(i);
                Value1.replace( QString("PCLINUXOS"), QString(Carpeta));
                Value1.replace( QString("ETIQUETA"), QString("DRAKLIVE"));
                Value1.replace("\n","");
                ValorGrub << ""+Value1+"";
            }
            ValorGrub << "#";
        }
        if (Nombre == "Fedora")
        {
            QString Fedora;
            QSqlQuery query(db);
            query.exec("SELECT grub2 FROM Distro WHERE NomDistro='"+Nombre+"' AND Version='"+Version+"' AND Arquitectura='"+Arquitectura+"'");
            query.first();
            if (query.isValid())
                Fedora=query.value(0).toString();
            QStringList FedoraDat  = Fedora.split("\n");
            for(int i=0;i<FedoraDat.count();i++)
            {
                QString Value1 = FedoraDat.value(i);
                Value1.replace( QString("FEDORA"), QString(Carpeta));
                Value1.replace( QString("ETIQUETA"), QString("DRAKLIVE"));
                Value1.replace("\n","");
                ValorGrub << ""+Value1+"";
            }
            ValorGrub << "#";
        }
        if (Nombre == "Ubuntu")
        {
            QString Ubuntu;
            QSqlQuery query(db);
            query.exec("SELECT grub2 FROM Distro WHERE NomDistro='"+Nombre+"' AND Version='"+Version+"' AND Arquitectura='"+Arquitectura+"'");
            query.first();
            if (query.isValid())
                Ubuntu=query.value(0).toString();
            QStringList UbuntuDat  = Ubuntu.split("\n");
            for(int i=0;i<UbuntuDat.count();i++)
            {
                QString Value1 = UbuntuDat.value(i);
                Value1.replace( QString("UBUNTU"), QString(Carpeta));
                Value1.replace( QString("ETIQUETA"), QString("DRAKLIVE"));
                Value1.replace("\n","");
                ValorGrub << ""+Value1+"";
            }
            ValorGrub << "#";
        }
        if (Nombre == "ArchLinux")
        {
            QString ArchLinux;
            QSqlQuery query(db);
            query.exec("SELECT grub2 FROM Distro WHERE NomDistro='"+Nombre+"' AND Version='"+Version+"' AND Arquitectura='"+Arquitectura+"'");
            query.first();
            if (query.isValid())
                ArchLinux=query.value(0).toString();
            QStringList ArchLinuxDat  = ArchLinux.split("\n");
            for(int i=0;i<ArchLinuxDat.count();i++)
            {
                QString Value1 = ArchLinuxDat.value(i);
                Value1.replace( QString("ARCHLINUX"), QString(Carpeta));
                Value1.replace( QString("ETIQUETA"), QString("DRAKLIVE"));
                Value1.replace("\n","");
                ValorGrub << ""+Value1+"";
            }
            ValorGrub << "#";
        }
        if (Nombre == "CentOS")
        {
            QString CentOS;
            QSqlQuery query(db);
            query.exec("SELECT grub2 FROM Distro WHERE NomDistro='"+Nombre+"' AND Version='"+Version+"' AND Arquitectura='"+Arquitectura+"'");
            query.first();
            if (query.isValid())
                CentOS=query.value(0).toString();
            QStringList CentOSDat  = CentOS.split("\n");
            for(int i=0;i<CentOSDat.count();i++)
            {
                QString Value1 = CentOSDat.value(i);
                Value1.replace( QString("CENTOS"), QString(Carpeta));
                Value1.replace( QString("ETIQUETA"), QString("DRAKLIVE"));
                Value1.replace("\n","");
                ValorGrub << ""+Value1+"";
            }
            ValorGrub << "#";
        }
        if (Nombre == "Debian")
        {
            QString Debian;
            QSqlQuery query(db);
            query.exec("SELECT grub2 FROM Distro WHERE NomDistro='"+Nombre+"' AND Version='"+Version+"' AND Arquitectura='"+Arquitectura+"'");
            query.first();
            if (query.isValid())
                Debian=query.value(0).toString();
            QStringList DebianDat  = Debian.split("\n");
            for(int i=0;i<DebianDat.count();i++)
            {
                QString Value1 = DebianDat.value(i);
                Value1.replace( QString("DEBIAN"), QString(Carpeta));
                Value1.replace( QString("ETIQUETA"), QString("DRAKLIVE"));
                Value1.replace("\n","");
                ValorGrub << ""+Value1+"";
            }
            ValorGrub << "#";
        }
        if (Nombre == "Knoppix")
        {
            QString Knoppix;
            QSqlQuery query(db);
            query.exec("SELECT grub2 FROM Distro WHERE NomDistro='"+Nombre+"' AND Version='"+Version+"' AND Arquitectura='"+Arquitectura+"'");
            query.first();
            if (query.isValid())
                Knoppix=query.value(0).toString();
            QStringList KnoppixDat  = Knoppix.split("\n");
            for(int i=0;i<KnoppixDat.count();i++)
            {
                QString Value1 = KnoppixDat.value(i);
                Value1.replace( QString("KNOPPIX"), QString(Carpeta));
                Value1.replace( QString("ETIQUETA"), QString("DRAKLIVE"));
                Value1.replace("\n","");
                ValorGrub << ""+Value1+"";
            }
            ValorGrub << "#";
        }
        if (Nombre == "OpenSUSE")
        {
            QString OpenSUSE;
            QSqlQuery query(db);
            query.exec("SELECT grub2 FROM Distro WHERE NomDistro='"+Nombre+"' AND Version='"+Version+"' AND Arquitectura='"+Arquitectura+"'");
            query.first();
            if (query.isValid())
                OpenSUSE=query.value(0).toString();
            QStringList OpenSUSEDat  = OpenSUSE.split("\n");
            for(int i=0;i<OpenSUSEDat.count();i++)
            {
                QString Value1 = OpenSUSEDat.value(i);
                Value1.replace( QString("OPENSUSE"), QString(Carpeta));
                Value1.replace( QString("ETIQUETA"), QString("DRAKLIVE"));
                Value1.replace("\n","");
                ValorGrub << ""+Value1+"";
            }
            ValorGrub << "#";
        }
        if (Nombre == "LinuxMint")
        {
            QString LinuxMint;
            QSqlQuery query(db);
            query.exec("SELECT grub2 FROM Distro WHERE NomDistro='"+Nombre+"' AND Version='"+Version+"' AND Arquitectura='"+Arquitectura+"'");
            query.first();
            if (query.isValid())
                LinuxMint=query.value(0).toString();
            QStringList LinuxMintDat  = LinuxMint.split("\n");
            for(int i=0;i<LinuxMintDat.count();i++)
            {
                QString Value1 = LinuxMintDat.value(i);
                Value1.replace( QString("LINUXMINT"), QString(Carpeta));
                Value1.replace( QString("ETIQUETA"), QString("DRAKLIVE"));
                Value1.replace("\n","");
                ValorGrub << ""+Value1+"";
            }
            ValorGrub << "#";
        }
        if (Nombre == "BootRepair")
        {
            QString BootRepair;
            QSqlQuery query(db);
            query.exec("SELECT grub2 FROM Distro WHERE NomDistro='"+Nombre+"' AND Version='"+Version+"' AND Arquitectura='"+Arquitectura+"'");
            query.first();
            if (query.isValid())
                BootRepair=query.value(0).toString();
            QStringList BootRepairDat  = BootRepair.split("\n");
            for(int i=0;i<BootRepairDat.count();i++)
            {
                QString Value1 = BootRepairDat.value(i);
                Value1.replace( QString("BootRepair"), QString(Carpeta));
                Value1.replace( QString("ETIQUETA"), QString("DRAKLIVE"));
                Value1.replace("\n","");
                ValorGrub << ""+Value1+"";
            }
            ValorGrub << "#";
        }
        if (Nombre == "Clonezilla")
        {
            QString Clonezilla;
            QSqlQuery query(db);
            query.exec("SELECT grub2 FROM Distro WHERE NomDistro='"+Nombre+"' AND Version='"+Version+"' AND Arquitectura='"+Arquitectura+"'");
            query.first();
            if (query.isValid())
                Clonezilla=query.value(0).toString();
            QStringList ClonezillaDat  = Clonezilla.split("\n");
            for(int i=0;i<ClonezillaDat.count();i++)
            {
                QString Value1 = ClonezillaDat.value(i);
                Value1.replace( QString("Clonezilla"), QString(Carpeta));
                Value1.replace( QString("ETIQUETA"), QString("DRAKLIVE"));
                Value1.replace("\n","");
                ValorGrub << ""+Value1+"";
            }
            ValorGrub << "#";
        }
        if (Nombre == "GParted")
        {
            QString GParted;
            QSqlQuery query(db);
            query.exec("SELECT grub2 FROM Distro WHERE NomDistro='"+Nombre+"' AND Version='"+Version+"' AND Arquitectura='"+Arquitectura+"'");
            query.first();
            if (query.isValid())
                GParted=query.value(0).toString();
            QStringList GPartedDat  = GParted.split("\n");
            for(int i=0;i<GPartedDat.count();i++)
            {
                QString Value1 = GPartedDat.value(i);
                Value1.replace( QString("GParted"), QString(Carpeta));
                Value1.replace( QString("ETIQUETA"), QString("DRAKLIVE"));
                Value1.replace("\n","");
                ValorGrub << ""+Value1+"";
            }
            ValorGrub << "#";
        }
        if (Nombre == "SystemRescue")
        {
            QString SystemRescue;
            QSqlQuery query(db);
            query.exec("SELECT grub2 FROM Distro WHERE NomDistro='"+Nombre+"' AND Version='"+Version+"' AND Arquitectura='"+Arquitectura+"'");
            query.first();
            if (query.isValid())
                SystemRescue=query.value(0).toString();
            QStringList SystemRescueDat  = SystemRescue.split("\n");
            for(int i=0;i<SystemRescueDat.count();i++)
            {
                QString Value1 = SystemRescueDat.value(i);
                Value1.replace( QString("SystemRescue"), QString(Carpeta));
                Value1.replace( QString("ETIQUETA"), QString("DRAKLIVE"));
                Value1.replace("\n","");
                ValorGrub << ""+Value1+"";
            }
            ValorGrub << "#";
        }
        if (Nombre == "OpenMandriva")
        {
            QString OpenMandriva;
            QSqlQuery query(db);
            query.exec("SELECT grub2 FROM Distro WHERE NomDistro='"+Nombre+"' AND Version='"+Version+"' AND Arquitectura='"+Arquitectura+"'");
            query.first();
            if (query.isValid())
                OpenMandriva=query.value(0).toString();
            QStringList OpenMandrivaDat  = OpenMandriva.split("\n");
            for(int i=0;i<OpenMandrivaDat.count();i++)
            {
                QString Value = OpenMandrivaDat.value(i);
                Value1.replace( QString("OPENMANDRIVA"), QString(Carpeta));
                Value1.replace( QString("ETIQUETA"), QString("DRAKLIVE"));
                Value1.replace("\n","");
                ValorGrub << ""+Value+"";
            }
            ValorGrub << "#";
        }
    }
}

void UsbLive::on_pushButton_34_clicked()
{
    QString cm2= "qemu-system-x86_64 %2";
    cm2=cm2.arg(ui->label_18->text());
    comandos << cm2;
    if (mib != 0)
        delete mib;
    mib = new DrakeProcesos(comandos, this);
    connect(mib, SIGNAL(finProceso()), this, SLOT(mibFin()));
    int valor= comandos.count();
    mib->Valor(valor,5);
    mib->Mascara(cantidad51,cantidad50,cantidad49,DatoTalla,cantidad47,DatoNegro);
    mib->iniciarProceso();
}

void UsbLive::on_pushButton_33_clicked()
{
    if (ui->radioButton_16->isChecked())
            TipoGrub = "grub2";
    else if( ui->radioButton_15->isChecked())
            TipoGrub = "grub";
    ui->textEdit_4->setText("");
    QTableWidgetItem *item, *item1, *item2, *item3, *item4;
    QString activo1, Nombre, Version, Arquitectura, Carpeta;
    int itemCount = ui->tableWidget_2->rowCount();
    if (itemCount == -1)
    {
        QMessageBox m;
        if (Stilo == "A")
            m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
        m.setText(tr("No hay distros/utilidades registradas, por lo que no puedes procesar la solicitud."));
        m.exec();
        return;
    }
    else
    {
        int respuesta = 0;
        respuesta = QMessageBox::question(this, QString::fromUtf8(tr("Reconstruir GRUB/GRUB2")),
                           QString::fromUtf8(tr("<center><b>Reconstruccion del GRUB<b></center><p>"
                            "Si por algun motivo hemos perdido el GRUB/GRUB2 de nuestro DRAKLIVE, con esta  "
                            "utilidad procedemos a recuperalo.<p>"
                           "&iquest;Reconstruir GRUB/GRUB2?")), QMessageBox::Ok, QMessageBox::No);
        if (respuesta == QMessageBox::Ok)
        {
            Ctrl = 2;
            if (ui->radioButton_15->isChecked())
            {
                QString Kill0 = "rm -vR -f "+ruta+"%1/boot/";
                Kill0 = Kill0.arg("DRAKLIVE");
                RecoUSB << Kill0;
                QString gru1 = "echo Reinstalando grub en el USB...";
                QString grub1 = "/sbin/grub-install --root-directory="+ruta+"%1 %2 --no-floppy"; //ver grub-efi
                grub1 = grub1.arg("DRAKLIVE").arg(ui->label_18->text());
                RecoUSB << gru1 << grub1;
                QString Kill = "echo Creando fichero de mapeo de unidades y menu.lst...";
                QString Kill2 = "touch "+ruta+"%1/boot/grub/menu.lst";
                Kill2 = Kill2.arg("DRAKLIVE");
                QString device = "echo Reconstruyendo datos de mapeo y configuracion de inicio de distros para grub...";
                RecoUSB << Kill << Kill2 << device;
                QString procesando = "echo Realizando nuevo proceso de insercion de datos de distros en menu.lst...";
                RecoUSB << procesando;
                ValorGrub << "timeout 10" << "#color black/cyan yellow/cyan" << "default 0" << "";
            }
            else
            {
                QString Kill0 = "rm -vR -f "+ruta+"%1/boot/";
                Kill0 = Kill0.arg("DRAKLIVE");
                RecoUSB << Kill0;
                QString gru1 = "echo Reinstalando grub en el USB...";
                QString grub1 = "/sbin/grub2-install --root-directory="+ruta+"%1 %2 --no-floppy";
                grub1 = grub1.arg("DRAKLIVE").arg(ui->label_18->text());
                RecoUSB << gru1 << grub1;
                QString Kill = "echo Creando fichero de mapeo de unidades y grub.cfg...";
                QString Kill1 = "touch "+ruta+"%1/boot/grub2/device.map";
                Kill1 = Kill1.arg("DRAKLIVE");
                QString Kill2 = "touch "+ruta+"%1/boot/grub2/grub.cfg";
                Kill2 = Kill2.arg("DRAKLIVE");
                QString device0 = "echo Introduciendo datos de mapeo y configuracion de inicio de distros...";
                RecoUSB << Kill << Kill1 << Kill2 << device0;
                QString gru2 = "echo Reinstalando grub en el USB...";
                QString grub2 = "/usr/sbin/grub2-install --root-directory="+ruta+"%1 %2 --no-floppy";
                grub2 = grub2.arg("DRAKLIVE").arg(ui->label_18->text());
                RecoUSB << gru2 << grub2;
                QString procesando = "echo Realizando nuevo proceso de insercion de datos de distros en grub.cfg...";
                RecoUSB << procesando;
                ValorGrub << "set default=0" << "set timeout=10" << "set root=(hd0,1)" << "terminal console" << "";
            }
            for (int b=0;b<itemCount;b++)
            {
                item1 = ui->tableWidget_2->item(b,4);
                item2 = ui->tableWidget_2->item(b,0);
                item3 = ui->tableWidget_2->item(b,1);
                item4 = ui->tableWidget_2->item(b,2);
                item = ui->tableWidget_2->item(b,3);
                activo1 = item1->text();
                Nombre = item2->text();
                Arquitectura = item3->text();
                Version = item4->text();
                Carpeta = item->text();
                this->Reconstruir(Nombre,Version,Arquitectura,Carpeta);
            }
            if (mib != 0)
                delete mib;
            mib = new DrakeProcesos(RecoUSB, this);
            connect(mib, SIGNAL(publicarDatos(QString)), this, SLOT(mibEscribir(QString)));
            connect(mib, SIGNAL(DatoComand(QString)), this, SLOT(mibReferencia(QString)));
            connect(mib, SIGNAL(finProceso()), this, SLOT(mibFin()));
            int valor = RecoUSB.count();
            mib->Valor(valor,3);
            mib->Mascara(cantidad51,cantidad50,cantidad49,DatoTalla,cantidad47,DatoNegro);
            mib->iniciarProceso();
        }
    }
}

void UsbLive::on_pushButton_26_clicked()
{
    if (ui->radioButton_16->isChecked())
            TipoGrub = "grub2";
    else if( ui->radioButton_15->isChecked())
            TipoGrub = "grub";
    ui->textEdit_4->setText("");
    QTableWidgetItem *item, *item1, *item2, *item3, *item4;
    QString activo1, Nombre, Version, Arquitectura, Carpeta;
    int itemCount = ui->tableWidget_2->rowCount();
    if (itemCount == -1)
    {
        QMessageBox m;
        if (Stilo == "A")
            m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
        m.setText(tr("No has distros/utilidades registradas, por lo que no puedes procesar la solicitud."));
        m.exec();
        return;
    }
    else
    {
        QString Antes, Despues;
        if (ui->radioButton_15->isChecked())
        {
            Antes = "GRUB";
            Despues = "GRUB2";
        }
        else
        {
            Antes = "GRUB2";
            Despues = "GRUB";
        }
        int respuesta = 0;
        respuesta = QMessageBox::question(this, QString::fromUtf8(tr("Cambiar tipo de GRUB")),
                           QString::fromUtf8(tr("<center><b>Cambiar tipo de GRUB</b></center><p>"
                           "Si quieres cambiar el GRUB de arranque entre los dos tipos que hay; GRUB y GRUB2, "
                           "con solo utilizar esta opcion se realizara.<p>"
                           "En estos momentos el cambio que se realizara sera el de...<p>"
                           "<b>Cambiar "+Antes+" por "+Despues+"</b><p>"
                           "&iquest;Cambiar tipo de GRUB?")), QMessageBox::Ok, QMessageBox::No);
        if (respuesta == QMessageBox::Ok)
        {
            Ctrl = 2;
            if (ui->radioButton_15->isChecked())
            {
                ui->radioButton_16->setChecked(true);
                QString Kill0 = "rm -vR -f "+ruta+"%1/boot/";
                Kill0 = Kill0.arg("DRAKLIVE");
                RecoUSB << Kill0;
                QString gru1 = "echo Reinstalando grub en el USB...";
                QString grub1 = "/sbin/grub2-install --root-directory="+ruta+"%1 %2 --no-floppy";
                grub1 = grub1.arg("DRAKLIVE").arg(ui->label_18->text());
                RecoUSB << gru1 << grub1;
                QString Kill = "echo Creando fichero de mapeo de unidades y grub.cfg...";
                QString Kill1 = "touch "+ruta+"%1/boot/grub2/device.map";
                Kill1 = Kill1.arg("DRAKLIVE");
                QString Kill2 = "touch "+ruta+"%1/boot/grub2/grub.cfg";
                Kill2 = Kill2.arg("DRAKLIVE");
                QString device0 = "echo Introduciendo datos de mapeo y configuracion de inicio de distros...";
                RecoUSB << Kill << Kill1 << Kill2 << device0;
                QString gru2 = "echo Reinstalando grub en el USB...";
                QString grub2 = "/usr/sbin/grub2-install --root-directory="+ruta+"%1 %2 --no-floppy";
                grub2 = grub2.arg("DRAKLIVE").arg(ui->label_18->text());
                RecoUSB << gru2 << grub2;
                QString procesando = "echo Realizando nuevo proceso de insercion de datos de distros en grub.cfg...";
                RecoUSB << procesando;
                ValorGrub << "set default=0" << "set timeout=10" << "set root=(hd0,1)" << "terminal console" << "";
            }
            else
            {
                ui->radioButton_15->setChecked(true);
                QString Kill0 = "rm -vR -f "+ruta+"%1/boot/";
                Kill0 = Kill0.arg("DRAKLIVE");
                RecoUSB << Kill0;
                QString gru1 = "echo Reinstalando grub en el USB...";
                QString grub1 = "/sbin/grub-install --root-directory="+ruta+"%1 %2 --no-floppy"; //ver grub-efi
                grub1 = grub1.arg("DRAKLIVE").arg(ui->label_18->text());
                RecoUSB << gru1 << grub1;
                QString Kill = "echo Creando fichero de mapeo de unidades y menu.lst...";
                QString Kill2 = "touch "+ruta+"%1/boot/grub/menu.lst";
                Kill2 = Kill2.arg("DRAKLIVE");
                QString device = "echo Reconstruyendo datos de mapeo y configuracion de inicio de distros para grub...";
                RecoUSB << Kill << Kill2 << device;
                QString procesando = "echo Realizando nuevo proceso de insercion de datos de distros en menu.lst...";
                RecoUSB << procesando;
                ValorGrub << "timeout 10" << "#color black/cyan yellow/cyan" << "default 0" << "";
            }
            for (int b=0;b<itemCount;b++)
            {
                item1 = ui->tableWidget_2->item(b,4);
                item2 = ui->tableWidget_2->item(b,0);
                item3 = ui->tableWidget_2->item(b,1);
                item4 = ui->tableWidget_2->item(b,2);
                item = ui->tableWidget_2->item(b,3);
                activo1 = item1->text();
                Nombre = item2->text();
                Arquitectura = item3->text();
                Version = item4->text();
                Carpeta = item->text();
                this->Reconstruir(Nombre,Version,Arquitectura,Carpeta);
            }
            if (mib != 0)
                delete mib;
            mib = new DrakeProcesos(RecoUSB, this);
            connect(mib, SIGNAL(publicarDatos(QString)), this, SLOT(mibEscribir(QString)));
            connect(mib, SIGNAL(DatoComand(QString)), this, SLOT(mibReferencia(QString)));
            connect(mib, SIGNAL(finProceso()), this, SLOT(mibFin()));
            int valor = RecoUSB.count();
            mib->Valor(valor,3);
            mib->Mascara(cantidad51,cantidad50,cantidad49,DatoTalla,cantidad47,DatoNegro);
            mib->iniciarProceso();
        }
    }
}

void UsbLive::on_pushButton_35_clicked()
{
    QMessageBox m;
    if (Stilo == "A")
        m.setStyleSheet("background-color: "+cantidad51+"; color: "+cantidad50+"; font-size: "+cantidad49+"pt; font-style: "+DatoTalla+"; font-family: "+cantidad47+"; font-weight: "+DatoNegro+"");
    m.setText(tr("Tienes instalado el sistema grub-efi y no es compatible con grub legacy.<p>Si quieres utilizar grub legacy debes ir a \"Solucion a problemas/Sistemas de arranque/Incompatiblidad de grub legacy con grub-efi\""));
    m.exec();
    return;
}
