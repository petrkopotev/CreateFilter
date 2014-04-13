#ifndef FILTER_H
#define FILTER_H

#include <QString>
#include <QStringList>
#include <QUuid>

class Filter
{
public:
    typedef std::list<std::string> StringList;

    Filter();
    explicit Filter(const std::string &name);
    explicit Filter(const std::string &name, const StringList &sources, const StringList &headers);

    std::string getFilterName() const { return m_name; }
    void setFilterName(const std::string &name);

    QString getUuid() const { return m_uuid.toString(); }

    StringList getSources() const { return m_sources; }
    void appendSourceFile(const std::string &fileName);

    StringList getHeaders() const { return m_headers; }
    void appendHeaderFile(const std::string &fileName);

private:

    std::string     m_name;
    StringList m_sources;
    StringList m_headers;
    QUuid       m_uuid;
};

#endif // FILTER_H
