#ifndef STATS_HPP__
#define STATS_HPP__

#include <vector>
#include <fstream>
#include <limits> // UNIX

const int IMAX = std::numeric_limits<int>::max(); // UNIX

// abstract base class - do not instantiate
class Stats {
   public:
      // assignment method
      virtual void load(const std::vector<float>&);
      // accessor methods
      int getSize() const;
      float computeMean() const;
      float computeVar() const;
      float computeVar(float) const;
      float computeStd() const;
      float computeStd(float) const;
      float computeMin() const;
      float computeMax() const;
      float computeMedian() const;
      float computeMedianDev() const;
      float computeMedianDev(float) const;
      float computeMeanDev() const;
      float computeMeanDev(float) const;
      float computeSkewness() const;
      float computeSkewness(float) const;
      float computeMedianSkew() const;
      float computeMedianSkew(float, float) const;
      float computeCV() const;
      void computeTables();
      void displayTables(std::ofstream&) const;
      void htmlTables() const;
   protected:
      int n;
      std::vector<float> x;
      float standard_right[41][10];
      float standard_left[41][10];
      float median_right[41][10];
      float median_left[41][10];
      float mean_right[41][10];
      float mean_left[41][10];
      float mean(const std::vector<float>&, int) const;
      virtual float var() const = 0;
      virtual float var(float) const = 0;
      virtual float std() const = 0;
      virtual float std(float) const = 0;
      float quick_select(const std::vector<float>&, int, int) const;
      float minimum() const;
      float maximum() const;
      float median(const std::vector<float>&, int) const;
      float median_dev() const;
      float median_dev(float) const;
      float mean_dev() const;
      float mean_dev(float) const;
      float skewness() const;
      float skewness(float) const;
      float median_skew() const;
      float median_skew(float, float) const;
      float coefficient_of_variation() const;
      void compute_tables(float[][10], float[][10], float, float);
      void display_tables(std::ofstream&, const float[][10], const float[][10]) const;
      void html_tables(const float[][10], const float[][10]) const;
};

// population derived class
// must explicitly call computeTable if you want them
class Pop : public virtual Stats {
   private:
      float var() const;
      float var(float) const;
      float std() const;
      float std(float) const;
};

// sample derived class
class Sample : public virtual Stats {
   private:
      float var() const;
      float var(float) const;
      float std() const;
      float std(float) const;
};

#endif
