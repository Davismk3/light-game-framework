#include "geometry.hpp"

namespace {

    // Clamp Value: Returns value limited to [lower, upper]. lower <= upper.
    template <typename T> T clampValue(const T& value, const T& lower, const T& upper) {
        return std::max(lower, std::min(value, upper));
    }
}

// Point Distance To Point: other_point: any point.
double Point::pointDistanceToPoint(const Point& other_point) const {
    return Vector3D{x - other_point.x, y - other_point.y, z - other_point.z}.vectorMagnitude();
}

// Point Vector To Point: Returns the vector from this point to other_point. other_point: any point.
Vector3D Point::pointVectorToPoint(const Point& other_point) const {
    return {other_point.x - x, other_point.y - y, other_point.z - z};
}

// Segment Length: Returns the distance between the endpoints.
double Segment::segmentLength() const {
    return point_1.pointDistanceToPoint(point_2);
}

// Segment Midpoint: Returns the point halfway between the endpoints.
Point Segment::segmetMidpoint() const {
    return {
        (point_1.x + point_2.x) / 2.0,
        (point_1.y + point_2.y) / 2.0,
        (point_1.z + point_2.z) / 2.0
    };
}

// Point On Segment: True if point_3 is on the segment's line (cross product below 1e-8) and between its endpoints. point_3: any point.
bool Segment::pointOnSegment(const Point& point_3) const {
    double eps = 1e-8;
    Vector3D vect_1 = point_1.pointVectorToPoint(point_2);
    Vector3D vect_2 = point_1.pointVectorToPoint(point_3);
    Vector3D cross_product = crossProduct(vect_1, vect_2);
    bool on_line = (cross_product.vectorMagnitude() < eps);
    if (!on_line) {
        return false;
    }
    bool on_segment = (
        std::min(point_1.x, point_2.x) <= point_3.x && point_3.x <= std::max(point_1.x, point_2.x) &&
        std::min(point_1.y, point_2.y) <= point_3.y && point_3.y <= std::max(point_1.y, point_2.y) &&
        std::min(point_1.z, point_2.z) <= point_3.z && point_3.z <= std::max(point_1.z, point_2.z)
    );
    if (on_segment) {
        return true;
    }
    return false;
}

// Segment Intersects Segment: True if the closest points of the two segments are within 1e-8. other_segment: any segment, in 3D.
bool Segment::segmentIntersectsSegment(const Segment& other_segment) const {
    constexpr double eps = 1e-8;
    constexpr double eps_squared = eps * eps;

    Vector3D direction_1 = point_1.pointVectorToPoint(point_2);
    Vector3D direction_2 = other_segment.point_1.pointVectorToPoint(other_segment.point_2);
    Vector3D offset = other_segment.point_1.pointVectorToPoint(point_1);

    double a = dotProduct(direction_1, direction_1);
    double b = dotProduct(direction_1, direction_2);
    double c = dotProduct(direction_1, offset);
    double e = dotProduct(direction_2, direction_2);
    double f = dotProduct(direction_2, offset);
    double s = 0.0;
    double t = 0.0;

    if (a <= eps_squared && e <= eps_squared) {
        return point_1.pointDistanceToPoint(other_segment.point_1) <= eps;
    }

    if (a <= eps_squared) {
        t = clampValue(f / e, 0.0, 1.0);
    } else if (e <= eps_squared) {
        s = clampValue(-c / a, 0.0, 1.0);
    } else {
        double denominator = a * e - b * b;
        if (std::abs(denominator) > eps_squared) {
            s = clampValue((b * f - c * e) / denominator, 0.0, 1.0);
        }
        t = (b * s + f) / e;
        if (t < 0.0) {
            t = 0.0;
            s = clampValue(-c / a, 0.0, 1.0);
        } else if (t > 1.0) {
            t = 1.0;
            s = clampValue((b - c) / a, 0.0, 1.0);
        }
    }
    Point closest_1{
        point_1.x + s * direction_1.x,
        point_1.y + s * direction_1.y,
        point_1.z + s * direction_1.z
    };
    Point closest_2{
        other_segment.point_1.x + t * direction_2.x,
        other_segment.point_1.y + t * direction_2.y,
        other_segment.point_1.z + t * direction_2.z
    };
    return closest_1.pointDistanceToPoint(closest_2) <= eps;
}

// Segment Distance To Point: Returns the distance from point_3 to the nearest point on this segment. point_3: any point.
double Segment::segmentDistanceToPoint(const Point& point_3) const {
    double eps = 1e-12;
    Vector3D vect_1 = point_1.pointVectorToPoint(point_2);
    Vector3D vect_2 = point_1.pointVectorToPoint(point_3);
    double numerator = dotProduct(vect_2, vect_1);
    double denominator = dotProduct(vect_1, vect_1);
    if (denominator <= eps * eps) {
        return point_1.pointDistanceToPoint(point_3);
    }
    double t = std::max(0.0, std::min(1.0, numerator / denominator));

    Point segment_point = {
        point_1.x + t * (point_2.x - point_1.x),
        point_1.y + t * (point_2.y - point_1.y),
        point_1.z + t * (point_2.z - point_1.z)
    };

    return segment_point.pointDistanceToPoint(point_3);
}

// Nearest Point On This Segment: Returns the point on this segment nearest to other_point. other_point: any point.
Point Segment::nearestPointOnThisSegment(const Point& other_point) {
    double eps = 1e-12;
    Vector3D vect_1 = point_1.pointVectorToPoint(point_2);
    Vector3D vect_2 = point_1.pointVectorToPoint(other_point);
    double numerator = dotProduct(vect_2, vect_1);
    double denominator = dotProduct(vect_1, vect_1);
    if (denominator <= eps * eps) {
        return point_1;
    }
    double t = std::max(0.0, std::min(1.0, numerator / denominator));

    Point segment_point = {
        point_1.x + t * (point_2.x - point_1.x),
        point_1.y + t * (point_2.y - point_1.y),
        point_1.z + t * (point_2.z - point_1.z)
    };
    return segment_point;
}

// Polygon Normal: Returns the unit normal from the first three points, or {0, 0, 0} if there are fewer than 3 or they are in a line.
Vector3D Polygon::polygonNormal() const {
    double eps = 1e-8;

    if (points.size() < 3) {
        return {0.0, 0.0, 0.0};
    }

    Vector3D vect_1 = points[0].pointVectorToPoint(points[1]);
    Vector3D vect_2 = points[0].pointVectorToPoint(points[2]);
    Vector3D normal = crossProduct(vect_1, vect_2);
    double normal_length = normal.vectorMagnitude();

    if (normal_length < eps) {
        return {0.0, 0.0, 0.0};
    }
    return {normal.x / normal_length, normal.y / normal_length, normal.z / normal_length};
}

// Polygon Is Valid: True if there are at least 3 points, the first three are not in a line, and every point is on their plane within 1e-8.
bool Polygon::polygonIsValid() const {
    double eps = 1e-8;
    Vector3D norm = polygonNormal();

    if (points.size() < 3) {
        return false;
    }

    if (norm.vectorMagnitude() < eps) {
        return false;
    }

    for (std::size_t i = 3; i < points.size(); ++i) {
        Vector3D vect_3 = points[0].pointVectorToPoint(points[i]);
        double error = std::abs(dotProduct(norm, vect_3));
        if (error > eps) {
            return false;
        }
    }
    return true;
}

// Polygon Perimeter: Returns the length around the closed border. With 2 points, returns their distance.
double Polygon::polygonPerimeter() const {
    double perim = 0.0;

    if (points.size() == 1) {
        return perim;
    }

    if (points.size() == 2) {
        Point point_1 = points[0];
        Point point_2 = points[1];
        return point_1.pointDistanceToPoint(point_2);
    }

    for (std::size_t i = 0; i < points.size(); ++i) {
        const Point& point_a = points[i];
        const Point& point_b = points[(i + 1) % points.size()];

        perim += point_a.pointDistanceToPoint(point_b);
    }
    return perim;
}

// Polygon Area: Returns 0 for fewer than 3 points. points: planar, in order around the border.
double Polygon::polygonArea() const {
    if (points.size() < 3) {
        return 0.0;
    }
    Vector3D area_vector{0.0, 0.0, 0.0};
    for (std::size_t i = 0; i < points.size(); ++i) {
        const Point& point_a = points[i];
        const Point& point_b = points[(i + 1) % points.size()];

        area_vector.x += (point_a.y - point_b.y) * (point_a.z + point_b.z);
        area_vector.y += (point_a.z - point_b.z) * (point_a.x + point_b.x);
        area_vector.z += (point_a.x - point_b.x) * (point_a.y + point_b.y);
    }
    return 0.5 * area_vector.vectorMagnitude();
}

// Polygon Segments: Returns each border edge, including the one from the last point back to the first. With 2 points, returns one segment.
std::vector<Segment> Polygon::polygonSegments() const {
    std::vector<Segment> segments = {};

    if (points.size() <= 1) {
        return segments;
    }

    if (points.size() == 2) {
        Segment segment = {points[0], points[1]};
        segments.push_back(segment);
        return segments;
    }

    for (std::size_t i = 0; i < points.size(); ++i) {
        const Point& point_a = points[i];
        const Point& point_b = points[(i + 1) % points.size()];
        Segment segment = {point_a, point_b};
        segments.push_back(segment);
    }
    return segments;
}

// Polygon Encloses Point: True if a ray from the point crosses the border an odd number of times. point: on the polygon's plane, within 1e-8.
bool Polygon::polygonEnclosesPoint(const Point& point) const {
    double inf = 1e12;
    double eps = 1e-8;

    if (points.size() < 3) {
        return false;
    }

    Vector3D vect_1 = points[0].pointVectorToPoint(point);
    Vector3D vect_2 = polygonNormal();

    if (std::abs(dotProduct(vect_1, vect_2)) > eps) {
        return false;
    }

    Vector3D helper_axis = {0.0, 0.0, 1.0};
    if (std::abs(vect_2.x) <= std::abs(vect_2.y) && std::abs(vect_2.x) <= std::abs(vect_2.z)) {
        helper_axis = {1.0, 0.0, 0.0};
    } else if (std::abs(vect_2.y) <= std::abs(vect_2.z)) {
        helper_axis = {0.0, 1.0, 0.0};
    }

    Vector3D ray_direction = crossProduct(vect_2, helper_axis);
    Segment ref_ray = {point, {point.x + inf * ray_direction.x, point.y + inf * ray_direction.y, point.z + inf * ray_direction.z}};

    std::vector<Segment> polygon_segments = polygonSegments();
    int intersection_count = 0;
    for (const Segment& polygon_segment : polygon_segments) {
        if (ref_ray.segmentIntersectsSegment(polygon_segment)) {
            ++intersection_count;
        }
    }
    return intersection_count % 2 != 0;
}

// Polygon Border Distance To Point: Returns the distance from the point to the nearest border edge. point: any point. The polygon must have at least 2 points.
double Polygon::polygonBorderDistanceToPoint(const Point& point) const {
    std::vector<Segment> segments = polygonSegments();
    std::vector<double> distances = {};
    for (std::size_t i = 0; i < segments.size(); ++i) {
        distances.push_back(segments[i].segmentDistanceToPoint(point));
    }
    return *std::min_element(distances.begin(), distances.end());
}

// Polygon Clip: Returns the part of the polygon on the kept side. It may have fewer than 3 points. clipping_plane: its normal points toward the kept side.
Polygon Polygon::polygonClip(const Plane& clipping_plane) const {
    double eps = 1e-12;
    std::vector<Point> new_points = {};

    auto _signedDistance = [&](const Point& point) {
        Vector3D dist_vect = clipping_plane.plane_point.pointVectorToPoint(point);
        return dotProduct(dist_vect, clipping_plane.plane_normal);
    };
    auto _inside = [&](const Point& point) {
        return _signedDistance(point) >= -eps;
    };
    auto _intersection = [&](const Point& point_a, const Point& point_b) {
        double dist_a = _signedDistance(point_a);
        double dist_b = _signedDistance(point_b);
        double t = dist_a / (dist_a - dist_b);
        Point intersection_point = {
            point_a.x + t * (point_b.x - point_a.x),
            point_a.y + t * (point_b.y - point_a.y),
            point_a.z + t * (point_b.z - point_a.z)
        };
        return intersection_point;
    };
    auto _dedupeSequentialPoints = [&](const std::vector<Point>& points_to_dedupe) {
        std::vector<Point> deduped_points = {};
        if (points_to_dedupe.size() == 0) {
            return deduped_points;
        }
        for (std::size_t i = 0; i < points_to_dedupe.size(); ++i) {
            if (deduped_points.size() == 0 || points_to_dedupe[i].pointDistanceToPoint(deduped_points.back()) > eps) {
                deduped_points.push_back(points_to_dedupe[i]);
            }
        }
        if (deduped_points.size() > 1 && deduped_points[0].pointDistanceToPoint(deduped_points.back()) <= eps) {
            deduped_points.pop_back();
        }
        return deduped_points;
    };

    for (std::size_t i = 0; i < points.size(); ++i) {
        Point point_a = points[i];
        Point point_b = points[(i + 1) % points.size()];
        bool point_a_inside = _inside(point_a);
        bool point_b_inside = _inside(point_b);

        if (point_a_inside && point_b_inside) {
            new_points.push_back(point_b);
        } else if (point_a_inside && !point_b_inside) {
            Point cap_point = _intersection(point_a, point_b);
            new_points.push_back(cap_point);
        } else if (!point_a_inside && point_b_inside) {
            Point cap_point = _intersection(point_a, point_b);
            new_points.push_back(cap_point);
            new_points.push_back(point_b);
        }
    }
    new_points = _dedupeSequentialPoints(new_points);
    return Polygon{new_points};
}

// Polyhedron Clip: Returns the part on the kept side, closed with a new cap face where it was cut. clipping_plane: its normal points toward the kept side. The polyhedron must be convex.
Polyhedron Polyhedron::polyhedronClip(const Plane& clipping_plane) {
    constexpr double eps = 1e-8;
    std::vector<Polygon> clipped_polygons;
    std::vector<Point> cap_points;

    auto signedDistance = [&](const Point& point) {
        return dotProduct(clipping_plane.plane_point.pointVectorToPoint(point), clipping_plane.plane_normal);
    };
    auto intersection = [&](const Point& a, const Point& b, double da, double db) {
        double t = da / (da - db);
        return Point{a.x + t * (b.x - a.x), a.y + t * (b.y - a.y), a.z + t * (b.z - a.z)};
    };
    auto dedupeSequential = [&](std::vector<Point>& points) {
        std::vector<Point> deduped;
        for (const Point& point : points) {
            if (deduped.empty() || point.pointDistanceToPoint(deduped.back()) > eps) {
                deduped.push_back(point);
            }
        }
        if (deduped.size() > 1 && deduped.front().pointDistanceToPoint(deduped.back()) <= eps) {
            deduped.pop_back();
        }
        points = std::move(deduped);
    };

    for (const Polygon& polygon : polygons) {
        std::vector<Point> face_points;
        const std::vector<Point>& points = polygon.points;
        for (std::size_t j = 0; j < points.size(); ++j) {
            const Point& a = points[j];
            const Point& b = points[(j + 1) % points.size()];
            double da = signedDistance(a);
            double db = signedDistance(b);
            bool a_inside = da >= -eps;
            bool b_inside = db >= -eps;

            if (a_inside && b_inside) {
                face_points.push_back(b);
            } else if (a_inside && !b_inside) {
                Point cut = intersection(a, b, da, db);
                face_points.push_back(cut);
                cap_points.push_back(cut);
            } else if (!a_inside && b_inside) {
                Point cut = intersection(a, b, da, db);
                face_points.push_back(cut);
                face_points.push_back(b);
                cap_points.push_back(cut);
            }
        }
        dedupeSequential(face_points);
        if (face_points.size() >= 3) clipped_polygons.emplace_back(face_points);
    }

    std::vector<Point> unique_cap_points;
    for (const Point& point : cap_points) {
        bool duplicate = false;
        for (const Point& existing : unique_cap_points) {
            if (point.pointDistanceToPoint(existing) <= eps) {
                duplicate = true;
                break;
            }
        }
        if (!duplicate) unique_cap_points.push_back(point);
    }

    if (unique_cap_points.size() >= 3) {
        Point center{0.0, 0.0, 0.0};
        for (const Point& point : unique_cap_points) {
            center.x += point.x;
            center.y += point.y;
            center.z += point.z;
        }
        center.x /= unique_cap_points.size();
        center.y /= unique_cap_points.size();
        center.z /= unique_cap_points.size();

        Vector3D normal = clipping_plane.plane_normal.vectorNormalized();
        Vector3D reference = std::abs(normal.x) < 0.9 ? Vector3D{1.0, 0.0, 0.0} : Vector3D{0.0, 1.0, 0.0};
        Vector3D u = crossProduct(reference, normal).vectorNormalized();
        Vector3D v = crossProduct(normal, u);
        std::sort(unique_cap_points.begin(), unique_cap_points.end(), [&](const Point& a, const Point& b) {
            Vector3D da = center.pointVectorToPoint(a);
            Vector3D db = center.pointVectorToPoint(b);
            return std::atan2(dotProduct(da, v), dotProduct(da, u)) >
                   std::atan2(dotProduct(db, v), dotProduct(db, u));
        });
        clipped_polygons.emplace_back(unique_cap_points);
    }

    return Polyhedron{clipped_polygons};
}

// Polyhedron Cross Section: Returns the polygon where the plane cuts the polyhedron, with points sorted around its center. It has fewer than 3 points if the plane misses. cross_section_plane: any plane. The polyhedron must be convex.
Polygon Polyhedron::polyhedronCrossSection(const Plane& cross_section_plane) const {
    std::vector<Point> intersection_points = {};
    Point plane_point = cross_section_plane.plane_point;
    Vector3D plane_normal = cross_section_plane.plane_normal;
    constexpr double eps = 1e-8;
    auto addUnique = [&](const Point& point) {
        for (const Point& existing : intersection_points) {
            if (point.pointDistanceToPoint(existing) <= eps) return;
        }
        intersection_points.push_back(point);
    };
    for (std::size_t i = 0; i < polygons.size(); ++i) {
        Polygon polygon = polygons[i];
        std::vector<Segment> segments = polygon.polygonSegments();
        for (std::size_t j = 0; j < segments.size(); ++j) {
            Segment segment = segments[j];
            Point point_1 = segment.point_1;
            Point point_2 = segment.point_2;
            double distance_a = dotProduct(plane_point.pointVectorToPoint(point_1), plane_normal);
            double distance_b = dotProduct(plane_point.pointVectorToPoint(point_2), plane_normal);

            if (std::abs(distance_a) <= eps) addUnique(point_1);
            if (distance_a * distance_b < 0.0) {
                double t = distance_a / (distance_a - distance_b);
                addUnique(Point{
                    point_1.x + t * (point_2.x - point_1.x),
                    point_1.y + t * (point_2.y - point_1.y),
                    point_1.z + t * (point_2.z - point_1.z)
                });
            }
        }
    }
    if (intersection_points.size() < 3) return Polygon{intersection_points};

    Point center{0.0, 0.0, 0.0};
    for (const Point& point : intersection_points) {
        center.x += point.x;
        center.y += point.y;
        center.z += point.z;
    }
    center.x /= intersection_points.size();
    center.y /= intersection_points.size();
    center.z /= intersection_points.size();

    Vector3D normal = plane_normal.vectorNormalized();
    Vector3D reference = std::abs(normal.x) < 0.9 ? Vector3D{1.0, 0.0, 0.0} : Vector3D{0.0, 1.0, 0.0};
    Vector3D u = crossProduct(reference, normal).vectorNormalized();
    Vector3D v = crossProduct(normal, u);
    std::sort(intersection_points.begin(), intersection_points.end(), [&](const Point& a, const Point& b) {
        Vector3D da = center.pointVectorToPoint(a);
        Vector3D db = center.pointVectorToPoint(b);
        return std::atan2(dotProduct(da, v), dotProduct(da, u)) <
               std::atan2(dotProduct(db, v), dotProduct(db, u));
    });
    return Polygon{intersection_points};
}
