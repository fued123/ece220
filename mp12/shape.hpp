#ifndef SHAPE_H_
#define SHAPE_H_

#include<iostream>
#include<cmath>
#include<string>
#include<algorithm>
#include<list>
#include<fstream>
#include <iomanip>    
#include <vector>

using namespace std;

//Base class
//Please implement Shape's member functions
//constructor, getName()
class Shape{
public:
	//Base class' constructor should be called in derived classes'
	//constructor to initizlize Shape's private variable 
  	Shape(string name) {
			name_ = name;

	}
	
  	string getName() {
			return name_;
	}
	
  	virtual double getArea() const = 0;
  	virtual double getVolume() const = 0;
	
private:
  string name_;


};

//Rectangle
//Please implement the member functions of Rectangle:
//constructor, getArea(), getVolume(), operator+, operator-

template <class T>
class Rectangle : public Shape{
public:
Rectangle<T>(T width = 0, T length = 0):Shape("Rectangle") {
		width_ = width;		//constructors for width and length
		length_ = length;
	}
	
  	double getArea() const {
		return (T)(length_*width_);	//return area equation for rectangle
	}
	
  	double getVolume() const {
		return 0;		//return 0 since volume of rectangle is not possible 
	}
	
	Rectangle<T> operator + (const Rectangle<T>& rec) {
		Rectangle<T> c = Rectangle(length_ + rec.length_, width_ + rec.width_);
    	return c;	//return rectangle with correct operators 
	}
	
	Rectangle<T> operator - (const Rectangle<T>& rec) {
		T w = max((T) 0, (width_ - rec.width_));	//width R3 = width R1- Width R2
		T l = max((T) 0, (length_ - rec.length_)); 	//length R3 = length R1- length R2

		Rectangle<T> c = Rectangle(w,l);	
		return c;		//return rectangle with correct operators 
	} 

	
	T getWidth() const { 
			return width_;
	}
	
	T getLength() const { 
			return length_;
	}
private:
	T width_;
    T length_;

};

//Circle
//Please implement the member functions of Circle:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here

class Circle : public Shape{
public:
  	Circle(double radius):Shape("Circle") {
		  	radius_ = radius;
	}
	
  	double getArea() const{
			return radius_ * radius_ * M_PI;
	}
	
 	double getVolume() const{
			return 0;
	}
	
  	Circle operator + (const Circle& cir){
		  	Circle c = Circle (0.0);
			c.radius_ = radius_ + cir.radius_;
			return c;
	}
	
	Circle operator - (const Circle& cir){
			Circle c = Circle(0.0);
			c.radius_ = max(0.0, radius_ - cir.radius_);
			return c;
	} 

	double getRadius() const { 
			return radius_;
	}
	
private:
	double radius_;

};



//Sphere
//Please implement the member functions of Sphere:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here

class Sphere : public Shape{
public:
  	Sphere(double radius):Shape("Sphere") {
		  	radius_ = radius;
	}

  	double getVolume() const {
		return (4.0 / 3.0) * radius_ * radius_ * radius_ * M_PI;
	}	
	
  	double getArea() const {
		return 4 * radius_ * radius_ * M_PI;
	}

	Sphere operator + (const Sphere& sph) {
			Sphere c = Sphere(0.0);
			c.radius_ = radius_ + sph.radius_;
			return c;
	}

	Sphere operator - (const Sphere& sph) {
			Sphere c = Sphere(0.0);
			c.radius_ = max(0.0, radius_ - sph.radius_);
			return c;
	} 
	
	double getRadius() const {
			return radius_;
	}

private:
	double radius_;

};

//Rectprism
//Please implement the member functions of RectPrism:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here
class RectPrism : public Shape{
public:
  	RectPrism(double width, double length, double height):Shape("RectPrism") {
		  	width_ = width;
			length_ = length;
			height_ = height;
	}
	
  	double getVolume() const {
			return width_ * length_ * height_;
	}
  	
	double getArea() const {
			return 2 * (length_ * width_ + length_ * height_ + width_ * height_);
	}
	
	RectPrism operator + (const RectPrism& rectp){
			RectPrism c = RectPrism(0.0, 0.0, 0.0);
			c.length_ = length_ + rectp.length_;
			c.width_ = width_ + rectp.width_;
			c.height_ = height_ + rectp.height_;
			return c;
	}
	
	RectPrism operator - (const RectPrism& rectp){
			RectPrism c = RectPrism(0.0, 0.0, 0.0);
			c.length_ = max(0.0, length_ - rectp.length_);
			c.width_ = max(0.0, width_ - rectp.width_);
			c.height_ = max(0.0, height_ - rectp.height_);
			return c;
	}
	
	double getWidth() const { 
			return width_;
	}
	
	double getLength() const { 
			return length_;
	}
	
	double getHeight() const { 
			return height_;
	}
private:
  	double length_;
  	double width_;
  	double height_;

};

// Read shapes from test.txt and initialize the objects
// Return a vector of pointers that points to the objects 

static list<Shape*> CreateShapes(char* file_name) {
	//@@Insert your code here
	// create and open an input file stream with the given file name
	ifstream input_file (file_name, std::ifstream::in);

	// read in the number of shapes contained in the input file
	int num_shapes;
	input_file >> num_shapes;

	// declare return vector and temp pointer for adding shapes


	std::list<Shape *> shape_vector;
	Shape* newShape;

	// iterate over the input file to read in shapes
	for (int i = 0; i < num_shapes; i++) {
		string shape_name;
		input_file >> shape_name;

		// read in the appropriate data based on the name of the shape
		if (shape_name == "Rectangle") {
			double w, l;
			input_file >> w >> l;
			//Rectangle<T> newShape(w, l);
			newShape = new Rectangle<double>(w,l);
		}
		else if (shape_name == "Circle") {
			double r;
			input_file >> r;
			newShape = new Circle(r);
		}
		else if (shape_name == "Sphere") {
			double r;
			input_file >> r;
			newShape = new Sphere(r);
		}
		else if (shape_name == "RectPrism") {
			double w, l, h;
			input_file >> w >> l >> h;
			newShape = new RectPrism(w,l,h);
		}

		// store the newly created shape in the vector
		shape_vector.push_back(newShape);
	}

	// close the input stream and return the vector
	input_file.close();
	return shape_vector;

}

// call getArea() of each object 
// return the max area
static double MaxArea(list<Shape*> shapes){
	double max_area = 0;
	//@@Insert your code here
	for (std::list<Shape*>::iterator i = shapes.begin(); i != shapes.end(); i++) {
		if((*i)->getArea() > max_area){
			max_area = (*i) -> getArea();
		}
	}
	return max_area;
}

// call getVolume() of each object 
// return the max volume
static double MaxVolume(list<Shape*> shapes){
	double max_volume = 0;
	//@@Insert your code here
	for (std::list<Shape*>::iterator i = shapes.begin(); i != shapes.end(); i++) {
		if((*i)->getVolume() > max_volume){
		//list<shapes>::iterator x = shapes.begin();
		//max_volume = fmax(max_volume, i* -> getVolume());
		max_volume = (*i) -> getVolume();
		}
	}
	return max_volume;


	// double max_volume = 0;
	// //@@Insert your code here
	// for (int i = 0; i < shapes.size(); i++) {
	// 	max_volume = fmax(max_volume, shapes[i]->getVolume());
	// }
	// return max_volume;
}
#endif

