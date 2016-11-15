#ifndef SPHEREITEM_H
#define SPHEREITEM_H


class sphereItem
{
public:
    sphereItem(int id, double x, double y, double z, double rad, int r, int b, int g);
    sphereItem();
    double xPos,yPos,zPos;
    double radius;
    int red,blue,green;
    int ID;
};

#endif // SPHEREITEM_H
