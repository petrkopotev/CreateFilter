#include "vcprojectreader.h"
#include "StringUtil.h"
#include <regex>


VcProjectReader::VcProjectReader(const std::string &fileName) :
    m_fileName(fileName)
{
    m_document = new tinyxml2::XMLDocument;
}

VcProjectReader::~VcProjectReader()
{
    delete m_document;
}

bool VcProjectReader::open()
{
    tinyxml2::XMLError error = m_document->LoadFile(m_fileName.data());
    if(error == tinyxml2::XML_ERROR_FILE_NOT_FOUND)
    {
        return false;
    }

    m_filterMap.clear();

    return true;
}


///TODO extract this to something more neat
void VcProjectReader::lookupForSources(tinyxml2::XMLNode* node)
{
    if(tinyxml2::XMLElement *element = node->ToElement())
    {
        if(strcmp(element->Name(), "ItemGroup") == 0)
        {
            //look for ClInclude, ClCompile now
            for(tinyxml2::XMLNode* node = element->FirstChild(); node; node = node->NextSibling())
            {
                if(tinyxml2::XMLElement* el = node->ToElement())
                {
                    //Lookup for ClInclude, ClSource and extract
                    if(strcmp(el->Name(), "ClInclude") == 0 || strcmp(el->Name(), "ClCompile") == 0)
                    {
                        readSources(el->FirstAttribute()->Value());
                    }
                }
            }
        }
    }
}

void VcProjectReader::readChildren(tinyxml2::XMLNode* node)
{
    if(node->GetDocument() != node)
    {
        lookupForSources(node);
    }
    
    for(tinyxml2::XMLNode* child = node->FirstChild(); child; child = child->NextSibling())
    {
        readChildren(child);
    }
}

const VcProjectReader::FilterMap &VcProjectReader::read()
{
    readChildren(m_document);
    
    return m_filterMap;
}

std::list<std::string> split(std::string &toSplit, const std::string &separator)
{
    size_t pos = 0;
    std::string token;
    std::list<std::string> result;
    while((pos = toSplit.find(separator)) != std::string::npos)
    {
        token = toSplit.substr(0, pos);
        result.push_back(token);
        toSplit.erase(0, pos + separator.length());
    }

    result.push_back(toSplit);
    return result;
}

void VcProjectReader::readSources(const char *str)
{
    std::string row = str;
    char separator = StringUtil::separator()[0];
    std::list<std::string> list = split(row, std::string(1, separator));

    //QString row = QString::fromLocal8Bit(str);
    //QStringList list = row.split(QDir::separator());
    createFilters(list);
}

void VcProjectReader::createFilters(const std::list<std::string> &rawStrings)
{
    std::string filterName, fileName;

    for(std::list<std::string>::const_iterator it = rawStrings.cbegin();
        it != rawStrings.cend(); it++)
    {
        if(!std::regex_search(*it, std::regex("\\.(h|c(c)|c(pp)?)$")))
        {
            filterName.append(*it);
            //filterName.erase(filterName.find("..\\") - 1);
            insertFilter(filterName);
            filterName.append("\\");
        } else {
            fileName.append(filterName).append(*it);
            //filterName.erase(filterName.find("..\\"));
            //filterName.erase(filterName.find_last_of("\\"));
            insertFilter(filterName.erase(filterName.find_last_of("\\")), fileName);
        }
    }
}

void VcProjectReader::insertFilter(const std::string &filterName, const std::string &fileName)
{
    if(filterName.empty())
        return;



    if(m_filterMap.find(filterName) != m_filterMap.cend())
    {
        if(!fileName.empty())
        {
            Filter filter = m_filterMap[filterName];
            if(StringUtil::endsWith(fileName, ".cpp") ||
                    StringUtil::endsWith(fileName, ".cc") ||
                    StringUtil::endsWith(fileName, ".c"))
            {
                filter.appendSourceFile(fileName);
            } else
            {
                filter.appendHeaderFile(fileName);
            }
            m_filterMap[filterName] = filter;
        }
    } else {
        std::list<std::string> sources, headers;
        if(StringUtil::endsWith(fileName, ".cpp") ||
                StringUtil::endsWith(fileName, ".cc") ||
                StringUtil::endsWith(fileName, ".c"))
        {
            sources.push_back(fileName);

        } else if(!fileName.empty()){
            headers.push_back(fileName);
        }
        m_filterMap.insert(std::pair<std::string, Filter>(filterName, Filter(filterName, sources, headers)));
    }
}
