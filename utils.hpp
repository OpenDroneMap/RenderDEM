#ifndef UTILS_H
#define UTILS_H

static inline std::vector<std::string> split(const std::string &s, const std::string &delimiter){
    size_t posStart = 0, posEnd, delimLen = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((posEnd = s.find(delimiter, posStart)) != std::string::npos) {
        token = s.substr(posStart, posEnd - posStart);
        posStart = posEnd + delimLen;
        res.push_back(token);
    }

    res.push_back(s.substr(posStart));
    return res;
}

static inline std::vector<double> parseCSV(const std::string &s){
    std::vector<double> res;
    const auto values = split(s, ",");
    for (const std::string &v : values){
        res.push_back(std::stod(v));
    }
    return res;
}

#endif