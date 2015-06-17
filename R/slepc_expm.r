#### expm(t*x)y, x matrix, y vector
#slepc_expm <- function(t, x_dim, x_ldim, x_data, x_row_ptr, x_col_ind, y_length, y_llength, y_data, y_row_ptr, y_col_ind)
#{
#  storage.mode(t) <- "double"
#  storage.mode(x_dim) <- "integer"
#  storage.mode(x_ldim) <- "integer"
#  storage.mode(x_data) <- "double"
#  storage.mode(x_row_ptr) <- "integer"
#  storage.mode(x_col_ind) <- "integer"
#  storage.mode(y_length) <- "integer"
#  storage.mode(y_llength) <- "integer"
#  storage.mode(y_data) <- "double"
#  storage.mode(y_row_ptr) <- "integer"
#  storage.mode(y_col_ind) <- "integer"
#  
#  x_data <- x_data * t
#  
#  out <- .Call(slepc_expm, t,
#            x_dim, x_ldim, x_data, x_row_ptr, x_col_ind, 
#            y_length, y_llength, y_data, y_row_ptr, y_col_ind)
#  
#  return( out )
#}

