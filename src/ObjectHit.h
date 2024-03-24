#ifndef OBJECTHIT_H_INCLUDED
#define OBJECTHIT_H_INCLUDED
#include "rayon.h"
#include "rt.h"
#include "BoundingBox.h"
#include "../include/tinyxml2.h"
class materiau;
struct EnregIntersect {
    point p;
    vecteur3 surface_normal;
    shared_ptr<materiau> materiau_ptr;
    double t;
    bool is_front_face;

    void compute_face_normal(const rayon& r, const vecteur3& surface_normal_at_intersection) {
        double product = produit_scalaire(r.direction(), surface_normal_at_intersection);
        if (product < 0) {
            is_front_face = true;
            surface_normal = surface_normal_at_intersection;
        } else {
                       is_front_face = false;
            surface_normal = -surface_normal_at_intersection;
        }
    }
};


class Object {
public:
    virtual bool intersect(const rayon& ray, double min_t, double max_t, EnregIntersect& record) ;
    virtual bool bounding_box(double start_time, double end_time, BoundingBox& output_box) ;
    virtual tinyxml2::XMLElement* to_xml(tinyxml2::XMLDocument& xmlDoc) ;
};



#endif // OBJECTHIT_H_INCLUDED
