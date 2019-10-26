#include <string>

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
