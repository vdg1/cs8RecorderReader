#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>
#include <QFileDialog>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{



    QCoreApplication::setApplicationName("cs8RecorderReeader");
    QCoreApplication::setOrganizationName("SAXE Swiss System AB");
    ui->setupUi(this);

    windowMapper = new QSignalMapper(this);
    updateWindowMenu();
    connect(ui->menuWindows, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));

    projectData = new FormProjectData(this);
    connect(projectData,SIGNAL(addedPolygonLimit(cs8PolygonLimit*)),this,SLOT(slotPolygonLimitAdded(cs8PolygonLimit*)));
    connect(projectData,SIGNAL(modified()),this,SLOT(slotModified()));
    ui->mdiArea->addSubWindow(projectData);


    QSettings settings;
    projectData->loadProjectFile(settings.value("lastProjectDir",QDir::currentPath()).toString());

    updateWindowTitle(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent ( QCloseEvent *event )
{
    if (isWindowModified())
    {
        if (QMessageBox::question(this,tr("Unsaved changes"),tr("You have unsaved changes in the current project! Do you want to close anyway?"),
                                  QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
            return;
        event->ignore();
    }

}


void MainWindow::on_action_Open_triggered()
{


}


void MainWindow::slotSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    /*
    foreach(QModelIndex index,selected.indexes())
    {
        qDebug() << "Selected: " << index.row();
    }
    foreach(QModelIndex index,deselected.indexes())
    {
        qDebug() << "Deselected: " << index.row();
    }

    if (ui->lvSelectedColumns->selectionModel()->selectedRows().count()>2)
    {
        ui->lvSelectedColumns->selectionModel()->select(selected,QItemSelectionModel::Deselect);
        return;
    }

    if (selected.indexes().count()>0)
    {
        if (selectedXAxis==-1)
            selectedXAxis=selected.indexes().at(0).row();
        else
            selectedYAxis=selected.indexes().at(0).row();
        updateWindowTitle(true);
    }

    if (deselected.indexes().count()>0)
    {
        if (selectedXAxis==deselected.indexes().at(0).row())
            selectedXAxis=-1;
        else
            selectedYAxis=-1;
        updateWindowTitle(true);
    }

    updatePlot();
    */

}

void MainWindow::slotPolygonLimitAdded(cs8PolygonLimit *l)
{

}


void MainWindow::on_actionOpen_Project_triggered()
{
    QFileDialog dlg;
    QSettings settings;
    QString fileName=dlg.getOpenFileName(this,"Select Project File",settings.value("lastProjectDir",QDir::currentPath()).toString(),"*.RecorderProject");
    if (!fileName.isEmpty())
    {
        projectData->loadProjectFile(fileName);
        updateWindowTitle(false);
    }

}

void MainWindow::on_actionSave_Project_As_triggered()
{
    QFileDialog dlg;
    QSettings settings;
    QString fileName=dlg.getSaveFileName(this,"Select Project File",settings.value("lastProjectDir",QDir::currentPath()).toString(),"*.RecorderProject");
    if (!fileName.isEmpty())
    {
        projectData->saveProjectFile(fileName);
        updateWindowTitle(false);
    }
}

void MainWindow::updateWindowTitle(bool modified)
{
    setWindowTitle(projectData->projectFileName()+"[*]");
    setWindowModified(modified);
}



void MainWindow::on_actionSave_Project_triggered()
{
    projectData->saveProjectFile();
    updateWindowTitle(false);
}


void MainWindow::on_actionAdd_triggered()
{

}

void MainWindow::updateWindowMenu()
{
    ui->menuWindows->clear();
    //ui->menuWindows->addAction(ui->actionClose);
    //ui->menuWindows->addAction(ui->actionCloseAll);
    ui->menuWindows->addSeparator();
    //ui->menuWindows->addAction(ui->actionTile);
    //ui->menuWindows->addAction(ui->actionCascade);
    ui->menuWindows->addSeparator();

    QList<QMdiSubWindow *> windows = ui->mdiArea->subWindowList();
    //separatorAct->setVisible(!windows.isEmpty());

    for (int i = 0; i < windows.size(); ++i)
    {
        QWidget *child = qobject_cast<QWidget *>(windows.at(i)->widget());

        QString text;
        if (i < 9)
        {
            text = tr("&%1 %2").arg(i + 1)
                   .arg(child->windowTitle ());
        }
        else
        {
            text = tr("%1 %2").arg(i + 1)
                   .arg(child->windowTitle ());
        }
        QAction *action  = ui->menuWindows->addAction(text);
        action->setCheckable(true);
        action ->setChecked(child == activeMdiChild());
        connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
        windowMapper->setMapping(action, windows.at(i));
    }
}

void MainWindow::slotModified()
{
    updateWindowTitle(true);
}

QMdiSubWindow *MainWindow::activeMdiChild()
{
    if (QMdiSubWindow *activeSubWindow = ui->mdiArea->activeSubWindow())
        return qobject_cast<QMdiSubWindow *>(activeSubWindow->widget());
    return 0;
}

void MainWindow::on_actionAdd_limit_bounds_triggered()
{
}

void MainWindow::on_actionOpen_Recorder_File_triggered()
{
    projectData->openRecorderFile();
}

void MainWindow::on_actionExport_Recording_triggered()
{
    QFileDialog dlg;
    QSettings settings;
    QString fileName=dlg.getSaveFileName(this,"Export recording data to CSV file",settings.value("lastProjectDir",QDir::currentPath()).toString(),"*.csv");
    if (!fileName.isEmpty())
    {
        projectData->exportToCSV(fileName);
    }
}
