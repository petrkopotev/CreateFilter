#ifndef VCFILTERWRITER_H
#define VCFILTERWRITER_H

#include "Filter.h"

class QXmlStreamWriter;
class QFile;
class FilterWriter;

typedef std::list<Filter> FilterList;

class VcFilterWriter
{
public:
    VcFilterWriter(const std::string& fileName, const FilterList &filterList);
    ~VcFilterWriter();

    bool open();
    void write();

private:
    std::string        m_fileName;
    FilterWriter      *m_filterWriter;
    FilterList         m_filterList;
};

#endif // VCFILTERWRITER_H
