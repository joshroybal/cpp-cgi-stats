#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include "stats.hpp"

const int MAX_BYTES = 1000000;

void printHeader();
void printFooter();

int main()
{
   // ouput
   printHeader();
   int length = atoi(getenv("CONTENT_LENGTH"));
   if (length >= MAX_BYTES) { // error trap for more data than we want to allow
      std::cout << "<p>I'm sorry Dave, I'm afraid I can't do that.</p>\n";
      printFooter();  
      return 1;
   }
   // input
   std::string str;
   std::vector<float> x;
   while ( std::cin >> str ) {
      std::istringstream ss(str);
      float r;
      if (ss >> r)
         x.push_back(r);
   }
   int n = x.size();
   if (n <= 0) { // error trap for less data than we want to allow
      std::cerr << "<p>I'm sorry Dave, I'm afraid I can't do that.</p>\n";
      printFooter();  
      return 1;
   }   
   float t1 = clock();
   // processing
   Pop p;
   Sample s;
   Stats *pp = &p;
   Stats *ps = &s;
   pp->load(x, n);
   ps->load(x, n);
   // output
   std::cout << "<ul>\n";
   std::cout << "<li>n = " << pp->getSize() << "</li>\n";
   std::cout << "<li>mean = " << pp->computeMean() << "</li>\n";
   std::cout << "<li>population variance = " << pp->computeVar() << "</li>\n";
   std::cout << "<li>population standard deviation = " << pp->computeStd() << "</li>\n";
   std::cout << "<li>population skewness = " << pp->computeSkewness() << "</li>\n";
   std::cout << "<li>sample variance = " << ps->computeVar() << "</li>\n";
   std::cout << "<li>sample standard deviation = " << ps->computeStd() << "</li>\n";
   std::cout << "<li>sample skewness = " << ps->computeSkewness() << "</li>\n";
   std::cout << "<li>minimum = " << pp->computeMin() << "</li>\n";
   std::cout << "<li>maximum = " << pp->computeMax() << "</li>\n";
   std::cout << "<li>median = " << pp->computeMedian() << "</li>\n";
   std::cout << "<li>median deviation = " << pp->computeMedianDev() << "</li>\n";
   std::cout << "<li>mean deviation = " << pp->computeMeanDev() << "</li>\n";
   std::cout << "<li>median skewness = " << pp->computeMedianSkew() << "</li>\n";
   std::cout << "<li>coefficient of variation = " << pp->computeCV() << "</li>\n";
   std::cout << "</ul>\n";
   float t2 = clock();
   std::cout << "<p>processing time = " << (t2-t1)/CLOCKS_PER_SEC << " seconds</p>\n";
   // output cumulative from central tendency tables - we shall use sample std
   // ps->displayTables();
   ps->htmlTables();
   std::cout << "<p>processing time = " << (t2-t1)/CLOCKS_PER_SEC << " seconds</p>\n";
   printFooter();
   return 0;
}

void printHeader()
{
    std::cout << "Content-Type: text/html; charset=utf-8\n\n";
    std::cout << "<!DOCTYPE HTML>\n";
    std::cout << "<html>\n";
    std::cout << "<head>\n";
    std::cout << "<meta charset=\"utf-8\">\n";
    std::cout << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    std::cout << "<link rel=\"stylesheet\" media=\"all\"  href=\"/includes/style.css\">\n";
    std::cout << "<title>cgi-bin C++ statistics computations</title>\n";
    std::cout << "</head>\n";
    std::cout << "<body>\n";
    std::cout << "<header><p>cgi-bin C++ statistical computations</p></header>\n";
    std::cout << "<div><a href=\"/index.php\">Home</a> | <a href=\"/cppstats.html\">Back</a></div>\n";  
}

void printFooter()
{
    std::cout << "<div><a href=\"/index.php\">Home</a> | <a href=\"/cppstats.html\">Back</a></div>\n";
    std::cout << "<footer><p>CopyLeft (c) 2017 Josh Roybal - All Wrongs Reserved.</p></footer>\n";
    std::cout << "</html>";
}
