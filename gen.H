# ifndef GEN_H
# define GEN_H

# include <gsl/gsl_rng.h>
# include <cassert>

# include "defs.H"

DynList<Ulong> generate_random_perm(const size_t n, gsl_rng * r)
{
  DynList<Ulong> ret;
  for (size_t i = 0; i < n; ++i)
    ret.append(gsl_rng_get(r));  
  
  return ret;
}


DynList<Ulong> generate_semi_sorted_perm(const size_t n, 
					 const double sort_fact,
					 gsl_rng * r)
{
  assert(sort_fact >= 0 and sort_fact <= 1);

  if (sort_fact == 1)
    return generate_random_perm(n, r);
 
  DynArray<Ulong> elems = range<DynArray, Ulong>(n); // an array for fast access
    
  size_t num_inversions = sort_fact*n;
  for (size_t i = 0; i < num_inversions; ++i)
    {
      size_t idx1, idx2;
      do 
	{
	  idx1 = gsl_rng_uniform_int(r, n);
	  idx2 = gsl_rng_uniform_int(r, n);
	} while (elems(idx1) > elems(idx2)); // while there is no inversion
      swap(elems(idx1), elems(idx2));
    }

  return elems.map<Ulong, DynList>();
}


# endif // GEN_H
