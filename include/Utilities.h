#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <filesystem>
#include <vector>
#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>

#include "ImageMetrics.h"

namespace fs = std::filesystem;

/**
 * @brief Holds basic video properties needed for reconstruction.
 */
struct VideoProperties {
    double fps;
    int width;
    int height;
};

/**
 * @brief Remove all given directories and recreate them empty.
 * 
 * @param directories Vector of directories to reset.
 */
void resetDirectories(const std::vector<std::filesystem::path>& directories);

/**
 * @brief Extract frames from a video and save color frames.
 * 
 * @param videoPath Path to the input video.
 * @param outputDir Directory to save color frames.
 * @param framesColor Output vector storing color frames.
 * @param frameFiles Output vector storing saved frame filenames.
 * @return VideoProperties Struct containing FPS, width, and height.
 */
VideoProperties  extractFrames(
    const std::string& videoPath,
    const std::string& outputDir,
    std::vector<cv::Mat>& framesColor,
    std::vector<std::string>& frameFiles);

/**
 * @brief Compute the SSIM distance matrix for a set of color frames.
 * 
 * @param framesColor Input vector of frames.
 * @param ssimValues Output symmetric distance vector.
 * @param sumSSIM Output sum of all SSIM values.
 * @param squareSumSSIM Output sum of squares of all SSIM values.
 */
void computeSSIMDistance(
    const std::vector<cv::Mat>& framesColor,
    std::vector<double>& ssimValues,
    double& sumSSIM,
    double& squareSumSSIM);

/**
 * @brief Detect corrupted frames based on a SSIM distance threshold and move them to a separate folder.
 * 
 * @param ssimValues Input SSIM distance.
 * @param frameFiles Input vector of frame file paths.
 * @param Threshold Threshold value above which a frame is considered corrupted.
 * @param CorruptedFramesDir Directory where corrupted frames are moved.
 * @param corruptedIndices Output vector storing indices of corrupted frames.
 * @param cleanIndices Output vector storing indices of clean frames.
 */
void detectCorruptedFrames(
    std::vector<double>& ssimValues,
    const std::vector<std::string>& frameFiles,
    const double Threshold,
    const std::string& CorruptedFramesDir,
    std::vector<int>& corruptedIndices,
    std::vector<cv::Mat>& cleanFrames);

/**
 * @brief Build a reduced SSIM distance matrix containing only the clean frames.
 * 
 * @param cleanFrames Vector of clean frames.
 * @param distMatrixSSIM_clean Output reduced distance matrix for clean frames only.
 */
void buildCleanDistMatrix(
    const std::vector<cv::Mat>& cleanFrames,
    std::vector<std::vector<double>>& distMatrixSSIM_clean);

#endif // UTILITIES_HPP
