#ifndef FORMPROJECTDATA_H
#define FORMPROJECTDATA_H

#include "cs8polygonlimit.h"
#include <cs8recordermodel.h>
#include <QWidget>

namespace Ui
{
class FormProjectData;
}

class FormProjectData : public QWidget
{
    Q_OBJECT

public:
    explicit FormProjectData(QWidget *parent = 0);
    ~FormProjectData();
    void loadRecording(QString fileName);

    cs8RecorderModel *model() const;

    QString projectFileName() const;
    void setProjectFileName(const QString &value);
    void loadProjectFile(const QString &fileName);
    void saveProjectFile(const QString &fileName=QString());

    QList<cs8PolygonLimit *> polygonList() const;

    void openRecorderFile();

signals:
    void addedPolygonLimit(cs8PolygonLimit*);
    void modified();
    void cycleBoundsChanged(int, int);

public slots:
    cs8PolygonLimit *createPolygonLimit();
    void exportToCSV(const QString & fileName);

private slots:
    void on_tbSelectRecording_clicked();
    void slotModified();

    void on_spCycleStart_valueChanged(int arg1);

    void on_spCyleStop_valueChanged(int arg1);

    void on_spCycleStart_customContextMenuRequested(const QPoint &pos);

private:
    Ui::FormProjectData *ui;
    cs8RecorderModel *m_model;


    QString m_projectFileName;
    QString m_recordingFileName;
    QList<cs8PolygonLimit *> m_polygonList;
};

#endif // FORMPROJECTDATA_H
