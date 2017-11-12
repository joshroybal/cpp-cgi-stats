#ifndef _STATISTICS_HPP__
#define _STATISTICS_HPP__

#include <vector>

// Statistics class is abstract base class with pure virtual functions
// not to be invoked as a run time object
class Statistics {
   public:
      Statistics() : n(0) {}
      Statistics(unsigned u) : n(u) {}
      // public accessor methods
      unsigned getSize() const;
      float getMean() const;
      float getVar() const;
      float getStdDev() const;
      float getMin() const;
      float getMax() const;
      float getMedian() const;
      float getMdnDev() const;
      float getMeanDev() const;
      void unloadData(std::vector<float>&) const;
   protected:
      unsigned n;
      std::vector<float> x;
      unsigned size() const;
      float mean() const;
      virtual float var() const = 0;
      virtual float std() const = 0;
      float quick_select(const std::vector<float>&, unsigned) const;
      float minimum() const;
      float maximum() const;
      float median(const std::vector<float>&) const;
      float median_deviation() const;
      float mean_deviation() const;
};

// derived class inherits from abstract base class Statistics
class Population : public virtual Statistics {
   public:
      Population() : Statistics() {}
      Population(unsigned u) : Statistics(u) { load_random();}
      Population(const Population& p) : Statistics(p.getSize()) { p.unloadData(x); }
      void loadData(const std::vector<float>&);
      float getSkew() const;
   protected:
      void load_random();
      float var() const;
      float std() const;
      float skewness() const;
};

// derived class inherits from derived class Population
class Sample : public virtual Population {
   public:
      Sample() : Statistics() {}
      Sample(unsigned u) : Statistics(u), Population(u) {}
      Sample(const Population& p) : Statistics(p.getSize()), Population(p) {}
      Sample(const Population& p, unsigned u) : Population(p) { load(p, u); }
   protected:
      float var() const;
      float std() const;
      void load(const Population&, unsigned);
};

class devTables : public virtual Sample {
   public:
      // Do not instantiate this derived class without extant Population and
      // Sample object instances.
      // calling Statistics base contructor initializes n to population size
      devTables(const Population& p) : Statistics(p.getSize()) { p.unloadData(x); }
      const static unsigned no_slices = 8;
      int stdDevs[no_slices];
      int mdnDevs[no_slices];
      int meanDevs[no_slices];
      void computeTables();
   private:
      void compute_standard_deviations();
      void compute_median_deviations();
      void compute_mean_deviations();
};

#endif
