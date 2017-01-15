#ifndef FORMPOLYGONLIMITEDITOR_H
#define FORMPOLYGONLIMITEDITOR_H

#include "cs8polygonlimit.h"
#include "cs8recordermodel.h"
#include <QWidget>

namespace Ui
{
class FormPolygonLimitEditor;
}

class FormPolygonLimitEditor : public QWidget
{
    Q_OBJECT

public:
    explicit FormPolygonLimitEditor(QWidget *parent = 0, cs8RecorderModel *recorderModel=0, cs8PolygonLimit *model=0);
    ~FormPolygonLimitEditor();

    double scaleFactor();
protected:
    void resizeEvent(QResizeEvent *e);

private:
    Ui::FormPolygonLimitEditor *ui;
    //QwtPlotCurve *d_curve;
    //QwtPlotCurve *d_bounds;
    //QwtPlotRescaler *m_scaler;
    //QwtPicker *picker;
    //QwtPlotMarker *startMarker, *stopMarker;
    cs8PolygonLimit *m_polygonLimit;
    cs8RecorderModel *m_model;
    bool bBlockUpdate;
    int cycleStart,cycleStop;
    bool m_flipBounds;
    QVector<QPointF > m_bounds;
    void removeLoops(QVector<QPointF > & polygon);

public slots:
    void addBounds();
    void setCycleBounds(int start, int stop);

protected slots:
    void updatePlot();
    void plotRedrawn();

private slots:
    void on_cbXAxis_currentIndexChanged(int index);
    void slotPointAppended(const QPoint &pos);
    void on_cbYAxis_currentIndexChanged(int index);


    void on_checkBox_toggled(bool checked);
};

#endif // FORMPOLYGONLIMITEDITOR_H
