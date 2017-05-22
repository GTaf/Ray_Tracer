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
#include "basics.h"

using namespace std;

/*
 * 
 */
Vector::Vector(){
    x = 0; y = 0; z = 0;
}
Vector::Vector(int a, int b, int c){
    x = a; y = b; z = c;
}
Vector Vector::multiply(int number){
    return Vector(x*number,y*number,z*number);
}
Vector Vector::add(Vector a){
    return Vector(a.x+x, a.y+y, a.z+z);
}
int Vector::dotProduct(Vector a){
    return x*a.x+y*a.y+z*a.z;
}


Ray::Ray(Vector gv, Vector gp){v = gv; p = gp;}
Vector Ray::getPoint(){return p;}
Vector Ray::getVector(){return v;}

Vector Sphere::getCenter(){return p;}
int Sphere::getRadius(){return radius;}

Sphere::Sphere(int r, Vector c){
    radius = r;
    p = c;
}

void Color::setValue(const double value,const int channel){
    if(value>1 || value<0) return;
    switch(channel){
        case 1:
            r=value;break;
        case 2:
            g=value;break;
        case 3:
            b=value;break;
        default:
            break;       
    }
}