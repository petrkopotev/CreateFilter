#ifndef FILTER_H
#define FILTER_H

#include <QString>
#include <QStringList>
#include <QUuid>

class Filter
{
public:
    Filter();
    explicit Filter(const QString &name);
    explicit Filter(const QString &name, const QStringList &sources, const QStringList &headers);

    QString getFilterName() const { return m_name; }
    void setFilterName(const QString& name);

    QString getUuid() const { return m_uuid.toString(); }

    QStringList getSources() const { return m_sources; }
    void appendSourceFile(const QString &fileName);

    QStringList getHeaders() const { return m_headers; }
    void appendHeaderFile(const QString &fileName);

private:
    QString     m_name;
    QStringList m_sources;
    QStringList m_headers;
    QUuid       m_uuid;
};

#endif // FILTER_H
