#ifndef VCPROJECTREADER_H
#define VCPROJECTREADER_H

#include<QString>
#include <QPointer>
#include <QMap>
#include "Filter.h"

class QXmlStreamReader;
class QFile;

class VcProjectReader
{
public:
    VcProjectReader(const QString& fileName);
    ~VcProjectReader();

    bool open();
    void close();
    const QMap<QString, Filter>& read();



protected:
    void readSources();
    void createFilters(QStringList filterNames);
    void insertFilter(const QString &filterName, const QString &fileName = QString());

private:
    QString               m_fileName;
    QFile                *m_file;
    QXmlStreamReader     *m_reader;
    QMap<QString, Filter> m_filterMap;

};

#endif // VCPROJECTREADER_H
