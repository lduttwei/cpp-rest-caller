#include <iostream>
#include <cpr/cpr.h>
#include <main.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

const std::string url = "http://localhost:8080/entries/";


namespace rest 
{
	cpr::Response Get()
	{
		cpr::Response r = cpr::Get(cpr::Url{url});
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


int main(int argc, char** argv)
{
	char* date = argv[1];
	char* time = argv[2];
	auto res = parse_date_time(date, time);
	std::cout << res << '\n';

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
