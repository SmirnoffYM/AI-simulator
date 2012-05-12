#include "processcontainer.h"

ProcessContainer::ProcessContainer()
{
    applications = std::vector<std::pair<QString, QProcess *> >();
}

void ProcessContainer::addApplication(QString command)
{
    QProcess *application = new QProcess();
    applications.push_back(std::pair<QString, QProcess*>(command, application));
}

void ProcessContainer::launchApplications()
{
    for (unsigned int i = 0; i < applications.size(); i++) {
        applications.at(i).second->start(applications.at(i).first);
    }
}

void ProcessContainer::stopApplications()
{
    for (unsigned int i = 0; i < applications.size(); i++) {
        applications.at(i).second->kill();
    }
    applications.clear();
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
