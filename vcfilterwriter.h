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
    VcFilterWriter(const std::string& fileName, const std::list<Filter> &filterList);
    ~VcFilterWriter();

    bool open();
    void close();
    void write();

private:
    std::string        m_fileName;
    FilterWriter      *m_filterWriter;
    std::list<Filter>  m_filterList;
};

#endif // VCFILTERWRITER_H
