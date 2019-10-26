#include <iostream>
#include <condition_variable>
#include <thread>
#include <fstream>
#include <string>

std::condition_variable cv;
std::mutex cv_m;
bool notified = false;
bool done = false;
int current_function_value = 1;
std::fstream function_result;

const char filename[] = "../results";

void init_file(char filename[]) {
    function_result.open(filename,  std::fstream::in | std::fstream::out | std::fstream::trunc);
    function_result.close();
}

void log_writer() {
    const char filename[] = "../log";

    init_file(const_cast<char *>(filename));
}

void write_function_result(char filename[]) {
    function_result.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);
    function_result << current_function_value << "\n";
    function_result.close();
}

void result_listener() {
    const char filename[] = "../results";

    init_file(const_cast<char *>(filename));

    std::unique_lock<std::mutex> lock(cv_m);

    while (!done) {
        while (notified) {
            notified = false;
            write_function_result(const_cast<char *>(filename));
        }
        cv.wait(lock);
    }
}

int factorial_calculation(int n) {
    for (int j = 1; j <= n; j++) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::unique_lock<std::mutex> lock(cv_m);

        current_function_value *= j;
        notified = true;
        cv.notify_all();
    }

    done = true;
    cv.notify_all();
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