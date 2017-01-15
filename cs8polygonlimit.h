#ifndef CS8POLYGONLIMIT_H
#define CS8POLYGONLIMIT_H

#include <QObject>
#include <QStringListModel>

class cs8PolygonLimit : public QObject
{
    Q_OBJECT
public:
    explicit cs8PolygonLimit(QObject *parent = 0);
    ~cs8PolygonLimit();

    int selectedXAxis() const;
    void setSelectedXAxis(int selectedXAxis);

    int selectedYAxis() const;
    void setSelectedYAxis(int selectedYAxis);


signals:
    void modified();

public slots:

private:
    int  m_selectedXAxis, m_selectedYAxis;

};

#endif // CS8POLYGONLIMIT_H
