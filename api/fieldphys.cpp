#include "utility.h"
#include <vector>
#include <cmath>
#include <map>
#include <list>
#include "fieldphys.h"


std::map<int,agentInfo>* memory::lastPositions = new std::map<int,agentInfo>;

std::map<int,std::vector<double> >* memory::momentum = new std::map<int,std::vector<double> >;

std::vector<double> fieldEffects(agentInfo myAgent, double scale){
    double G=6.674*pow(10,-11)*scale; // 6.674×10−11 N⋅m2/kg2
    double K=8.99*pow(10,9)*scale; // 8.99×10^9 N.m2/C2
    double e=1.60217662*pow(10,-19);
    std::vector<double> netAcc{0,0,0};

    double xDif,yDif,zDif,R;
    double gAcc;
    double cAcc;
    double instAccX, instAccY, instAccZ;

    typedef std::map<std::string, std::map<std::string, std::string>>::iterator it_type;
    for(std::_Rb_tree_iterator<std::pair<const int,agentInfo>> info = memory::lastPositions->begin(); info != memory::lastPositions->end(); info++) {
        if(info->second.id!=myAgent.id){
            xDif = info->second.x - myAgent.x;
            yDif = info->second.y - myAgent.y;
            zDif = info->second.z - myAgent.z;
            R=pow(xDif,2)+pow(yDif,2)+pow(zDif,2);
            if (R!=0){
                gAcc = info->second.mass/R;
                cAcc = -info->second.charge/R;

                instAccX=(xDif/sqrt(R))*(gAcc*G+cAcc*K*e*e*myAgent.charge/myAgent.mass);
                instAccY=(yDif/sqrt(R))*(gAcc*G+cAcc*K*e*e*myAgent.charge/myAgent.mass);
                instAccZ=(zDif/sqrt(R))*(gAcc*G+cAcc*K*e*e*myAgent.charge/myAgent.mass);

                netAcc[0]+=instAccX;
                netAcc[1]+=instAccY;
                netAcc[2]+=instAccZ;
            }
        }
    }
    return netAcc;
    //f= gmm/r^2

}
