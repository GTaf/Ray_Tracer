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

#include <vector>


#endif /* BASICS_H */

class Vector{
public:
    Vector();//vecteur nul
    Vector(double a, double b, double c);
    Vector multiply(double n);//multiplication par un scalaire
    Vector add(Vector a);//additionne 2 vecteurs
    double dotProduct(Vector a);//produit vectoriel
    Vector orth(Vector v);
    Vector minus(Vector v);
    double norm();
    Vector normalize();
    
    
private:
    double x, y ,z;
    
};


class Color{
public:
    Color(double R,double G,double B):r(R),g(G),b(B){}
    Color(){Color(1.,1.,1.);}
    void setValue(const double value,const int channel);
private:
    double r;
    double g;
    double b;
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
    Sphere();
    Sphere(int r, Vector c);
    Vector getCenter();
    int getRadius();
    
private:
    int radius;
    Vector p;
    Color color;
};

class Source{
private:
    Vector p;
};

class Light{
public:
    Vector getPoint();
    Color getColor();
 
private:
    Vector p;
    Color color; //Les trois composantes de couleur
};

class Camera{
public:
    Vector getEye();
    Vector getTarget();
    Vector getUp();
    int getW();
    int getH();
    Ray Rayf(double x, double y);//ray thas goes from eye to target + (x,y)
    
    
private:
    Vector eye, target, up;//up défini l'unité de base de l'image
    int h,w;//dimensions
};

class Scene{
public:
    Scene(int size);
    int size();
    void addSphere(Sphere s);
    Sphere getSphere(int index);
private:
    std::vector<Sphere> set; 
};