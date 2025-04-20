#include "Utils.h"

#include <iomanip>
#include <chrono>

string getCurrentTimestamp() {
    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);
    tm *now_tm = localtime(&now_time);

    ostringstream oss;
    oss << put_time(now_tm, "%d-%m_%H-%M-%S");
    return oss.str();
}

string doubleToString(double value) {
    ostringstream oss;
    oss << fixed << value;
    string str = oss.str();

    str.erase(str.find_last_not_of('0') + 1);
    if (str.back() == '.') str.pop_back();
    return str;
}
