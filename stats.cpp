#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include "stats.hpp"

// stats class public methods - constructors, destructors and accessors

// load array into object
void Stats::load(const std::vector<float>& v)
{
   n = v.size();
   x.clear();
   x = v;
}

// method accesses size n
int Stats::getSize() const
{
   return n;
}

// method accesses mean
float Stats::computeMean() const
{
   return mean(x, n);
}

// method accesses variance
float Stats::computeVar() const
{
   return var();
}

// overloaded method accesses variance - mean already processed
float Stats::computeVar(float m) const
{
   return var(m);
}

// method accesses standard deviation
float Stats::computeStd() const
{
   return std();
}

// overloaded method accesses standard deviation - mean already processed
float Stats::computeStd(float m) const
{
   return std(m);
}

// method accesses minimum
float Stats::computeMin() const
{
   return minimum();
}

// method accesses maximum
float Stats::computeMax() const
{
   return maximum();
}

// method accesses median
float Stats::computeMedian() const
{
   return median(x, n);
}

// method accesses median deviation
float Stats::computeMedianDev() const
{
   return median_dev();
}

// overloaded method accesses median deviation
float Stats::computeMedianDev(float mdn) const
{
   return median_dev(mdn);
}

// method accesses mean deviation
float Stats::computeMeanDev() const
{
   return mean_dev();
}

// overloaded method accesses mean deviation
float Stats::computeMeanDev(float m) const
{
   return mean_dev();
}

// method accesses skewness
float Stats::computeSkewness() const
{
   return skewness();
}

// overloaded method accesses skewness
float Stats::computeSkewness(float m) const
{
   return skewness(m);
}

// method accesses median skewness
float Stats::computeMedianSkew() const
{
   return median_skew();
}

// overloaded method accesses median skewnewss
float Stats::computeMedianSkew(float m, float mdn) const
{
   return median_skew(m, mdn);
}

// method accesses coefficient of variation
float Stats::computeCV() const
{
   return coefficient_of_variation();
}

// method computes cumulative from center tables
void Stats::computeTables()
{
   float m = computeMean();
   float mdn = computeMedian();
   float std = computeStd(m);
   float mad = computeMedianDev(mdn);
   float aad = computeMeanDev(m);
   compute_tables(standard_right, standard_left, m, std);
   compute_tables(median_right, median_left, mdn, mad);
   compute_tables(mean_right, mean_left, m, aad);
}

// method displays cumulative from center tables
void Stats::displayTables(std::ofstream& ofstr) const
{
   ofstr << "\n standard deviation tables - cumulative from mean";
   display_tables(ofstr, standard_right, standard_left);
   ofstr << "\n median deviation tables - cumulative from median";
   display_tables(ofstr, median_right, median_left);
   ofstr << "\n mean deviation tables - cumulative from mean";
   display_tables(ofstr, mean_right, mean_left);
}

// method displays cumulative from center tables - html format
void Stats::htmlTables() const
{
   std::cout << "<p>standard deviation tables - cumulative from mean</p>\n";
   html_tables(standard_right, standard_left);
   std::cout << "<p>median deviation tables - cumulative from median</p>\n";
   html_tables(median_right, median_left);
   std::cout << "<p>mean deviation tables - cumulative from mean</p>\n";
   html_tables(mean_right, mean_left);
}

// stats class protected methods - computers

// method computes mean
float Stats::mean(const std::vector<float>& x, int n) const
{
   float s = 0;
   for (int i = 0; i < n; i++)
      s += x[i];
   return s/n;
}

// method returns kth element of x
// C. A. R. Hoare's quick select algorithm
float Stats::quick_select(const std::vector<float>& x, int n, int k) const
{
   int i, j, left = 0, right = n - 1;
   float pivot;
   std::vector<int> idx;   // index vector
   // initialize index vector to natural order (zero indexed)
   for (i = 0; i < n; i++) idx.push_back(i);
   while (left < right) {
      pivot = x[idx[k]];
      i = left;
      j = right;
      do {
         while (x[idx[i]] < pivot) i++;
         while (pivot < x[idx[j]]) j--;
         if (i <= j) {
            int idxtmp = idx[i];
            idx[i] = idx[j];
            idx[j] = idxtmp;
            i++;
            j--;
          }
      } while (i <= j);
      if (j < k) left = i;
      if (k < i) right = j;
   }
   float result = x[idx[k]];
   return result;
}

// method finds minimum
float Stats::minimum() const
{
   float min = x[0];
   for (int i = 1; i < n; i++)
      if (x[i] < min) min = x[i];
   return min;
}

// method finds maximum
float Stats::maximum() const
{
   float max = x[0];
   for (int i = 1; i < n; i++)
      if (x[i] > max) max = x[i];
   return max;
}

// method returns median
float Stats::median(const std::vector<float>& x, int n) const
{
   if (n % 2 != 0)
      return quick_select(x, n, n/2);
   else {
      float mdn = quick_select(x, n, n/2) + quick_select(x, n, n/2-1);
      return mdn/2;
   }
}

// method computes median deviation
float Stats::median_dev() const
{
   float mdn = median(x, n);
   std::vector<float> abs_devs;
   for (int i = 0; i < n; i++) abs_devs.push_back( std::abs(x[i]-mdn) );
   return median(abs_devs, n);
}

// overloaded method computes median deviation - median already processed
float Stats::median_dev(float mdn) const
{
   std::vector<float> abs_devs;
   for (int i = 0; i < n; i++) abs_devs.push_back( std::abs(x[i]-mdn) );
   return median(abs_devs, n);
}

// method computes mean deviation
float Stats::mean_dev() const
{
   float m = mean(x, n);
   std::vector<float> abs_devs;
   for (int i = 0; i < n; i++) abs_devs.push_back( std::abs(x[i]-m) );
   return mean(abs_devs, n);
}

// overloaded method computes mean deviation - mean already processed
float Stats::mean_dev(float m) const
{
   std::vector<float> abs_devs;
   for (int i = 0; i < n; i++) abs_devs.push_back( std::abs(x[i]-m) );
   return mean(abs_devs, n);
}

// method computes skewness
float Stats::skewness() const
{
   float m = mean(x, n);
   float s = 0;
   for (int i = 0; i < n; i++)
      s = ((i * s) + pow(x[i] - m, 3)) / (i + 1);
   return s / pow(std(), 3);
}

// overloaded method computes skewness - mean already processed
float Stats::skewness(float m) const
{
   float s = 0;
   for (int i = 0; i < n; i++)
      s = ((i * s) + pow(x[i] - m, 3)) / (i + 1);
   return s / pow(std(), 3);
}

// method computes median skewness - Pearson's second skewness coefficient
float Stats::median_skew() const
{
   return 3*(mean(x,n)-median(x,n))/std();
}

// overloaded method computes median skewness
// Pearson's second skewness coefficient
// mean and median already processed
float Stats::median_skew(float m, float mdn) const
{
   return 3*(m-mdn)/std(m);
}

// method computes coefficient of variation
float Stats::coefficient_of_variation() const
{
   return std() / mean(x, n);
}

// method computes right and left z tables
void Stats::compute_tables(float z_right[][10], float z_left[][10], float avg, float dev)
{
   int right_counter[41][10];
   int left_counter[41][10];
   std::vector<float> z(n);
   for (int i = 0; i < n; i++) z[i] = (avg-x[i])/dev; 
   // initialize counter arrays to all zeros
   for (int i = 0; i < 41; i++)
      for (int j = 0; j < 10; j++)
         right_counter[i][j] = 0;
   for (int i = 0; i < 41; i++)
      for (int j = 0; j < 10; j++)
         left_counter[i][j] = 0;
   // count z value occurences - both tails - to be parallelized
   for (int i = 0; i < 41; i++)
      for (int j = 0; j < 10; j++) {
         float lub = i/10.0 + j/100.0;
         for (int k = 0; k < n; k++)
            if (std::abs(z[k]) <= lub) {
               if (z[k] > 0) ++(right_counter[i][j]);
               if (z[k] < 0) ++(left_counter[i][j]);
            }
      }

   // normalize to float - right tail
   for (int i = 0; i < 41; i++)
      for (int j = 0; j < 10; j++)
         z_right[i][j] = (float)right_counter[i][j] / (float)n;
   // normalize to float - left tail
   for (int i = 0; i < 41; i++)
      for (int j = 0; j < 10; j++)
         z_left[i][j] = (float)left_counter[i][j] / (float)n;
}

// method displays cumulative from central tendency tables
void Stats::display_tables(std::ofstream& ofstr, const float z_right[][10], const float z_left[][10]) const
{
   // display right tail table
   // print header row
   ofstr << "\n right tail" << std::endl;
   ofstr << "  z ";
   for (int j = 0; j < 10; j++)
      ofstr << " +" << std::fixed << std::setprecision(2) << float(j)/float(100);
   ofstr << std::endl;
   for (int i = 0; i < 41; i++) {
      ofstr << " " << std::fixed << std::setprecision(1) << float(i)/float(10);
      for (int j = 0; j < 10; j++)
         ofstr << " " << std::fixed << std::setprecision(3) << z_right[i][j];
      ofstr << std::endl;
   }
   // display left tail table
   // print header row
   ofstr << "\n left tail" << std::endl;
   ofstr << "  z ";
   for (int j = 0; j < 10; j++)
      ofstr << " +" << std::fixed << std::setprecision(2) << float(j)/float(100);
   ofstr << std::endl;
   for (int i = 0; i < 41; i++) {
      ofstr << " " << std::fixed << std::setprecision(1) << float(i)/float(10);
      for (int j = 0; j < 10; j++)
         ofstr << " " << std::fixed << std::setprecision(3) << z_left[i][j];
      ofstr << std::endl;
   }
   // display unity table - has nobody thought of this before??
   // print header row
   ofstr << "\n right and left tails" << std::endl;
   ofstr << "  z ";
   for (int j = 0; j < 10; j++)
      ofstr << " +" << std::fixed << std::setprecision(2) << float(j)/float(100);
   ofstr << std::endl;
   for (int i = 0; i < 41; i++) {
      ofstr << " " << std::fixed << std::setprecision(1) << float(i)/float(10);
      for (int j = 0; j < 10; j++)
         ofstr << " " << std::fixed << std::setprecision(3) << z_right[i][j]+z_left[i][j];
      ofstr << std::endl;
   }
}

// method displays cumulative from central tendency tables - html format
void Stats::html_tables(const float z_right[][10], const float z_left[][10]) const
{
   // display right tail table
   // print header row
   std::cout << "<p>right tail</p>\n";
   std::cout << "<table>\n";
   std::cout << "<tr><th>z</th>";
   for (int j = 0; j < 10; j++)
      std::cout << "<th>+" << std::fixed << std::setprecision(2) << float(j)/float(100) << "</th>";
   std::cout << "</tr>\n";
   for (int i = 0; i < 41; i++) {
      std::cout << "<tr><th>" << std::fixed << std::setprecision(1) << float(i)/float(10) << "</th>";
      for (int j = 0; j < 10; j++)
         std::cout << "<td>" << std::fixed << std::setprecision(5) << z_right[i][j] << "</td>";
      std::cout << "</tr>\n";
   }
   std::cout << "</table>\n";

   // display right tail table
   // print header row
   std::cout << "<p>left tail</p>\n";
   std::cout << "<table>\n";
   std::cout << "<tr><th>z</th>";
   for (int j = 0; j < 10; j++)
      std::cout << "<th>+" << std::fixed << std::setprecision(2) << float(j)/float(100) << "</th>";
   std::cout << "</tr>\n";
   for (int i = 0; i < 41; i++) {
      std::cout << "<tr><th>" << std::fixed << std::setprecision(1) << float(i)/float(10) << "</th>";
      for (int j = 0; j < 10; j++)
         std::cout << "<td>" << std::fixed << std::setprecision(5) << z_left[i][j] << "</td>";
      std::cout << "</tr>\n";
   }
   std::cout << "</table>\n";

   // display unity table - has nobody thought of this before??
   // print header row
   // display right tail table
   // print header row
   std::cout << "<p>right and left tail</p>\n";
   std::cout << "<table>\n";
   std::cout << "<tr><th>z</th>";
   for (int j = 0; j < 10; j++)
      std::cout << "<th>+" << std::fixed << std::setprecision(2) << float(j)/float(100) << "</th>";
   std::cout << "</tr>\n";
   for (int i = 0; i < 41; i++) {
      std::cout << "<tr><th>" << std::fixed << std::setprecision(1) << float(i)/float(10) << "</th>";
      for (int j = 0; j < 10; j++)
         std::cout << "<td>" << std::fixed << std::setprecision(5) << z_right[i][j]+z_left[i][j] << "</td>";
      std::cout << "</tr>\n";
   }
   std::cout << "</table>\n";
}

// population class public methods - constructors and accessors

// population class private methods - computers

// method computes population variance
float Pop::var() const
{
   float s = 0;
   float m = mean(x, n);
   for (int i = 0; i < n; i++)
      s += pow((x[i]-m), 2);
   return s / n;
}

// overloaded method computes population variance - mean already processed
float Pop::var(float m) const
{
   float s = 0;
   for (int i = 0; i < n; i++)
      s += pow((x[i]-m), 2);
   return s / n;
}

// method computes population standard variation
float Pop::std() const
{
   return sqrt(var());
}

// overloaded method computes population standard deviation
// mean already proceesed
float Pop::std(float m) const
{
   return sqrt(var(m));
}

// sample class private methods - computers

// method computes sample variance
float Sample::var() const
{
   float s = 0;
   float m = mean(x, n);
   for (int i = 0; i < n; i++)
      s += pow((x[i]-m), 2);
   return s/(n-1);
}

// overloaded method computes sample variance - mean already processed
float Sample::var(float m) const
{
   float s = 0;
   for (int i = 0; i < n; i++)
      s += pow((x[i]-m), 2);
   return s/(n-1);
}

// method computes sample standard deviation
float Sample::std() const
{
   return sqrt(var());
}

// overloaded method computes sample standard variation - mean aleady processed
float Sample::std(float m) const
{
   return sqrt(var(m));
}
