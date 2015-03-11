#include "doctor.h"
#include "output.h"

Master* Doctor::master = NULL;

void Doctor::InitDoctor(Master *arg_master)
{
	master = arg_master;
	//activeNestene = arg_activeNestene;
}

int Doctor::addLuaAuton(double x, double y, double z, std::string path)
{
	int id = master->addAuton(x, y, z, path, "Lua");
	Output::Inst()->addGraphicAuton(id,x,y);

	return id;
}

void Doctor::removeAuton(int Id)
{
	master->removeAuton(Id);
	Output::Inst()->removeGraphicAuton(Id);
}



