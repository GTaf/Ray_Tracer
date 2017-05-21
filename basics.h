/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   basics.h
 * Author: gtaf
 *
 * Created on 21 mai 2017, 14:10
 */

#ifndef BASICS_H
#define BASICS_H


#endif /* BASICS_H */

class Vector{
public:
    Vector();
    Vector(int a, int b, int c);
    Vector multiply(int number);
    Vector add(Vector a);
    int dotProduct(Vector a);
    
    
private:
    int x, y ,z;
    
};

class Ray{
public:
    Ray(Vector gv, Vector gp);
    Vector getPoint();
    Vector getVector();
    
    
private:
    Vector v,p;//p est le point, v le vecteur
};

class Sphere{
public:
    Sphere(int r, Vector c);
    Vector getCenter();
    int getRadius();
    
private:
    int radius;
    Vector p;
    int color;
};

class Source{
private:
    Vector p;
};

