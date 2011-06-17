
// loop9 generated by makeloops.py Fri Jun 17 15:34:51 2011

#include <blitz/vector2.h>
#include <blitz/array.h>
#include <random/uniform.h>
#include <blitz/benchext.h>

#ifdef BZ_HAVE_VALARRAY
 #define BENCHMARK_VALARRAY
#endif

#ifdef BENCHMARK_VALARRAY
#include <valarray>
#endif

BZ_NAMESPACE(blitz)
extern void sink();
BZ_NAMESPACE_END

BZ_USING_NAMESPACE(blitz)
BZ_USING_NAMESPACE(std)

#if defined(BZ_FORTRAN_SYMBOLS_WITH_TRAILING_UNDERSCORES)
 #define loop9_f77 loop9_f77_
 #define loop9_f77overhead loop9_f77overhead_
 #define loop9_f90 loop9_f90_
 #define loop9_f90overhead loop9_f90overhead_
#elif defined(BZ_FORTRAN_SYMBOLS_WITH_DOUBLE_TRAILING_UNDERSCORES)
 #define loop9_f77 loop9_f77__
 #define loop9_f77overhead loop9_f77overhead__
 #define loop9_f90 loop9_f90__
 #define loop9_f90overhead loop9_f90overhead__
#elif defined(BZ_FORTRAN_SYMBOLS_CAPS)
 #define loop9_f77 LOOP9_F77
 #define loop9_f77overhead LOOP9_F77OVERHEAD
 #define loop9_f90 LOOP9_F90
 #define loop9_f90overhead LOOP9_F90OVERHEAD
#endif

extern "C" {
  void loop9_f77(const int& N, double* a, double* x);
  void loop9_f77overhead(const int& N, double* a, double* x);
  void loop9_f90(const int& N, double* a, double* x);
  void loop9_f90overhead(const int& N, double* a, double* x);

}

void VectorVersion(BenchmarkExt<int>& bench);
void ArrayVersion(BenchmarkExt<int>& bench);
void doTinyVectorVersion(BenchmarkExt<int>& bench);
void F77Version(BenchmarkExt<int>& bench);
#ifdef FORTRAN_90
void F90Version(BenchmarkExt<int>& bench);
#endif
#ifdef BENCHMARK_VALARRAY
void ValarrayVersion(BenchmarkExt<int>& bench);
#endif

const int numSizes = 20;
const int Nmax=1<<(numSizes-1);
const int tvNmax=7;
const bool runvector=true;

int main()
{
    int numBenchmarks = runvector ? 6 : 5;
#ifndef BENCHMARK_VALARRAY
    numBenchmarks--;   // No  valarray
#endif
#ifndef FORTRAN_90
    numBenchmarks--;   // No fortran 90
#endif

    BenchmarkExt<int> bench("loop9: $x = $x+$a", numBenchmarks);

    bench.setNumParameters(numSizes);

    Array<int,1> parameters(numSizes);
    Array<long,1> iters(numSizes);
    Array<double,1> flops(numSizes);

    for (int i=0; i < numSizes; ++i)
    {
      parameters(i) = Nmax>>i;
      iters(i) = 50000000L / parameters(i);
	
        if (iters(i) < 2)
            iters(i) = 2;
        flops(i) = 2 * parameters(i);
    }

    bench.setParameterVector(parameters);
    bench.setIterations(iters);
    bench.setFlopsPerIteration(flops);

    bench.beginBenchmarking();



    ArrayVersion(bench );
    doTinyVectorVersion(bench );
    F77Version(bench );
#ifdef FORTRAN_90
    F90Version(bench );
#endif
#ifdef BENCHMARK_VALARRAY
    ValarrayVersion(bench );
#endif

    if(runvector)
      VectorVersion(bench );

    bench.endBenchmarking();

    bench.saveMatlabGraph("loop9.m");
    return 0;
}

template<class T>
void initializeRandomDouble(T* data, int numElements, int stride = 1)
{
    ranlib::Uniform<T> rnd;

    for (int i=0; i < numElements; ++i)
        data[size_t(i*stride)] = rnd.random();
}

template<class T>
void initializeRandomDouble(valarray<T>& data, int numElements, int stride = 1)
{
    ranlib::Uniform<T> rnd;

    for (int i=0; i < numElements; ++i)
        data[size_t(i*stride)] = rnd.random();
}

void VectorVersion(BenchmarkExt<int>& bench)
{
    bench.beginImplementation("Vector<T>");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        long iters = bench.getIterations();

        cout << "Vector<T>: N = " << N << endl;

        Vector<double> a(N);
        initializeRandomDouble(a.data(), N);
        Vector<double> x(N);
        initializeRandomDouble(x.data(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = x+a;
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
            sink();
	}

        bench.stopOverhead();
    }

    bench.endImplementation();
}


  void ArrayVersion(BenchmarkExt<int>& bench)
{
    bench.beginImplementation("Array<T,1>");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        long iters = bench.getIterations();

        cout << "Array<T,1>: N = " << N << endl;

        Array<double,1> a(N);
        initializeRandomDouble(a.dataFirst(), N);
        Array<double,1> x(N);
        initializeRandomDouble(x.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = x+a;
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
            sink();
	}

        bench.stopOverhead();
    }

    bench.endImplementation();
}


template<int N>
void TinyVectorVersion(BenchmarkExt<int>& bench)
{
        cout << "Tinyvector<T, " << N << ">" << endl;
        const int sz = bench.getParameter();
        assert(N==sz);
                           
        long iters = bench.getIterations();

        TinyVector<double,N> a(N);
        initializeRandomDouble(a.dataFirst(), N);
        TinyVector<double,N> x(N);
        initializeRandomDouble(x.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = x+a;
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
            sink();
	}
        bench.stopOverhead();

	TinyVectorVersion<N>>1>(bench);
}

// end recursion
template<>
void TinyVectorVersion<0>(BenchmarkExt<int>& bench)
{}

void doTinyVectorVersion(BenchmarkExt<int>& bench)
{
  bench.beginImplementation("TinyVector<T>");
  // can't run tinyvector with full length because meta-unrolling
  // kills compiler...
  int N=Nmax;
  while(N> 1<<tvNmax) {
   bench.getParameter();
   bench.getIterations();
   bench.skip();
   N>>=1;
  }

  TinyVectorVersion< 1<<tvNmax >(bench);
  bench.endImplementation();
}


#ifdef BENCHMARK_VALARRAY
void ValarrayVersion(BenchmarkExt<int>& bench)
{
    bench.beginImplementation("valarray<T>");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        cout << "valarray<T>: N = " << N << endl;

        long iters = bench.getIterations();

        valarray<double> a(N);
        initializeRandomDouble(a, N);
        valarray<double> x(N);
        initializeRandomDouble(x, N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = x+a;
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
	  sink();
	}
        bench.stopOverhead();
    }

    bench.endImplementation();
}
#endif

void F77Version(BenchmarkExt<int>& bench)
{
    bench.beginImplementation("Fortran 77");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        cout << "Fortran 77: N = " << N << endl;

        int iters = bench.getIterations();

        double* a = new double[N];
        initializeRandomDouble(a, N);
        double* x = new double[N];
        initializeRandomDouble(x, N);
        

        bench.start();
        for (int iter=0; iter < iters; ++iter)
            loop9_f77(N, a, x);
        bench.stop();

        bench.startOverhead();
        for (int iter=0; iter < iters; ++iter)
            loop9_f77overhead(N, a, x);

        bench.stopOverhead();

        delete [] a;
        delete [] x;

    }

    bench.endImplementation();
}

#ifdef FORTRAN_90
void F90Version(BenchmarkExt<int>& bench)
{
    bench.beginImplementation("Fortran 90");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        cout << "Fortran 90: N = " << N << endl;

        int iters = bench.getIterations();

        double* a = new double[N];
        initializeRandomDouble(a, N);
        double* x = new double[N];
        initializeRandomDouble(x, N);


        bench.start();
        for (int iter=0; iter < iters; ++iter)
            loop9_f90(N, a, x);
        bench.stop();

        bench.startOverhead();
        for (int iter=0; iter < iters; ++iter)
            loop9_f90overhead(N, a, x);

        bench.stopOverhead();
        delete [] a;
        delete [] x;

    }

    bench.endImplementation();
}
#endif

