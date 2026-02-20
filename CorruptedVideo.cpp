#include "Utilities.h"
#include "VideoReconstruction.h"

/**
 * @brief Main pipeline for detecting corrupted video frames and reconstructing the video.
 * 
 * Steps performed:
 * 1. Reset or create output directories for frames and reconstructed video.
 * 2. Open the input video and extract all frames as images.
 * 3. Compute the SSIM distance matrix between all frames.
 * 4. Calculate mean and standard deviation of SSIM to define a corruption threshold.
 * 5. Detect corrupted frames, move them to a separate folder, and keep clean frames.
 * 6. Build a reduced SSIM matrix for clean frames only.
 * 7. Reconstruct the optimal frame order based on SSIM distances.
 * 8. Write the reconstructed video using the ordered clean frames.
 */
int main()
{
    std::string VideoInputName = "corrupted_video.mp4";
    std::string VideoOutputName = "video_reconstructed.mp4";

    std::string CorruptedVideoDir = "../../corrupted_video/";
    std::string OutputDir = "../../output/";
    std::string CorruptedFramesDir = OutputDir + "corrupted_frames";
    std::string OriginalFramesDir = OutputDir + "frames";
    std::string VideoReconstructedDir = OutputDir + "video_reconstructed/";

    resetDirectories({
        CorruptedFramesDir,
        OriginalFramesDir,
        VideoReconstructedDir
    });
    

    std::cout << " Opening video and extracting frames" << std::endl;
    std::vector<std::string> frameFiles;
    std::vector<cv::Mat> framesColor;
    VideoProperties imageProperties = extractFrames(
        CorruptedVideoDir + VideoInputName,
        OriginalFramesDir,
        framesColor, 
        frameFiles
    );

    
    size_t NumberFrames = framesColor.size();
    std::cout << " Number of frames: " << NumberFrames << std::endl;


    std::cout << " Computing SSIM distance" << std::endl;
    std::vector<double> ssimValues;
    double sumSSIM = 0.0, squareSumSSIM = 0.0;
    computeSSIMDistance(
        framesColor,
        ssimValues,
        sumSSIM,
        squareSumSSIM
    );
    
    
    double meanSSIM = sumSSIM / NumberFrames;
    double stdDevSSIM = std::sqrt((squareSumSSIM/NumberFrames) - meanSSIM*meanSSIM);
    std::cout << " SSIM mean: " << meanSSIM << ", std deviation: " << stdDevSSIM << std::endl;


    double Threshold = meanSSIM - stdDevSSIM;
    std::cout << " Threshold: " << Threshold << std::endl;


    std::cout << " Detect corrupted frames and separate clean frames" << std::endl;
    std::vector<int> corruptedIndices;
    std::vector<cv::Mat> cleanFrames; 
    detectCorruptedFrames(
        ssimValues,
        frameFiles,
        Threshold,
        CorruptedFramesDir,
        corruptedIndices,
        cleanFrames
    );


    size_t NumberCleanFrames = cleanFrames.size();
    std::cout << " Number of corrupted frames: " << corruptedIndices.size() << std::endl;    
    std::cout << " Number of clean frames: " << NumberCleanFrames << std::endl;


    std::cout << " Build distance matrix for clean frames only" << std::endl;
    std::vector<std::vector<double>> distMatrixSSIM_clean(NumberCleanFrames, std::vector<double>(NumberCleanFrames, 0.0));
    buildCleanDistMatrix(
        cleanFrames,
        distMatrixSSIM_clean
    );


    std::cout << " Reconstructing video with optimal frame order" << std::endl;
    std::vector<int> optimalOrderFrames = reconstructOrder(distMatrixSSIM_clean);


    std::cout << " Saving reconstructed video: " << VideoOutputName << std::endl;
    cv::VideoWriter writer(
        VideoReconstructedDir + VideoOutputName,
        cv::VideoWriter::fourcc('m','p','4','v'),
        imageProperties.fps,
        cv::Size(imageProperties.width, imageProperties.height)
    );


    for (int idx : optimalOrderFrames) 
        writer.write(cleanFrames[idx]);
    writer.release();

    return 0;
}