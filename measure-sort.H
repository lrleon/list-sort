
# include <htlist.H>

using Measure = tuple<int, DynList<double>>;

# define MEASURE_SORT_METHOD(sortname) \
  double measure_##sortname(const DynList<Ulong> & l, int n = 3)	\
  {								\
    double t = numeric_limits<double>::max();			\
    for (int i = 0; i < n; ++i)					\
      {								\
	DynList<Ulong> ll = l;					\
	t = now();						\
	sortname(ll);						\
	t = min(now_delta(&t), t);				\
								\
	if (i == 0 and not is_sorted(ll))			\
	  ERROR("Not sorted");					\
      }								\
    return t;							\
  }


MEASURE_SORT_METHOD(insertion_sort);
MEASURE_SORT_METHOD(mergesort);
MEASURE_SORT_METHOD(quicksort);

inline DynList<double> measure_random(const size_t n,
				      const size_t num_exp, 
				      const int m, gsl_rng * r)
{
  DynList<double> ret;
  for (int i = 0; i < num_exp; ++i)
    {
      DynList<Ulong> l = generate_random_perm(n, r);
      ret.append(measure_insertion_sort(l, m));
      ret.append(measure_mergesort(l, m));
      ret.append(measure_quicksort(l, m));
    }
  return ret;
}

inline DynList<double> measure_semi_sorted(const size_t n, 
					   const double sort_fact,
					   const size_t num_exp, 
					   const int m, gsl_rng * r)
{
  DynList<double> ret;
  for (int i = 0; i < num_exp; ++i)
    {
      DynList<Ulong> l = generate_semi_sorted_perm(n, sort_fact, r);
      ret.append(measure_insertion_sort(l, m));
      ret.append(measure_mergesort(l, m));
      ret.append(measure_quicksort(l, m));
    }
  return ret;
}

inline  ostream & operator << (ostream & s, const DynList<Measure> & l)
{
  size_t num_samples = get<1>(l.get_first()).size() / 3;
  assert(get<1>(l.get_first()).size() % 3 == 0);

  s << "n";
  for (int i = 0; i < num_samples; ++i)
    s << ",i" << i << ",m" << i << ",q" << i;
  cout << endl;

  l.for_each([&s] (const Measure & m)
    {
      int i = get<0>(m);
      s << i;
      get<1>(m).for_each([&s] (double r)
	{
	  s << "," << r;
	});
      s << endl;
    });
  
  return s;
}
