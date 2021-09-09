//Library
#include <cpr/cpr.h>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <nlohmann/json.hpp>

//Standard-Library
#include <iostream>
#include <fstream>

//Project-Files
#include <main.hpp>
#include "project_parser.hpp"

using json = nlohmann::json;

const std::string url = "http://localhost:8080";


namespace rest 
{
	cpr::Response Get()
	{
		std::string url_finished = {url + "/users"};
		cpr::Response r = cpr::Get(cpr::Url{url_finished});
		return r; 
	}

	cpr::Response Get(char* id)
	{
		std::string url_finished = {url + id};
		cpr::Response r = cpr::Get(cpr::Url{url_finished},
			     cpr::Authentication{"user", "pass"});
		return r; 
	}

	cpr::Response Delete(char* id)
	{
		std::string url_finished = {url + id};
		return cpr::Delete(cpr::Url{url_finished});
	}

	cpr::Response Post(const json j)
	{
		return cpr::Post(cpr::Url{url}, 
				cpr::Body{j.dump()},
				cpr::Header{{"Content-Type", "text/plain"}}
				);
	}
}

void login_or_register(const char* prefix, const std::string username, const std::string password)
{
	json j;
	j["username"] = username;
	j["password"] = password;
	std::string url_finished = {url + prefix};
	std::string token {};
	cpr::Response r = cpr::Post(cpr::Url{url_finished},
			cpr::Header{{"Content-Type", "application/json"}},
			cpr::Body{j.dump()}
		     );
	std::cout << r.text << '\n';
	std::cout << r.status_code << '\n';
	std::cout << r.url << '\n';
}

void print_response(cpr::Response r)
{
	std::cout << r.status_code << '\n';;                  // 200
	//std::cout << r.header["content-type"] << '\n';       // application/json; charset=utf-8
	std::cout << r.text << '\n';   
	std::cout << r.url << '\n';
}

std::string parse_date_time(char* date, char* time)
{
	boost::gregorian::date d = boost::gregorian::from_string({date});
	std::string res {to_iso_extended_string(d) + "T" + time + "Z"};
	return res;
}

int cmp_arg(const char* arg, const char* cmp)
{
	return strcmp(arg, cmp) == 0;
}

void parse(int argc, const char** argv)
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
}

int main(int argc, const char** argv)
{
	std::string username {};
	std::string password {};
	std::cout << "Username:";
	std::cin >> username;
	std::cout << "Password:";
	std::cin >> password;
	login_or_register("/auth/login", username, password); 
	auto r = rest::Get();
	std::cout << r.text << '\n';
	//std::ifstream s_file("sesson.json");
	//json session = json::parse(s_file);
	//std::cout << session["id"] << '\n';
	//char* date = argv[1];
	//char* time = argv[2];
	//auto res = parse_date_time(date, time);
	//std::cout << res << '\n';

	//parse(argc, argv);

	//auto r = rest::Get();
	//print_response(r);
	//auto j = json::parse((r.text));
	//j.erase("id");
	//time_t now;
	//time(&now);
	//char buf[sizeof "2011-10-08T07:08:09Z"];
	//strftime(buf, sizeof buf, "%FT%TZ", gmtime(&now));
	//json j;
	//j["chrono"]={buf};
	//std::cout << j.dump() << '\n';
	//r = rest::Post(j);
	//print_response(r);
	return 0;
}
