#ifndef VCFILTERWRITER_H
#define VCFILTERWRITER_H

#include <QString>
#include <QMap>
#include "Filter.h"

class QXmlStreamWriter;
class QFile;
class FilterWriter;

class VcFilterWriter
{
public:
    VcFilterWriter(const QString& fileName, const QList<Filter> &filterList);
    ~VcFilterWriter();

    bool open();
    void close();
    void write();

private:
    QString               m_fileName;
    QFile                *m_file;
    QXmlStreamWriter     *m_writer;
    FilterWriter         *m_filterWriter;
    QList<Filter>         m_filterList;
};

#endif // VCFILTERWRITER_H
