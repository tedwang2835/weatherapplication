#include <iostream>
#include <sstream>
#include "curl.h"
using namespace std;

size_t handle_data(void*c, size_t s, size_t n, void*p)
{
	*static_cast<string*>(p) += static_cast<char*>(c); // accumulation of the chunks returend from internet
	return s * n; // allow the libcurl to know if we are done with this callback
}

