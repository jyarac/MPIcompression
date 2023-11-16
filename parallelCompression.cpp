
#include <mpi.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char** argv) {
    //opening and saving video frames
    cv::VideoCapture cap("video.mp4"); // open the video file
    if(!cap.isOpened()) { // check if we succeeded
        std::cout << "Could not open the video file" << std::endl;
        return -1;
    }

    cv::Mat frame;
    int frame_number = 0;
    while(true) {
        bool bSuccess = cap.read(frame); // read a new frame from video
        if (!bSuccess) { // if not success, break loop
            std::cout << "Cannot read the frame from video file" << std::endl;
            break;
        }

        std::string filename = "./input/frame" + std::to_string(frame_number) + ".jpg";
        cv::imwrite(filename, frame); // save frame to file

        frame_number++;
    }


    //start of parallel compression
    MPI_Init(&argc, &argv);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);





    // Finalize the MPI environment.
    MPI_Finalize();
}
