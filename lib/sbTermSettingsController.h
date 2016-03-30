#ifndef SBTERMSETTINGSCONTROLLER_H
#define SBTERMSETTINGSCONTROLLER_H

#include "sbTermSettings.h"

class sbTermSettingsController
{
public:
    sbTermSettingsController();
    virtual ~sbTermSettingsController();
    
    void initialize() const;
    
    void readSerialSettings(sbTerm::PortSettings &settings) const;
    void writeSerialSettings(const sbTerm::PortSettings &settings) const;
    
    void readConsoleOptions(sbTerm::ConsoleOptions &options) const;
    void writeConsoleOptions(const sbTerm::ConsoleOptions &options) const;
    
private:
    class Private;
    Private *d;
};

#endif // SBTERMSETTINGSCONTROLLER_H
