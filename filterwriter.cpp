#include <QDebug>
#include "filterwriter.h"
#include "filter.h"

FilterWriter::FilterWriter(QXmlStreamWriter *writer) :
  m_writer(writer)
{
    m_writer->setAutoFormattingIndent(2);
}

FilterWriter::~FilterWriter()
{
}

void FilterWriter::StartWrite()
{
    m_writer->writeStartDocument();
}

void FilterWriter::WriteHeaders()
{
    m_writer->writeStartElement("Project");
    m_writer->writeAttribute("ToolsVersion", "4.0");
    m_writer->writeAttribute("xmlns", "http://schemas.microsoft.com/developer/msbuild/2003");
}

void FilterWriter::WriteBody(const QList<Filter> &filterList)
{
    m_writer->writeStartElement("ItemGroup");
    WriteFilterNames(filterList);
    m_writer->writeEndElement();

    m_writer->writeStartElement("ItemGroup");
    ParseFilterHeadersFiles(filterList);
    m_writer->writeEndElement();

    m_writer->writeStartElement("ItemGroup");
    ParseFilterSourceFiles(filterList);
    m_writer->writeEndElement();
}

void FilterWriter::EndWrite()
{
    m_writer->writeEndDocument();
}

void FilterWriter::WriteFilter(QXmlStreamWriter &writer, const Filter &filter)
{
    writer.writeStartElement("Filter");
    writer.writeAttribute("Include", filter.getFilterName());
    writer.writeTextElement("UniqueIdentifier", filter.getUuid());
    writer.writeEndElement();
}

void FilterWriter::WriteFilterSourceFile(QXmlStreamWriter &writer, const Filter &filter, const QString &fileName)
{
    //<ClInclude Include="..\wtf\OwnPtr.h">
    //      <Filter>wtf</Filter>
    //</ClInclude>
    writer.writeStartElement("CLCompile");
    writer.writeAttribute("Include", fileName);
    writer.writeTextElement("Filter", filter.getFilterName());
    writer.writeEndElement();
}

void FilterWriter::WriteFilterHeaderFile(QXmlStreamWriter &writer, const Filter &filter, const QString &fileName)
{
    //<ClInclude Include="..\wtf\OwnPtr.h">
    //      <Filter>wtf</Filter>
    //</ClInclude>
    writer.writeStartElement("CLInclude");
    writer.writeAttribute("Include", fileName);
    writer.writeTextElement("Filter", filter.getFilterName());
    writer.writeEndElement();
}

void FilterWriter::WriteFilterNames(const QList<Filter> &filterList)
{
    foreach (Filter filter, filterList) {
        WriteFilter(*m_writer, filter);
    }
}

QString FilterWriter::ConcatenateDirs(const QString &dir, const QString& localDirName)
{
    if(dir.isEmpty())
        return localDirName;

    return dir + QDir::separator() + localDirName;
}

void FilterWriter::ParseFilterSourceFiles(const QList<Filter> &filterList)
{
    foreach(Filter filter, filterList)
    {
        foreach(QString fileName, filter.getSources())
        {
            WriteFilterSourceFile(*m_writer, filter, fileName);
        }
    }
}

void FilterWriter::ParseFilterHeadersFiles(const QList<Filter> &filterList)
{
    foreach(Filter filter, filterList)
    {
        foreach(QString fileName, filter.getHeaders())
        {
            WriteFilterHeaderFile(*m_writer, filter, fileName);
        }
    }
}
