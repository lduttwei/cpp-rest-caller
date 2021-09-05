#include <iostream>
#include <cpr/cpr.h>
#include <main.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

const std::string url = "http://localhost:8080/entries/";


namespace rest 
{
	cpr::Response Get()
	{
		return {};
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

void print_response(cpr::Response r)
{
	std::cout << r.status_code << '\n';;                  // 200
	//std::cout << r.header["content-type"] << '\n';       // application/json; charset=utf-8
	//std::cout << r.text << '\n';   
	std::cout << r.url << '\n';
}



int main(int argc, char** argv)
{
	if ( argc > 1 )
	{
		char* id = argv[1];
		auto r = rest::Get(id);
		auto j = json::parse((r.text));
		j.erase("id");
		r = rest::Post(j);
		print_response(r);
	}
	return 0;
}
