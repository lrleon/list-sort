
# ifndef SORT_H
# define SORT_H

# include <now.h>
# include <gsl/gsl_rng.h>
# include <tpl_sort_utils.H>
# include "defs.H"
# include "gen.H"

size_t insertion_threshold = 20;

// advance it1 in one position and it2 in two positions 
inline bool double_next(DynList<Ulong>::Iterator & it1, 
			DynList<Ulong>::Iterator & it2, 
			size_t & count,
			Ulong & prev)
{
  Ulong curr = it2.get_curr();
  bool ret = curr >= prev; 
  
  it2.next();
  ++count; 
  prev = curr; 
  if (not it2.has_curr())
    return ret;
  
  it1.next();

  curr = it2.get_curr(); 
  ret = curr >= prev and ret;

  it2.next();
  ++count; 
  prev = curr; 

  return ret;
} 

// return 0 if the list is already sorted 
size_t split_compare(DynList<Ulong> & l, 
		     DynList<Ulong> & l1, DynList<Ulong> & l2)
{
  I(l1.is_empty() and l2.is_empty()); 

  if (l.is_empty() or l.is_unitarian())
    return 0; // no split

  Ulong prev = l.get_first();
  bool is_sorted = true;

  size_t count = 0;

  DynList<Ulong>::Iterator it1(l), it2(l);
  while (it2.has_curr())
    is_sorted = double_next(it1, it2, count, prev) and is_sorted;

  if (is_sorted)
    {
      I(Aleph::is_sorted(l));
      return 0; // dot not split either
    }

  I(it1.has_curr());
  Slinknc * link = reinterpret_cast<HTList::Iterator&>(it1).get_curr(); 

  l.cut_list(link, l2);
  l.swap(l1);

  return count;
}

    inline 
size_t split_list(DynList<Ulong> & l, DynList<Ulong> & l1, DynList<Ulong> & l2)
{
  size_t n = 0;
  while (not l.is_empty())
    {
      l1.append(l.remove_first()); ++n;
      if (l.is_empty())
	return n;
      l2.append(l.remove_first()); ++n;
    }
  return n;
}

inline void mergesort_w(DynList<Ulong> & l, const size_t)
{
  mergesort(l);
}

inline void quicksort_w(DynList<Ulong> & l, const size_t)
{
  quicksort(l);
}

inline void insertionsort_w(DynList<Ulong> & l, const size_t)
{
  insertion_sort(l);
}

inline void mergecmp(DynList<Ulong> & l, 
		     const size_t lsz = insertion_threshold)
{
  if (lsz < insertion_threshold)
    {
      insertion_sort(l);
      return;
    }

  DynList<Ulong> l1, l2;
  size_t n = split_compare(l, l1, l2);

  if (n == 0)
    return;

  size_t n2 = n/2;
  mergecmp(l1, n % 2 == 0 ? n2 : n2 + 1);
  mergecmp(l2, n2); 

  merge_lists<DynList<Ulong>, std::less<Ulong>>(l1, l2, l);
}


inline void merge_insertsort(DynList<Ulong> & l, 
			     const size_t lsz = insertion_threshold)
{
  if (lsz < insertion_threshold)
    {
      insertion_sort(l);
      return;
    }

  DynList<Ulong> l1, l2;
  size_t n = l.split_list(l1, l2);

  size_t n2 = n/2;
  merge_insertsort(l1, n % 2 == 0 ? n2 : n2 + 1);
  merge_insertsort(l2, n2);

  merge_lists<DynList<Ulong>, std::less<Ulong>>(l1, l2, l);
}


inline void quicksort_insertion(DynList<Ulong> & l, 
				const size_t lsz = insertion_threshold)
{
  if (lsz < insertion_threshold)
    {
      insertion_sort(l);
      return;
    }

  size_t n_smaller = 0, n_greater = 0, ni = 0;
  DynList<Ulong> smaller, greater, li;
  const Ulong pivot = l.remove_first();
  while (not l.is_empty())
    {
      Ulong item = l.remove_first();
      if (item < pivot) 
	{
	  smaller.append(item);
	  ++n_smaller;
	}
      else if (pivot < item)
	{
	  greater.append(item);
	  ++n_greater;
	}
      else
	{
	  li.append(item);
	  ++ni;
	}
    }

  if (n_smaller < n_greater)
    {
      quicksort_insertion(smaller, n_smaller);
      quicksort_insertion(greater, n_greater);
    }
  else
    {
      quicksort_insertion(greater, n_greater);
      quicksort_insertion(smaller, n_smaller);
    }	

  l.concat_list(smaller);
  l.concat_list(li);
  l.concat_list(greater);
}


void quickmergesort(DynList<Ulong> & l, const size_t lsz); 

void merge_quicksort_insertion(DynList<Ulong> & l, 
			       const size_t lsz = insertion_threshold)
{
  if (lsz < insertion_threshold)
    {
      insertion_sort(l);
      return;
    }

  size_t n_smaller_sorted = 0, n_greater_sorted = 0, 
    n_smaller = 0, n_greater = 0;
  DynList<Ulong> smaller_sorted, greater_sorted, smaller, greater;

  const Ulong pivot = l.remove_first();
  
  while (not l.is_empty())
    {
      Ulong item = l.remove_first();
      if (item < pivot) 
	{
	  if (smaller_sorted.is_empty() or item <= smaller_sorted.get_first())
	    {
	      smaller_sorted.insert(item);
	      ++n_smaller_sorted;
	    }
	  else if (item >= smaller_sorted.get_last())
	    {
	      smaller_sorted.append(item);
	      ++n_smaller_sorted;
	    }
	  else
	    {
	      smaller.append(item);
	      ++n_smaller;
	    }
	}
      else if (pivot < item)
	{
	  if (greater_sorted.is_empty() or item <= greater_sorted.get_first())
	    {
	      greater_sorted.insert(item);
	      ++n_greater_sorted;
	    }
	  else if (item >= greater_sorted.get_last())
	    {
	      greater_sorted.append(item);
	      ++n_greater_sorted;
	    }
	  else
	    {
	      greater.append(item);
	      ++n_greater;
	    }
	}
    }
  
  quickmergesort(smaller, n_smaller);
  quickmergesort(greater, n_greater);
  I(is_sorted(smaller));
  I(is_sorted(greater)); 

  DynList<Ulong> ls;
  merge_lists<DynList<Ulong>, std::less<Ulong>>(smaller, smaller_sorted, ls);

  DynList<Ulong> lg;
  merge_lists<DynList<Ulong>, std::less<Ulong>>(greater, greater_sorted, lg);

  l.concat_list(ls);
  l.concat_list(lg);
  I(is_sorted(l));
}

void quickmergesort(DynList<Ulong> & l, 
		    const size_t lsz = insertion_threshold)
{
  if (lsz < insertion_threshold)
    {
      insertion_sort(l);
      return;
    }

  DynList<Ulong> l1, l2;
  size_t n = l.split_list(l1, l2);
  size_t n2 = n/2;

  Ulong i1 = l1.get_first(), i2 = l1.get_last(), 
    i3 = l2.get_first(), i4 = l2.get_last();
  if ((i1 < i2 and i2 < i3 and i3 < i4) or 
      (i1 > i2 and i2 > i3 and i3 > i4)) // probably sorted?
    {
      quickmergesort(l1, n % 2 == 0 ? n2 : n2 + 1);
      quickmergesort(l2, n2);
    }
  else if (i2 > i3 or i1 > i4) // probably a good partition
    {
      merge_quicksort_insertion(l1, n % 2 == 0 ? n2 : n2 + 1);
      merge_quicksort_insertion(l2, n2);
    }
  else
    {
      quickmergesort(l1, n % 2 == 0 ? n2 : n2 + 1);
      quickmergesort(l2, n2);
    }

  merge_lists<DynList<Ulong>, std::less<Ulong>>(l1, l2, l);
  I(is_sorted(l));
} 

void super_quickmergesort(DynList<Ulong> & l, 
			  const size_t lsz = insertion_threshold)
{
  if (lsz < insertion_threshold)
    {
      insertion_sort(l);
      return;
    }

  DynList<Ulong> l1, l2;
  size_t n = split_compare(l, l1, l2);
  if (n == 0)
    {
      I(is_sorted(l));
      return; // l is already sorted 
    }

  size_t n2 = n/2;

  Ulong i1 = l1.get_first(), i2 = l1.get_last(), 
    i3 = l2.get_first(), i4 = l2.get_last();
  if ((i1 < i2 and i2 < i3 and i3 < i4) or 
      (i1 > i2 and i2 > i3 and i3 > i4)) // probably sorted?
    {
      quickmergesort(l1, n % 2 == 0 ? n2 : n2 + 1);
      quickmergesort(l2, n2);
    }
  else if (i2 > i3 or i1 > i4) // probably a good partition
    {
      merge_quicksort_insertion(l1, n % 2 == 0 ? n2 : n2 + 1);
      merge_quicksort_insertion(l2, n2);
    }
  else
    {
      quickmergesort(l1, n % 2 == 0 ? n2 : n2 + 1);
      quickmergesort(l2, n2);
    }

  merge_lists<DynList<Ulong>, std::less<Ulong>>(l1, l2, l);
  I(is_sorted(l));
} 

# define TEST_SORT(name) \
  double test_##name(const size_t n, const double sortfact, gsl_rng * r) \
  {									\
    cout << "Generating partial sorted sequence n = " << n << " "	\
	 << 100*sortfact << " %" << endl;				\
    DynList<Ulong> l = generate_semi_sorted_perm(n, sortfact, r);	\
									\
    cout << "Sorting ..." << endl;					\
									\
    double t = now();							\
    name(l);								\
    t = min(now_delta(&t), t);						\
    if (not is_sorted(l))						\
      {									\
	cout << "Not sorted";						\
	l.for_each([] (Ulong i) { cout << " " << i; });			\
	cout << endl;							\
	exit(0);							\
      }									\
    return t;								\
  }

# define MAIN_SORT(name)						\
  TEST_SORT(name);							\
									\
  void usage(char * argv[])						\
  {									\
    cout << "usage is " << endl						\
	 << "    " << argv[0] << " n threshold sortfact seed" << endl;	\
    exit(0);								\
  }									\
									\
  int main(int argc, char *argv[])					\
  {									\
    if (argc != 5)							\
      usage(argv);							\
									\
    const size_t n = atol(argv[1]);					\
    const size_t threshold = atol(argv[2]);				\
    const double sortfact = atof(argv[3]);				\
    const unsigned long seed = atol(argv[4]);				\
									\
    gsl_rng * r = gsl_rng_alloc(gsl_rng_mt19937);			\
    gsl_rng_set(r, seed % gsl_rng_max(r));				\
									\
    insertion_threshold = threshold;					\
    cout << "n = " << n << endl						\
    << "insertion_threshold = " << insertion_threshold << endl		\
    << "sort factor = " << 100*sortfact << endl				\
    << "seed " << seed << endl;\
									\
    double time = test_##name(n, sortfact, r);				\
									\
    cout << "Time was " << time << endl;				\
									\
    gsl_rng_free(r);							\
  }

# endif // SORT_H
