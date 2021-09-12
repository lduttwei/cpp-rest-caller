#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include "user_parser.hpp"
#include "../utility/parse_utility.hpp"
#include "../session/session.hpp"

using json = nlohmann::json;

namespace user
{

	const char prefix[] = "/users/";

	void answer(long code, std::string good)
	{
		if ( code >= 200 && code <= 299 )
		{
			std::cout << good << '\n';
		}	
		else if ( code == 0 )
		{
			std::cout << "You cant change an active user. Log in as a different user." << '\n';
		}
		else
		{
			std::cout << "Something went wrong. Code: " << code << '\n';
		}
	}

	int convert_id(const char** argv)
	{

			std::string str_id{*argv};
			long id = 0;
			id = std::stol(str_id, nullptr, 10); 
			return id;
	}

	void help()
	{

		std::cout << 
			"Usage: [ENTITY] [ACTION] \n" <<
			"actions for user:     \n" << 
			"	LIST              \n"  
			"	POST   {name} {password}     \n"  
			"	PUT    {id} {name} {password}\n"  
			"	DELETE {id} \n"  
			;
	}

	cpr::Response post_request(std::string username, std::string password)
	{
		json session = Session();
		json j;
		j["password"] = password;
		j["username"] = username;

		std::string url_finished = {Url() + prefix};
		std::string auth_string {"Bearer " + Token()};

		cpr::Response r = cpr::Post(
				cpr::Url{url_finished}, 
				cpr::Header{{"Content-Type","application/json"},{"Authorization", auth_string}},
				cpr::Body{j.dump()}
				);
		return r;
		return r;
	}

	long put_request(long id, std::string username, std::string password)
	{
		json session = Session();
		if ( id != session["id"] )
		{
			json j;
			j["username"] = username;
			j["password"] = password;
			j["id"] = id;
			std::string url_finished = {Url() + prefix};
			std::string auth_string {"Bearer " + Token()};

			cpr::Response r = cpr::Put(
					cpr::Url{url_finished}, 
					cpr::Header{{"Content-Type","application/json"},{"Authorization", auth_string}},
					cpr::Body{j.dump()}
					);
			return r.status_code;
		}
		return 0;
	}	

	long delete_request(long id)
	{
		json session = Session();
		if ( id != session["id"] )
		{
			std::string url_finished {Url() + prefix + std::to_string(id)};
			std::string auth_string {"Bearer " + Token()};

			cpr::Response r = cpr::Delete(
					cpr::Url{url_finished}, 
					cpr::Header{{"Content-Type","application/json"},{"Authorization", auth_string}}
					);
			return r.status_code;
		}
		return 0;
	}

	void post_user(int argc, const char** argv)
	{
		--argc;
		if ( argc < 2 )
		{
			std::cout << "name and password required" << '\n';;
		}	
		else
		{
			*argv++;
			std::string username{*argv};
			*argv++;
			std::string password{*argv};
			auto response = post_request(username, password);
			long code = response.status_code;
			if ( code >= 200 && code <= 299 )  
			{
				json jr = json::parse(response.text);
				answer(code, {"User: " + username + ", ID: " + to_string(jr["id"]) + " created"});
			}
			else
			{
				answer(code, "");
			}
		}
	}

	void list_user(int argc, const char** argv)
	{
		std::string url_finished = {Url() + prefix};
		std::string auth_string {"Bearer " + Token()};

		cpr::Response r = cpr::Get(
				cpr::Url{url_finished}, 
				cpr::Header{{"Content-Type","application/json"},{"Authorization", auth_string}}
				);

		if ( r.status_code >= 200 && r.status_code <= 299 )
		{
			json j = json::parse(r.text);
			std::cout << "#ID | Username" << '\n'; 
			std::for_each(j.begin(), j.end(), [](json temp) { 
					std::cout << temp["id"] << "|" <<temp["username"] << '\n';
					});
		}
		else
		{
			std::cout << "Something went wrong. Code: " << r.status_code << '\n';
		}
	}

	void put_user(int argc, const char** argv)
	{

		*argv++;
		--argc;
		if ( argc < 2 )
		{
			std::cout << "id, name and password required" << '\n';
		}	
		else
		{
			std::string str_id{*argv};
			long id = 0;
			try {
				id = convert_id(argv);
				*argv++;
				std::string username{*argv};
				*argv++;
				std::string password{*argv};
				long code = put_request(id, username, password);
				std::string reply{"Updated user: " + username + ", ID: " + std::to_string(id)};
				answer(code, reply);
			} catch ( const std::invalid_argument& e )
			{
				std::cout << "ID has to be a number" << '\n';
			}
		}
	}	

	void delete_user(int argc, const char** argv)
	{
		*argv++;
		--argc;
		if ( argc == 0 )
		{
			std::cout << "id required" << '\n';;
		}	
		else
		{
			long id = 0;
			try
			{
				id = convert_id(argv);
				long code = delete_request(id);
				std::string reply{"Removed user, ID: " + std::to_string(id)};
				answer(code, reply);
			}
			catch ( const std::invalid_argument& e )
			{
				std::cout << "ID has to be a number" << '\n';
			}
		}
	}

	void parse(int argc, const char** argv)
	{
		const char* arg = *argv;
		if ( cmp_arg(arg, "POST") )
		{
			user::post_user(argc, argv);
		}
		else if ( cmp_arg(arg, "LIST") )
		{
			user::list_user(argc, argv);
		}
		else if ( cmp_arg(arg, "PUT") )
		{
			user::put_user(argc, argv);
		}
		else if ( cmp_arg(arg, "DELETE") )
		{
			user::delete_user(argc, argv);
		}
		else
		{
			help();
		}
	}
}

void parse_user(int argc, const char** argv)
{
	--argc;
	*argv++;
	if ( argc > 0 )
	{
		user::parse(argc, argv);
	}
	else
	{
		user::help();
	}
}
