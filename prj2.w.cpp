#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include "curl.h"
using namespace std;

size_t handle_data(void*c, size_t s, size_t n, void*p)
{
	*static_cast<string*>(p) += static_cast<char*>(c); // accumulation of the chunks returend from internet
	return s * n; // allow the libcurl to know if we are done with this callback
}

double f2c(double f)
{
	double c = (f - 32) * (0.555555555555555555555555555555);
	return c;
}

int main()
{
	//This retrieves data from internet

	char response = 'y';

while(response == 'y' || response == 'Y')
{
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
	

	//  This is to find the current temperature

	size_t pos_of_clear = page.find("+Clear+");
	size_t tempnow_ld = page.find("(", pos_of_clear);
	size_t tempnow_rd = page.find("+", tempnow_ld);
	string tempnow = page.substr(tempnow_ld + 1 , tempnow_rd - tempnow_ld - 1);
	
	if( tempnow.length() > 5) // quits the program if it doesn't find +Clear+ and does not continue.
	{
		cout<<"Error: Wrong zip code "<<endl;
		cout<<"Please enter a valid zip code: ";
		input = 0;
		cin>>input;
		ss.clear();
		ss.str("");
		ss << input;
	}
	else
	{
		cout<<"It is now "<<tempnow<<" degrees F"<<endl;
	}	

	//  This is to find High of the day
	size_t pos_of_later = page.find("Later%3A+");
	size_t high_ld = page.find("+(" , pos_of_later);
	size_t high_rd = page.find("+F" , high_ld);
	string high = page.substr(high_ld + 2 , high_rd - high_ld - 2);
	
	//  This is to find Low of the day
	size_t low_ld = page.find("%2F+" , pos_of_later);
	size_t low_rd = page.find("F)" , low_ld);
	string low = page.substr( low_ld + 4 , low_rd - low_ld - 5);
	cout<<"High: "<<high<<" degrees F \t"<<"Low: "<<low<<" degrees F"<<endl;
	
	// This is to convert strings of tempnow, high, low back to doubles
	double tnow = atof(tempnow.c_str());
	cout<<tnow<<" degrees F is about "<<fixed<<setprecision(1)<<f2c(tnow)<<" in degrees C."<<endl;
	double t_low = atof(low.c_str());
	double t_high = atof(high.c_str());
	// This is to display tempnow, high, low in Celsius from the global function

	cout<<"It is now "<<tnow<<" degrees F, which is about "<<fixed<<setprecision(1)<<f2c(tnow)<<" in degrees C."<<endl;
	cout<<"Today's high is "<<high<<" degrees F, which is about "<<fixed<<setprecision(1)<<f2c(t_high)<<" in degrees C."<<endl;
	cout<<"Today's low is "<<low<<" degrees F, which is about "<<fixed<<setprecision(1)<<f2c(t_low)<<" in degrees C."<<endl;

	// This finds the current humidity level
	size_t pos_of_pwsrtb = page.find("pwsrt b");
	size_t h_ld = page.find("value=" , pos_of_pwsrtb);
	size_t h_rd = page.find("><" , h_ld);
	string humidity_now = page.substr( h_ld + 7 , h_rd - h_ld - 8 );
	cout<<"The humidity level is now at "<<humidity_now<<" %"<<endl;


	cout<<"Enter another zip code? (Y/N): \t";
	cin>>response;
	if(response == 'y' || response == 'Y' )
	{
		zipcode = "";
		website = "";
		input = 0;
		system("cls");
		//cout<<"Enter the next zip code: ";
		//cin>>input;
		//ss << input;
		//zipcode = ss.str();
		//website = URL+zipcode;
	}
}
	return 0;
}
