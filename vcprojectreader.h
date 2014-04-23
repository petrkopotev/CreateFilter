#ifndef VCPROJECTREADER_H
#define VCPROJECTREADER_H

#include <map>
#include "Filter.h"
#include "tinyxml/tinyxml2.h"


class VcProjectReader
{
public:
    typedef std::map<std::string, Filter> FilterMap;

    VcProjectReader(const std::string& fileName);
    ~VcProjectReader();

    bool open();
    const FilterMap& read();

protected:
    void readSources(const std::string& str, bool isFilterCustom = false);
    void readChildren(tinyxml2::XMLNode* node);
    void lookupForSources(tinyxml2::XMLNode* node);

    void createFilters(const std::list<std::string> &filterNames, bool isFilterCustom);
    void insertFilter(const std::string &filterName, bool isFilterCustom, const std::string &fileName = std::string());

private:
    std::string            m_fileName;
    tinyxml2::XMLDocument *m_document;
    FilterMap              m_filterMap;
};

#endif // VCPROJECTREADER_H
