#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QCoreApplication>
#include <QSettings>

class SettingsManager
{
public:
    SettingsManager();

private:
    QSettings* settings;
};

#endif // SETTINGSMANAGER_H
