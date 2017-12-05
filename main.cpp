#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <ctime>
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
      std::cout << "<p>I'm sorry Dave, I'm afraid I can't do that.</p>\n";
      printFooter();  
      return 1;
   }   
   float t1 = clock();
   // processing
   Pop p;
   Sample s;
   Stats* pp = &p;
   Stats* ps = &s;
   pp->load(x);
   ps->load(x);
   // compute general statistics
   float m = pp->computeMean();
   float mdn = pp->computeMedian();
   float popvar = pp->computeVar();
   float popstd = sqrt(popvar);
   float samvar = ((float)n/(float)(n-1))*popvar;
   float samstd = sqrt(samvar);
   float aad = pp->computeMeanDev();
   float mad = pp->computeMedianDev();
   float skw = pp->computeSkewness();
   float mdnskw = pp->computeMedianSkew();
   float lo = pp->computeMin();
   float hi = pp->computeMax();
   float cv = pp->computeCV();
   // compute cumulative from central tendency tables
   ps->computeTables();
   float t2 = clock();
   // output
   std::cout << "<p>processing time = " << (t2-t1)/CLOCKS_PER_SEC << " seconds</p>\n";
   std::cout << "<ul>\n";
   std::cout << "<li>n = " << pp->getSize() << "</li>\n";
   std::cout << "<li>mean = " << m << "</li>\n";
   std::cout << "<li>median = " << mdn << "</li>\n";
   std::cout << "<li>population variance = " << popvar << "</li>\n";
   std::cout << "<li>population standard deviation = " << popstd << "</li>\n";
   std::cout << "<li>sample variance = " << samvar << "</li>\n";
   std::cout << "<li>sample standard deviation = " << samstd << "</li>\n";
   std::cout << "<li>mean deviation = " << aad << "</li>\n";
   std::cout << "<li>median deviation = " << mad << "</li>\n";
   std::cout << "<li>skewness = " << skw << "</li>\n";
   std::cout << "<li>median skewness = " << mdnskw << "</li>\n";
   std::cout << "<li>minimum = " << lo << "</li>\n";
   std::cout << "<li>maximum = " << hi << "</li>\n";
   std::cout << "<li>coefficient of variation = " << cv << "</li>\n";
   std::cout << "</ul>\n";
   // output cumulative from central tendency tables - we shall use sample std
   ps->htmlTables();
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
