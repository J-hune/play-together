#ifndef PLAY_TOGETHER_POINT_H
#define PLAY_TOGETHER_POINT_H

/**
 * @file Point.h
 * @brief Defines the Point structure for 2D coordinates.
 */

/**
 * @struct Point
 * @brief Represents a point in 2D space with x and y coordinates.
 */
struct Point {
    float x; /**< The x-coordinate of the point. */
    float y; /**< The y-coordinate of the point. */

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
        ar & x;
        ar & y;
    }

};

#endif //PLAY_TOGETHER_POINT_H

