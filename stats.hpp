#ifndef _STATS_HPP__
#define _STATS_HPP__

#include <vector>

// abstract base class - do not instantiate
class Stats {
   public:
      // assignment method
      virtual void load(const std::vector<float>&, int);
      // accessor methods
      int getSize() const;
      float computeMean() const;
      float computeVar() const;
      float computeStd() const;
      float computeMin() const;
      float computeMax() const;
      float computeMedian() const;
      float computeMedianDev() const;
      float computeMeanDev() const;
      float computeSkewness() const;
      float computeMedianSkew() const;
      float computeCV() const;
      void computeTables();
      void displayTables() const;
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
      virtual float std() const = 0;
      float quick_select(const std::vector<float>&, int, int) const;
      float minimum() const;
      float maximum() const;
      float median(const std::vector<float>&, int) const;
      float median_dev() const;
      float mean_dev() const;
      float skewness() const;
      float median_skew() const;
      float coefficient_of_variation() const;
      void compute_tables(float[][10], float[][10], float, float);
      void display_tables(const float[][10], const float[][10]) const;
      void html_tables(const float[][10], const float[][10]) const;
};

// population derived class
// must explicitly call computeTable if you want them
class Pop : public virtual Stats {
   private:
      float var() const;
      float std() const;
};

// sample derived class
// overloaded load function makes tables automatically when called
class Sample : public virtual Stats {
   public:
      void load(const std::vector<float>&, int); // overload for tables
   private:
      float var() const;
      float std() const;
};

#endif
