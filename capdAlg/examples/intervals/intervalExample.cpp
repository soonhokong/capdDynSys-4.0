/*
   author: Tomasz Kapela, Sept. 2007
   the file provides a quick tutorial on using template class Interval
*/
#include <iostream>
#include <iomanip>
#include "capd/intervals/Interval.hpp"
#include "capd/rounding/DoubleRounding.h"
#include "capd/rounding/IntRounding.h"

// template class Interval has two parameters
// type of endpoints and class that switches rounding modes
// The following lines defines new names for intervals with endpoints type: double, int, MpFloat correspondigly
typedef capd::intervals::Interval< double, capd::rounding::DoubleRounding>  DInterval;
typedef capd::intervals::Interval< int, capd::rounding::IntRounding> ZInterval;
// For the next line to work you need CAPD library compiled with Multiple Precission support 
// typedef capd::DIntervals::Interval< capd::MpFloat> MPInterval;
// Because most of the functions are common for intervals with arbitrary endpoints 
// we use as an example  DInterval in the following sections.
using namespace std;
void basicsTest(){
  // We construct five intervals
  DInterval a;                   // a = [0.0, 0.0] (*)
  DInterval b(1.0);              // b = [1.0, 1.0]
  DInterval c(2.0, 3.0);         // c = [2.0, 3.0]
  DInterval d(c);                // d = [2.0, 3.0]
  DInterval e("2.5","3.0");      // e = [2.4999999999999996, 3.0000000000000004]
  // These functions return endpoints of an DInterval as double or as zero width DInterval
   // The output depends on parameters of a stream e.g precision.
   // Interval bounds are rounded to the nearest floating point with given
   // number of decimal places
   cout << "\n\n Output with various precisions : ";
   DInterval x(1.0, 2.0);
   cout << x << "  ";                          // output: [1,2]
   cout << fixed << setprecision(6) << x;      // output: [1.000000,2.000000]

   // To save and then restore the same interval you can set high enough precision (at least 17 for doubles)
   // or use bit or hex format for text streams or binary format.

   x = DInterval(-1.0,2.0);
   cout << "\n\n [-1,2] in bit format : ";
   bitWrite(cout, x);
   cout << "\n [-1,2] in hex format : ";
   hexWrite(cout, x) << endl;

   std::stringstream inout("", ios::binary | ios::in | ios::out );
   binWrite(inout, x);
   binRead(inout, a);
   cout << "\n [-1,2] written and read from binary stream : " << a;


   DInterval ia(1.0, 10.0), ib(3.0, 5.0);
   cout << "\n\n imin([1,10],[3,5]) = " << imin(ia,ib);                  // result:  [1.0, 5.0]
   cout << "\n imax([1,10],[3,5]) = " << imax(ia,ib);                  // result:  [3.0, 10.0]


   // We split DInterval c into two intervals: center a and zero centered DInterval b,
   // We can also split interval into center and radius
}
 
void arithmeticOperatorsTest(){
void functionsTest(){
   DInterval y(1.0, 2.0);
   y = DInterval(-3, -2);
   y = DInterval(4, 9);
int main(int, char**){
  } catch(capd::intervals::IntervalError<double> &e){

