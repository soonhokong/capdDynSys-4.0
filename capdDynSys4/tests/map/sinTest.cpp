//////////////////////////////////////////////////////////////////////////////
//   Package:          CAPD

/////////////////////////////////////////////////////////////////////////////
//
/// @file sinTest.cpp
///
/// @author Daniel Wilczak
//
/////////////////////////////////////////////////////////////////////////////

// Copyright (C) CAPD group
//
// This file constitutes a part of the CAPD library,
// distributed under the terms of the GNU General Public License.
// Consult  http://capd.ii.uj.edu.pl/ for details.

//#define BOOST_TEST_MODULE sinTest
#include "compare.h"
BOOST_AUTO_TEST_SUITE(sinSuite)

std::vector<double> computeSinDer(MapType::VectorType & u){
  double x = u[0].leftBound();
  double y = u[1].leftBound();

  // code generated by the following Mathematica code
  // W[n_,m_]:=D[Sin[x*y],{x,n},{y,m}]/(n!m!)//FullSimplify
  // Table[Table[W[m-n,n]//CForm,{n,0,m}],{m,0,5}]//Flatten

  capd::rounding::DoubleRounding::roundNearest();
  double r[] = {Sin(x*y),y*Cos(x*y),x*Cos(x*y),-(Power(y,2)*Sin(x*y))/2.,Cos(x*y) - x*y*Sin(x*y),-(Power(x,2)*Sin(x*y))/2.,-(Power(y,3)*Cos(x*y))/6.,-(y*(x*y*Cos(x*y) + 2*Sin(x*y)))/2.,-(x*(x*y*Cos(x*y) + 2*Sin(x*y)))/2.,-(Power(x,3)*Cos(x*y))/6.,(Power(y,4)*Sin(x*y))/24.,(Power(y,2)*(-3*Cos(x*y) + x*y*Sin(x*y)))/6.,-(x*y*Cos(x*y)) + ((-2 + Power(x,2)*Power(y,2))*Sin(x*y))/4.,(Power(x,2)*(-3*Cos(x*y) + x*y*Sin(x*y)))/6.,(Power(x,4)*Sin(x*y))/24.,(Power(y,5)*Cos(x*y))/120.,(Power(y,3)*(x*y*Cos(x*y) + 4*Sin(x*y)))/24.,(y*((-6 + Power(x,2)*Power(y,2))*Cos(x*y) + 6*x*y*Sin(x*y)))/12.,(x*((-6 + Power(x,2)*Power(y,2))*Cos(x*y) + 6*x*y*Sin(x*y)))/12.,(Power(x,3)*(x*y*Cos(x*y) + 4*Sin(x*y)))/24.,(Power(x,5)*Cos(x*y))/120.};
  return std::vector<double> (r,r+sizeof(r)/sizeof(double));
}

BOOST_AUTO_TEST_CASE(xsin)
{
  std::string txt = "var:x,y;fun:sin(x*y);",
              msg = "Function \"" + txt + "\"  x = " ;
  MapType f(txt,5);
  VectorType x(2);
  x[0] = .5; x[1] = 3.;
  std::vector<double> expected = computeSinDer(x);
  JetType df(1,2,5);
  f(x,df);

  compareResults(expected, df, msg+"(0.5,3.)");

  MapType g("var:x,y;fun:-sin(-x*y);",5);
  g(x,df);
  compareResults(expected, df, msg+"(0.5,3.)");

  x[0] = -0.75; x[1] = 1.;
  expected = computeSinDer(x);
  f(x,df);
  compareResults(expected, df, msg+"(-0.75,1)");

  x[0] = 0.0; x[1] = 0.0;
  expected = computeSinDer(x);
  f(x,df);
  compareResults(expected, df, msg+"(0.0,0.0)");

}

using capd::autodiff::Node;

void _f(Node t, Node in[], int dimIn, Node out[], int dimOut, Node params[], int noParams)
{
  out[0] = -sin(-in[0]*in[1]);
}

BOOST_AUTO_TEST_CASE(xsinnode)
{
  std::string msg = "Function \"-sin(-x*y)\"  x = " ;
  MapType f(_f,2,1,0,5);
  VectorType x(2);
  x[0] = .125; x[1]=0.25;
  std::vector<double> expected = computeSinDer(x);
  JetType df(1,2,5);
  f(x,df);
  compareResults(expected, df, msg+"(0.125,0.25)");

  x[0] = -0.75; x[1]=2;
  expected = computeSinDer(x);
  f(x,df);
  compareResults(expected, df, msg+"(-0.75,2.)");

  x[0] = 0.0; x[1] = 0.0;
  expected = computeSinDer(x);
  f(x,df);
  compareResults(expected, df, msg+"(0.0,0.0)");
}

BOOST_AUTO_TEST_SUITE_END()