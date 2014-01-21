petsc_matprinter <- function(fmt="default")
{
  fmt_choices <- c("default", "matlab", "dense", "impl", "info", "info_detail", "common", "index", "symmodu", "vtk", "native", "basic", "lg", "contour")
  fmt <- match.arg(tolower(fmt), fmt_choices)
  
  fmt_int <- which(fmt_choices == fmt) - 1L
  
  out <- .Call("sbase_petsc_matprinter", as.integer(fmt_int), PACKAGE="pbdSBASE")
  
  invisible( out )
}

