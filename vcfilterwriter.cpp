#include "vcfilterwriter.h"
#include "filterwriter.h"

VcFilterWriter::VcFilterWriter(const std::string &fileName, const std::list<Filter> &filterList) :
    m_fileName(fileName)
  , m_filterList(filterList)
{
    m_filterWriter = new FilterWriter(m_fileName);
}

VcFilterWriter::~VcFilterWriter()
{
    delete m_filterWriter;

}

bool VcFilterWriter::open()
{
    return true;
}

void VcFilterWriter::write()
{
    m_filterWriter->StartWrite();
    m_filterWriter->WriteHeaders();
    m_filterWriter->WriteBody(m_filterList);
    m_filterWriter->EndWrite();
}
