#ifndef RT_H_INCLUDED
#define RT_H_INCLUDED
#include "vecteur3.h"
#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>
#include <random>

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

inline double deg_rad(double deg) {
    return deg * pi / 180.0;
}


   inline double random_double() {

    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {

    return min + (max-min)*random_double();
}


inline double constrain(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}



#endif // RT_H_INCLUDED
