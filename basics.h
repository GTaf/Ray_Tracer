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
    Vector();//vecteur nul
    Vector(int a, int b, int c);
    Vector multiply(int number);//multiplication par un scalaire
    Vector add(Vector a);//additionne 2 vecteurs
    int dotProduct(Vector a);//produit vectoriel
    
    
private:
    int x, y ,z;
    
};

<<<<<<< HEAD

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


=======
>>>>>>> ccd9ccb90db401bd9cd0de772e3bceaf38cacb5a
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
<<<<<<< HEAD
    Color color;
=======
    int color;
>>>>>>> ccd9ccb90db401bd9cd0de772e3bceaf38cacb5a
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

<<<<<<< HEAD
=======
class Color{
public:
    Color(R,G,B):r(R),g(G),b(B){}
    Color(){Color(1.,1.,1.);}
    setValue(const double value,const int channel);
private:
    double r;
    double g;
    double b;
};
>>>>>>> ccd9ccb90db401bd9cd0de772e3bceaf38cacb5a
