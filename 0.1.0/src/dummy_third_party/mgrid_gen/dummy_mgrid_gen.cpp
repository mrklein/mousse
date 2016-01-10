// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

//extern "C"
//{
#include "mgridgen.h"
//}
#include "error.hpp"
using namespace mousse;
static const char* notImplementedMessage =
"You are trying to use MGridGen but do not have the MGridGen library loaded.\n"
"This message is from the dummy MGridGen stub library instead.\n"
"\n"
#if defined(darwin64)
"Normally the MGridGen library will be loaded through the DYLD_LIBRARY_PATH\n"
"environment variable but you are picking up this dummy library from the\n"
"$MOUSSE_LIBBIN/dummy directory. Please install MGridGen and make sure the\n"
"libMGridGen.so is in your DYLD_LIBRARY_PATH.";
#else
"Normally the MGridGen library will be loaded through the LD_LIBRARY_PATH\n"
"environment variable but you are picking up this dummy library from the\n"
"$FOAM_LIBBIN/dummy directory. Please install MGridGen and make sure the\n"
"libMGridGen.so is in your LD_LIBRARY_PATH.";
#endif
#ifdef __cplusplus
extern "C"
#endif
void MGridGen(int, idxtype *, realtype *, realtype *, idxtype *, realtype *,
       int, int, int *, int *, int *, idxtype *)
{
  FATAL_ERROR_IN("MGridGen(..)")
    << notImplementedMessage
    << mousse::exit(mousse::FatalError);
}
