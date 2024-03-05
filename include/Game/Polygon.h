#ifndef PLAY_TOGETHER_POLYGON_H
#define PLAY_TOGETHER_POLYGON_H

#include <vector>
#include <cmath>
#include "Point.h"
#include <boost/serialization/vector.hpp>

/**
 * @file Polygon.h
 * @brief Defines the Polygon class responsible for handling polygons in 2D space.
 */

/**
 * @class Polygon
 * @brief Represents a polygon in 2D space.
 */
class Polygon {
public:
    /** CONSTRUCTOR **/

    Polygon(const std::vector<Point> &vertices);


    /** ACCESSORS **/

    [[nodiscard]] std::vector<Point> getVertices() const;


    /** PUBLIC METHODS **/

    /**
     * @brief Checks if a polygon is convex.
     * @param polygon The polygon to check.
     * @return True if the polygon is convex, false otherwise.
     */
    [[nodiscard]] bool isConvex() const;

private:
    /** ATTRIBUTES **/

    std::vector<Point> vertices; /**< The vertices of the polygon. */


    /** PRIVATE METHODS **/

    /**
     * @brief Calculate the distance between two points.
     * @param a The first point.
     * @param b The second point.
     * @return The distance between points a and b.
     */
    static double distance(const Point &a, const Point &b);

    /**
     * @brief Calculate the angle between three points.
     * @param a The first point.
     * @param b The second point.
     * @param c The third point.
     * @return The angle in radians between the three points.
     */
    static double calculateAngle(const Point &a, const Point &b, const Point &c);

    /**
     * @brief Calculate the sum of interior angles of the polygon.
     * @return The sum of interior angles in degrees.
     */
    [[nodiscard]] double totalAngles() const;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
        ar & vertices;
    }
};

#endif //PLAY_TOGETHER_POLYGON_H
