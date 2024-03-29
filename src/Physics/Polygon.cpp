#include "../../include/Physics/Polygon.h"

/**
 * @file Polygon.cpp
 * @brief Implements the Polygon class functions for handling polygons in 2D space.
 */

/** CONSTRUCTOR **/

Polygon::Polygon(const std::vector<Point> &vertices) : vertices(vertices) {}


/** ACCESSORS **/

std::vector<Point> Polygon::getVertices() const {
    return vertices;
}


/** METHODS **/

double Polygon::distance(const Point& a, const Point& b) {
    return std::hypot(b.x - a.x, b.y - a.y);
}

double Polygon::calculateAngle(const Point& a, const Point& b, const Point& c) {
    // Using the Law of Cosines to calculate the angle between three points.

    // Calculate the distances between points a, b, and c.
    double ab = distance(a, b);
    double bc = distance(b, c);
    double ac = distance(a, c);

    // Apply the Law of Cosines to find the cosine of the angle.
    double cosTheta = (ab * ab + bc * bc - ac * ac) / (2 * ab * bc);
    double angle = std::acos(cosTheta);

    return angle;
}

double Polygon::totalAngles() const {
    // Calculate the sum of interior angles of the polygon.

    double sumAngles = 0;

    for (size_t i = 0; i < vertices.size(); ++i) {
        const Point& a = vertices[i];
        const Point& b = vertices[(i + 1) % vertices.size()];
        const Point& c = vertices[(i + 2) % vertices.size()];

        // Calculate the angle in radians using the Law of Cosines.
        double angleInRadians = calculateAngle(a, b, c);

        // Convert the angle to degrees.
        double angleInDegrees = angleInRadians * 180.0 / M_PI;

        // Accumulate the angles.
        sumAngles += angleInDegrees;
    }

    return sumAngles;
}

bool Polygon::isConvex() const{
    size_t n = vertices.size();
    if (n < 3) {
        return false;
    }

    // Check if the sum of interior angles equals (n - 2) * 180 degrees (convex polygon property) with a tolerance
    const double tolerance = 1e-3;
    return std::abs(totalAngles() - static_cast<double>(n - 2) * 180) < tolerance;
}