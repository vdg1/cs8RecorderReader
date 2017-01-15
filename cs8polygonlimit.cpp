#include "cs8polygonlimit.h"

cs8PolygonLimit::cs8PolygonLimit(QObject *parent) : QObject(parent)
{
    m_selectedXAxis=0;
    m_selectedYAxis=0;
}

cs8PolygonLimit::~cs8PolygonLimit()
{

}
int cs8PolygonLimit::selectedXAxis() const
{
    return m_selectedXAxis;
}

void cs8PolygonLimit::setSelectedXAxis(int selectedXAxis)
{
    m_selectedXAxis = selectedXAxis;
    emit modified();
}

int cs8PolygonLimit::selectedYAxis() const
{
    return m_selectedYAxis;
}

void cs8PolygonLimit::setSelectedYAxis(int selectedYAxis)
{
    m_selectedYAxis = selectedYAxis;
    emit modified();
}


