#include "vcprojectreader.h"
#include "vcfilterwriter.h"

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("%s", "Usage: CreateFilter YourFile.vcxproj\n");
        return 1;
    }

    std::string projectFile = argv[1];
    std::string filterFile = projectFile + ".filters";
    VcProjectReader reader(projectFile.data());

    if(!reader.open())
    {
        fprintf(stderr, "%s %s\n", "Error opening file", projectFile.data());
        exit(-1);
    }

    std::map<std::string, Filter> result = reader.read();
    std::list<Filter> filterList;
    for(std::map<std::string, Filter>::const_iterator it = result.cbegin();
        it != result.cend(); ++it)
    {
        filterList.push_back(it->second);
    }

    VcFilterWriter writer(filterFile, filterList);
    writer.open();
    writer.write();
    writer.close();
    reader.close();

    return 0;
}
