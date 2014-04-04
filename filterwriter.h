#ifndef FILTERWRITER_H
#define FILTERWRITER_H

#include <QXmlStreamWriter>
#include <QDir>
#include <QList>
#include "Filter.h"

class FilterWriter
{
public:
    FilterWriter(QXmlStreamWriter *writer);
    ~FilterWriter();

    void StartWrite();
    void WriteHeaders();
    void WriteBody(const QList<Filter> &filterList);
    void WriteFilter(QXmlStreamWriter &writer, const Filter &filter);
    void EndWrite();

private:
    void WriteFilterNames(const QList<Filter> &filterList);
    void ParseFilterSourceFiles(const QList<Filter> &filterList);
    void ParseFilterHeadersFiles(const QList<Filter> &filterList);
    void WriteFilterSourceFile(QXmlStreamWriter &writer, const Filter &filter, const QString &fileName);
    void WriteFilterHeaderFile(QXmlStreamWriter &writer, const Filter &filter, const QString &fileName);
    QString ConcatenateDirs(const QString &dir, const QString& localDirName);

    QXmlStreamWriter *m_writer;

};

#endif // FILTERWRITER_H
