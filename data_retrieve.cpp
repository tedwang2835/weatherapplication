#include <iostream>
#include <sstream>
#include "curl.h"
using namespace std;

size_t handle_data(void*c, size_t s, size_t n, void*p)
{
	*static_cast<string*>(p) += static_cast<char*>(c); // accumulation of the chunks returend from internet
	return s * n; // allow the libcurl to know if we are done with this callback
}

void data_retrieval()
{

	//This retrieves data from internet

	int input;
	cout<<"Please enter a zip code: ";
	cin>>input;
	stringstream ss;
	ss << input;

	if((ss.str()).length() < 5)  // if user enters a zip code that is not 5 characters,prompts the user again to re-enter. 
	{
		cout<<"Error, please re-enter a zip code: ";
		input = 0;
		cin>>input;
		ss.clear();
		ss.str("");
		ss << input;
	}
	string zipcode = ss.str();
	cout<<"String of zipcode: "<<zipcode<<endl;
	const string URL = "http://www.wunderground.com/cgi-bin/findweather/getForecast?query=";
	//cout<<URL<<endl;
	string website = URL+zipcode;
	cout<<"website: "<<website<<endl;
	string page;  // initalizes automatically to ""
	CURL* curl = curl_easy_init(); // a "handle" or "pointer" into the library
	
	if(curl) // check to make sure if the handle is valid
	{
		curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,handle_data);
		curl_easy_setopt(curl,CURLOPT_WRITEDATA,&page);
		curl_easy_setopt(curl,CURLOPT_URL,website.c_str());
		CURLcode result = curl_easy_perform(curl);
		if(result==0)
		{
			cout<<"access successful"<<endl;
		}
		else
		{
			cout<<"Access failed. Please try again later."<<endl;
			curl_easy_cleanup(curl);
		}
	}
	else
	{
	cout<<"Error in curl handle"<<endl;
	exit(1); // failure to get exchange rate, so quit program
	}
}
