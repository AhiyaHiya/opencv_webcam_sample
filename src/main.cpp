/**
 * Jaime O. Rios
 * 2019-06-16
 *
 * Sample app to connect to the first available Web camera and display a preview window.
 * */

#include <iostream>
#include <memory>
#include <string>

#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>

using namespace std::string_literals;

enum running_state { rs_unknown = -1, rs_stopped = 0, rs_running = 1, rs_stopping = 2 };

static const auto window_name   = "Webcam preview"s;
static auto       running_state = rs_unknown;

/** Creates the shared_ptr for the video capture session we are using in this sample application */
auto create_video_capture_session(const int32_t width, const int32_t height)
    -> std::shared_ptr< cv::VideoCapture > {
    // zero based camera id, and we are attempting to connect to the first available one
    const auto camera_id     = 0;
    auto       video_capture = std::make_shared< cv::VideoCapture >(camera_id);

    video_capture->set(CV_CAP_PROP_FRAME_WIDTH, width);
    video_capture->set(CV_CAP_PROP_FRAME_HEIGHT, height);

    return video_capture;
}

/** Creates a window and pins it to be the front most window */
void create_window(const int32_t x, const int32_t y, const int32_t width, const int32_t height) {
    cv::namedWindow(window_name, CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED);
    // CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED | CV_WINDOW_FRONTMOST);
    cv::moveWindow(window_name, x, y);
    cv::resizeWindow(window_name, width, height);
}

/** This functions runs a loop that displays the images to the preview window */
void run_camera(std::shared_ptr< cv::VideoCapture > video_session,
                const int32_t                       width,
                const int32_t                       height) {
    while (running_state == rs_running) {
        cv::Mat frame;
        (*video_session) >> frame;

        if (frame.empty())
            break; // end of video stream

        cv::Mat resized;
        cv::resize(frame, resized, cv::Size(width, height));
        cv::imshow(_windowName.c_str(), resized);

        // stop capturing by pressing ESC
        if (cv::waitKey(10) == 27) {
            running_state = rs_stopping;
            break;
        }
    }

    running_state = rs_stopped;
}

int main(int argc, char* argv[]) {

    // The mechanism we use for getting the images off of the Webcam preview window
    const auto width  = 340;
    const auto height = 240;

    auto video_capture = create_video_capture_session(width, height);
    create_window(10, 10, width, height);
    run_camera(video_capture, width, height));

    std::cout << "App shutting down" << std::endl;
}