#ifndef CS8RECORDERMODEL_H
#define CS8RECORDERMODEL_H

#include <QAbstractTableModel>
#include <QStringList>
#include <QStringListModel>
#include <QVector>


class cs8RecorderModel : public QAbstractTableModel
{
public:
    cs8RecorderModel(QObject * parent = 0);
    ~cs8RecorderModel();
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool loadRecording(const QString & fileName);


    QString configurationVersion() const;
    void setConfigurationVersion(const QString &configurationVersion);

    QString val3Version() const;
    void setVal3Version(const QString &val3Version);

    QString robotType() const;
    void setRobotType(const QString &robotType);

    QString serialNumber() const;
    void setSerialNumber(const QString &serialNumber);

    QStringListModel *columnHeaders() const;
    QVector<double> values(const int column);
    QVector<QPointF> valuePairs(const int col1, const int col2, int start, int stop) const;
    QVector<QPointF> sampleSeries(const int col1) const;

    bool exportToCSV(const QString & fileName);



protected:
    QString mRobotType;
    QString mVal3Version;
    QString mConfigurationVersion;
    QString mSerialNumber;
    QByteArray mControlData;
    QByteArray mJointNumbers;
    QStringListModel* mColumnHeaders;
    int mColumnCount, mRowCount;

    bool parseHeader(QByteArray & data);
    bool parseColumnHeaders(QByteArray & data);
    bool parseData(QByteArray & data_);
    void removeLeadingZero(QByteArray &ba);
    int nextEndTag(const QByteArray &ba);
    double calcMinValue(const int column);
    double calcMaxValue(const int column);

    QVector<double> mData;
    QVector<double> mMinValues;
    QVector<double> mMaxValues;




};

#endif // CS8RECORDERMODEL_H
