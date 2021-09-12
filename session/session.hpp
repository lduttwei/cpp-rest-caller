#ifndef SESSION_UTILITY_HPP
#define SESSION_UTILITY_HPP

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

const std::string session_path = "../session/session.json";

const std::string url = "http://localhost:8080";

//std::string token {};



const std::string Url();

const std::string Token();

const std::string Username();

json Session();

void write_session(std::string, bool, long, std::string);

void set_const_vars();

#endif
