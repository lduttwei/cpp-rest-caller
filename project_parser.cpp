#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include "project_parser.hpp"
#include "parse_utility.hpp"
#include "session.hpp"

using json = nlohmann::json;

namespace project
{
	void help()
	{

		std::cout << 
			"Usage: [ENTITY] [ACTION] \n" <<
			"actions for project:     \n" << 
			"	LIST              \n"  
			"	POST   {name}     \n"  
			"	PUT    {id} {name}\n"  
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
			if ( argc == 0 )
			{
				std::cout << "name required" << '\n';;
			}	
			else
			{
				std::string name{*argv};
				json session = get_session();
				json j;
				j["name"] = name;
				std::string temp {"/projects"};
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
					std::cout << "Project " << name << " with ID " << jr["id"] << '\n'; 
				}	
				else
				{
					std::cout << "Something went wrong. Code: " << r.status_code << '\n';
				}
			}
		}
		else if ( cmp_arg(arg, "LIST") )
		{
			std::string name{*argv};
			std::string temp {"/projects"};
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
				std::cout << "#ID | Name" << '\n'; 
				std::for_each(j.begin(), j.end(), [](json temp) { 
						std::cout << temp["id"] << "|" << temp["name"] << '\n';
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
				std::cout << "id and name required" << '\n';;
			}	
			else
			{
				std::string str_id{*argv};
				*argv++;
				long id = 0;
				try {
					std::string name{*argv};
					id = std::stol(str_id, nullptr, 10); 
					json session = get_session();
					json j;
					j["id"] = id;
					j["name"] = name;
					std::string temp {"/projects"};
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
						std::cout << "Updated project: " << name << ", ID " << jr["id"] << '\n'; 
					}	
					else
					{
						std::cout << "Something went wrong. Code: " << r.status_code << '\n';
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
				char prefix[] = "/projects/";
				std::string url_finished {getUrl() + prefix + *argv};
				std::string auth_string {"Bearer " + getToken()};

				cpr::Header h{{"Content-Type","application/json"},{"Authorization", auth_string}};
				cpr::Response r = cpr::Delete(
						cpr::Url{url_finished}, 
						h
						);

				if ( r.status_code >= 200 && r.status_code <= 299 )
				{
					std::cout << "Removed project with ID " << *argv << '\n'; 
				}	
				else
				{
					std::cout << "Something went wrong. Code: " << r.status_code << '\n';
				}

			}
		}
		else
		{
			project::help();
		}
	}
}

void parse_project(int argc, const char** argv)
{
	--argc;
	*argv++;
	if ( argc > 0 )
	{
		project::parse(argc, argv);
	}
	else
	{
		project::help();
	}
}
