#include "session.hpp"

namespace session
{
	std::string active_username{};

	std::string active_token{};

	void create_session_file()
	{
		std::ofstream session_file{session_path};
		json session;	
		session["active"] = false;
		session["token"] = "";
		session["id"] = 0;
		session["username"] = "";
		session::active_token = "";
		session::active_username = "";
		session_file << session;
		session_file.close();
	}

	void handle_session_file()
	{
		std::ifstream s_file(session_path);
		if ( !s_file.good() )
		{
			create_session_file();	
		}
		s_file.close();
	}
}

const std::string Url()
{
	return url;
}

const std::string Username()
{
	return session::active_username;
}

void cut_token(std::string& t)
{
	t.erase(0,0);
	t.erase(t.size(),1);
}

const std::string Token()
{
	return session::active_token;
}

json Session()
{
	session::handle_session_file();
	std::ifstream s_file(session_path);
	json session = json::parse(s_file);
	return session;
}

void set_const_vars()
{
	auto session = Session();
	session::active_token = session["token"];
	cut_token(session::active_token);
	session::active_username = session["username"]; 
}

void write_session(std::string token, bool active, long id, std::string username)
{
	cut_token(token);
	json session = Session();;
	session["token"] = token;
	session["active"] = active;
	session["id"] = id;
	session["username"] = username;
	session::active_username = username;
	session::active_token = token;
	std::ofstream new_session(session_path);
	new_session << session;
}
