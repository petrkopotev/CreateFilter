#include "vcprojectreader.h"
#include <QDir>
#include <QDebug>


VcProjectReader::VcProjectReader(const QString &fileName) :
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
    m_document->LoadFile(m_fileName.toStdString().data());
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
                        qDebug() << el->Name() << el->FirstAttribute()->Name() << el->FirstAttribute()->Value();
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

const QMap<QString, Filter> &VcProjectReader::read()
{
    readChildren(m_document);
    
    return m_filterMap;
}

void VcProjectReader::readSources(const char *str)
{
    QString row = QString::fromLocal8Bit(str);
    QStringList list = row.split(QDir::separator());
    createFilters(list);
}

void VcProjectReader::createFilters(const QStringList &rawStrings)
{
    QString filterName, fileName;

    foreach (QString name, rawStrings) {
        QRegExp rx("\\.(h|c(c)|c(pp)?)$");
        if(!name.contains(rx))
        {
            filterName += name;
            insertFilter(filterName.remove("..\\"));
            filterName += "\\";
        } else {
            fileName = filterName + name;
            insertFilter(filterName.remove("..\\").remove(filterName.lastIndexOf("\\"), 1), fileName);
        }
    }
}

void VcProjectReader::insertFilter(const QString &filterName, const QString &fileName)
{
    if(filterName.isEmpty())
        return;

    if(m_filterMap.contains(filterName))
    {
        if(!fileName.isEmpty())
        {
            Filter filter = m_filterMap[filterName];
            if(fileName.endsWith(".cpp") || fileName.endsWith(".cc") || fileName.endsWith(".c"))
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
        if(fileName.endsWith(".cpp") || fileName.endsWith(".cc") || fileName.endsWith(".c"))
        {
            sources << fileName;

        } else if(!fileName.isEmpty()){
            headers << fileName;
        }
        m_filterMap.insert(filterName, Filter(filterName, sources, headers));
    }
}
