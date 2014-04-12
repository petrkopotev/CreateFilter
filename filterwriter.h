#ifndef FILTERWRITER_H
#define FILTERWRITER_H

#include <QList>
#include "Filter.h"
#include "tinyxml/tinyxml2.h"

class FilterWriter
{
public:
    FilterWriter(const QString &fileName);
    ~FilterWriter();

    void StartWrite();
    void WriteHeaders();
    void WriteBody(const QList<Filter> &filterList);
    void WriteFilter(tinyxml2::XMLNode *parentNode, const Filter &filter);
    void EndWrite();

private:
    void WriteFilterNames(tinyxml2::XMLNode *parentNode, const QList<Filter> &filterList);
    void ParseFilterSourceFiles(tinyxml2::XMLNode* parentNode, const QList<Filter> &filterList);
    void ParseFilterHeadersFiles(tinyxml2::XMLNode* parentNode, const QList<Filter> &filterList);
    void WriteFilterSourceFile(tinyxml2::XMLNode* parentNode, const Filter &filter, const QString &fileName);
    void WriteFilterHeaderFile(tinyxml2::XMLNode* parentNode, const Filter &filter, const QString &fileName);
    void WriteFilterFile(const char* tag, tinyxml2::XMLNode* parentNode, const Filter &filter, const QString &fileName);

    QString m_fileName;
    tinyxml2::XMLDocument *m_document;

};

#endif // FILTERWRITER_H
