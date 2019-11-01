#include <string>

class Point {
public:
    int x;
    int y;
    bool deletable = false;
    std::string initialized_time;

    Point(int x_value, int y_value, std::string initialized_time) {
        this -> x = x_value;
        this -> y = y_value;
        this -> initialized_time = initialized_time;
    }

    ~Point() {
        if (!this -> deletable)
            std::cout << "The point didn't logged yet\n";
    }

    std::string log_value() {
        std::string record = std::to_string(this -> x) + " " + std::to_string(this -> y) + " " + this -> initialized_time + "\n";
        make_deletable();
        return record;
    }

private:
    void make_deletable() {
        this -> deletable = true;
    }
};
