
// Namespace
//     C linkage
//
// Description
//   Dummy stub for mgridgen library functions.
//   Only implements the absolute minimum we are using.
//
// SourceFiles
//   dummy_mgrid_gen.cpp

#ifndef mgridgen_h_
#define mgridgen_h_
#include "scalar.hpp"
#ifndef idxtype
#define idxtype int
#define realtype mousse::scalar
#endif
#ifdef __cplusplus
extern "C"
#endif
void MGridGen(int, idxtype *, realtype *, realtype *, idxtype *, realtype *,
              int, int, int *, int *, int *, idxtype *);
#endif
