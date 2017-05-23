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
#include "CImg.hpp"

using namespace std;
using namespace cimg_library;


/*
 * 
 */
bool ray_sphere_intersect(Ray r, Sphere s);
void tracer();

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
    //according to 
    //https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
    return d >= 0;
}

void tracer(Camera ca, Scene s){
    //pour tous les spheres
    
    Vector eye = ca.getEye();
    Vector target = ca.getTarget();
    
    //code for Cimg found at 
    //https://stackoverflow.com/questions/14914709/
    CImg<float> img(ca.getH(),ca.getW(),1,3); //create image
    
    cimg_forXYC(img,x,y,c) {  //for each pixel of the image
        
        Ray r = ca.Rayf(x,y);
        for(int i = 0; i < s.size(); i++){
            if(ray_sphere_intersect(r,s.getSphere(i))){//compute color
                break;
            }
            img(x,y,0) = 1;
            img(x,y,1) = 1;
            img(x,y,2) = 1;
            
        }
        //img(x,y,c) = pixel_value_at(x,y,c); 
    }
    
    img.save("file.bmp");
    
}

