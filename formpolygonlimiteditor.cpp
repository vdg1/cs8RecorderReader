#include "formpolygonlimiteditor.h"
#include "ui_formpolygonlimiteditor.h"
//#include "qwt_picker_machine.h"
#include <QDebug>

FormPolygonLimitEditor::FormPolygonLimitEditor(QWidget *parent, cs8RecorderModel *recorderModel, cs8PolygonLimit *model) :
    QWidget(parent),
    ui(new Ui::FormPolygonLimitEditor)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinMaxButtonsHint);
    /*
    d_bounds=new QwtPlotCurve();
    d_curve=new QwtPlotCurve();
    picker = new QwtPicker(ui->plot);
    picker->setStateMachine(new QwtPickerClickPointMachine );
    //picker->setMousePattern(0,Qt::LeftButton,Qt::SHIFT);
    picker->setTrackerMode(QwtPicker::AlwaysOn);
    picker->setRubberBand(QwtPicker::RectRubberBand);

    startMarker=new QwtPlotMarker("Cycle Start");
    stopMarker=new QwtPlotMarker("Cycle Stop");
    startMarker->attach(ui->plot);
    stopMarker->attach(ui->plot);
    startMarker->setLineStyle(QwtPlotMarker::Cross);
    stopMarker->setLineStyle(QwtPlotMarker::Cross);
    startMarker->setLabel(QwtText("Start"));
    stopMarker->setLabel(QwtText("Stop"));
    startMarker->setLinePen(Qt::blue);
    stopMarker->setLinePen(Qt::blue);

    //m_scaler=new QwtPlotRescaler(ui->plot->canvas());
    //m_scaler->setRescalePolicy( QwtPlotRescaler::Fitting );
    //m_scaler->setAspectRatio(1);
    //m_scaler=ui->plot->axisScaleEngine();
    connect(picker,SIGNAL(appended(QPoint)),this,SLOT(slotPointAppended(QPoint)));
    */
    //
    if (model!=0)
    {
        m_polygonLimit=model;
        qDebug() << "use existing limit";
    }
    else
    {
        m_polygonLimit=new cs8PolygonLimit(this);
    }

    if (recorderModel!=0)
        m_model=recorderModel;
    else
        m_model=new cs8RecorderModel(this);
    //
    bBlockUpdate=true;
    ui->cbXAxis->setModel(m_model->columnHeaders());
    ui->cbYAxis->setModel(m_model->columnHeaders());

    ui->cbXAxis->setCurrentIndex(m_polygonLimit->selectedXAxis());
    ui->cbYAxis->setCurrentIndex(m_polygonLimit->selectedYAxis());
    qDebug() << "ctor: Selected XAxis " << m_polygonLimit->selectedXAxis() << " Selected YAxis " << m_polygonLimit->selectedYAxis();
    bBlockUpdate=false;
    cycleStart=0;
    cycleStop=m_model->rowCount();
    updatePlot();
}

FormPolygonLimitEditor::~FormPolygonLimitEditor()
{
    delete ui;
}

double FormPolygonLimitEditor::scaleFactor()
{
    //double start=ui->plot->transform(QwtPlot::xBottom,0);
    //double stop=ui->plot->transform(QwtPlot::yLeft,1);
    // double scale=1/abs(start-stop);
    //qDebug() << "scale factor start" << start << " stop " << stop << ":" << scale <<"deg/pixel";
    return 0;
}

void FormPolygonLimitEditor::resizeEvent(QResizeEvent *e)
{
    double scale = scaleFactor();
    //d_curve->setPen(Qt::black,2.5/scaleFactor());
    //ui->plot->replot();
}

void FormPolygonLimitEditor::removeLoops(QVector<QPointF> &polygon)
{
    QLineF l1,l2;
    QPointF intersection;
    bool loopFound;
    do
    {
        loopFound=false;
        for (int i=0; i<(polygon.count()-1); i++)
        {
            l1=QLineF(polygon[i],polygon[i+1]);
            for (int j=i+2; j<(polygon.count()-2); j++)
            {
                l2=QLineF(polygon[j],polygon[j+1]);
                if (l1.intersect(l2,&intersection)==QLineF::BoundedIntersection)
                {
                    qDebug() << "intersection found at " << intersection << " i:" << i << ", j:" << j;
                    qDebug() << l1 << l2;
                    polygon.remove(i,j-i+1);
                    loopFound=true;
                    i=0;
                    goto exitLoop;
                }
            }
        }
exitLoop:
        ;
    }
    while (loopFound);
}

void FormPolygonLimitEditor::addBounds()
{
    return;
    QVector<QPointF> values=m_model->valuePairs(m_polygonLimit->selectedXAxis(),m_polygonLimit->selectedYAxis(),cycleStart,cycleStop);
    int stepWidth=10;
    m_bounds.clear();
    m_bounds.reserve(values.count());
    for (int i=0; i<(values.count()-stepWidth); i=i+stepWidth)
    {
        QPointF start=values.at(i);
        QPointF stop=values.at(i+stepWidth);

        QLineF line=QLineF(start,stop);
        QLineF orthogonal=line.normalVector();
        orthogonal.setLength(m_flipBounds?-2.5:2.5);
        //m_bounds.append(orthogonal.p1());
        m_bounds.append(orthogonal.p2());
        //m_bounds.append(orthogonal.p1());
    }
    removeLoops(m_bounds);
    //d_bounds->setPen(Qt::red);
    //d_bounds->setSamples(m_bounds);
    //d_bounds->attach(ui->plot);
    //ui->plot->replot();
    //m_scaler->rescale();
}

void FormPolygonLimitEditor::setCycleBounds(int start, int stop)
{
    cycleStart=start;
    cycleStop=stop;
    updatePlot();
}

void FormPolygonLimitEditor::updatePlot()
{
    qDebug() << "Selected XAxis " << m_polygonLimit->selectedXAxis() << " Selected YAxis " << m_polygonLimit->selectedYAxis();
    int xCol= m_polygonLimit->selectedXAxis();
    int yCol= m_polygonLimit->selectedYAxis();

    if (xCol!=-1 && yCol!=-1 && m_model->rowCount()>0)
    {

        //d_curve->setSamples(m_model->values(m_polygonLimit->selectedXAxis()), m_model->values(m_polygonLimit->selectedYAxis()));
        QVector<QPointF> values=m_model->valuePairs(xCol,yCol,cycleStart,cycleStop);
        QPolygon pol;
        foreach(QPointF p,values)
        {
            pol << p.toPoint();
        }
        QRect box=pol.boundingRect();
        m_bounds << box.topLeft() << box.topRight() << box.bottomRight() << box.bottomLeft() << box.topLeft();
        /*d_bounds->setPen(Qt::red);
        d_bounds->setSamples(m_bounds);
        d_bounds->attach(ui->plot);

        d_curve->setSamples(values);
        startMarker->setValue(values.first());
        stopMarker->setValue(values.last());
        //double *xvalues=m_model->values(m_polygonLimit->selectedXAxis()).data();
        //double *yvalues=m_model->values(m_polygonLimit->selectedYAxis()).data();
        //d_curve->setRawSamples(xvalues,yvalues,m_model->values(m_polygonLimit->selectedYAxis()).count());
        d_curve->attach(ui->plot);
        //d_curve->setPen(Qt::black,2.5/scaleFactor());
        ui->plot->setAxisTitle( QwtPlot::xBottom, ui->cbXAxis->model()->data(ui->cbXAxis->model()->index(xCol,0)).toString() );
        ui->plot->setAxisTitle( QwtPlot::yLeft, ui->cbYAxis->model()->data(ui->cbYAxis->model()->index(yCol,0)).toString());
        ui->plot->setAxisAutoScale(QwtPlot::xBottom);
        ui->plot->setAxisAutoScale(QwtPlot::yLeft);
        ui->plot->replot();
        //m_scaler->rescale();
        //m_scaler->setIntervalHint(QwtPlot::yLeft,QwtInterval(ui->cbYAxis->model()->));
        setWindowTitle(QString("%1 <-> %2")
                       .arg(ui->plot->axisTitle(QwtPlot::xBottom).text())
                       .arg(ui->plot->axisTitle(QwtPlot::yLeft).text()));
                       */
    }
    else
    {
    }
}

void FormPolygonLimitEditor::plotRedrawn()
{

}

void FormPolygonLimitEditor::on_cbXAxis_currentIndexChanged(int index)
{
    if (bBlockUpdate)
        return;
    m_polygonLimit->setSelectedXAxis(index);
    updatePlot();
}

void FormPolygonLimitEditor::slotPointAppended(const QPoint &pos)
{
    qDebug() << "Appended:" << pos;
}


void FormPolygonLimitEditor::on_cbYAxis_currentIndexChanged(int index)
{
    if (bBlockUpdate)
        return;
    m_polygonLimit->setSelectedYAxis(index);
    updatePlot();
}

void FormPolygonLimitEditor::on_checkBox_toggled(bool checked)
{
    m_flipBounds=checked;
    addBounds();
}
