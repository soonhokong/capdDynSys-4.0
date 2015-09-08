/// @addtogroup dynsys
/// @{

/////////////////////////////////////////////////////////////////////////////
/// @file CnSolver_templateMembers.h
///
/// @author Daniel Wilczak
/////////////////////////////////////////////////////////////////////////////

// Copyright (C) 2000-2014 by the CAPD Group.
//
// This file constitutes a part of the CAPD library,
// distributed under the terms of the GNU General Public License.
// Consult  http://capd.ii.uj.edu.pl/ for details.

#ifndef _CAPD_DYNSYS_CNSOLVER_TEMPLATEMEMBERS_H_
#define _CAPD_DYNSYS_CNSOLVER_TEMPLATEMEMBERS_H_

#include "capd/dynsys/CnSolver.h"
#include "capd/dynsys/enclosure.h"
#include "capd/dynsys/approveRemainder.h"

namespace capd{
namespace dynsys{

// ####################################################################

template<typename MapType,typename StepControlT, typename CurveT>
template<class JetT>
typename  CnSolver<MapType,StepControlT,CurveT>::VectorType
CnSolver<MapType,StepControlT,CurveT>::encloseCnMap(
        const ScalarType& t,
        const VectorType& x,
        const VectorType& xx,
        JetT& phi,
        JetT& rem,
        JetT& enc
      )
{
  size_type i,j;
  this->setInitialCondition(t,x,xx);
  this->m_vField->computeODECoefficients(this->getCoefficients(),phi.degree(),this->getOrder());
  this->m_vField->computeODECoefficients(this->getCoefficientsAtCenter(),this->getOrder());

  capd::dynsys::computeAndApproveRemainder(*this,t,xx,phi,rem,enc);

  VectorType v = this->getCoefficientsAtCenter()[this->getOrder()];
  for(int r = this->getOrder() - 1; r >= 0; --r)
    capd::vectalg::multiplyAssignObjectScalarAddObject(v,this->m_step,this->getCoefficientsAtCenter()[r]);

  return v;
}

// ####################################################################

template<typename MapType,typename StepControlT, typename CurveT>
template<class JetT>
void CnSolver<MapType,StepControlT,CurveT>::sumTaylorSeries(JetT& phi)
{
  size_type i,j;
  for(i=0;i<this->dimension();++i) {
    ScalarType* p = this->getCoefficients()[this->getOrder()].begin(i);
    typename JetT::iterator b = phi.begin(i), e = phi.end(i);
    for(;b!=e;++b,++p)
      *b = *p;
    for(int r = this->getOrder()-1;r>=0;--r){
      ScalarType* p = this->getCoefficients()[r].begin(i);
      typename JetT::iterator b = phi.begin(i), e = phi.end(i);
      for(;b!=e;++b,++p)
        *b = (*b)*this->m_step + (*p);
    }
  }
}

//###########################################################//

template<typename MapType, typename StepControlType,typename CurveType>
template<class JetT>
void CnSolver<MapType,StepControlType,CurveType>::cnRemainder(const JetT& enc, JetT& result)
{
  size_type degree = result.degree();
  for(size_type i=0;i<this->dimension();++i){
    typename JetT::const_iterator b = enc.begin(i), e=enc.end(i);
    ScalarType* p = &this->remainderCoefficient(i,0);
    while(b!=e)
    {
      *p = *b;
      b++;
      p++;
    }
  }

  this->m_vField->computeODECoefficients(this->getRemainderCoefficients(),degree,this->getOrder()+1);

  ScalarType factor = power(this->m_step,this->getOrder()+1);
  for(size_type i=0;i<this->dimension();++i)
  {
    ScalarType* p = &this->remainderCoefficient(i,this->getOrder()+1);
    typename JetT::iterator b = result.begin(i), e = result.end(i);
    for(;b!=e;++b,++p)
      *b = (*p)*factor;
  }
}

//###########################################################//

template<typename MapT, typename StepControlT,typename CurveT>
template<class JetT>
typename CnSolver<MapT,StepControlT,CurveT>::VectorType
CnSolver<MapT,StepControlT,CurveT>::cnEnclosure(const ScalarType& t, const VectorType& x, JetT& result)
{
  VectorType enc = this->enclosure(t,x);
  ScalarType logNormOfDerivative;
  MatrixType jacEnc = capd::dynsys::jacEnclosure(*(this->m_vField),t,this->m_step,enc,capd::vectalg::EuclLNorm<VectorType,MatrixType>(),&logNormOfDerivative);

  result() = enc;
  result.setMatrix(jacEnc);

  // computation of rough enclosure for C^d part, d=2,...,degree
  size_type stride = binomial(result.dimension()+result.degree(),result.degree());
  for(size_type i=2;i<=result.degree();++i)
  {
    this->m_vField->homogenousPolynomial(result,i);
    typename JetT::iterator b = result.begin(0,i), e = result.end(0,i);

    for(;b!=e;++b)
    {
      typename JetT::RefVectorType refVector(b,stride,this->dimension());
      ScalarType delta = refVector.euclNorm().rightBound();
      typename TypeTraits<ScalarType>::Real size;
      if(subset(ScalarType(0.0),logNormOfDerivative))
        size = abs(delta*abs(m_step).rightBound()).rightBound();
      else
        size = (abs(delta * (exp(logNormOfDerivative*m_step)-ScalarType(1.))/logNormOfDerivative)).rightBound();
      ScalarType res(-size,size);
      for(size_type j=0;j<dimension();++j)
        refVector[j] = res;
    }
  }
  return enc;
}


// ---------------------------------------------------------------------------------------

template <typename MapT, typename StepControlT,typename CurveT>
template<class JetT>
void CnSolver<MapT, StepControlT,CurveT>::computeRemainder(ScalarType t, const VectorType& xx, JetT& o_enc, JetT& o_rem)
{
  this->cnEnclosure(t,xx,o_enc);
  this->cnRemainder(o_enc,o_rem);
}

}}

#endif // _CAPD_DYNSYS_CNSOLVER_TEMPLATEMEMBERS_H_

/// @}
