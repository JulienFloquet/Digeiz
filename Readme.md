
# Corrupted Video Reconstruction Pipeline

This program processes a potentially corrupted video and reconstructs it by analyzing frame similarities. It performs the following sequence of operations:

First, it ensures that all necessary output directories exist and are clean. Directories are created for storing extracted frames, corrupted frames, and the final reconstructed video. Any existing content in these directories is removed to avoid conflicts with previous runs.

Next, the program opens the input video and extracts each frame as an image. Both color frames and the corresponding file paths are stored for later processing. This allows the program to work with individual frames while keeping a reference to their original positions in the video.

Once all frames are extracted, the program computes the Structural Similarity Index (SSIM) distance matrix between all pairs of frames. This matrix provides a quantitative measure of similarity between frames, helping to identify frames that differ significantly from the rest.

Using the SSIM distance matrix, the program calculates the mean and standard deviation of SSIM values. These statistics are used to define a threshold for detecting corrupted frames. Frames whose average SSIM distance exceeds this threshold are considered corrupted.

Corrupted frames are then moved to a separate folder to keep them distinct from the clean frames. The program records the indices of both corrupted and clean frames, allowing subsequent processing to focus only on reliable frames.

A reduced SSIM distance matrix is created for the clean frames only. This smaller matrix preserves the similarity relationships among clean frames, enabling accurate reconstruction without interference from corrupted frames.

Based on the reduced SSIM matrix, the program determines the optimal order of the clean frames to reconstruct the video. This step ensures that the video sequence is as close as possible to the original, preserving the temporal continuity.

Finally, the program writes the reconstructed video by sequentially adding frames in the computed optimal order. The output video is saved to the specified directory, completing the reconstruction process.

This pipeline allows automated detection and handling of corrupted frames while producing a coherent reconstructed video, suitable for further analysis or viewing.


## Building 

Method for Windows with MinGW64 compiler

 - Create build folder:
cd path_to\DIGEIZ
mkdir build
cd build

 - To create Debug solution :

cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=C:/msys64/mingw64/bin/gcc.exe -DCMAKE_CXX_COMPILER=C:/msys64/mingw64/bin/g++.exe

 - To create Release solution :

cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=C:/msys64/mingw64/bin/gcc.exe -DCMAKE_CXX_COMPILER=C:/msys64/mingw64/bin/g++.exe

 - Now for debug or release you go in the respective folder and you build with :

mingw32-make

 - Inside the folder you obtain the file to execute the program :

CorruptionVideo.exe


## Execution

 - Before execution put a video in folder "corrupted_video/" , example "myVideo.p4"
 - Replace the name of the video in l.19 of CorruptedVideo.cpp
 - Change the name of the final product as you prefer in l.20

 N.B. in the future you will put the video names as inputs of the executable


## Output

In folder "output/" :
 - "frames/" contains the frames not corrupted 
 - "corrupted_frames/" contains the others
 - "video_reconstructed/" contains the final product