#include "filter.h"

Filter::Filter()
{
    m_uuid = QUuid::createUuid();
}

Filter::Filter(const std::string &name) :
    m_name(name)
{
    m_uuid = QUuid::createUuid();
}

Filter::Filter(const std::string &name, const StringList &sources, const StringList &headers) :
    m_name(name)
  , m_sources(sources)
  , m_headers(headers)
{
    m_uuid = QUuid::createUuid();
}

void Filter::setFilterName(const std::string &name)
{
    m_name = name;
}

void Filter::appendSourceFile(const std::string &fileName)
{
    m_sources.push_back(fileName);
}

void Filter::appendHeaderFile(const std::string &fileName)
{
    m_headers.push_back(fileName);
}
