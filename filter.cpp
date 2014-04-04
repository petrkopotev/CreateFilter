#include "filter.h"
#include <QStringList>

Filter::Filter()
{
    m_uuid = QUuid::createUuid();
}

Filter::Filter(const QString &name) :
    m_name(name)
{
    m_uuid = QUuid::createUuid();
}

Filter::Filter(const QString &name, const QStringList &sources, const QStringList &headers) :
    m_name(name)
  , m_sources(sources)
  , m_headers(headers)
{
    m_uuid = QUuid::createUuid();
}

void Filter::setFilterName(const QString &name)
{
    m_name = name;
}

void Filter::appendSourceFile(const QString &fileName)
{
    m_sources.append(fileName);
}

void Filter::appendHeaderFile(const QString &fileName)
{
    m_headers.append(fileName);
}
