#include "Utils.h"

string getCurrentTimestamp() {
    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);
    tm *now_tm = localtime(&now_time);

    ostringstream oss;
    oss << put_time(now_tm, "%d:%m:%Y-%H:%M:%S");
    return oss.str();
}

std::string doubleToString(double value) {
    ostringstream oss;
    oss << fixed << setprecision(6) << value;

    string str = oss.str();
    str.erase(str.find_last_not_of('0') + 1, string::npos);
    if (str.back() == '.') str.pop_back();
    return str;
}
