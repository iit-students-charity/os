#include <iostream>
#include <condition_variable>
#include <thread>
#include <fstream>
#include <string>
#include <ctime>

std::condition_variable cv;
std::mutex cv_m;
bool notified_result_listener = false;
bool notified_logger = false;
bool done = false;
int current_function_value = 1;
int current_argument_value = 1;
std::fstream function_result;


class Point {
public:
    int x;
    int y;
    std::string initialized_time;

     Point(int x_value, int y_value, std::string initialized_time) {
        this -> x = x_value;
        this -> y = y_value;
        this -> initialized_time = initialized_time;
     }

     std::string log_value() {
         return std::to_string(this -> x) + " " + std::to_string(this -> y) + " " + this -> initialized_time + "\n";
     }
};


void init_file(char filename[]) {
    function_result.open(filename,  std::fstream::in | std::fstream::out | std::fstream::trunc);
    function_result.close();
}

void write_function_result(char filename[], std::string result) {
    function_result.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);
    function_result << result << "\n";
    function_result.close();
}

void result_listener() {
    const char filename[] = "../results";

    init_file(const_cast<char *>(filename));

    std::unique_lock<std::mutex> lock(cv_m);

    while (!done) {
        while (notified_result_listener) {
            notified_result_listener = false;
            write_function_result(const_cast<char *>(filename), std::to_string(current_function_value));

            notified_logger = true;
            cv.notify_all();
        }
        cv.wait(lock);
    }
}

int factorial_calculation(int n) {
    for (current_argument_value; current_argument_value <= n; current_argument_value++) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::unique_lock<std::mutex> lock(cv_m);

        current_function_value *= current_argument_value;

        notified_result_listener = true;
        cv.notify_all();
    }

    done = true;
    cv.notify_all();
}

void log_writer() {
    const char filename[] = "../log";
    init_file(const_cast<char *>(filename));

    std::unique_lock<std::mutex> lock(cv_m);

    while (!done) {
        while (notified_logger) {
            notified_logger = false;
            time_t now = time(0);
            std::string initialized_time = ctime(&now);

            Point current_point(current_argument_value, current_function_value, initialized_time);
            write_function_result(const_cast<char *>(filename), current_point.log_value());
        }
        cv.wait(lock);
    }
}

int main() {
    std::thread factorial(factorial_calculation, rand() % 10);
    std::thread result(result_listener);
    std::thread log(log_writer);

    factorial.join();
    result.join();
    log.join();

    std::cout << "complete";
    return 0;
}