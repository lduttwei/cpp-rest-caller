//Library
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

//Standard-Library
#include <iostream>
#include <fstream>

//Project-Files
#include <main.hpp>
#include "project_parser.hpp"
#include "parse_utility.hpp"
#include "session.hpp"

using json = nlohmann::json;

cpr::Response register_user(const char* prefix, const std::string username, const std::string password)
{
	json j;
	j["username"] = username;
	j["password"] = password;
	std::string url_finished = {getUrl() + prefix};
	cpr::Response r = cpr::Post(cpr::Url{url_finished},
			cpr::Header{{"Content-Type", "application/json"}},
			cpr::Body{j.dump()}
		     );
	return r;
}

cpr::Response login_user(const char* prefix, const std::string username, const std::string password)
{
	json j;
	j["username"] = username;
	j["password"] = password;
	std::string url_finished = {getUrl() + prefix};
	std::string token {};
	cpr::Response r = cpr::Post(cpr::Url{url_finished},
			cpr::Header{{"Content-Type", "application/json"}},
			cpr::Body{j.dump()}
		     );
	return r;
}

void handle_login(const std::string username, const std::string password)
{
	json session = get_session();
	auto r = login_user("/auth/login", username, password);
	if ( r.status_code == 200 )
	{
		json jr = json::parse(r.text);
		std::string token = jr["token"];
		token.erase(0,0);
		token.erase(token.size(),1);
		write_session(token, true, jr["id"], username);
		std::cout << "Logged in: " << username << '\n';
	}
	else if ( r.status_code == 400 )
	{
		std::cout << "Invalid user credentials" << '\n';
	}
	else
	{
		std::cout << "Something went wrong" << '\n';
	}
}

void handle_register(const std::string username, const std::string password)
{
	json session = get_session();
	auto r = register_user("/users", username, password);
	if ( r.status_code == 200 )
	{
		handle_login(username, password);
		std::cout << "Created user and logged in: " << username << '\n';
	}
	else if ( r.status_code == 400 )
	{
		std::cout << "Invalid user credentials" << '\n';
	}
	else
	{
		std::cout << "Something went wrong" << '\n';
	}
}

void user_login()
{
	std::cout << "Login" << '\n';
	std::string username {};
	std::string password {};
	std::cout << "Username:";
	std::cin >> username;
	std::cout << "Password:";
	std::cin >> password;
	handle_login(username, password); 
}

void user_signup()
{
	std::cout << "Register" << '\n';
	std::string username {};
	std::string password {};
	std::cout << "Username:";
	std::cin >> username;
	std::cout << "Password:";
	std::cin >> password;
	handle_register(username, password); 
}

void not_authenticated_parse(int argc, const char** argv)
{
	argc--;
	*argv++;
	if ( argc > 0 )
	{
		const char* arg = *argv; 
		if ( cmp_arg(arg, "login") )
		{
			user_login();
		}
		else if ( cmp_arg(arg, "register") )
		{
			user_signup();
		} 
	}
	else
	{
		std::cout << "HELP" << '\n';
	}

}

void parse(int argc, const char** argv)
{
	argc--;
	if ( argc > 0 )
	{
		*argv++;
		const char* arg = *argv; 
		if ( cmp_arg(arg, "entry") )
		{
			std::cout << "Entry: " << arg << '\n';
		}
		else if ( cmp_arg(arg, "user") )
		{
			std::cout << "User: " << arg << '\n';
		} 
		else if ( cmp_arg(arg, "project") ) 	
		{
			parse_project(argc, argv);
		} 
		else if ( cmp_arg(arg, "place") ) 	
		{
			std::cout << "Place: " << arg << '\n';
		}
		else if ( cmp_arg(arg, "logout") ) 	
		{
			write_session("", false, 0, "");
		}
	}
	else
	{
		std::cout << 
			"Usage: [ENTITY] [ACTION]\n" <<
			"entities:               \n" << 
			"	project          \n"  
			;
	}
}

int main(int argc, const char** argv)
{
	json prev_session = get_session();
	if ( !prev_session["active"] )
	{
		std::cout << "Currently not logged in" << '\n';
		not_authenticated_parse(argc, argv);
	}
	else
	{
		std::cout << "Currently logged in as " << prev_session["username"] << '\n';
		parse(argc, argv);
	}
	return 0;
}
