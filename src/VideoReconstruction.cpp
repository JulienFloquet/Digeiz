#include "VideoReconstruction.h"

std::vector<int> reconstructOrder(const std::vector<std::vector<double>>& distMatrix)
{
    size_t NumberFrames = distMatrix.size();
    std::vector<bool> frameUsed(NumberFrames, false);
    std::vector<int> segment;

    // Finding pair of frames with minimal distance
    double minDist = 1e9;
    size_t start = 0, end = 1;

    for (auto i = 0; i < NumberFrames; i++) 
    {
        for (auto j = i+1; j < NumberFrames; j++) 
        {
            if (distMatrix[i][j] < minDist) 
            {
                minDist = distMatrix[i][j];
                start = i;
                end = j;
            }
        }
    }

    segment.push_back(start);
    segment.push_back(end);
    frameUsed[start] = frameUsed[end] = true;

    // Progressive extension of the segment adding before or after the closest frames
    while (segment.size() < NumberFrames)
    {
        double bestDist = 1e9;
        int bestImage = -1;
        bool insertAtFront = false;

        int currentStart = segment.front();
        int currentEnd = segment.back();

        for (auto index = 0; index < NumberFrames; index++) 
        {
            if (!frameUsed[index]) 
            {
                double dStart = distMatrix[index][currentStart];
                double dEnd = distMatrix[index][currentEnd];

                if (dStart < bestDist) 
                {
                    bestDist = dStart;
                    bestImage = index;
                    insertAtFront = true;
                }

                if (dEnd < bestDist) 
                {
                    bestDist = dEnd;
                    bestImage = index;
                    insertAtFront = false;
                }
            }
        }

        if (insertAtFront)
            segment.insert(segment.begin(), bestImage);
        else
            segment.push_back(bestImage);

        frameUsed[bestImage] = true;
    }

    return segment;
}
