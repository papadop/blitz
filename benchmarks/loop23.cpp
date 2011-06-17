
// loop23 generated by makeloops.py Fri Jun 17 15:34:51 2011

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
 #define loop23_f77 loop23_f77_
 #define loop23_f77overhead loop23_f77overhead_
 #define loop23_f90 loop23_f90_
 #define loop23_f90overhead loop23_f90overhead_
#elif defined(BZ_FORTRAN_SYMBOLS_WITH_DOUBLE_TRAILING_UNDERSCORES)
 #define loop23_f77 loop23_f77__
 #define loop23_f77overhead loop23_f77overhead__
 #define loop23_f90 loop23_f90__
 #define loop23_f90overhead loop23_f90overhead__
#elif defined(BZ_FORTRAN_SYMBOLS_CAPS)
 #define loop23_f77 LOOP23_F77
 #define loop23_f77overhead LOOP23_F77OVERHEAD
 #define loop23_f90 LOOP23_F90
 #define loop23_f90overhead LOOP23_F90OVERHEAD
#endif

extern "C" {
  void loop23_f77(const int& N, double* x, double* a, double* b, double* c, double* d, double* y);
  void loop23_f77overhead(const int& N, double* x, double* a, double* b, double* c, double* d, double* y);
  void loop23_f90(const int& N, double* x, double* a, double* b, double* c, double* d, double* y);
  void loop23_f90overhead(const int& N, double* x, double* a, double* b, double* c, double* d, double* y);

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

    BenchmarkExt<int> bench("loop23: $x = $a*$b + $c*$d; $y = $b+$d", numBenchmarks);

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

    bench.saveMatlabGraph("loop23.m");
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

        Vector<double> x(N);
        initializeRandomDouble(x.data(), N);
        Vector<double> a(N);
        initializeRandomDouble(a.data(), N);
        Vector<double> b(N);
        initializeRandomDouble(b.data(), N);
        Vector<double> c(N);
        initializeRandomDouble(c.data(), N);
        Vector<double> d(N);
        initializeRandomDouble(d.data(), N);
        Vector<double> y(N);
        initializeRandomDouble(y.data(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = a*b + c*d; y = b+d;
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

        Array<double,1> x(N);
        initializeRandomDouble(x.dataFirst(), N);
        Array<double,1> a(N);
        initializeRandomDouble(a.dataFirst(), N);
        Array<double,1> b(N);
        initializeRandomDouble(b.dataFirst(), N);
        Array<double,1> c(N);
        initializeRandomDouble(c.dataFirst(), N);
        Array<double,1> d(N);
        initializeRandomDouble(d.dataFirst(), N);
        Array<double,1> y(N);
        initializeRandomDouble(y.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = a*b + c*d; y = b+d;
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

        TinyVector<double,N> x(N);
        initializeRandomDouble(x.dataFirst(), N);
        TinyVector<double,N> a(N);
        initializeRandomDouble(a.dataFirst(), N);
        TinyVector<double,N> b(N);
        initializeRandomDouble(b.dataFirst(), N);
        TinyVector<double,N> c(N);
        initializeRandomDouble(c.dataFirst(), N);
        TinyVector<double,N> d(N);
        initializeRandomDouble(d.dataFirst(), N);
        TinyVector<double,N> y(N);
        initializeRandomDouble(y.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = a*b + c*d; y = b+d;
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

        valarray<double> x(N);
        initializeRandomDouble(x, N);
        valarray<double> a(N);
        initializeRandomDouble(a, N);
        valarray<double> b(N);
        initializeRandomDouble(b, N);
        valarray<double> c(N);
        initializeRandomDouble(c, N);
        valarray<double> d(N);
        initializeRandomDouble(d, N);
        valarray<double> y(N);
        initializeRandomDouble(y, N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = a*b + c*d; y = b+d;
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

        double* x = new double[N];
        initializeRandomDouble(x, N);
        double* a = new double[N];
        initializeRandomDouble(a, N);
        double* b = new double[N];
        initializeRandomDouble(b, N);
        double* c = new double[N];
        initializeRandomDouble(c, N);
        double* d = new double[N];
        initializeRandomDouble(d, N);
        double* y = new double[N];
        initializeRandomDouble(y, N);
        

        bench.start();
        for (int iter=0; iter < iters; ++iter)
            loop23_f77(N, x, a, b, c, d, y);
        bench.stop();

        bench.startOverhead();
        for (int iter=0; iter < iters; ++iter)
            loop23_f77overhead(N, x, a, b, c, d, y);

        bench.stopOverhead();

        delete [] x;
        delete [] a;
        delete [] b;
        delete [] c;
        delete [] d;
        delete [] y;

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

        double* x = new double[N];
        initializeRandomDouble(x, N);
        double* a = new double[N];
        initializeRandomDouble(a, N);
        double* b = new double[N];
        initializeRandomDouble(b, N);
        double* c = new double[N];
        initializeRandomDouble(c, N);
        double* d = new double[N];
        initializeRandomDouble(d, N);
        double* y = new double[N];
        initializeRandomDouble(y, N);


        bench.start();
        for (int iter=0; iter < iters; ++iter)
            loop23_f90(N, x, a, b, c, d, y);
        bench.stop();

        bench.startOverhead();
        for (int iter=0; iter < iters; ++iter)
            loop23_f90overhead(N, x, a, b, c, d, y);

        bench.stopOverhead();
        delete [] x;
        delete [] a;
        delete [] b;
        delete [] c;
        delete [] d;
        delete [] y;

    }

    bench.endImplementation();
}
#endif

