#include <QCoreApplication>
#include <QDebug>
#include "vcprojectreader.h"
#include "vcfilterwriter.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    VcProjectReader reader("./WebCore.vcxproj");

    if(!reader.open())
    {
        qDebug() << "Error opening file";
        exit(100);
    }
    VcFilterWriter writer("./WebCore.vcxproj.filters", reader.read().values());
    writer.open();
    writer.write();
    writer.close();
    reader.close();

    qDebug() << "Done";

    return a.exec();
}
