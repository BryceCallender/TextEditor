#include "settingsmanager.h"

SettingsManager::SettingsManager()
{
    //Required to be set in order to use the QSettings!
    QCoreApplication::setOrganizationName("Senior Project");
    QCoreApplication::setApplicationName("TextEditor");
    settings = new QSettings();
}
