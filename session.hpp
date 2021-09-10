#ifndef SESSION_UTILITY_HPP
#define SESSION_UTILITY_HPP

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

const std::string session_path = "session.json";

const std::string getUrl();
std::string getToken();

json get_session();

void write_session(std::string, bool, long, std::string);
#endif
