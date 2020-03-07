#include "settingsmanager.h"

#include <QStandardPaths>
#include <QDebug>

SettingsManager::SettingsManager()
{
    //Required to be set in order to use the QSettings!
    QCoreApplication::setOrganizationName("Senior Project");
    QCoreApplication::setApplicationName("TextEditor");

    settings = new QSettings();
}

SettingsManager::~SettingsManager()
{
    settings->sync();
    qDebug() << "Saving...";
    delete settings;
}

void SettingsManager::loadSettings()
{

}

void SettingsManager::saveSettings()
{

}

QVariant SettingsManager::getValue(const QString &key)
{
    if(key.contains("geometry")) {
        return settings->value(key, QRect(0,0,880,700));
    }
}

void SettingsManager::saveValue(const QString &groupName, const QString &name, QVariant data)
{
    settings->beginGroup(groupName);
        settings->setValue(name, data);
    settings->endGroup();
}
