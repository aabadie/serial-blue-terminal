#ifndef SBTERMSETTINGSCONTROLLER_H
#define SBTERMSETTINGSCONTROLLER_H

#include "sbTermPortSettings.h"

class sbTermSettingsController
{
public:
    sbTermSettingsController();
    virtual ~sbTermSettingsController();
    
    void initialize() const;
    void read(sbTerm::PortSettings &settings) const;
    void write(const sbTerm::PortSettings &settings) const;
    
private:
    class Private;
    Private *d;
};

#endif // SBTERMSETTINGSCONTROLLER_H
