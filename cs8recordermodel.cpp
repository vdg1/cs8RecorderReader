#include "cs8recordermodel.h"

#include <QFile>
#include <QDebug>
#include <QDataStream>

const double PI  =3.141592653589793238463;

cs8RecorderModel::cs8RecorderModel(QObject *parent) :
    QAbstractTableModel(parent),mColumnCount(0)
{
    mColumnHeaders=new QStringListModel(this);
}

cs8RecorderModel::~cs8RecorderModel()
{

}

int cs8RecorderModel::rowCount(const QModelIndex &parent) const
{
    return mRowCount;
}

int cs8RecorderModel::columnCount(const QModelIndex &parent) const
{
    return mColumnCount;
}

QVariant cs8RecorderModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role)
    {
    case Qt::DisplayRole:
        double value=mData.at(index.column()+(index.row()*mColumnCount));
        if (mColumnHeaders->data(mColumnHeaders->index(index.column(), 0),Qt::DisplayRole).toString()
                .contains("Pos["))
            value=value*180/PI;
        return value;
        break;
    }
    return QVariant();
}

QVariant cs8RecorderModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ((section<mColumnCount) && (role==Qt::DisplayRole))
    {
        if (orientation==Qt::Horizontal)
            return  mColumnHeaders->data(mColumnHeaders->index(section,0), Qt::DisplayRole).toString();
        //QString("%1[%2]")
        //.arg(mColumnHeaders->data(mColumnHeaders->index(section,0), Qt::DisplayRole).toString())
        //.arg((int)mJointNumbers.at(section));
    }
    return QVariant();
}

bool cs8RecorderModel::loadRecording(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        return false;

    QByteArray data=file.readAll();
    if (!parseHeader(data))
        return false;


    if (!parseColumnHeaders(data))
        return false;

    if (!parseData(data))
        return false;

    return true;
}
QString cs8RecorderModel::configurationVersion() const
{
    return mConfigurationVersion;
}

void cs8RecorderModel::setConfigurationVersion(const QString &configurationVersion)
{
    mConfigurationVersion = configurationVersion;
}
QString cs8RecorderModel::val3Version() const
{
    return mVal3Version;
}

void cs8RecorderModel::setVal3Version(const QString &val3Version)
{
    mVal3Version = val3Version;
}
QString cs8RecorderModel::robotType() const
{
    return mRobotType;
}

void cs8RecorderModel::setRobotType(const QString &robotType)
{
    mRobotType = robotType;
}
QString cs8RecorderModel::serialNumber() const
{
    return mSerialNumber;
}

void cs8RecorderModel::setSerialNumber(const QString &serialNumber)
{
    mSerialNumber = serialNumber;
}
QStringListModel* cs8RecorderModel::columnHeaders() const
{
    return mColumnHeaders;
}

QVector<double> cs8RecorderModel::values(const int column)
{
    QVector<double> vec;
    for(int row=0; row<mRowCount; row++)
        vec.append(data(index(row,column)).toDouble());
    return vec;
}

QVector<QPointF> cs8RecorderModel::valuePairs(const int col1, const int col2, int start, int stop) const
{
    QVector<QPointF> vec;
    for(int row=start; row<stop; row++)
        vec.append(QPointF(data(index(row,col1)).toDouble(),data(index(row,col2)).toDouble()));
    return vec;
}

QVector<QPointF> cs8RecorderModel::sampleSeries(const int col1) const
{
    QVector<QPointF> vec;
    for(int row=0; row<mRowCount; row++)
        vec.append(QPointF(row,data(index(row,col1)).toDouble()));
    return vec;
}

bool cs8RecorderModel::exportToCSV(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly))
        return false;
    QString line;
    QTextStream out(&file);

    // write meta data
    out << mSerialNumber << "," << mConfigurationVersion << "," << mVal3Version << "," << mRobotType << "\n";
    // write headers
    out << mColumnHeaders->stringList().join(",") << "\n";
    for (int r=0; r<rowCount(); r++)
    {
        for (int c=0; c<columnCount(); c++)
        {
            out << data(index(r,c)).toDouble() << (c<columnCount()?",":"");
        }
        out << "\n";
    }
    file.close();
    return true;
}

double cs8RecorderModel::calcMinValue(const int column)
{
    double min=9999999;
    for(int row=0; row<mRowCount; row++)
        min=qMin(min,data(index(row,column)).toDouble());
    return min;
}

double cs8RecorderModel::calcMaxValue(const int column)
{
    double max=-9999999;
    for(int row=0; row<mRowCount; row++)
        max=qMax(max,data(index(row,column)).toDouble());
    return max;
}

void cs8RecorderModel::removeLeadingZero(QByteArray & ba)
{
    while (ba.length()>0 && ba.at(0)==0x00)
        ba.remove(0,1);
}

int cs8RecorderModel::nextEndTag(const QByteArray &ba)
{
    for (int i=0; i<ba.length()-1; i++)
    {
        if (ba[i]==0x0 && ba[i+1]!=0x0)
            return i;
    }
    return -1;
}

bool cs8RecorderModel::parseHeader(QByteArray &data)
{
    int pos;
    // check file is a recording file
    QByteArray ba;
    ba.resize(4);
    ba.clear();
    ba[0]= 0x03;
    ba[1]= 0x00;
    ba[2]= 0x00;
    ba[3]= 0x00;
    qDebug() << "valid recorder file " << (data.indexOf(ba)>=0?"Yes":"No");
    if (data.indexOf(ba)!=0)
        return false;
    // remove prefix
    data.remove(0,8);
    removeLeadingZero(data);
    //
    // extract robot type
    // end of robot type field
    pos=nextEndTag(data);
    if (pos==-1)
        return false;
    ba=data.mid(0,pos);
    removeLeadingZero(ba);
    mRobotType=ba;
    data.remove(0,pos+2);

    removeLeadingZero(data);
    // val3 version
    // end of val3 version field
    pos=nextEndTag(data);
    if (pos==-1)
        return false;
    ba=data.mid(0,pos);
    removeLeadingZero(ba);
    mVal3Version=ba;
    data.remove(0,pos+2);
    removeLeadingZero(data);

    // configuration version
    pos=nextEndTag(data);
    if (pos==-1)
        return false;
    ba=data.mid(0,pos);
    removeLeadingZero(ba);
    mConfigurationVersion=ba;
    data.remove(0,pos+2);
    removeLeadingZero(data);

    // serial number
    pos=nextEndTag(data);
    if (pos==-1)
        return false;
    ba=data.mid(0,pos);
    removeLeadingZero(ba);
    mSerialNumber=ba;
    data.remove(0,pos+2);

    // extract control data
    pos=data.indexOf(0x05);
    pos=((data.indexOf(0x09)<pos) && (data.indexOf(0x09)!=-1))?data.indexOf(0x09):pos;
    if (pos==-1)
        return false;
    ba=data.mid(0,pos);
    mControlData=ba;
    data.remove(0,pos+1);

    mColumnCount=mControlData.at(2);
    return true;
}

bool cs8RecorderModel::parseColumnHeaders(QByteArray &data)
{
    QByteArray ba;
    int pos;
    QStringList columnHeaders;
    mColumnCount=12;
    mJointNumbers.resize(mColumnCount);

    for (int i=0; i<mColumnCount; i++)
    {
        removeLeadingZero(data);
        pos=nextEndTag(data);
        if (pos==-1)
            return false;
        ba=data.mid(0,pos);
        while (ba.at(0)==0x00)
            ba.remove(0,1);
        while (ba.at(ba.length()-1)==0x00)
            ba.chop(1);

        int jointNumber=ba.at(ba.length()-1)<6?ba.at(ba.length()-1):0;
        if (ba.at(ba.length()-1)<6 && ba.at(ba.length()-1)>0)
            ba.chop(1);
        mJointNumbers[i]=jointNumber;
        QString header;
        header=ba;
        header=QString("%1[%2]").arg(header).arg((int)mJointNumbers.at(i));
        columnHeaders << header;
        data.remove(0,i<mColumnCount-1?pos+2:pos+1);
    }
    mColumnHeaders->setStringList(columnHeaders);
    return true;
}

bool cs8RecorderModel::parseData(QByteArray &data_)
{
    //mColumnCount=12;
    mData.clear();
    QDataStream st(data_);
    qDebug() << data_.size();
    double *xy =  reinterpret_cast<double*>(data_.data());
    int i = 0, n =  (data_.size()) / 2 / sizeof(double);
    int row=0;
    while (i < n)
    {
        for (int col=0; col<mColumnCount; col++)
        {
            double val=xy[i++];
            //qDebug() << val;
            mData.append(val);
        }
        row++;
    }
    mRowCount=row;
    beginResetModel();
    endResetModel();
    for (int r=0; r<rowCount(); r++)
    {
        QString rowText;
        for (int c=0; c<columnCount(); c++)
        {
            //qDebug() << data(index(r,c)).toLongLong();
            rowText+=QString("%1,").arg(data(index(r,c)).toDouble(),6);

        }
        qDebug() << rowText;
    }
    return true;
}
