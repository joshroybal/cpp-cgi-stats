#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <ctime>
#include "statistics.hpp"

const int MAX_BYTES = 1000000;

void printReport(const Population&, const Sample&);
void printTable(int [], float, float, float, float);
static std::string floattoString(float, unsigned);

int main()
{
	float t1, t2, t3, t4;
	Population population;
	// Sample sample;
	char data[MAX_BYTES];
	char* pd = NULL;
	std::vector<float> numeric_list;

	int length = atoi(getenv("CONTENT_LENGTH"));
	if (length >= MAX_BYTES) return 1;	// that takes care of that
	// std::cout << "Content-Type:text/html;charset=iso-8859-1\n\n";
   std::cout << "Content-Type:text/html;charset=utf-8\n\n";
	t1 = clock();
	fread(data, length, 1, stdin);
	/* if we tear the data buffer about with this strtok call our list of
	numbers will be the 3rd element of the tokenization  -the empirical 
	determination of the foregoing was painful */
	pd = strtok(data, "\"=,");
	int count = 0;
	while (pd != NULL) {   
		++count;
		if (count == 3) {
			std::stringstream ss;
			ss << pd;

    		do
    		{
        		// read as many numbers as possible.
        		for (float number; ss >> number;) {
            		numeric_list.push_back(number);
        		}
        		// consume and discard token from stream.
        		if (ss.fail())
        		{
            		ss.clear();
            		std::string token;
            		ss >> token;
        		}
    		}
    		while (!ss.eof());			
		}
		pd = strtok(NULL, "\"=,");
	}
	population.loadData(numeric_list);	// basis for a future copy constructor
   Sample sample(population);
	t2 = clock();
	t3 = clock();
	// population.getStats();
	t4 = clock();
	t1 /= static_cast<float>(CLOCKS_PER_SEC);
	t2 /= static_cast<float>(CLOCKS_PER_SEC);
	t3 /= static_cast<float>(CLOCKS_PER_SEC);
	t4 /= static_cast<float>(CLOCKS_PER_SEC);
	std::cout << "<!DOCTYPE HTML>\n";
	std::cout << "<html>\n";
	std::cout << "<head><title>CGI/C++ Online Statistical Calculator</title>\n";
   std::cout << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
   std::cout << "<link rel=\"stylesheet\" media=\"all\"  href=\"/includes/style.css\">\n";
   std::cout << "</head>\n";   
	std::cout << "<body>\n";
	std::cout << "<h1>statistical report</h1>\n";
	std::cout << "<div>data retrieved in " << t2 - t1 << " seconds</div>\n";
	std::cout << "<div>data processed in " << t4 - t3 << " seconds</div>\n";
   printReport(population, sample);
   // compute central tendencies deviations tables
	devTables tables(population);
   tables.computeTables();
   // get some data ready
   float min = tables.getMin();
   float max = tables.getMax();
   float mean = tables.getMean();
   float mdn = tables.getMedian();
   float std = tables.getStdDev();
   float mad = tables.getMdnDev();
   float aad = tables.getMeanDev();
   // display tables
   // write mean deviations report
   std::cout << "<p>mean deviations</p>" << std::endl;
   printTable(tables.meanDevs, mean, aad, min, max);
   // write standard deviations report
   std::cout << "<p>standard deviations</p>" << std::endl;
   printTable(tables.stdDevs, mean, std, min, max);
   // write median deviations report
   std::cout << "<p>median deviations</p>" << std::endl;
   printTable(tables.mdnDevs, mdn, mad, min, max);   
	std::cout << "</body>\n";
	std::cout << "</html>";
	return 0;
}

// write summary statistical report
void printReport(const Population& pop, const Sample& sam)
{
   std::vector<float> v, u;
   // pop.unloadData(v);
   // displayData(v);
	std::cout << "<ul>\n";
	std::cout << "	<li>n = " << pop.getSize() << "</li>\n";
	std::cout << "	<li>mean = " << pop.getMean() << "</li>\n";
	std::cout << "	<li>population variance = " << pop.getVar() << "</li>\n";
	std::cout << "	<li>population standard deviation = " << pop.getStdDev() << "</li>\n";
	std::cout << "	<li>sample variance = " << sam.getVar() << "</li>\n";
	std::cout << "	<li>sample standard deviation = " << sam.getStdDev() << "</li>\n";
	std::cout << "	<li>minimum = " << pop.getMin() << "</li>\n";
	std::cout << "	<li>maximum = " << pop.getMax() << "</li>\n";
	std::cout << "	<li>median = " << pop.getMedian() << "</li>\n";
	std::cout << "	<li>median deviation = " << pop.getMdnDev() << "</li>\n";
   std::cout << "	<li>mean deviation = " << pop.getMeanDev() << "</li>\n";
   std::cout << "	<li>skewness = " << pop.getSkew() << "</li>\n";
	std::cout << "</ul>\n";
}

// display deviation table
void printTable(int table[], float m, float dev, float min, float max)
{
   int j = 0;
   float glb = min, lub;
   // find the start of the data
   lub = m - 3*dev;
   while (lub < min) {
      lub += dev;
      ++j;
   }
   std::cout << "<table class=\"right-nowrap\">\n";
   // std::cout << "<table>\n";
   std::cout << "<tr><th>glb</th><th>lub</th><th>n</th></tr>\n";
   std::cout << "<tr>";
   for (int i = j; i < 8 && glb < max; i++) {
      std::cout << "<tr>";
      // std::string glbstr = floattoString(glb, 6);
      // std::string lubstr = floattoString(lub, 6);
      // std::cout << "<td>" << glbstr << "</td><td>" << lubstr << "</td>";
      std::cout << std::fixed << std::setprecision(6) << "<td>" << glb << "</td>";
      std::cout << std::fixed << std::setprecision(6) << "<td>" << lub << "</td>";
      std::cout << "<td>" << table[i] << "</td>";
      glb = lub;
      lub += dev;
      if (lub > max) lub = max;
      // glb += .000001;
   }   
   std::cout << "</tr>\n";
   std::cout << "</table>" << std::endl;
}

static std::string floattoString(float r, unsigned n)
{
   std::stringstream ss;
   // convert double to string w fixed notation, hi precision
   ss << std::fixed << std::setprecision(n) << r;
   // output to std::string
   std::string float_str = ss.str();
   //remove trailing 000s    (123.1200 => 123.12,  123.000 => 123.)
   float_str.erase(float_str.find_last_not_of('0') + 1, std::string::npos);
   //remove dangling decimal (123. => 123)
   if (float_str[float_str.size()-1] == '.') {
      float_str.erase(float_str.end()-1);
   }
   return float_str;
}
