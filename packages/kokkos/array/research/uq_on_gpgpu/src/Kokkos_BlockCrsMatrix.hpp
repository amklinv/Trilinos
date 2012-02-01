/*
//@HEADER
// ************************************************************************
// 
//                         Kokkos Array
//              Copyright (2012) Sandia Corporation
// 
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact H. Carter Edwards (hcedwar@sandia.gov)
// 
// ************************************************************************
//@HEADER
*/

#ifndef KOKKOS_BLOCKCRSMATRIX_HPP
#define KOKKOS_BLOCKCRSMATRIX_HPP

#include <Kokkos_CrsMap.hpp>
#include <Kokkos_MultiVector.hpp>

namespace Kokkos {

/** \brief  CRS matrix of dense blocks.
 *
 *  Matrix coefficients are stored by block and then by Crs entry.
 *    m_values( block.size() , m_graph.entry_count() )
 *
 *  Vectors are conformally stored as
 *    MultiVector( block.dimension() , m_graph.row_count() )
 */
template< class BlockSpec , typename ValueType , class Device >
class BlockCrsMatrix {
public:
  typedef Device     device_type ;
  typedef ValueType  value_type ;
  typedef BlockSpec  block_spec ;

  MultiVector< value_type, device_type >  values ;
  CrsMap< device_type >                   graph ;
  block_spec                              block ;
};

namespace Impl {

template< class A , class X , class Y >
class Multiply ;

}

template< class BlockSpec ,
          typename MatrixValueType ,
          typename VectorValueType ,
          class Device >
void multiply( const BlockCrsMatrix<BlockSpec,MatrixValueType,Device> & A ,
               const MultiVector<VectorValueType,Device>              & x ,
               const MultiVector<VectorValueType,Device>              & y )
{
  typedef BlockCrsMatrix<BlockSpec,MatrixValueType,Device> matrix_type ;
  typedef MultiVector<VectorValueType,Device>              vector_type ;

  Impl::Multiply<matrix_type,vector_type,vector_type>::execute( A , x , y );
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

} // namespace Kokkos

#endif /* #ifndef KOKKOS_BLOCKCRSMATRIX_HPP */
