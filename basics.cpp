/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   basics.cpp
 * Author: gtaf
 *
 * Created on 21 mai 2017, 14:06
 */

#include <cstdlib>
#include <set>
#include "basics.h"
#include <math.h>

using namespace std;

/*
 *
 */
Vector::Vector(){x = 0; y = 0; z = 0;}
Vector::Vector(double a, double b, double c){x = a; y = b; z = c;}
Vector Vector::multiply(double n)const {return Vector(x*n,y*n,z*n);}
Vector Vector::add(Vector a)const {return Vector(a.x+x, a.y+y, a.z+z);}
double Vector::dotProduct(Vector a)const {return x*a.x+y*a.y+z*a.z;}
Vector Vector::orth(Vector v)const {return Vector(y*v.z-z*v.y,z*v.x-v.z*x,x*v.y-y*v.x);}
Vector Vector::minus(Vector v)const {return this->add(v.multiply(-1));}
double Vector::norm()const {return sqrt(x*x+y*y+z*z);}
Vector Vector::normalize()const {double n = this->norm(); return Vector(x/n,y/n,z/n);}


Ray::Ray(Vector gv, Vector gp){v = gv; p = gp;}
Vector Ray::getPoint(){return p;}
Vector Ray::getVector(){return v;}

Vector Sphere::getCenter(){return p;}
int Sphere::getRadius(){return radius;}

Sphere::Sphere(int r, Vector c){
    radius = r;
    p = c;
}
Sphere::Sphere(){}


void Color::setValue(const double value,const int channel){
    if(value>1 || value<0 || channel <0 || channel>2) return;
    rgb[channel]=value;
}

double Color::getValue(const int channel){
    if (channel<0 || channel >2) return -1.;
    return rgb[channel];
}

Vector Camera::getEye(){return eye;}
Vector Camera::getTarget(){return target;}
Vector Camera::getUp(){return up;}
int Camera::getW(){return h;}
int Camera::getH(){return w;}
Ray Camera::Rayf(double x, double y){
    Vector final = target;
    final = final.add(up.normalize().multiply(y-h/2));
    final = final.add(up.orth(target.minus(eye)).normalize().multiply(x-w/2));
    Ray r = Ray(final.minus(eye), eye);
    return r;
}

Scene::Scene(int size){std::vector<Sphere> fset(size); set = fset;}
int Scene::size(){return set.size();}
void Scene::addSphere(Sphere s){set.push_back(s);}
void Scene::setAmbiantLighting(double l){
  if(l<0) ambiantLighting=0;
  else if(l>1) ambiantLighting=1;
  else ambiantLighting=l;
}
Sphere Scene::getSphere(int index){return set[index];}
