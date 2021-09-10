#include "session.hpp"

const std::string url = "http://localhost:8080";
std::string token {};

const std::string getUrl()
{
	return url;
}

std::string getToken()
{
	std::ifstream s_file(session_path);
	json session = json::parse(s_file);
	std::string t{session["token"]}; 
	t.erase(0,0);
	t.erase(t.size(),1);
	return t;
}

json get_session()
{
	std::ifstream s_file(session_path);
	json session = json::parse(s_file);
	return session;
}

void write_session(std::string token, bool active, long id, std::string username)
{
	json session = get_session();;
	session["token"] = token;
	session["active"] = active;
	session["id"] = id;
	session["username"] = username;
	std::ofstream new_session(session_path);
	new_session << session;
}
