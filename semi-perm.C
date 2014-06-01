

/* 
   This programa generates lists of random permutations
*/

# include <now.h>
# include <gsl/gsl_rng.h>
# include <cassert>
# include <limits>
# include <htlist.H>
# include <tpl_sort_utils.H> 

# include "defs.H"

# include "sort.H"

# include "gen.H"
# include "measure-sort.H"

using namespace Aleph;

void usage()
{
  cout << "./semi-perf num-items step sort-fact num-reps seed" << endl;
  exit(0);
}

int main(int argc, char *argv[])
{
  if (argc != 6)
    usage();

  const size_t n = atoi(argv[1]);
  const size_t step = atoi(argv[2]);
  const double sortfact = atof(argv[3]);
  const size_t num_samples = atoi(argv[4]);
  const unsigned long seed = atol(argv[5]);

  gsl_rng * r = gsl_rng_alloc(gsl_rng_mt19937);
  gsl_rng_set(r, seed % gsl_rng_max(r));

  DynList<Measure> measures;
  for (long sz = 1; sz < n; sz += step)
    measures.append(make_tuple(sz, measure_semi_sorted(sz, sortfact,
						       num_samples, 3, r)));

  cout << measures;

  gsl_rng_free(r);
  return 0;
}
