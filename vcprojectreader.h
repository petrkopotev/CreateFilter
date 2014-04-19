#ifndef VCPROJECTREADER_H
#define VCPROJECTREADER_H

#include <map>
#include "Filter.h"
#include "tinyxml/tinyxml2.h"


class VcProjectReader
{
public:
    VcProjectReader(const std::string& fileName);
    ~VcProjectReader();

    bool open();
    void close();
    const std::map<std::string, Filter>& read();

protected:
    void readSources(const char* str);
    void readChildren(tinyxml2::XMLNode* node);
    void lookupForSources(tinyxml2::XMLNode* node);

    void createFilters(const std::list<std::string> &filterNames);
    void insertFilter(const std::string &filterName, const std::string &fileName = std::string());

private:
    std::string            m_fileName;
    tinyxml2::XMLDocument *m_document;
    std::map<std::string, Filter>  m_filterMap;
};

#endif // VCPROJECTREADER_H
