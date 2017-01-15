#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "formprojectdata.h"
#include <QItemSelection>
#include <QMainWindow>
#include <QMdiSubWindow>
#include <QSettings>
#include <QSignalMapper>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void updateWindowTitle(bool modified);

private slots:
    void on_action_Open_triggered();
    void slotSelectionChanged(const QItemSelection & selected, const QItemSelection & deselected);
    void slotPolygonLimitAdded(cs8PolygonLimit* l);

    void on_actionOpen_Project_triggered();

    void on_actionSave_Project_As_triggered();

    void on_actionSave_Project_triggered();

    void on_actionAdd_triggered();

    void updateWindowMenu();

    void slotModified();

    void on_actionAdd_limit_bounds_triggered();

    void on_actionOpen_Recorder_File_triggered();

    void on_actionExport_Recording_triggered();

private:
    Ui::MainWindow *ui;
    QSignalMapper *windowMapper;

    FormProjectData *projectData;

    QMdiSubWindow *activeMdiChild();
    void closeEvent ( QCloseEvent * event );


};

#endif // MAINWINDOW_H
