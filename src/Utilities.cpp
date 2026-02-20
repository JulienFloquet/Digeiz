#include "Utilities.h"

void resetDirectories(const std::vector<std::filesystem::path>& directories)
{
    std::error_code ec;
    for (const auto& directory : directories) 
    {
        fs::remove_all(directory, ec);
        if (ec) throw std::runtime_error("Cannot remove directory " + directory.string() + ": " + ec.message());

        fs::create_directories(directory, ec);
        if (ec) throw std::runtime_error("Cannot create directory " + directory.string() + ": " + ec.message());
    }  
}

VideoProperties  extractFrames(
    const std::string& videoPath,
    const std::string& outputDir,
    std::vector<cv::Mat>& framesColor,
    std::vector<std::string>& frameFiles)
{

    cv::VideoCapture cap(videoPath);
    if (!cap.isOpened()) { throw std::runtime_error("Cannot open video: " + videoPath); }
    
    VideoProperties imageProperties;
    imageProperties.fps    = cap.get(cv::CAP_PROP_FPS);
    imageProperties.width  = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    imageProperties.height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));

    cv::Mat frameColor;
    size_t frameCount = 0;

    while (cap.read(frameColor)) 
    {
        framesColor.push_back(frameColor.clone());

        // ---------  Save frames as images -----------------
        std::stringstream ss;
        ss << outputDir << "/frame_" << std::setfill('0') << std::setw(5) << frameCount << ".png";
        std::string filename = ss.str();
        cv::imwrite(filename, frameColor);
        frameFiles.push_back(filename);
        frameCount++;
        // --------------------------------------------------  
    }

    cap.release();
    return imageProperties;
}

void computeSSIMDistance(
    const std::vector<cv::Mat>& framesColor,
    std::vector<double>& ssimValues,
    double& sumSSIM,
    double& squareSumSSIM)
{
    cv::Mat img1 = framesColor[0];
    for (size_t  i = 0; i < framesColor.size()-1; i++) 
    {       
        cv::Mat img2 = framesColor[i+1];
        double ssim = getSSIM(img1, img2);
        ssimValues.push_back(ssim);
        sumSSIM += ssim;
        squareSumSSIM += ssim * ssim;
    }
}

void detectCorruptedFrames(
    std::vector<double>& ssimValues,
    const std::vector<std::string>& frameFiles,
    const double Threshold,
    const std::string& CorruptedFramesDir,
    std::vector<int>& corruptedIndices,
    std::vector<cv::Mat>& cleanFrames)
{
    size_t NumberFrames = frameFiles.size();

    for (auto i = 1; i < NumberFrames; i++)
    {
        if (ssimValues[i-1] < Threshold)
        {
            corruptedIndices.push_back(i);

            // ---------  Move corrupted frame -----------------
            std::string srcPath = frameFiles[i];
            std::string dstPath = CorruptedFramesDir + "/" + fs::path(srcPath).filename().string();
            try {
                fs::rename(srcPath, dstPath);
            } catch (const fs::filesystem_error& e) {
                std::cerr << "Error moving frame " << srcPath << ": " << e.what() << std::endl;
            }
            // --------------------------------------------------
        } else {
            cleanFrames.push_back(cv::imread(frameFiles[i]));
        }
    }
}

void buildCleanDistMatrix(
    const std::vector<cv::Mat>& cleanFrames,
    std::vector<std::vector<double>>& distMatrixSSIM_clean)
{
    size_t NumberCleanFrames = cleanFrames.size();

    #pragma omp parallel for schedule(dynamic)
    for (auto i = 0; i < NumberCleanFrames; ++i)
    {
        for (auto j = i + 1; j < NumberCleanFrames; ++j)
        {
            cv::Mat img1 = cleanFrames[i];
            cv::Mat img2 = cleanFrames[j];
            double ssim = getSSIM(img1, img2);
            double dist = 1.0 - ssim;
            distMatrixSSIM_clean[i][j] = dist;
            distMatrixSSIM_clean[j][i] = dist;
        }
        #pragma omp critical
        std::cout << " Progress : " << (i+1) << " frames" << std::endl;
    }
}
