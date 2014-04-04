#include "vcfilterwriter.h"
#include <QFile>
#include <QXmlStreamWriter>
#include "filterwriter.h"

VcFilterWriter::VcFilterWriter(const QString &fileName, const QList<Filter> &filterList) :
    m_fileName(fileName)
  , m_filterList(filterList)
{
    m_file = new QFile(m_fileName);
    m_writer = new QXmlStreamWriter();
    m_writer->setAutoFormattingIndent(2);
    m_writer->setAutoFormatting(true);
    m_filterWriter = new FilterWriter(m_writer);

}

VcFilterWriter::~VcFilterWriter()
{
    close();
    delete m_filterWriter;
    delete m_writer;
    delete m_file;

}

bool VcFilterWriter::open()
{
    if(!m_file->open(QIODevice::WriteOnly))
    {
        return false;
    }

    m_writer->setDevice(m_file);

    return true;
}

void VcFilterWriter::close()
{
    m_file->close();
}

void VcFilterWriter::write()
{
    m_filterWriter->StartWrite();
    m_filterWriter->WriteHeaders();

    m_filterWriter->WriteBody(m_filterList);

    m_filterWriter->EndWrite();
}
