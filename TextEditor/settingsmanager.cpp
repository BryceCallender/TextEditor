#include "settingsmanager.h"

#include <QStandardPaths>
#include <QDebug>

bool SettingsManager::created = false;
SettingsManager* SettingsManager::instance = nullptr;

SettingsManager::SettingsManager()
{
    //Required to be set in order to use the QSettings!
    QCoreApplication::setOrganizationName("Senior Project");
    QCoreApplication::setApplicationName("TextEditor");

    settings = new QSettings();

    qDebug() << QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
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
    settings->sync();

    //update CodeSyntaxHighlighter colors and code tab length (from now on). Text tab need to decide on how to procede

}

QVariant SettingsManager::getValue(const QString &key)
{
    if(key.contains("geometry"))
    {
        return settings->value(key, QRect(0,0,880,700));
    }
    else if(key.contains("fontSize"))
    {
        return settings->value(key, 12);
    }
    else if(key.contains("fontFamily"))
    {
        return settings->value(key, QFont("Times New Roman"));
    }
    else if(key.contains("KeywordColor"))
    {
        return settings->value(key, QColor(Qt::darkBlue));
    }
    else if(key.startsWith("text") && key.contains("tabLength"))
    {
        return settings->value(key, 8);
    }
    else
    {
        return settings->value(key, 4);
    }

    return QVariant(); //Doesnt match a key for some reason?
}

void SettingsManager::saveValue(const QString &groupName, const QString &name, QVariant data)
{
    settings->beginGroup(groupName);
        settings->setValue(name, data);
    settings->endGroup();
}
