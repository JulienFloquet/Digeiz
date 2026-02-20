#ifndef VIDEO_RECONSTRUCTION_HPP
#define VIDEO_RECONSTRUCTION_HPP

#include <vector>

/**
 * @brief Reconstructs an ordered sequence of image indices using a greedy strategy.
 *
 * This function reconstructs an image ordering that minimizes pairwise
 * distances progressively. The algorithm works as follows:
 *
 * 1. It finds the pair of images with the smallest distance in the
 *    distance matrix and initializes the segment with this pair.
 * 2. It iteratively adds the closest unused image to either the
 *    beginning or the end of the current segment.
 * 3. The process continues until all images are included.
 *
 *
 * @param distMatrix A symmetric NxN matrix containing pairwise distances
 *                   between images. distMatrix[i][j] represents the
 *                   distance between image i and image j.
 *
 * @return std::std::vector<int> A std::vector of image indices representing
 *                          the reconstructed ordering.
 */
std::vector<int> reconstructOrder(const std::vector<std::vector<double>>& distMatrix);

#endif // VIDEO_RECONSTRUCTION_HPP  
