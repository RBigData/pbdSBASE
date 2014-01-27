petsc_matprinter_fmt <- function(fmt="default")
{
  fmt_choices <- c("default", "matlab", "dense", "impl", "info", "info_detail", "common", "index", "symmodu", "vtk", "native", "basic", "lg", "contour")
  fmt <- match.arg(tolower(fmt), fmt_choices)
  
  fmt_int <- which(fmt_choices == fmt) - 1L
  
  .Call("sbase_petsc_matprinter_fmt", as.integer(fmt_int), PACKAGE="pbdSBASE")
  
  invisible()
}



petsc_matprinter <- function(dim, ldim, data, row_ptr, col_ind, fmt="default")
{
  petsc_matprinter_fmt(fmt=fmt)
  
  .Call("sbase_petsc_matprinter", dim, ldim, data, row_ptr, col_ind, PACKAGE="pbdSBASE")
  
  invisible()
}


