/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: gtaf
 *
 * Created on 21 mai 2017, 14:05
 */

#include <cstdlib>
#include "basics.h"
#include <iostream>

using namespace std;

/*
 * 
 */
bool ray_sphere_intersect(Ray r, Sphere s);

int main(int argc, char** argv) {
    
    Sphere s = Sphere(1, Vector(0,0,0));
    Ray r = Ray(Vector(1,0,0), Vector(0,5,0));
    bool b = ray_sphere_intersect(r,s);
    cout << b << endl;
    
    return 0;
}

bool ray_sphere_intersect(Ray r, Sphere s){
    double d;
    Vector m = r.getPoint().add(s.getCenter().multiply(-1));
    d = r.getVector().dotProduct(m);
    d = d*d;
    d -= m.dotProduct(m);
    d += s.getRadius()*s.getRadius();
    //according to https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
    return d >= 0;
}

