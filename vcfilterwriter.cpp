#include "vcfilterwriter.h"
#include <QFile>
#include <QXmlStreamWriter>
#include "filterwriter.h"

VcFilterWriter::VcFilterWriter(const QString &fileName, const QList<Filter> &filterList) :
    m_fileName(fileName)
  , m_filterList(filterList)
{
    m_filterWriter = new FilterWriter(m_fileName);
}

VcFilterWriter::~VcFilterWriter()
{
    close();
    delete m_filterWriter;

}

bool VcFilterWriter::open()
{
    return true;
}

void VcFilterWriter::close()
{
}

void VcFilterWriter::write()
{
    m_filterWriter->StartWrite();
    m_filterWriter->WriteHeaders();

    m_filterWriter->WriteBody(m_filterList);

    m_filterWriter->EndWrite();
}
