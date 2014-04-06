#include "vcprojectreader.h"
#include <QXmlStreamReader>
#include <QFile>
#include <QDir>
#include <QDebug>

VcProjectReader::VcProjectReader(const QString &fileName) :
    m_fileName(fileName)
{
    m_file = new QFile(m_fileName);
    m_reader = new QXmlStreamReader();
}

VcProjectReader::~VcProjectReader()
{
    if(m_file->isOpen())
    {
        m_file->close();
    }

    delete m_file;
    delete m_reader;
}

bool VcProjectReader::open()
{
    if(!m_file->open(QIODevice::ReadOnly))
    {
        return false;
    }

    m_reader->setDevice(m_file);
    m_filterMap.clear();

    return true;
}

void VcProjectReader::close()
{
    m_file->close();
}

const QMap<QString, Filter> &VcProjectReader::read()
{
    while (!m_reader->atEnd()) {
        QXmlStreamReader::TokenType type = m_reader->readNext();
        if(type == QXmlStreamReader::StartElement && m_reader->name() == "ItemGroup")
        {
            m_reader->readNextStartElement();
            if(m_reader->name() != "ClCompile" && m_reader->name() != "ClInclude" ) continue;
            while (!m_reader->isEndElement() || m_reader->name() != "ItemGroup") {
                readSources();
            }
        }
    }

    return m_filterMap;
}

void VcProjectReader::readSources()
{
    if(m_reader->attributes().size() != 0)
    {
        QString row = m_reader->attributes().at(0).value().toString();
        QStringList list = row.split(QDir::separator());
        createFilters(list);
    }

    m_reader->readNext();
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
