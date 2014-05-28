

/* 
    Programa para estudiar los desempeños de loas algotirmo de
    ordenamiento quicksort, mergesort y insertion 
*/

# include <now.h>
# include <gsl/gsl_rng.h>
# include <cassert>
# include <limits>
# include <htlist.H>
# include <tpl_sort_utils.H> 

using namespace Aleph;

DynList<unsigned long> generate_random_perm(const size_t n, gsl_rng * r)
{
  DynList<unsigned long> ret;
  for (size_t i = 0; i < n; ++i)
    ret.append(gsl_rng_get(r)/10000);  
    // ret.append(gsl_rng_uniform_int(r, 100000)); 
  
  return ret;
}


void usage()
{
  cout << "./raw-perf num-items num-reps seed" << endl;
  exit(0);
}


double measure_insertion_sort(const DynList<unsigned long> & l, int n = 3)
{
  double t = numeric_limits<double>::max();
  for (int i = 0; i < n; ++i)
    {
      DynList<unsigned long> ll = l;
      t = now();
      insertion_sort(ll);
      t = min(now_delta(&t), t);
      
      if (i == 0 and not is_sorted(ll))
	ERROR("Not sorted");
    }
  return t;
}

double measure_mergesort(const DynList<unsigned long> & l, int n = 3)
{
  double t = numeric_limits<double>::max();
  for (int i = 0; i < n; ++i)
    {
      DynList<unsigned long> ll = l;
      t = now();
      mergesort(ll);
      t = min(now_delta(&t), t);
      
      if (i == 0 and not is_sorted(ll))
	ERROR("Not sorted");
    }
  return t;
}

double measure_quicksort(const DynList<unsigned long> & l, int n = 3)
{
  double t = numeric_limits<double>::max();
  for (int i = 0; i < n; ++i)
    {
      DynList<unsigned long> ll = l;
      t = now();
      quicksort(ll);
      t = min(now_delta(&t), t);
      
      if (i == 0 and not is_sorted(ll))
	ERROR("Not sorted");
    }
  return t;
}


DynList<double> 
measure(const size_t n, const size_t num_samples, 
	const int m, gsl_rng * r)
{
  DynList<double> ret;
  for (int i = 1; i < num_samples; ++i)
    {
      DynList<unsigned long> l = generate_random_perm(n, r);
      ret.append(measure_insertion_sort(l, m));
      ret.append(measure_mergesort(l, m));
      ret.append(measure_quicksort(l, m));
    }
  return ret;
}

using Measure = tuple<int, DynList<double>>;

void print(const DynList<Measure> & l)
{
  size_t num_samples = get<1>(l.get_first()).size() / 3;
  assert(get<1>(l.get_first()).size() % 3 == 0);

  cout << "n";
  for (int i = 0; i < num_samples; ++i)
    cout << ",i" << i << ",m" << i << ",q" << i;
  cout << endl;

  l.for_each([] (const Measure & m)
    {
      int i = get<0>(m);
      cout << i;
      get<1>(m).for_each([] (double r)
	{
	  cout << "," << r;
	});
      cout << endl;
    });

}

int main(int argc, char *argv[])
{
  if (argc != 4)
    usage();

  const size_t n = atoi(argv[1]);
  const size_t num_samples = atoi(argv[2]);
  const unsigned long seed = atol(argv[3]);

  gsl_rng * r = gsl_rng_alloc(gsl_rng_mt19937);
  gsl_rng_set(r, seed % gsl_rng_max(r));

  DynList<Measure> measures;
  for (long int k = 2; k < n; ++k)
    measures.append(make_tuple(k, measure(k, num_samples, 3, r)));

  print(measures);

  gsl_rng_free(r);
  return 0;
}
