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
#define IS 1000
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
Vector Ray::getPoint()const{return p;}
Vector Ray::getVector()const{return v;}

Vector Sphere::getCenter()const{return p;}
double Sphere::getRadius()const{return radius;}
Sphere::Sphere(double r, Vector c){
    radius = r;
    p = c;
}
Sphere::Sphere(){}


void Color::setValue(const double value,const int channel){
    if(channel <0 || channel>2) return;
    rgb[channel]=value;
}
double Color::getValue(const int channel)const{
    if (channel<0 || channel >2) return -1.;
    return rgb[channel];
}

Vector Camera::getEye()const{return eye;}
Vector Camera::getTarget()const{return target;}
Vector Camera::getUp()const{return up;}
int Camera::getW()const{return h;}
int Camera::getH()const{return w;}
Ray Camera::Rayf(double x, double y){
    Vector final = target;
    final = final.add(up.normalize().multiply(y-h/2));
    final = final.add(up.orth(target.minus(eye)).normalize().multiply(x-w/2));
    Ray r = Ray(final.minus(eye).normalize(), eye);
    return r;
}

Scene::Scene(int size){std::vector<Sphere> fset(size); set = fset;}
int Scene::size()const{return set.size();}
void Scene::addSphere(Sphere s){set.push_back(s);}
void Scene::setAmbiantLighting(double l){
  if(l<0) ambiantLighting=0;
  else if(l>1) ambiantLighting=1;
  else ambiantLighting=l;
}
Sphere Scene::getSphere(int index)const{return set[index];}

void World::createWorld(){
  Sphere s = Sphere(1.5, Vector(10,2,2), Color(255,0,0), Material(0.5,0.5,0.5,20.));
  s.setR(0.5);
  Sphere t = Sphere(1, Vector(10,0,0), Color(0,255,0), Material(1,0.5,0.5,50));
  Sphere u = Sphere(1, Vector(12,0.5,4), Color(30,55,250), Material(1,0.5,0.5,50));
  Sphere v = Sphere(0.4, Vector(8,-0.5,-1), Color(120,155,250), Material(1,0.5,0.5,50));
  Sphere w = Sphere(0.4, Vector(10,-2,-1.5), Color(120,155,250), Material(1,0.5,0.5,1));
  t.setR(1);
  u.setR(1);
  v.setR(0.2);
  w.setR(0.1);
  Camera ca = Camera(Vector(0,0,0), Vector(110,0,0), Vector(0,1,0), IS, IS);
  Scene sc = Scene(0);
  sc.setAmbiantLighting(0.7);
  sc.addSphere(s);
  sc.addSphere(t);
  sc.addSphere(u);
  sc.addSphere(v);
  sc.addSphere(w);
  std::vector<Light> lights;
  lights.push_back(Light(Vector(0,100,200), Color(1.,1.,1.),Color(0.,0.,0.)));
  lights.push_back(Light(Vector(0,-100,-200), Color(1.,1.,1.),Color(1.,1.,1.)));
  addLight(lights[0]);
  addLight(lights[1]);
  setScene(sc);
  setCamera(ca);
}
