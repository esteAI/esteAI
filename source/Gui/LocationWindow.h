#pragma once

#include "LocationWidgets.h"

class LocationWindow
{
public:
    void init(LocationWidgets const& widgets);
    void process();

    bool isOn() const;

    int getLocationIndex() const;
    void setLocationIndex(int locationIndex);

private:
    LocationWidgets _widgets;
    int _id = 0;
    bool _on = false;
};
