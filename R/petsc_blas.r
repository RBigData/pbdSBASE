petsc_matmatmult <- function(A_dim, A_ldim, A_data, A_row_ptr, A_col_ind, B_dim, B_ldim, B_data, B_row_ptr, B_col_ind)
{
  storage.mode(A_dim) <- "integer"
  storage.mode(A_ldim) <- "integer"
  storage.mode(A_data) <- "double"
  storage.mode(A_row_ptr) <- "integer"
  storage.mode(A_col_ind) <- "integer"
  storage.mode(B_dim) <- "integer"
  storage.mode(B_ldim) <- "integer"
  storage.mode(B_data) <- "double"
  storage.mode(B_row_ptr) <- "integer"
  storage.mode(B_col_ind) <- "integer"
  
  out <- .Call("sbase_petsc_matmatmult", 
            A_dim, A_ldim, A_data, A_row_ptr, A_col_ind, 
            B_dim, B_ldim, B_data, B_row_ptr, B_col_ind, 
            PACKAGE="pbdSBASE")
  
  return( out )
}



petsc_mattranspose <- function(dim, ldim, Data, row_ptr, col_ind)
{
  storage.mode(dim) <- "integer"
  storage.mode(ldim) <- "integer"
  storage.mode(Data) <- "double"
  storage.mode(row_ptr) <- "integer"
  storage.mode(col_ind) <- "integer"
  
  out <- .Call("sbase_petsc_mattranspose", dim, ldim, Data, row_ptr, col_ind, PACKAGE="pbdSBASE")
  
  return( out )
}

