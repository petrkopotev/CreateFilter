#include "vcprojectreader.h"
#include <QDir>
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

void VcProjectReader::close()
{
    // DO we need to reinitialize it?
    delete m_document;
    m_document = new tinyxml2::XMLDocument;
}


///TODO extract this to something more neat
void VcProjectReader::LookupForSources(tinyxml2::XMLNode* node)
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
        LookupForSources(node);
    }
    
    for(tinyxml2::XMLNode* child = node->FirstChild(); child; child = child->NextSibling())
    {
        readChildren(child);
    }
}

const QMap<std::string, Filter> &VcProjectReader::read()
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

    return result;
}

void VcProjectReader::readSources(const char *str)
{
    std::string row = str;
    char separator = QDir::separator().toLatin1();
    std::list<std::string> list = split(row, std::string(&separator));

    //QString row = QString::fromLocal8Bit(str);
    //QStringList list = row.split(QDir::separator());
    createFilters(list);
}

void VcProjectReader::createFilters(const std::list<std::string> &rawStrings)
{
    std::string filterName, fileName;

    for(auto it = rawStrings.cbegin();
        it != rawStrings.cend(); it++)
    {
        if(!std::regex_match(*it, std::regex("\\.(h|c(c)|c(pp)?)$")))
        {
            filterName += *it;
            fileName.erase(fileName.find("..\\"));
            insertFilter(fileName);
            filterName += "\\";
        } else {
            fileName = filterName + *it;
            filterName.erase(filterName.find("..\\"));
            filterName.erase(filterName.find_last_of("\\"));
            insertFilter(filterName, fileName);
        }
    }


//    foreach (QString name, rawStrings) {

//        QRegExp rx("\\.(h|c(c)|c(pp)?)$");
//        if(!name.contains(rx))
//        {
//            filterName += name;
//            insertFilter(filterName.remove("..\\"));
//            filterName += "\\";
//        } else {
//            fileName = filterName + name;
//            insertFilter(filterName.remove("..\\").remove(filterName.lastIndexOf("\\"), 1), fileName);
//        }
//    }
}

bool endsWith(std::string const &fullString, std::string const &ending)
{
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

void VcProjectReader::insertFilter(const std::string &filterName, const std::string &fileName)
{
    if(filterName.empty())
        return;

    if(m_filterMap.contains(filterName))
    {
        if(!fileName.empty())
        {
            Filter filter = m_filterMap[filterName];
            if(endsWith(fileName, ".cpp") || endsWith(fileName, ".cc") || endsWith(fileName, ".c"))
            {
                filter.appendSourceFile(fileName);
            } else
            {
                filter.appendHeaderFile(fileName);
            }
            m_filterMap[filterName] = filter;
        }
    } else {
        QStringList sources, headers;
        if(endsWith(fileName, ".cpp") || endsWith(fileName, ".cc") || endsWith(fileName, ".c"))
        {
            sources << fileName;

        } else if(!fileName.isEmpty()){
            headers << fileName;
        }
        m_filterMap.insert(filterName, Filter(filterName, sources, headers));
    }
}
