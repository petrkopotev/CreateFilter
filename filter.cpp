#include "filter.h"
#include "uuid.h"

Filter::Filter()
{
    m_uuid = GlobalUuids.back();
    GlobalUuids.pop_back();
}

Filter::Filter(const std::string &name) :
    m_name(name)
{
    m_uuid = GlobalUuids.back();
    GlobalUuids.pop_back();
}

Filter::Filter(const std::string &name, const StringList &sources, const StringList &headers) :
    m_name(name)
  , m_sources(sources)
  , m_headers(headers)
{
    m_uuid = GlobalUuids.back();
    GlobalUuids.pop_back();
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

void Filter::show() const
{
    printf("Filter name: %s\n", m_name.data());
    printf("Sources:\n");
    for(StringList::const_iterator it = m_sources.cbegin(); it != m_sources.cend(); ++it)
    {
        printf("  %s\n", it->data());
    }

    printf("Headers:\n");
    for(StringList::const_iterator it = m_headers.cbegin(); it != m_headers.cend(); ++it)
    {
        printf("  %s\n", it->data());
    }
}
