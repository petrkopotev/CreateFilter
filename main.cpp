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
    VcFilterWriter writer(filterFile.data(), reader.read().values());
    writer.open();
    writer.write();
    writer.close();
    reader.close();

    return 0;
}
