//Library
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

//Standard-Library
#include <iostream>
#include <fstream>

//Project-Files
#include <main.hpp>
#include "project/project_parser.hpp"
#include "user/user_parser.hpp"
#include "utility/parse_utility.hpp"
#include "session/session.hpp"

using json = nlohmann::json;

void help()
{
		std::cout << 
			"Usage: [ENTITY] [ACTION]\n" <<
			"entities:               \n" << 
			"	project          \n"  
			"	user          \n"  
			"	logout           \n"  
			;
}

cpr::Response register_user(const char* prefix, const std::string username, const std::string password)
{
	json j;
	j["username"] = username;
	j["password"] = password;
	std::string url_finished = {Url() + prefix};
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
	std::string url_finished = {Url() + prefix};
	std::string token {};
	cpr::Response r = cpr::Post(cpr::Url{url_finished},
			cpr::Header{{"Content-Type", "application/json"}},
			cpr::Body{j.dump()}
		     );
	return r;
}

void handle_register_answer(long code)
{
	if ( code == 200 )
	{
		std::cout << "New user:" << Username() << '\n';
	}
	else if ( code == 400 )
	{
		std::cout << "Invalid user credentials" << '\n';
	}
	else
	{
		std::cout << "Something went wrong" << '\n';
	}
}

void handle_login_answer(long code)
{
	if ( code == 200 )
	{
		std::cout << "User: " << Username() << '\n';
	}
	else if ( code >= 400 && code <= 499 )
	{
		std::cout << "Invalid user credentials" << '\n';
	}
	else
	{
		std::cout << "Something went wrong" << '\n';
	}
}

long try_login(const std::string username, const std::string password)
{
	json session = Session();
	auto r = login_user("/auth/login", username, password);
	if ( r.status_code == 200 )
	{
		json jr = json::parse(r.text);
		std::string token = jr["token"];
		write_session(token, true, jr["id"], username);
	}
	return r.status_code;
}

long try_register(const std::string username, const std::string password)
{
	json session = Session();
	auto r = register_user("/users", username, password);
	if ( r.status_code == 200 )
	{
		return try_login(username, password);
	}
	return r.status_code;
}

std::pair<std::string, std::string> get_user_credentials()
{
	std::string username {};
	std::string password {};
	std::cout << "Username:";
	std::cin >> username;
	std::cout << "Password:";
	std::cin >> password;
	return {username, password};
}

void user_login()
{
	std::cout << "Login" << '\n';
	auto [username, password] = get_user_credentials();
	long code = try_login(username, password); 
	handle_login_answer(code);
}

void user_signup()
{
	std::cout << "Register" << '\n';
	auto [username, password] = get_user_credentials();
	long code = try_register(username, password);
	handle_register_answer(code); 
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
		std::cout << 
			"Usage: [OPTION]\n" <<
			"OPTIONS:               \n" << 
			"	login             \n"  
			"	register          \n"  

			;
	}

}

void start_mssg()
{
	json prev_session = Session();
	if ( !prev_session["active"] )
	{
		std::cout << "Currently not logged in" << '\n';
	}
	else
	{
		std::cout << "Currently logged in as " << prev_session["username"] << '\n';
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
			start_mssg();
			std::cout << "Entry: " << arg << '\n';
		}
		else if ( cmp_arg(arg, "user") )
		{
			start_mssg();
			parse_user(argc, argv);
		} 
		else if ( cmp_arg(arg, "project") ) 	
		{
			start_mssg();
			parse_project(argc, argv);
		} 
		else if ( cmp_arg(arg, "place") ) 	
		{
			start_mssg();
			std::cout << "Place: " << arg << '\n';
		}
		else if ( cmp_arg(arg, "logout") ) 	
		{
			write_session("", false, 0, "");
			std::cout << "now logged out" << '\n';
		}
		else
		{
			help();
		}
	}
	else
	{
		help();
	}
}

int main(int argc, const char** argv)
{
	json prev_session = Session();
	if ( !prev_session["active"] )
	{
		not_authenticated_parse(argc, argv);
	}
	else
	{
		set_const_vars();
		parse(argc, argv);
	}
	return 0;
}
