#ifndef FILTERWRITER_H
#define FILTERWRITER_H

#include "Filter.h"
#include "tinyxml/tinyxml2.h"

typedef std::list<Filter> FilterList;

class FilterWriter
{
public:
    FilterWriter(const std::string &fileName);
    ~FilterWriter();

    void StartWrite();
    void WriteHeaders();
    void WriteBody(const FilterList &filterList);
    void WriteFilter(tinyxml2::XMLNode *parentNode, const Filter &filter);
    void EndWrite();

private:
    void WriteFilterNames(tinyxml2::XMLNode *parentNode, const FilterList &filterList);
    void ParseFilterSourceFiles(tinyxml2::XMLNode* parentNode, const FilterList &filterList);
    void ParseFilterHeadersFiles(tinyxml2::XMLNode* parentNode, const FilterList &filterList);
    void WriteFilterSourceFile(tinyxml2::XMLNode* parentNode, const Filter &filter, const std::string &fileName);
    void WriteFilterHeaderFile(tinyxml2::XMLNode* parentNode, const Filter &filter, const std::string &fileName);
    void WriteFilterFile(const char* tag, tinyxml2::XMLNode* parentNode, const Filter &filter, const std::string &fileName);

    std::string m_fileName;
    tinyxml2::XMLDocument *m_document;

};

#endif // FILTERWRITER_H
