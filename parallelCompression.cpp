
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
    int total_frames = 0;
    while(true) {
        bool bSuccess = cap.read(frame); // read a new frame from video
        if (!bSuccess) { // if not success, break loop
            std::cout << "Cannot read the frame from video file" << std::endl;
            break;
        }

        std::string filename = "./input/frame" + std::to_string(total_frames) + ".jpg";
        cv::imwrite(filename, frame); // save frame to file

        total_frames++;
    }

    //start of parallel compression
    MPI_Init(&argc, &argv);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

   // Calculate the number of frames each process should handle
    int frames_per_process = total_frames / world_size;
    // Calculate the start and end frame for this process
    int start_frame = world_rank * frames_per_process;
    int end_frame = (world_rank == world_size - 1) ? total_frames : start_frame + frames_per_process;

    // Process the frames
    for (int i = start_frame; i < end_frame; i++) {
        std::string filename = "./input/frame" + std::to_string(i) + ".jpg";
        cv::Mat frame = cv::imread(filename, cv::IMREAD_COLOR); // Read the file

        if (!frame.data) { // Check for invalid input
            std::cout << "Could not open or find the image" << std::endl;
            return -1;
        }
        cv::Mat resized_frame;
        cv::resize(frame, resized_frame, cv::Size(), 0.5, 0.5);
        std::string output_filename = "./output/frame" + std::to_string(i) + ".jpg";
        cv::imwrite(output_filename, resized_frame);

    }

    // Finalize the MPI environment.
    MPI_Finalize();
}
