#ifndef SPHERE_H_INCLUDED
#define SPHERE_H_INCLUDED

#include "ObjectHit.h"
#include "vecteur3.h"
#include "rt.h"
#include "../include/tinyxml2.h"

#include "materiau.h"
class SphereObject :public Object {
public:
    point center;
    double radius;
    std::shared_ptr<materiau> mat;
    SphereObject() {}
    SphereObject(point center, double radius) : center(center), radius(radius) {};
    SphereObject(point center, double radius, std::shared_ptr<materiau> ma)
        : center(center), radius(radius), mat(ma) {};
    SphereObject(tinyxml2::XMLElement* element);

    virtual bool intersect(
        const rayon& r, double t_min, double t_max, EnregIntersect& record) ;

    virtual bool bounding_Box(double time0, double time1, BoundingBox& ob) ;

    virtual tinyxml2::XMLElement* to_xml(tinyxml2::XMLDocument& xmlDoc) ;


};

SphereObject::SphereObject(tinyxml2::XMLElement* element) {
    radius = element->DoubleAttribute("Radius");

    tinyxml2::XMLElement* centerXml = element->FirstChildElement("Center");
    center = point(centerXml->DoubleAttribute("x"), centerXml->DoubleAttribute("y"), centerXml->DoubleAttribute("z"));

    mat = materiau::materiau_from_xml(element->FirstChildElement("materiau"));
}

bool SphereObject::intersect(const rayon& r, double t_min, double t_max, EnregIntersect& record)  {
    vecteur3 oc = r.origine() - center;
    auto a = r.direction().norme2();
    auto half = produit_scalaire(oc, r.direction());
    auto c = oc.norme2() - radius * radius;

    auto discr = half * half - a * c;
    if (discr < 0) return false;
    auto sqrtD = sqrt(discr);

    auto root = (-half - sqrtD) / a;
    if (root < t_min || t_max < root) {
        root = (-half + sqrtD) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    record.t = root;
    record.p = r.pt_a_distance(record.t);
    vecteur3 surface_normal_at_intersection = (record.p - center) / radius;
    record.compute_face_normal(r, surface_normal_at_intersection);
    record.materiau_ptr = mat;

    return true;
}

bool SphereObject::bounding_Box(double time0, double time1, BoundingBox& ob)  {
    ob= BoundingBox(
        center - vecteur3(radius, radius, radius),
        center + vecteur3(radius, radius, radius));
    return true;
}

tinyxml2::XMLElement* SphereObject::to_xml(tinyxml2::XMLDocument& xmlDoc)  {
    tinyxml2::XMLElement* element = xmlDoc.NewElement("Sphere");

    element->SetAttribute("Radius", radius);

    tinyxml2::XMLElement* centerXml = xmlDoc.NewElement("Center");

    centerXml->SetAttribute("x", center.x());
    centerXml->SetAttribute("y", center.y());
    centerXml->SetAttribute("z", center.z());

    element->InsertEndChild(centerXml);

    tinyxml2::XMLElement* materiauXml = xmlDoc.NewElement("materiau");
    tinyxml2::XMLElement* materiauElement = mat->to_xml(xmlDoc);

    materiauXml->InsertEndChild(materiauElement);

    element->InsertEndChild(materiauXml);

    return element;
}



#endif // SPHERE_H_INCLUDED
