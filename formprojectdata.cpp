#include "formprojectdata.h"
#include "ui_formprojectdata.h"

#include "XMLSettings.h"
#include <QFile>
#include <QFileDialog>

FormProjectData::FormProjectData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormProjectData)
{
    ui->setupUi(this);

    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinMaxButtonsHint);
    m_model=new cs8RecorderModel(this);
    ui->tableView->setModel(m_model);


}

FormProjectData::~FormProjectData()
{
    delete ui;
}

void FormProjectData::loadRecording(QString fileName)
{
    if (m_model->loadRecording(fileName))
    {
        ui->leConfiguration->setText(m_model->configurationVersion());
        ui->leSerialNumber->setText(m_model->serialNumber());
        ui->leType->setText(m_model->robotType());
        ui->leVal3Version->setText(m_model->val3Version());
        ui->leRecording->setText(fileName);
        m_recordingFileName=fileName;

        ui->spCycleStart->setMinimum(0);
        ui->spCycleStart->setMaximum(m_model->rowCount());
        ui->spCyleStop->setMinimum(0);
        ui->spCyleStop->setMaximum(m_model->rowCount());



        /*
        connect(ui->lvSelectedColumns->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                this,SLOT(slotSelectionChanged(QItemSelection,QItemSelection)));

        selectedXAxis=-1;
        selectedYAxis=-1;
        if (projectSettings.contains("selectedXAxis"))
        {
            selectedXAxis=projectSettings["selectedXAxis"].toInt();
            ui->lvSelectedColumns->selectionModel()->select(ui->lvSelectedColumns->model()->index(selectedXAxis,0),QItemSelectionModel::Select);
        }
        if (projectSettings.contains("selectedYAxis"))
        {
            selectedYAxis=projectSettings["selectedYAxis"].toInt();
            ui->lvSelectedColumns->selectionModel()->select(ui->lvSelectedColumns->model()->index(selectedYAxis,0),QItemSelectionModel::Select);
        }
        */
    }
}

void FormProjectData::openRecorderFile()
{
    QFileDialog dlg;
    QString fileName=dlg.getOpenFileName(this,"Select Recording File",m_recordingFileName,"*.rec");
    if (!fileName.isEmpty())
    {
        loadRecording(fileName);
    }
}

void FormProjectData::on_tbSelectRecording_clicked()
{
    openRecorderFile();

}

void FormProjectData::slotModified()
{
    emit modified();
}
QList<cs8PolygonLimit *> FormProjectData::polygonList() const
{
    return m_polygonList;
}


QString FormProjectData::projectFileName() const
{
    return m_projectFileName;
}

void FormProjectData::setProjectFileName(const QString &value)
{
    m_projectFileName = value;
    setWindowTitle(m_projectFileName);
}

cs8RecorderModel *FormProjectData::model() const
{
    return m_model;
}

void FormProjectData::loadProjectFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        return;
    QSettings settings;
    settings.setValue("lastProjectDir",fileName);
    setProjectFileName(fileName);
    QMap<QString,QVariant> sets;
    readXmlFile(file,sets);
    qDebug() << sets;
    loadRecording(sets["Recording"].toString());

    ui->spCycleStart->setValue(sets["startCycle"].toInt());
    ui->spCyleStop->setValue(sets["stopCycle"].toInt());

    QMapIterator<QString, QVariant> i(sets);
    while (i.hasNext())
    {
        i.next();
        qDebug() << i.key() << ": " << i.value() << endl;
        if (i.key().startsWith("limit_"))
        {
            QStringList s=i.value().toString().split(",");
            qDebug() << s;
            cs8PolygonLimit *p=new cs8PolygonLimit(this);
            p->setSelectedXAxis(s[0].toInt());
            p->setSelectedYAxis(s[1].toInt());
            connect(p,SIGNAL(modified()),this,SLOT(slotModified()));
            m_polygonList.append(p);
            qDebug() << "read polygon: Selected XAxis " << p->selectedXAxis() << " Selected YAxis " << p->selectedYAxis();

            emit addedPolygonLimit(p);
        }
    }
}

void FormProjectData::saveProjectFile(const QString &fileName)
{
    QString name;
    if (fileName.isEmpty())
        name=m_projectFileName;
    else
        name=fileName;
    QFile file(name);
    file.open(QFile::WriteOnly);

    QSettings settings;
    settings.setValue("lastProjectDir",name);

    setProjectFileName(name);
    QMap<QString,QVariant> sets;

    sets["Recording"]=m_recordingFileName;
    sets["startCycle"]=ui->spCycleStart->value();
    sets["stopCycle"]=ui->spCyleStop->value();
    int c=0;
    foreach(cs8PolygonLimit* l,m_polygonList)
    {
        sets[QString("limit_%1/axis").arg(c)]=QString("%1,%2").arg(l->selectedXAxis()).arg(l->selectedYAxis());
        c++;
    }

    writeXmlFile(file,sets);
}

cs8PolygonLimit* FormProjectData::createPolygonLimit()
{
    cs8PolygonLimit *l=new cs8PolygonLimit(this);
    connect(l,SIGNAL(modified()),this,SLOT(slotModified()));
    m_polygonList.append(l);
    return l;
}

void FormProjectData::exportToCSV(const QString &fileName)
{
    m_model->exportToCSV(fileName);
}



void FormProjectData::on_spCycleStart_valueChanged(int arg1)
{

}

void FormProjectData::on_spCyleStop_valueChanged(int arg1)
{

}

void FormProjectData::on_spCycleStart_customContextMenuRequested(const QPoint &pos)
{

}
