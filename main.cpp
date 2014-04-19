#include "vcprojectreader.h"
#include "vcfilterwriter.h"

#include <string>

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("%s", "Usage: CreateFilter YourFile.vcxproj\n");
        return 1;
    }

    std::string projectFile = argv[1];
    std::string filterFile = projectFile + ".filters";
    VcProjectReader reader(projectFile);

    if(!reader.open())
    {
        fprintf(stderr, "%s %s\n", "Error opening file", projectFile.data());
        exit(-1);
    }

    VcProjectReader::FilterMap result = reader.read();
    FilterList filterList;
    for(VcProjectReader::FilterMap::const_iterator it = result.cbegin();
        it != result.cend(); ++it)
    {
        filterList.push_back(it->second);
    }

    VcFilterWriter writer(filterFile, filterList);
    writer.open();
    writer.write();

    return 0;
}
