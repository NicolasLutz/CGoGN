/*
 *  OpenNL<T>: Generic Numerical Library
 *  Copyright (C) 2004 Bruno Levy
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  In addition, as a special exception, the INRIA gives permission to link the 
 *  code of this program with the CGAL library, and distribute linked combinations 
 *  including the two. You must obey the GNU General Public License in all respects 
 *  for all of the code used other than CGAL. 
 *
 *  If you modify this software, you should include a notice giving the
 *  name of the person performing the modification, the date of modification,
 *  and the reason for such modification.
 *
 *  Contact: Bruno Levy
 *
 *     levy@loria.fr
 *
 *     ISA-ALICE Project
 *     LORIA, INRIA Lorraine, 
 *     Campus Scientifique, BP 239
 *     54506 VANDOEUVRE LES NANCY CEDEX 
 *     FRANCE
 *
 *  Note that the GNU General Public License does not permit incorporating
 *  the Software into proprietary programs. 
 */

#ifndef __BICGSTAB__
#define __BICGSTAB__

#include <cmath>
#include <cassert>
#include "blas.h"

/**
 *  The BICGSTAB algorithm without preconditioner:
 *  Ashby, Manteuffel, Saylor
 *     A taxononmy for conjugate gradient methods
 *     SIAM J Numer Anal 27, 1542-1568 (1990)
 *
 * This implementation is inspired by the lsolver library,
 * by Christian Badura, available from:
 * http://www.mathematik.uni-freiburg.de
 * /IAM/Research/projectskr/lin_solver/
 *
  * @param A generic matrix, a function
 *   mult(const MATRIX& M, const double* x, double* y)
 *   needs to be defined.
 * @param b right hand side of the system.
 * @param x initial value.
 * @param eps threshold for the residual.
 * @param max_iter maximum number of iterations.
  */    

template < class MATRIX, class VECTOR>
class Solver_BICGSTAB {
public:
    typedef MATRIX Matrix ;
    typedef VECTOR Vector ;
    typedef typename Vector::CoeffType CoeffType ;

    Solver_BICGSTAB() {
        epsilon_ = 1e-5 ;
        max_iter_ = 0 ;
    }

    void set_epsilon(CoeffType eps) { epsilon_ = eps ; }
    void set_max_iter(unsigned int max_iter) { max_iter_ = max_iter ; }

    void solve(const MATRIX &A, const VECTOR& b, VECTOR& x) {
    	assert(A.n() == A.m()) ;
		assert(A.has_symmetric_storage()) ;
        assert(A.n() == b.dimension()) ;
        assert(A.n() == x.dimension()) ;
        unsigned int n = A.n() ;
        unsigned int max_iter = max_iter_ ;
        if(max_iter == 0) {
            max_iter = 5 * n ;
        }
        Vector rT(n) ;
        Vector d(n) ;
        Vector h(n) ;
        Vector u(n) ;
        Vector Ad(n) ;
        Vector t(n) ;
        Vector& s = h ;
        CoeffType rTh, rTAd, rTr, alpha, beta, omega, st, tt ;
        unsigned int its = 0 ;
        CoeffType err = epsilon_ * epsilon_ * BLAS<Vector>::dot(b,b) ;
        Vector r(n) ;
        mult(A,x,r) ;
        BLAS<Vector>::axpy(-1,b,r) ;
        BLAS<Vector>::copy(r,d) ;
        BLAS<Vector>::copy(d,h) ;
        BLAS<Vector>::copy(h,rT) ;
        assert( BLAS<Vector>::dot(rT,rT)>1e-40 ) ;
        rTh=BLAS<Vector>::dot(rT,h) ;
        rTr=BLAS<Vector>::dot(r,r) ;
        while ( rTr > err && its < max_iter) {
            mult(A,d,Ad) ;
            rTAd = BLAS<Vector>::dot(rT,Ad) ;
            assert( fabs(rTAd)>1e-40 ) ;
            alpha = rTh/rTAd ;
            BLAS<Vector>::axpy(-alpha,Ad,r) ;
            BLAS<Vector>::copy(h,s) ;
            BLAS<Vector>::axpy(-alpha,Ad,s) ;
            mult(A,s,t) ;
            BLAS<Vector>::axpy(1,t,u) ;
            BLAS<Vector>::scal(alpha,u) ;
            st = BLAS<Vector>::dot(s,t) ;
            tt = BLAS<Vector>::dot(t,t) ;
            if ( fabs(st) < 1e-40 || fabs(tt) < 1e-40 )
                omega = 0 ;
            else
                omega = st/tt ;
            BLAS<Vector>::axpy(-omega,t,r) ;
            BLAS<Vector>::axpy(-alpha,d,x) ;
            BLAS<Vector>::axpy(-omega,s,x) ;
            BLAS<Vector>::copy(s,h) ;
            BLAS<Vector>::axpy(-omega,t,h) ;
            beta = (alpha/omega) / rTh ;
            rTh = BLAS<Vector>::dot(rT,h) ;
            beta *= rTh ;
            BLAS<Vector>::scal(beta,d) ;
            BLAS<Vector>::axpy(1,h,d) ;
            BLAS<Vector>::axpy(-beta*omega,Ad,d) ;
            rTr = BLAS<Vector>::dot(r,r) ;
            its++ ;
        }
    }

private:
    CoeffType epsilon_ ;
    unsigned int max_iter_ ;
} ;


#endif 
