#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include "user_parser.hpp"
#include "parse_utility.hpp"
#include "session.hpp"

using json = nlohmann::json;

namespace user
{
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

	void parse(int argc, const char** argv)
	{
		const char* arg = *argv;
		if ( cmp_arg(arg, "POST") )
		{
			*argv++;
			--argc;
			if ( argc < 2 )
			{
				std::cout << "name and password required" << '\n';;
			}	
			else
			{
				std::string username{*argv};
				*argv++;
				std::string password{*argv};
				json session = get_session();
				json j;
				j["password"] = password;
				j["username"] = username;

				std::string temp {"/users"};
				std::string url_finished = {getUrl() + temp};
				std::string auth_string {"Bearer " + getToken()};

				cpr::Header h{{"Content-Type","application/json"},{"Authorization", auth_string}};
				cpr::Response r = cpr::Post(
						cpr::Url{url_finished}, 
						h,
						cpr::Body{j.dump()}
						);
				if ( r.status_code >= 200 && r.status_code <= 299 )
				{
					json jr = json::parse(r.text);
					std::cout << "User " << username << " with ID " << jr["id"] << " created" << '\n'; 
				}	
				else
				{
					std::cout << "Something went wrong. Code: " << r.status_code << '\n';
				}

			}
		}
		else if ( cmp_arg(arg, "LIST") )
		{
			std::cout << "list " << '\n';
			std::string temp {"/users"};
			std::string url_finished = {getUrl() + temp};
			std::string auth_string {"Bearer " + getToken()};

			cpr::Header h{{"Content-Type","application/json"},{"Authorization", auth_string}};
			cpr::Response r = cpr::Get(
					cpr::Url{url_finished}, 
					h
					);

			if ( r.status_code >= 200 && r.status_code <= 299 )
			{
				json j = json::parse(r.text);
				std::cout << "#ID | Username" << '\n'; 
				std::for_each(j.begin(), j.end(), [](json temp) { 
						std::cout << temp["id"] << "|" << temp["username"] << '\n';
						});

			}
			else
			{
				std::cout << "Something went wrong. Code: " << r.status_code << '\n';
			}
		}
		else if ( cmp_arg(arg, "PUT") )
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
				*argv++;
				long id = 0;
				try {
					std::string username{*argv};
					*argv++;
					std::string password{*argv};
					id = std::stol(str_id, nullptr, 10); 
					json session = get_session();
					if ( id != session["id"] )
					{
						json j;
						j["username"] = username;
						j["password"] = password;
						j["id"] = id;
						std::string temp {"/users"};
						std::string url_finished = {getUrl() + temp};
						std::string auth_string {"Bearer " + getToken()};

						cpr::Header h{{"Content-Type","application/json"},{"Authorization", auth_string}};
						cpr::Response r = cpr::Put(
								cpr::Url{url_finished}, 
								h,
								cpr::Body{j.dump()}
								);

						if ( r.status_code >= 200 && r.status_code <= 299 )
						{
							json jr = json::parse(r.text);
							std::cout << "Updated user " << username << " with ID " << jr["id"] << '\n'; 
						}	
						else
						{
							std::cout << "Something went wrong. Code: " << r.status_code << '\n';
						}
					}
					else
					{
						std::cout << "You cant change an active user. Log in as a different user." << '\n';
					}
				} catch ( const std::invalid_argument& e )
				{
					std::cout << "User correct placement of arguments" << '\n';
					help();
				}

			}

		}
		else if ( cmp_arg(arg, "DELETE") )
		{
			*argv++;
			--argc;
			if ( argc == 0 )
			{
				std::cout << "id required" << '\n';;
			}	
			else
			{
				char prefix[] = "/users/";
				long id = 0;
				json session = get_session();
				try {
					std::string str_id{*argv};
					id = std::stol(str_id, nullptr, 10);
					if ( id != session["id"] )
					{
						std::string url_finished {getUrl() + prefix + *argv};
						std::string auth_string {"Bearer " + getToken()};

						cpr::Header h{{"Content-Type","application/json"},{"Authorization", auth_string}};
						cpr::Response r = cpr::Delete(
								cpr::Url{url_finished}, 
								h
								);

						if ( r.status_code >= 200 && r.status_code <= 299 )
						{
							std::cout << "Removed user with ID " << *argv << '\n'; 
						}	
						else
						{
							std::cout << "Something went wrong. Code: " << r.status_code << '\n';
						}

					}	
					else
					{
						std::cout << "You cant delete an active user. Log in as a different user." << '\n';
					}
				} catch ( const std::invalid_argument& e )
				{
					
					std::cout << "User correct placement of arguments" << '\n';
					help();
				}
			}
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
