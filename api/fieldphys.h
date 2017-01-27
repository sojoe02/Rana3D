#ifndef FIELDPHYS_H
#define FIELDPHYS_H

#include "utility.h"
#include <vector>
#include <cmath>
#include <map>
#include <list>

struct memory{
    static std::map<int,agentInfo>* lastPositions;
    static std::map<int,std::vector<double> >* momentum;
};

std::vector<double> fieldEffects(agentInfo myAgent, double scale);


#endif // FIELDPHYS_H
