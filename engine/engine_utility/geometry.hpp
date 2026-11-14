#pragma once

#include "vector.hpp"

#include <vector>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <cmath>

// Point [24 bytes]
struct Point {

    // Position
    double x;  // x coordinate
    double y;  // y coordinate
    double z;  // z coordinate

    // Constructors
    Point() : x(0.0), y(0.0), z(0.0) {} // needed for other class constructions to work for some reason
    template <typename T>
    Point(Vector3<T> vector) {x = double(vector.x); y = double(vector.y); z = double(vector.z); };  // from vector
    Point(double x_, double y_, double z_) { x = x_; y = y_; z = z_; };  // from points

    double pointDistanceToPoint(const Point& other_point) const;
    Vector3D pointVectorToPoint(const Point& other_point) const;
};

// Segment [48 bytes]
struct Segment {

    // Endpoints
    Point point_1;  // start of the segment
    Point point_2;  // end of the segment

    // Constructor
    Segment(Point point_1_, Point point_2_) { point_1 = point_1_; point_2 = point_2_; };

    double segmentLength() const;
    Point segmetMidpoint() const;
    bool pointOnSegment(const Point& point_3) const;
    bool segmentIntersectsSegment(const Segment& other_segment) const;
    double segmentDistanceToPoint(const Point& point_3) const;
    Point nearestPointOnThisSegment(const Point& other_point);
};

// Plane [48 bytes]
struct Plane {

    // Attributes
    Point plane_point;  // any point on the plane
    Vector3D plane_normal;  // points toward the kept side when clipping

    // Constructor
    Plane(Point plane_point_, Vector3D plane_normal_) { plane_point = plane_point_; plane_normal = plane_normal_; };
};

// Polygon (2D) [24 bytes Per Point]
struct Polygon {

    // Points
    std::vector<Point> points;  // in order around the border, all on one plane
    
    // Constructor
    Polygon(std::vector<Point> points_) { points = points_; };

    Vector3D polygonNormal() const;
    bool polygonIsValid() const;
    double polygonPerimeter() const;
    double polygonArea() const;
    std::vector<Segment> polygonSegments() const;
    bool polygonEnclosesPoint(const Point& point) const;
    double polygonBorderDistanceToPoint(const Point& point) const;
    Polygon polygonClip(const Plane& clipping_plane) const;  // plane normal points toward the retained part
};

// Polyhedron (3D) [24 bytes Per Point Per Polygon]
struct Polyhedron {

    // Faces
    std::vector<Polygon> polygons;  // each face is a planar polygon

    // Default Constructor
    Polyhedron(std::vector<Polygon> polygons_) { polygons = polygons_; };

    // Cube Constructor
    Polyhedron(Point center, double size) {
        double half_size = size * 0.5;

        Point ppp{center.x + half_size, center.y + half_size, center.z + half_size};
        Point ppm{center.x + half_size, center.y + half_size, center.z - half_size};
        Point pmp{center.x + half_size, center.y - half_size, center.z + half_size};
        Point pmm{center.x + half_size, center.y - half_size, center.z - half_size};
        Point mpp{center.x - half_size, center.y + half_size, center.z + half_size};
        Point mpm{center.x - half_size, center.y + half_size, center.z - half_size};
        Point mmp{center.x - half_size, center.y - half_size, center.z + half_size};
        Point mmm{center.x - half_size, center.y - half_size, center.z - half_size};

        std::vector<Point> face1_points = {ppp, pmp, pmm, ppm};
        std::vector<Point> face2_points = {mpp, mpm, mmm, mmp};
        std::vector<Point> face3_points = {ppp, ppm, mpm, mpp};
        std::vector<Point> face4_points = {pmp, mmp, mmm, pmm};
        std::vector<Point> face5_points = {ppp, mpp, mmp, pmp};
        std::vector<Point> face6_points = {ppm, pmm, mmm, mpm};

        polygons.push_back(Polygon(face1_points));
        polygons.push_back(Polygon(face2_points));
        polygons.push_back(Polygon(face3_points));
        polygons.push_back(Polygon(face4_points));
        polygons.push_back(Polygon(face5_points));
        polygons.push_back(Polygon(face6_points));
    };

    Polyhedron polyhedronClip(const Plane& clipping_plane);
    Polygon polyhedronCrossSection(const Plane& cross_section_plane) const;
};
