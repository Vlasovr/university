#pragma once
#include <string.h>
using namespace std;
class Shape
{
public: 
	virtual double getArea() = 0;
	virtual std::string getName() = 0;
	Shape(double raduis);
protected:
	double radius;

};

class TwoDShape : public Shape 
{
public :
	virtual double getArea();
	virtual std::String getName();
	TwoDShape(double raduis);
};

class Circle : public TwoDShape
{
public:
	double getArea();
	std::String getName();
	Circle(double radius);
};

Shape::Shape(double radius)
{
	this->radius = radius;
}
TwoDShape::TwoDShape(double radius) :Shape(radius) {}
Circle::Circle(double radius) : TwoDShape(radius) {}

double TwoDShape::getArea()
{
	return 0;
}