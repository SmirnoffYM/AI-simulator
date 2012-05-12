#ifndef PROCESSCONTAINER_H
#define PROCESSCONTAINER_H

#include <QProcess>
#include <vector>

class ProcessContainer
{
public:
    static ProcessContainer& getInstance()
    {
        static ProcessContainer instance;
        return instance;
    }

    // Managing robot apps
    void launchApplications();
    void stopApplications();
    void addApplication(QString command);

private:
    ProcessContainer();
    ProcessContainer(ProcessContainer const&);
    void operator=(ProcessContainer const&);

    std::vector<std::pair<QString, QProcess *> > applications;
};

#endif // PROCESSCONTAINER_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
