#include <QCoreApplication>
#include <QDebug>
#include "vcprojectreader.h"
#include "vcfilterwriter.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList argList = a.arguments();

    if(argList.count() != 2)
    {
        qDebug() << "Usage: CreateFilter YourFile.vcxproj";
        exit(1);
    }

    QString projectFile = argList.at(1);
    QString filterFile = projectFile + ".filters";

    VcProjectReader reader(projectFile);

    if(!reader.open())
    {
        qDebug() << "Error opening file" << projectFile;
        exit(-1);
    }
    VcFilterWriter writer(filterFile, reader.read().values());
    writer.open();
    writer.write();
    writer.close();
    reader.close();

    qDebug() << "Done";

    return a.exec();
}
