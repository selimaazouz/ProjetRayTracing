#ifndef RAYON_H_INCLUDED
#define RAYON_H_INCLUDED
#include "vecteur3.h"
#include "ObjectList.h"
#include "rt.h"
class rayon {
private:

    vecteur3 origineR;
    vecteur3 directionR;
    double tempsR;
public:

    rayon() {}
    rayon(const vecteur3& o, const vecteur3& d, double t = 0.0)
        : origineR(o), directionR(d), tempsR(t)
    {}

    vecteur3 origine() const  { return origineR; }


    vecteur3 direction() const { return directionR; }


    double temps() const    { return tempsR; }


    vecteur3 pt_a_distance(double distance) const {
        return origineR + distance * directionR;
    }


};



#endif // RAYON_H_INCLUDED
