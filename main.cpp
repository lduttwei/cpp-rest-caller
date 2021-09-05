#include <iostream>
#include <cpr/cpr.h>
#include <main.hpp>

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
		std::cout << r.text << '\n';
		print_response(r);
		r = rest::Delete(id);
		std::cout << r.text << '\n';
		print_response(r);
		r = rest::Get(id);
		std::cout << r.text << '\n';
		print_response(r);
	}
	return 0;
}
