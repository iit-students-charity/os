#include <iostream>
#include <condition_variable>
#include <thread>
#include <fstream>

std::condition_variable cv;
std::mutex cv_m;
bool notified = false;
bool done = false;
int current_function_value = 1;
std::fstream function_result;

void init_result_file();

void write_function_result();

const char filename[] = "../results";


void result_listener() {
    init_result_file();
    std::unique_lock<std::mutex> lock(cv_m);

    while (!done) {
        while (notified) {
            notified = false;
            write_function_result();
        }
        cv.wait(lock);
    }
}

void write_function_result() {
    function_result.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);
    function_result << current_function_value << "\n";
    function_result.close();
}

void init_result_file() {
    function_result.open(filename,  std::fstream::in | std::fstream::out | std::fstream::trunc);
    function_result.close();
}

int factorial_calculation(int n) {
    for (int j = 1; j <= n; j++) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::unique_lock<std::mutex> lock(cv_m);

        current_function_value *= j;
        notified = true;
        cv.notify_one();
    }

    done = true;
    cv.notify_one();
}

int main() {
    std::thread factorial(factorial_calculation, rand() % 10);
    std::thread result(result_listener);

    factorial.join();
    result.join();

    std::cout << "complete";
    return 0;
}