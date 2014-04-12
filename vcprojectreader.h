#ifndef VCPROJECTREADER_H
#define VCPROJECTREADER_H

#include<QString>
#include <QPointer>
#include <QMap>
#include "Filter.h"
#include "tinyxml/tinyxml2.h"

class QXmlStreamReader;
class QFile;

class VcProjectReader
{
public:
    VcProjectReader(const QString& fileName);
    ~VcProjectReader();

    bool open();
    void close();
    const QMap<QString, Filter>& read();



protected:
    void readSources();
    void readSources(const char* str);
    void readChildren(tinyxml2::XMLNode* node);
    void LookupForSources(tinyxml2::XMLNode* node);

    void createFilters(const QStringList &filterNames);
    void insertFilter(const QString &filterName, const QString &fileName = QString());

private:
    QString                m_fileName;
    tinyxml2::XMLDocument *m_document;
    QMap<QString, Filter>  m_filterMap;

};

#endif // VCPROJECTREADER_H
