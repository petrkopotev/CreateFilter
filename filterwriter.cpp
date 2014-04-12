#include "filterwriter.h"
#include "filter.h"

FilterWriter::FilterWriter( const QString &fileName) :
  m_fileName(fileName)
{
    m_document = new tinyxml2::XMLDocument;
}

FilterWriter::~FilterWriter()
{
    delete m_document;
}

void FilterWriter::StartWrite()
{
    m_document->InsertEndChild(m_document->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8"));
}

void FilterWriter::WriteHeaders()
{
    tinyxml2::XMLElement* startElement = m_document->NewElement("Project");
    startElement->SetAttribute("ToolsVersion", "4.0");
    startElement->SetAttribute("xmlns", "http://schemas.microsoft.com/developer/msbuild/2003");
    m_document->InsertEndChild(startElement);
}

void FilterWriter::WriteBody(const QList<Filter> &filterList)
{
    tinyxml2::XMLElement* element = m_document->NewElement("ItemGroup");
    m_document->LastChild()->InsertEndChild(element);
    WriteFilterNames(element, filterList);

    element = m_document->NewElement("ItemGroup");
    m_document->LastChild()->InsertEndChild(element);
    ParseFilterHeadersFiles(element, filterList);

    element = m_document->NewElement("ItemGroup");
    m_document->LastChild()->InsertEndChild(element);
    ParseFilterSourceFiles(element, filterList);
}

void FilterWriter::EndWrite()
{
    m_document->SaveFile(m_fileName.toStdString().data());
}

void FilterWriter::WriteFilter(tinyxml2::XMLNode* parentNode, const Filter &filter)
{
    tinyxml2::XMLElement* filterElement = m_document->NewElement("Filter");
    filterElement->SetAttribute("Include", filter.getFilterName().toStdString().data());

    tinyxml2::XMLElement* uuidElement = m_document->NewElement("UniqueIdentifier");
    uuidElement->SetText(filter.getUuid().toStdString().data());

    filterElement->InsertFirstChild(uuidElement);
    parentNode->InsertEndChild(filterElement);

}

void FilterWriter::WriteFilterSourceFile(tinyxml2::XMLNode* parentNode, const Filter &filter, const QString &fileName)
{
    //<ClInclude Include="..\wtf\OwnPtr.h">
    //      <Filter>wtf</Filter>
    //</ClInclude>
    tinyxml2::XMLElement *includeElement = m_document->NewElement("CLCompile");
    includeElement->SetAttribute("Include", fileName.toStdString().data());

    tinyxml2::XMLElement *filterElement = m_document->NewElement("Filter");
    filterElement->SetText(filter.getFilterName().toStdString().data());

    includeElement->InsertEndChild(filterElement);
    parentNode->InsertEndChild(includeElement);
    /*writer.writeStartElement("CLCompile");
    writer.writeAttribute("Include", fileName);
    writer.writeTextElement("Filter", filter.getFilterName());
    writer.writeEndElement();*/
}

void FilterWriter::WriteFilterHeaderFile(tinyxml2::XMLNode* parentNode, const Filter &filter, const QString &fileName)
{
    //<ClInclude Include="..\wtf\OwnPtr.h">
    //      <Filter>wtf</Filter>
    //</ClInclude>
    tinyxml2::XMLElement *includeElement = m_document->NewElement("CLInclude");
    includeElement->SetAttribute("Include", fileName.toStdString().data());

    tinyxml2::XMLElement *filterElement = m_document->NewElement("Filter");
    filterElement->SetText(filter.getFilterName().toStdString().data());

    includeElement->InsertEndChild(filterElement);
    parentNode->InsertEndChild(includeElement);
}

void FilterWriter::WriteFilterNames(tinyxml2::XMLNode* node, const QList<Filter> &filterList)
{
    foreach (Filter filter, filterList) {
        WriteFilter(node, filter);
    }
}

void FilterWriter::ParseFilterSourceFiles(tinyxml2::XMLNode *parentNode, const QList<Filter> &filterList)
{
    foreach(Filter filter, filterList)
    {
        foreach(QString fileName, filter.getSources())
        {
            WriteFilterSourceFile(parentNode, filter, fileName);
        }
    }
}

void FilterWriter::ParseFilterHeadersFiles(tinyxml2::XMLNode* parentNode, const QList<Filter> &filterList)
{
    foreach(Filter filter, filterList)
    {
        foreach(QString fileName, filter.getHeaders())
        {
            WriteFilterHeaderFile(parentNode, filter, fileName);
        }
    }
}
