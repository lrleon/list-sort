

# include <tclap/CmdLine.h>

# include "sort.H"
# include "measure-sort.H"

# ifdef N // It is bizarrely defined by nana
# undef N
# endif

using namespace TCLAP;
using namespace std;


# define SAMPLE_SORT_METHOD(sortname)				  \
  void sample_##sortname(const DynList<Ulong> & l,		  \
			 const size_t lsz,			  \
			 const double sortfact,			  \
			 const int n)				  \
  {								  \
    double t = numeric_limits<double>::max();			  \
    for (int i = 0; i < n; ++i)					  \
      {								  \
	DynList<Ulong> ll = l;					  \
	t = now();						  \
	sortname(ll, lsz);					  \
	t = min(now_delta(&t), t);				  \
								  \
	if (i == 0 and not is_sorted(ll))			  \
	  {							  \
	    cout << "Not sorted";				  \
	    l.for_each([] (Ulong i) { cout << " " << i; });	  \
	    cout << endl;					  \
	    exit(0);						  \
	  }							  \
      }								  \
    cout << lsz << "," << #sortname << "," << sortfact << ","     \
	 << insertion_threshold << "," << t << endl;		  \
  }

SAMPLE_SORT_METHOD(quicksort_w);
SAMPLE_SORT_METHOD(mergesort_w);
SAMPLE_SORT_METHOD(quicksort_insertion)
SAMPLE_SORT_METHOD(merge_insertsort);
SAMPLE_SORT_METHOD(mergecmp);
SAMPLE_SORT_METHOD(quickmergesort);
SAMPLE_SORT_METHOD(super_quickmergesort);
SAMPLE_SORT_METHOD(insertionsort_w);

typedef void (*Sample_Fct)(const DynList<Ulong>&, const size_t, 
			   const double, const int);

using Method = std::pair<string, Sample_Fct>;

DynList<Method> methods = { 
  Method("i", sample_insertionsort_w),
  Method("q", sample_quicksort_w),
  Method("m", sample_mergesort_w),
  Method("qi", sample_quicksort_insertion),
  Method("mi", sample_merge_insertsort),
  Method("mc", sample_mergecmp),
  Method("qms", sample_quickmergesort),
  Method("sqms", sample_super_quickmergesort)
};

void test(const DynList<Method> & methods, 
	  size_t init, size_t len, size_t step,
	  double sortfact, size_t num_samples,
	  gsl_rng * r)
{
  for (size_t n = init; n < len; n += step)
    {
      DynList<Ulong> l = generate_semi_sorted_perm(n, sortfact, r);
      methods.for_each([&] (const Method & method)
        {
	  Sample_Fct sample_fct = method.second;
	  (*sample_fct)(l, n, sortfact, num_samples);
	});
    }
}

void parser(int argc, char ** argv)
{
  CmdLine
    cmd("Sampler of of sorting methods for single linked list", ' ', "0.0");

  ValueArg<size_t> n("n", "init", "Initial number of keys to sample", 
			false, 2,
			"initial number of keys to be randomly generated");
  cmd.add(n);

  ValueArg<size_t> N("N", "last", "Last -1 number of keys to sample", 
			false, 500,
			"last -1 number of keys to be randomly generated");
  cmd.add(N);

  ValueArg<size_t> step("s", "step", "step for increment in iteration", 
			false, 1,
			"the step to increase in each iteration");
  cmd.add(step);

  ValueArg<unsigned long> seed("d", "seed", "seed for random number generator", 
			       false, 0,
			       "seed value for initialize random number");
  cmd.add(seed);

  ValueArg<size_t> ins("i", "insert-threshold", 
		       "threshold for calling insertion sort", false, 40,
		       "the threshold for calling insertion sort");
  cmd.add(ins);

  ValueArg<double> sortfact("f", "sort-factor", "shuffling factor",
			    false, 1.0, "% of unordering "
			    "1 ==> permutation is random "
			    "0.x ==> permutation is shuflle in x %");
  cmd.add(sortfact);

  vector<string> allowed;
  allowed.push_back("q"); // pure quicksort
  allowed.push_back("m"); // pure mergesort
  allowed.push_back("qi"); // quicksort with insertion sort
  allowed.push_back("mi"); // mergesort with insertion sort
  allowed.push_back("mc");  // mergesort with test for ordered
  allowed.push_back("qms");
  allowed.push_back("sqms");
  allowed.push_back("i");

  ValuesConstraint<string> allowed_types(allowed);

      // Sorting method type
  MultiArg<string> type("S", "sort-type", "sorting method type", true, 
			&allowed_types);

  ValueArg<size_t> num_samples("m", "num-samples", "number of runnings", false,
			       3, "number of running for for each sample "
			       "the smallest time is chosen");
  cmd.add(num_samples);

  SwitchArg print("p", "print-pars", "print command line parameters", false);
  cmd.add(print);

  SwitchArg header("H", "csv-header", "put csv header with field names", false);
  cmd.add(header);

  SwitchArg all("a", "all", "execute all sorting methods", false);

  cmd.xorAdd(all, type);

  ValueArg<size_t> num_runs("r", "num-runs", "number of runs", false, 1,
			    "number of runs");
  cmd.add(num_runs);

  cmd.parse(argc, argv);

  DynList<string> names;
  for (auto it = type.begin(); it != type.end(); it++)
    names.append(*it);

  // if (all.getValue())
  //   cout << "all es true" << endl;
  // else
  //   cout << "all es false" << endl;

  DynList<Method> mnames;
  if (all.getValue())
    mnames = methods;
  else
    mnames = methods.filter([&names] (const Method & method)
      {
	return names.exists([&method] (const string & name)
          {
	    return name == method.first;
	  });
      });

  if (print.getValue())
    {
      cout << "with header    = " << header.getValue() << endl
	   << "init           = " << n.getValue() << endl
	   << "N              = " << N.getValue() << endl
	   << "step           = " << step.getValue() << endl
	   << "seed           = " << seed.getValue() << endl
	   << "threshold      = " << ins.getValue() << endl
	   << "shuffle factor = " << 100*sortfact.getValue() << " %" << endl
	   << "num-samples    = " << num_samples.getValue() << endl
	   << "Methods: " << endl;
      mnames.for_each([] (const Method & m)
		      {
			cout << "    " << m.first << endl;
		      });
      exit(0);
    }

  insertion_threshold = ins.getValue();
 
  gsl_rng * r = gsl_rng_alloc(gsl_rng_mt19937);
  gsl_rng_set(r, seed.getValue() % gsl_rng_max(r));

  if (header.getValue())
    cout << "n, method.type, insertion.threshold, time" << endl;

  for (size_t i = 0; i < num_runs.getValue(); ++i)
    test(mnames, n.getValue(), N.getValue(), step.getValue(), 
	 sortfact.getValue(), num_samples.getValue(), r);
  
  gsl_rng_free(r);
}

int main(int argc, char **argv)
{
  parser(argc, argv);
}
