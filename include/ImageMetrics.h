#ifndef IMAGE_METRICS_HPP
#define IMAGE_METRICS_HPP

#include <opencv2/opencv.hpp>

/**
* @brief Computes the Structural Similarity Index (SSIM) between two images.
*
* SSIM is defined as:
*
* SSIM(x,y) =
*   ((2 * μx * μy + C1) * (2 * σxy + C2)) /
*   ((μx² + μy² + C1) * (σx² + σy² + C2))
*
* where:
*   μx, μy   : local means
*   σx², σy² : local variances
*   σxy      : local covariance
*   C1, C2   : stability constants
*
* @param img1 First input image.
* @param img2 Second input image.
* @return SSIM similarity score in [-1, 1].
* 
* @note Both images must have identical dimensions and data type.
* @note ref : section Image similarity in https://docs.opencv.org/4.x/d5/dc4/tutorial_video_input_psnr_ssim.html#autotoc_md242
*/
double getSSIM(const cv::Mat& img1, const cv::Mat& img2);

#endif // IMAGE_METRICS_HPP  
