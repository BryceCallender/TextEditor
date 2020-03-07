#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QCoreApplication>
#include <QSettings>
#include <QRect>

class SettingsManager
{
public:
    SettingsManager();
    ~SettingsManager();

    void loadSettings();
    void saveSettings();

    QVariant getValue(const QString& key);
    void saveValue(const QString& groupName, const QString &name, QVariant data);
private:
    QSettings* settings;
};

#endif // SETTINGSMANAGER_H
