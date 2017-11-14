#include <cstdlib>
#include <cmath>
#include "statistics.hpp"

// Statisics class public method definitions

// unload data in x into std::vector argument
void Statistics::unloadData(std::vector<float>& y) const
{
   y = x;
}

// access no. of elements in x
unsigned Statistics::getSize() const
{
   return size();
}

// access mean of items in vector x
float Statistics::getMean() const
{
   return mean();
}

// access variance of items in vector x
float Statistics::getVar() const
{
   return var();
}

// access standard deviation of items in vector x
float Statistics::getStdDev() const
{
   return std();
}

// access minimum value in vector x
float Statistics::getMin() const
{
   return minimum();
}

// access maximum value in vector x
float Statistics::getMax() const
{
   return maximum();
}

// access median value in vector x
float Statistics::getMedian() const
{
   return median(x);
}

// access value of median deviation of items in vector x
float Statistics::getMdnDev() const
{
   return median_deviation();
}

// access value of mean deviation of items in vector x
float Statistics::getMeanDev() const
{
   return mean_deviation();
}

// Statisics class private method definitions

// return no. of items in vector x
unsigned Statistics::size() const
{
   return n;
}

// return mean of items in vector x
float Statistics::mean() const
{
   float avg = 0;
   for (int i = 0; i < n; i++) avg = (i * avg + x[i]) / (i + 1);
   return avg;
}

// C. A. R. Hoare's quick selection algorithm
// neccesart auxlilary method to methods minimum, maximum, median,
// and median_deviation
float Statistics::quick_select(const std::vector<float>& v, unsigned k) const
{
   int i, j, left = 0, right = n - 1;
   float pivot;
   // initialize index vector to natural order (zero indexed)
   std::vector<int> idx;
   for (i = 0; i < n; i++) idx.push_back(i);
   while (left < right) {
      pivot = v[idx[k]];
      i = left;
      j = right;
      do {
         while (v[idx[i]] < pivot) i++;
         while (pivot < v[idx[j]]) j--;
         if (i <= j) {
            unsigned idxtmp = idx[i];
            idx[i] = idx[j];
            idx[j] = idxtmp;
            i++;
            j--;
          }
      } while (i <= j);
      if (j < k) left = i;
      if (k < i) right = j;
   }
   return v[idx[k]];
}

// return minimum value of items in vector x
float Statistics::minimum() const
{
   return quick_select(x, 0);
}

// return maximum value of items in vector x
float Statistics::maximum() const
{
   return quick_select(x, n-1);
}

// return median value of items in vector x
float Statistics::median(const std::vector<float>& v) const
{
   int k = n/2;
   if (n % 2 == 0) {
      float p = quick_select(v, k-1);
      float q = quick_select(v, k);
      return (p+q)/2.0;
   }
   else
      return quick_select(v, k);
}

// return median deviation of items in vector x
float Statistics::median_deviation() const
{
   std::vector<float> dev;
   float mdn = median(x);
   for (int i = 0; i < n; i++) dev.push_back( std::abs(x[i] - mdn) );
   return median(dev);
}

// return mean deviation of items in vector x
float Statistics::mean_deviation() const
{
   float mean_dev = 0, m = mean();
   for (int i = 0; i < n; i++) mean_dev = (i * mean_dev + std::abs(x[i] - m)) / (i + 1);
   return mean_dev;
}

// Population derived class implementation

// public methods

// load data from vector into class
void Population::loadData(const std::vector<float>& data)
{
   n = data.size();
   x = data;
}

// access skewness of data set
float Population::getSkew() const
{
   return skewness();
}

// protected methods

// load data set vector x with random floats between 0 and 1
void Population::load_random()
{
	for (int i = 0; i < n; i++) {
		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		x.push_back(r);
	}
}

// compute and return the population variance
float Population::var() const
{
   float var = 0, m = mean();
   for (int i = 0; i < n; i++) var = (i * var + (x[i] - m) * (x[i] - m)) / (i + 1);
   return var;
}

// compute and return the population standard deviation
float Population::std() const
{
   return sqrt(var());
}

// compute and return the skewness
float Population::skewness() const
{
   float skw = 0, m = mean();
   for (int i = 0; i < n; i++) skw = (i * skw + pow((x[i] - m), 3)) / (i + 1);
   return skw/pow(std(), 3);
}

// Sample derived class implementation

// protected methods

// compute and return the sample variance
float Sample::var() const
{
    return (float(n) / (n - 1)) * Population::var();
}

// compute and return the sample standard deviation
float Sample::std() const
{
    return sqrt(var());
}

// load sample from (first) n items of population
// debugging - change to random selection later
void Sample::load(const Population& pop, unsigned u)
{
   if (u > pop.getSize()) {
      x.clear();
      return;
   }
   int m = pop.getSize() - u;
   for (int i = 0; i < m; i++) x.pop_back();
   n = x.size();
}

// devTables derived class methods

// public methods
void devTables::computeTables()
{
   compute_mean_deviations();
   compute_median_deviations();
   compute_standard_deviations();
}

// private methods

// compute standard deviations table
void devTables::compute_standard_deviations()
{
   int idx;
   float m = mean(), sigma = std();
   // clear table
   for (int i = 0; i < 4; i++) stdDevs[i] = 0;
   // compute table
   for (int i = 0; i < n; i++) {
      float dev = std::abs(x[i] - m);
      if (dev <= sigma)
         ++(stdDevs[0]);
      else if (dev <= 2*sigma && dev > sigma)
         ++(stdDevs[1]);
      else if (dev <= 3*sigma && dev > 2*sigma)
         ++(stdDevs[2]);
      else
         ++(stdDevs[3]);
   }
}

// compute median deviations table
void devTables::compute_median_deviations()
{
   int idx;
   float mdn = median(x), mad = median_deviation();
   // clear table
   for (int i = 0; i < 4; i++) mdnDevs[i] = 0;
   // compute table
   for (int i = 0; i < n; i++) {
      float dev = std::abs(x[i] - mdn);
      if (dev <= mad)
         ++(mdnDevs[0]);
      else if (dev <= 2*mad && dev > mad)
         ++(mdnDevs[1]);
      else if (dev <= 3*mad && dev > 2*mad)
         ++(mdnDevs[2]);
      else
         ++(mdnDevs[3]);
   }
}

// compute mean deviations table
void devTables::compute_mean_deviations()
{
   int idx;
   float m = mean(), aad = mean_deviation();
   // clear table
   for (int i = 0; i < 4; i++) meanDevs[i] = 0;
   // compute table
   for (int i = 0; i < n; i++) {
      float dev = std::abs(x[i] - m);
      if (dev <= aad)
         ++(meanDevs[0]);
      else if (dev <= 2*aad && dev > aad)
         ++(meanDevs[1]);
      else if (dev <= 3*aad && dev > 2*aad)
         ++(meanDevs[2]);
      else
         ++(meanDevs[3]);
   }
}

