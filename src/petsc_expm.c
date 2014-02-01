/* This Source Code Form is subject to the terms of the BSD 2-Clause
 * License. If a copy of the this license was not distributed with this
 * file, you can obtain one from http://opensource.org/licenses/BSD-2-Clause. */

// Copyright 2013, Schmidt


#include "sbase.h"
#include <float.h>
#include <math.h>

/* Reference: 
 * "Computing the Action of the Matrix Exponential, with an Application
 * to Exponential Integrators", Mohy and Higham, March 2011 
 * 
 * Coefficients come from "Computing Matrix Functions", 2010 */

// 1:30, 35, 40, 45, 50, 55
const double theta_m[35] = {
  2.29e-16, 2.58e-8, 1.39e-5, 3.40e-4, 2.40e-3, 
  9.07e-3, 2.38e-2, 5.00e-2, 8.96e-2, 1.44e-1,
  2.14e-1, 3.00e-1, 4.00e-1, 5.14e-1, 6.41e-1,
  7.81e-1, 9.31e-1, 1.09e0, 1.26e0, 1.44e0,
  1.62e0, 1.82e0, 2.01e0, 2.22e0, 2.43e0,
  2.64e0, 2.86e0, 3.08e0, 3.31e0, 3.54e0,
  4.7e0,
  6.0e0,
  7.2e0,
  8.5e0,
  9.9e0
}

const int p_max = 8;
const int m_max = 55;


// alpha_p : (3.7)
PetscErrorCode alpha_p(Mat A, Mat A_p, Mat tmp, int p, double *alpha)
{
  Mat A_p, tmp;
  PetscErrorCode ierr;
  double nrm;
  
  
  ierr = MatDuplicate(A, MAT_COPY_VALUES, &A_p);CHKERRQ(ierr);
  
  
  ierr = MatMatMult(A, A_p, MAT_INITIAL_MATRIX, PETSC_DEFAULT, Mat &tmp);CHKERRQ(tmp);
  ierr = MatNorm(A_p, NORM_1, &nrm);CHKERRQ(ierr);
  nrm = pow(nrm, (double) 1/p);
  
  
  ierr = MatDuplicate(A, MAT_COPY_VALUES, &A_p);CHKERRQ(ierr);
  
  
  PetscFunctionReturn(0);
}


int condition_3_13()
{
  int cond, rhs, tmp;
  a = 2*p_max*(p_max + 3);
  b = theta_m[m_max] / ( (double) n0 * m_max);
  cond = ( nrm <= a*b );
  
  p_max = _compute_p_max(m_max)
  a = 2 * ell * p_max * (p_max + 3)
  
  b = _theta[m_max] / float(n0 * m_max)
  return nrm <= rhs;
}

// Starting parameters for matrix exponential
PetscErrorCode sbase_petsc_expm_params(Mat A, double tol, int m_max, int p_max, int *m, int *s)
{
  PetscReal       nrm;
  PetscErrorCode  ierr;
  const double    tol = DBL_EPSILON / 2.0; // 2^(-53)
  int i, i_min;
  int min, test;
  int cond;
  
  const int p_max = 8; // FIXME ?
  
  
  ierr = MatNorm(A, NORM_1, &nrm);CHKERRQ(ierr);
  
  // Condition 3.13 from the paper
  cond = condition_3_13();
  
  if(cond)
  {
    i = 0;
    
    for (i=0; i<m_max; i++)
    {
      test = (i*5) * (( (int) nrm/theta_m[i]) + 1);
      if (i == 0)
      {
        min = test;
        i_min = i;
      }
      else
      {
        if (test < min)
        {
          min = test;
          i_min = i;
        }
      }
      
      i++;
    }
    
    *m = i_min * 5;
    
    *s = ( (int) nrm/theta_m[i_min] ) + 1;
  }
  else
  {
    
  }
  
  PetscFunctionReturn(0);
}


// Computes exp(A)B

