#include "filterwriter.h"
#include "filter.h"

FilterWriter::FilterWriter(const std::string &fileName) :
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
    m_document->InsertEndChild(m_document->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\""));
}

void FilterWriter::WriteHeaders()
{
    tinyxml2::XMLElement* startElement = m_document->NewElement("Project");
    startElement->SetAttribute("ToolsVersion", "4.0");
    startElement->SetAttribute("xmlns", "http://schemas.microsoft.com/developer/msbuild/2003");
    m_document->InsertEndChild(startElement);
}

void FilterWriter::WriteBody(const FilterList &filterList)
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

	element = m_document->NewElement("ItemGroup");
    m_document->LastChild()->InsertEndChild(element);
    ParseFilterGeneratedFiles(element, filterList);
}

void FilterWriter::EndWrite()
{
    m_document->SaveFile(m_fileName.data());
}

void FilterWriter::WriteFilter(tinyxml2::XMLNode* parentNode, const Filter &filter)
{
    tinyxml2::XMLElement* filterElement = m_document->NewElement("Filter");
    filterElement->SetAttribute("Include", filter.getFilterName().data());

    tinyxml2::XMLElement* uuidElement = m_document->NewElement("UniqueIdentifier");
    uuidElement->SetText(filter.getUuid().data());

    filterElement->InsertFirstChild(uuidElement);
    parentNode->InsertEndChild(filterElement);

}

void FilterWriter::WriteFilterSourceFile(tinyxml2::XMLNode* parentNode, const Filter &filter, const std::string &fileName)
{
    //<CLCompile Include="..\wtf\OwnPtr.h">
    //      <Filter>wtf</Filter>
    //</CLCompile>
    WriteFilterFile("CLCompile", parentNode, filter, fileName);
}

void FilterWriter::WriteFilterHeaderFile(tinyxml2::XMLNode* parentNode, const Filter &filter, const std::string &fileName)
{
    //<ClInclude Include="..\wtf\OwnPtr.h">
    //      <Filter>wtf</Filter>
    //</ClInclude>
    WriteFilterFile("CLInclude", parentNode, filter, fileName);
}

void FilterWriter::WriteFilterGeneratedFile(tinyxml2::XMLNode* parentNode, const Filter &filter, const std::string &fileName)
{
    //<CustomBuild Include="..\wtf\OwnPtr.h">
    //      <Filter>wtf</Filter>
    //</CustomBuild>
    WriteFilterFile("CustomBuild", parentNode, filter, fileName);
}

void FilterWriter::WriteFilterFile(const char* tag, tinyxml2::XMLNode* parentNode, const Filter &filter, const std::string &fileName)
{
    tinyxml2::XMLElement *includeElement = m_document->NewElement(tag);
    includeElement->SetAttribute("Include", fileName.data());

    tinyxml2::XMLElement *filterElement = m_document->NewElement("Filter");
    filterElement->SetText(filter.getFilterName().data());

    includeElement->InsertEndChild(filterElement);
    parentNode->InsertEndChild(includeElement);
}

void FilterWriter::WriteFilterNames(tinyxml2::XMLNode* node, const FilterList &filterList)
{
    for (Filter filter : filterList) {
        WriteFilter(node, filter);
    }
}

void FilterWriter::ParseFilterSourceFiles(tinyxml2::XMLNode *parentNode, const FilterList &filterList)
{
    for(FilterList::const_iterator filterIterator = filterList.cbegin();
        filterIterator != filterList.cend();
        ++filterIterator)
    {
        for(std::list<std::string>::const_iterator it = filterIterator->getSources().cbegin();
            it != filterIterator->getSources().cend();
            ++it)
        {
            WriteFilterSourceFile(parentNode, *filterIterator, *it);
        }
    }
}

void FilterWriter::ParseFilterHeadersFiles(tinyxml2::XMLNode* parentNode, const FilterList &filterList)
{
    for(FilterList::const_iterator filterIterator = filterList.cbegin();
        filterIterator != filterList.cend();
        ++filterIterator)
    {
        for(std::list<std::string>::const_iterator it = filterIterator->getHeaders().cbegin();
            it != filterIterator->getHeaders().cend();
            ++it)
        {
            WriteFilterHeaderFile(parentNode, *filterIterator, *it);
        }
    }
}

void FilterWriter::ParseFilterGeneratedFiles(tinyxml2::XMLNode* parentNode, const FilterList &filterList)
{
	for(FilterList::const_iterator filterIterator = filterList.cbegin();
        filterIterator != filterList.cend();
        ++filterIterator)
    {
        for(std::list<std::string>::const_iterator it = filterIterator->getGenerated().cbegin();
			it != filterIterator->getGenerated().cend();
            ++it)
        {
            WriteFilterGeneratedFile(parentNode, *filterIterator, *it);
        }
    }
}
