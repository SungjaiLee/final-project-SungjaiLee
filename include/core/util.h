//
// Created by Jack Lee on 2020/11/27.
//

#ifndef NONEUCLIDEAN_RAY_CASTER_UTIL_H
#define NONEUCLIDEAN_RAY_CASTER_UTIL_H

#include <cinder/gl/gl.h>

// TODO look into quaternions
namespace room_explorer {

// Numeric Utilities ===================================================================================
/**
 * Utilizes weak relative-difference approximation to confirm that two floats approximate each other.
 * @param a First float.
 * @param b Second float.
 * @param epsilon Relative threshold for difference.
 *                  The smaller the better approximation gets.
 *                  By default .0000005
 * @return If the two floats are good enough approximation of each other.
 */
bool FloatApproximation(float a, float b, float epsilon = .0000005f);
/**
 * Utilizes weak relative-difference approximation to confirm that two 2d vectors approximate each other.
 * @param vec_a First 2d vector.
 * @param vec_b Second 2d vector.
 * @param epsilon Relative threshold for difference.
 *                  The smaller the better approximation gets.
 *                  By default .0000005
 * @return If the two 2d vectors are good enough approximation of each other.
 */
bool FloatApproximation(const glm::vec2& vec_a, const glm::vec2& vec_b, float epsilon = .0000005f);
// end of Numeric Utilities

// Geometric Utilities ======================================================================================

bool IsUnitVector(const glm::vec2& vec);

/**
 * Confirms if three points are collinear.
 * If 2 or more of these points are the same point, considered to be collinear.
 * @param point_a First point.
 * @param point_b Second point.
 * @param point_c Third point.
 * @return True if three points are collinear, or 2 or more of the points occupy the same point.
 */
bool AreCollinear(const glm::vec2& point_a, const glm::vec2& point_b, const glm::vec2& point_c);

/**
 * Confirms if two vectors are parallel.
 * Direction irrelevant.
 * Zero-vector parallel with every vector.
 * @param vec_a First vector.
 * @param vec_b Second vector.
 * @return True if the vectors are parallel.
 *          Zero vectors are always parallel with any vectors.
 */
bool AreParallel(const glm::vec2& vec_a, const glm::vec2& vec_b);

/**
 * Retrieve pure distance required for ray to intersect with a line.
 *      Distance can be negative if ray points towards the opposite direction.
 * In case of point-line, where head and tail occupy same point,
 *      assume line is defined as passing through the ray-position and thus distance is 0.
 *      Ray-direction may not be normalized, but must be non-zero.
 * @param line_head Head of line.
 * @param line_tail Tail of line. Can be same point as the head.
 * @param ray_pos Position from which the ray begins.
 * @param ray_dir Direction of the ray.
 *                  Can be not yet normalized.
 *                  Must not be zero-vector.
 * @return Positive or negative distance required for ray to each the line.
 *         If never intersecting, numerical limit infinity is returned.
 */
float GetRayToLineDistance(const glm::vec2& line_head, const glm::vec2& line_tail,
                           const glm::vec2& ray_pos, const glm::vec2& ray_dir);

/**
 * Confirms that ray intersects with segment defined by two end-points.
 *      This means that ray hits the finite segment of line between the two end points.
 *      Ray beginning on the segment, of course, is considered to intersect.
 *      Collinear ray is also considered to intersect with the segment.
 *      Ray must intersect in positive direction.
 *      Point segment is allowed, but ray must directly hit this single point for intersection.
 * @param segment_head Head end-point og the segment.
 * @param segment_tail Tail end-point of the
 * @param ray_pos Position from which the ray begins.
 * @param ray_dir Direction of the ray.
 *                  Can be not yet normalized.
 *                  Must not be zero-vector.
 * @return True if and only if ray intersects eventually with the segment in positive direction.
 */
bool RayIntersectsWithSegment(const glm::vec2& segment_head, const glm::vec2& segment_tail,
                              const glm::vec2& ray_pos, const glm::vec2& ray_dir,
                              bool parallel_hit_valid = true);

/**
 * Retrieves texture-index that ray intercepts from a segment.
 *      Texture index calculated from head of the segment.
 * Texture index can be negative.
 *      Post-processing of validity of the index not considered.
 *      Ray can be intercepting the segment in the negative direction.
 * If ray and segment collinear,
 *      index is 0 if ray points towards head,
 *      index is pure distance between ray and head.
 * Point wall always assumed to be on line from ray-pos to the point.
 * @param line_head Head of line.
 * @param line_tail Tail of line. Can be same point as the head.
 * @param ray_pos Position from which the ray begins.
 * @param ray_dir Direction of the ray.
 *                  Can be not yet normalized.
 *                  Must not be zero-vector.
 * @return Negative or positive index of texture of segment.
 *          Interpreted as distance of intersection from segment head.
 */
float TextureIndexOnLineOfRay(const glm::vec2& line_head, const glm::vec2& line_tail,
                              const glm::vec2& ray_pos, const glm::vec2& ray_dir);

/**
 * Rotate vector utilizing sine and cosine values pre-calculated.
 * Coseina dn Sine must
 * @param vec Vector to be rotated. Reference is passed and will be altered.
 * @param rotation_cos Cosine of angle of rotation.
 * @param rotation_sin Sine of angle of rotation.
 * @return Returns vector rotated by the appropriate angle.
 *          Vector will be normalized afterwards, if improper sine and cosine were given.
 */
glm::vec2& FastRotate(glm::vec2& vec, float rotation_cos, float rotation_sin);

// end of Geometric Utilities ==========================================================================================

}

#endif //NONEUCLIDEAN_RAY_CASTER_UTIL_H
