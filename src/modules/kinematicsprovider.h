#ifndef KINEMATICSPROVIDER_H
#define KINEMATICSPROVIDER_H

#include "../debughelperinterface.h"

namespace MVision {
class KinematicsProvider : public Debugable
{
public:
    KinematicsProvider();

    int getHorizon() { return 100; }
};
}

#endif // KINEMATICSPROVIDER_H
