// @HEADER
// ***********************************************************************
// 
//                           Stokhos Package
//                 Copyright (2009) Sandia Corporation
// 
// Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
// license for use of this work by or on behalf of the U.S. Government.
// 
// This library is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 2.1 of the
// License, or (at your option) any later version.
//  
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//  
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
// Questions? Contact Eric T. Phipps (etphipp@sandia.gov).
// 
// ***********************************************************************
// @HEADER

#ifndef SACADO_PCE_SCALARTRAITSIMP_HPP
#define SACADO_PCE_SCALARTRAITSIMP_HPP

#ifdef HAVE_SACADO_TEUCHOS

#include "Teuchos_ScalarTraits.hpp"
#include "Teuchos_SerializationTraits.hpp"
#include "Teuchos_Assert.hpp"
#include "Sacado_mpl_apply.hpp"

#include <iterator>

namespace Sacado {

  namespace PCE {

    //! Implementation for Teuchos::ScalarTraits for all PCE types
    template <typename PCEType>
    struct ScalarTraitsImp {
      typedef typename Sacado::ValueType<PCEType>::type ValueT;

      typedef typename mpl::apply<PCEType,typename Teuchos::ScalarTraits<ValueT>::magnitudeType>::type magnitudeType;
      typedef typename mpl::apply<PCEType,typename Teuchos::ScalarTraits<ValueT>::halfPrecision>::type halfPrecision;
      typedef typename mpl::apply<PCEType,typename Teuchos::ScalarTraits<ValueT>::doublePrecision>::type doublePrecision;

      static const bool isComplex = Teuchos::ScalarTraits<ValueT>::isComplex;
      static const bool isOrdinal = Teuchos::ScalarTraits<ValueT>::isOrdinal;
      static const bool isComparable = 
	Teuchos::ScalarTraits<ValueT>::isComparable;
      static const bool hasMachineParameters = 
	Teuchos::ScalarTraits<ValueT>::hasMachineParameters;
      static typename Teuchos::ScalarTraits<ValueT>::magnitudeType eps() {
	return Teuchos::ScalarTraits<ValueT>::eps();
      }
      static typename Teuchos::ScalarTraits<ValueT>::magnitudeType sfmin() {
	return Teuchos::ScalarTraits<ValueT>::sfmin();
      }
      static typename Teuchos::ScalarTraits<ValueT>::magnitudeType base()  {
	return Teuchos::ScalarTraits<ValueT>::base();
      }
      static typename Teuchos::ScalarTraits<ValueT>::magnitudeType prec()  {
	return Teuchos::ScalarTraits<ValueT>::prec();
      }
      static typename Teuchos::ScalarTraits<ValueT>::magnitudeType t()     {
	return Teuchos::ScalarTraits<ValueT>::t();
      }
      static typename Teuchos::ScalarTraits<ValueT>::magnitudeType rnd()   {
	return Teuchos::ScalarTraits<ValueT>::rnd();
      }
      static typename Teuchos::ScalarTraits<ValueT>::magnitudeType emin()  {
	return Teuchos::ScalarTraits<ValueT>::emin();
      }
      static typename Teuchos::ScalarTraits<ValueT>::magnitudeType rmin()  {
	return Teuchos::ScalarTraits<ValueT>::rmin();
      }
      static typename Teuchos::ScalarTraits<ValueT>::magnitudeType emax()  {
	return Teuchos::ScalarTraits<ValueT>::emax();
      }
      static typename Teuchos::ScalarTraits<ValueT>::magnitudeType rmax()  {
	return Teuchos::ScalarTraits<ValueT>::rmax();
      }
      static magnitudeType magnitude(const PCEType& a) {
#ifdef TEUCHOS_DEBUG
	TEUCHOS_SCALAR_TRAITS_NAN_INF_ERR(
	  a, "Error, the input value to magnitude(...) a = " << a << 
	  " can not be NaN!" );
	TEUCHOS_TEST_FOR_EXCEPTION(is_pce_real(a) == false, std::runtime_error,
			   "Complex magnitude is not a differentiable "
			   "function of complex inputs.");
#endif
	//return std::fabs(a); 
	magnitudeType b(a.expansion());
	b.val() = Teuchos::ScalarTraits<ValueT>::magnitude(a.val());
	if (Teuchos::ScalarTraits<ValueT>::real(a.val()) >= 0)
	  for (int i=1; i<a.size(); i++)
	    b.fastAccessCoeff(i) = 
	      Teuchos::ScalarTraits<ValueT>::magnitude(a.fastAccessCoeff(i));
	else
	  for (int i=1; i<a.size(); i++)
	    b.fastAccessCoeff(i) = 
	      -Teuchos::ScalarTraits<ValueT>::magnitude(a.fastAccessCoeff(i));
	return b;
      }
      static ValueT zero()  { 
	return ValueT(0.0); 
      }
      static ValueT one()   { 
	return ValueT(1.0); 
      }
      
      // Conjugate is only defined for real derivative components
      static PCEType conjugate(const PCEType& x) {
#ifdef TEUCHOS_DEBUG
	TEUCHOS_TEST_FOR_EXCEPTION(is_pce_real(x) == false, std::runtime_error,
			   "Complex conjugate is not a differentiable "
			   "function of complex inputs.");
#endif
	PCEType y = x;
	y.copyForWrite();
	y.val() = Teuchos::ScalarTraits<ValueT>::conjugate(x.val());
	return y;
      }   

      // Real part is only defined for real derivative components
      static PCEType real(const PCEType& x) { 
#ifdef TEUCHOS_DEBUG
	TEUCHOS_TEST_FOR_EXCEPTION(is_pce_real(x) == false, std::runtime_error,
			   "Real component is not a differentiable "
			   "function of complex inputs.");
#endif
	PCEType y = x;
	y.copyForWrite();
	y.val() = Teuchos::ScalarTraits<ValueT>::real(x.val());
	return y;
      }

      // Imaginary part is only defined for real derivative components
      static PCEType imag(const PCEType& x) { 
#ifdef TEUCHOS_DEBUG
	TEUCHOS_TEST_FOR_EXCEPTION(is_pce_real(x) == false, std::runtime_error,
			   "Imaginary component is not a differentiable "
			   "function of complex inputs.");
#endif
	return PCEType(Teuchos::ScalarTraits<ValueT>::imag(x.val()));
      }

      static ValueT nan() {
	return Teuchos::ScalarTraits<ValueT>::nan(); 
      }
      static bool isnaninf(const PCEType& x) { 
	for (int i=0; i<x.size(); i++)
	  if (Teuchos::ScalarTraits<ValueT>::isnaninf(x.fastAccessCoeff(i)))
	    return true;
	return false;
      }
      static void seedrandom(unsigned int s) { 
	Teuchos::ScalarTraits<ValueT>::seedrandom(s); 
      }
      static ValueT random() { 
	return Teuchos::ScalarTraits<ValueT>::random(); 
      }
      static std::string name() { 
	return Sacado::StringName<PCEType>::eval(); 
      }
      static PCEType squareroot(const PCEType& x) {
#ifdef TEUCHOS_DEBUG
	TEUCHOS_SCALAR_TRAITS_NAN_INF_ERR(
	  x, "Error, the input value to squareroot(...) a = " << x << 
	  " can not be NaN!" );
#endif
	return std::sqrt(x); 
      }
      static PCEType pow(const PCEType& x, const PCEType& y) { 
	return std::pow(x,y); 
      }

      // Helper function to determine whether a complex value is real
      static bool is_complex_real(const ValueT& x) {
	return 
	  Teuchos::ScalarTraits<ValueT>::magnitude(x-Teuchos::ScalarTraits<ValueT>::real(x)) == 0;
      }

      // Helper function to determine whether a Fad type is real
      static bool is_pce_real(const PCEType& x) {
	if (x.size() == 0)
	  return true;
	if (Teuchos::ScalarTraits<ValueT>::isComplex) {
	  for (int i=0; i<x.size(); i++)
	    if (!is_complex_real(x.fastAccessCoeff(i)))
	      return false;
	}
	return true;
      }

    }; // class ScalarTraitsImp


    //! Implementation of Teuchos::SerializationTraits for all PCE types
    template <typename Ordinal, typename PCEType>
    class SerializationTraitsImp {
      typedef typename Sacado::ValueType<PCEType>::type ValueT;
      typedef Teuchos::SerializationTraits<Ordinal,ValueT> vSerT;
      typedef Teuchos::SerializationTraits<Ordinal,int> iSerT;
      typedef Teuchos::SerializationTraits<Ordinal,Ordinal> oSerT;

    public:

      /// \brief Whether the type T supports direct serialization.
      static const bool supportsDirectSerialization = false;

      //! @name Indirect serialization functions (always defined and supported) 
      //@{

      /** \brief Return the number of bytes for <tt>count</tt> objects. */
      static Ordinal fromCountToIndirectBytes(const Ordinal count, 
					      const PCEType buffer[]) { 
	Ordinal bytes = 0;
	for (Ordinal i=0; i<count; i++) {
	  int sz = buffer[i].size();
	  Ordinal b1 = iSerT::fromCountToIndirectBytes(1, &sz);
	  Ordinal b2 = vSerT::fromCountToIndirectBytes(sz, buffer[i].coeff());
	  Ordinal b3 = oSerT::fromCountToIndirectBytes(1, &b2);
	  bytes += b1+b2+b3;
	}
	return bytes;
      }

      /** \brief Serialize to an indirect <tt>char[]</tt> buffer. */
      static void serialize (const Ordinal count, 
			     const PCEType buffer[], 
			     const Ordinal bytes, 
			     char charBuffer[]) { 
	for (Ordinal i=0; i<count; i++) {
	  // First serialize size
	  int sz = buffer[i].size();
	  Ordinal b1 = iSerT::fromCountToIndirectBytes(1, &sz);
	  iSerT::serialize(1, &sz, b1, charBuffer);
	  charBuffer += b1;
	
	  // Next serialize PCE coefficients
	  Ordinal b2 = vSerT::fromCountToIndirectBytes(sz, buffer[i].coeff());
	  Ordinal b3 = oSerT::fromCountToIndirectBytes(1, &b2);
	  oSerT::serialize(1, &b2, b3, charBuffer); 
	  charBuffer += b3;
	  vSerT::serialize(sz, buffer[i].coeff(), b2, charBuffer);
	  charBuffer += b2;
	}
      }

      /** \brief Return the number of objects for <tt>bytes</tt> of storage. */
      static Ordinal fromIndirectBytesToCount(const Ordinal bytes, 
					      const char charBuffer[]) {
	Ordinal count = 0;
	Ordinal bytes_used = 0;
	while (bytes_used < bytes) {
	
	  // Bytes for size
	  Ordinal b1 = iSerT::fromCountToDirectBytes(1);
	  bytes_used += b1;
	  charBuffer += b1;
	
	  // Bytes for PCE coefficients
	  Ordinal b3 = oSerT::fromCountToDirectBytes(1);
	  const Ordinal *b2 = oSerT::convertFromCharPtr(charBuffer);
	  bytes_used += b3;
	  charBuffer += b3;
	  bytes_used += *b2;
	  charBuffer += *b2;
	
	  ++count;
	}
	return count;
      }

      /** \brief Deserialize from an indirect <tt>char[]</tt> buffer. */
      static void deserialize (const Ordinal bytes, 
			       const char charBuffer[], 
			       const Ordinal count, 
			       PCEType buffer[]) { 
	for (Ordinal i=0; i<count; i++) {
	
	  // Deserialize size
	  Ordinal b1 = iSerT::fromCountToDirectBytes(1);
	  const int *sz = iSerT::convertFromCharPtr(charBuffer);
	  charBuffer += b1;

	  // Make sure PCE object is ready to receive values
	  // We assume it has already been initialized with the proper
	  // expansion object
	  if (buffer[i].size() != *sz)
	    buffer[i].reset(buffer[i].expansion(), *sz);
	  buffer[i].copyForWrite();
	
	  // Deserialize PCE coefficients
	  Ordinal b3 = oSerT::fromCountToDirectBytes(1);
	  const Ordinal *b2 = oSerT::convertFromCharPtr(charBuffer);
	  charBuffer += b3;
	  vSerT::deserialize(*b2, charBuffer, *sz, buffer[i].coeff());
	  charBuffer += *b2;
	}
      
      }
  
      //@}
      
    };


    //! Serializer object for all PCE types
    template <typename Ordinal, typename PCEType, typename ValueSerializer>
    class SerializerImp {

    protected:
      typedef typename Sacado::ValueType<PCEType>::type ValueT;
      typedef Teuchos::SerializationTraits<Ordinal,int> iSerT;
      typedef Teuchos::SerializationTraits<Ordinal,Ordinal> oSerT;
      typedef typename PCEType::expansion_type expansion_type;
      Teuchos::RCP<expansion_type> expansion;
      Teuchos::RCP<const ValueSerializer> vs;

    public:

      /// \brief Whether the type T supports direct serialization.
      static const bool supportsDirectSerialization = false;

      SerializerImp(const Teuchos::RCP<expansion_type>& expansion_,
		    const Teuchos::RCP<const ValueSerializer>& vs_) :
	expansion(expansion_), vs(vs_) {}

      //! @name Indirect serialization functions (always defined and supported) 
      //@{

      /** \brief Return the number of bytes for <tt>count</tt> objects. */
      Ordinal fromCountToIndirectBytes(const Ordinal count, 
				       const PCEType buffer[]) const { 
	Ordinal bytes = 0;
	for (Ordinal i=0; i<count; i++) {
	  int sz = buffer[i].size();
	  Ordinal b1 = iSerT::fromCountToIndirectBytes(1, &sz);
	  Ordinal b2 = vs->fromCountToIndirectBytes(sz, buffer[i].coeff());
	  Ordinal b3 = oSerT::fromCountToIndirectBytes(1, &b2);
	  bytes += b1+b2+b3;
	}
	return bytes;
      }

      /** \brief Serialize to an indirect <tt>char[]</tt> buffer. */
      void serialize (const Ordinal count, 
		      const PCEType buffer[], 
		      const Ordinal bytes, 
		      char charBuffer[]) const { 
	for (Ordinal i=0; i<count; i++) {
	  // First serialize size
	  int sz = buffer[i].size();
	  Ordinal b1 = iSerT::fromCountToIndirectBytes(1, &sz);
	  iSerT::serialize(1, &sz, b1, charBuffer);
	  charBuffer += b1;
	
	  // Next serialize PCE coefficients
	  Ordinal b2 = vs->fromCountToIndirectBytes(sz, buffer[i].coeff());
	  Ordinal b3 = oSerT::fromCountToIndirectBytes(1, &b2);
	  oSerT::serialize(1, &b2, b3, charBuffer); 
	  charBuffer += b3;
	  vs->serialize(sz, buffer[i].coeff(), b2, charBuffer);
	  charBuffer += b2;
	}
      }

      /** \brief Return the number of objects for <tt>bytes</tt> of storage. */
      Ordinal fromIndirectBytesToCount(const Ordinal bytes, 
				       const char charBuffer[]) const {
	Ordinal count = 0;
	Ordinal bytes_used = 0;
	while (bytes_used < bytes) {
	
	  // Bytes for size
	  Ordinal b1 = iSerT::fromCountToDirectBytes(1);
	  bytes_used += b1;
	  charBuffer += b1;
	
	  // Bytes for PCE coefficients
	  Ordinal b3 = oSerT::fromCountToDirectBytes(1);
	  const Ordinal *b2 = oSerT::convertFromCharPtr(charBuffer);
	  bytes_used += b3;
	  charBuffer += b3;
	  bytes_used += *b2;
	  charBuffer += *b2;
	
	  ++count;
	}
	return count;
      }

      /** \brief Deserialize from an indirect <tt>char[]</tt> buffer. */
      void deserialize (const Ordinal bytes, 
			const char charBuffer[], 
			const Ordinal count, 
			PCEType buffer[]) const { 
	for (Ordinal i=0; i<count; i++) {
	
	  // Deserialize size
	  Ordinal b1 = iSerT::fromCountToDirectBytes(1);
	  const int *sz = iSerT::convertFromCharPtr(charBuffer);
	  charBuffer += b1;
	
	  // Create empty PCE object of given size
	  buffer[i] = PCEType(expansion, *sz);
	
	  // Deserialize PCE coefficients
	  Ordinal b3 = oSerT::fromCountToDirectBytes(1);
	  const Ordinal *b2 = oSerT::convertFromCharPtr(charBuffer);
	  charBuffer += b3;
	  vs->deserialize(*b2, charBuffer, *sz, buffer[i].coeff());
	  charBuffer += *b2;
	}
      
      }
  
      //@}
      
    };

  } // namespace PCE

} // namespace Sacado

#endif // HAVE_SACADO_TEUCHOS

#endif // SACADO_FAD_SCALARTRAITSIMP_HPP