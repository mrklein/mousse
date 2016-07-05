// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "write_funs.hpp"
#include "vtk_topo.hpp"
#if defined(__mips)
#include <standards.h>
#include <sys/endian.h>
#endif
// MacOSX
#ifdef __DARWIN_BYTE_ORDER
#if __DARWIN_BYTE_ORDER==__DARWIN_BIG_ENDIAN
#undef LITTLE_ENDIAN
#else
#undef BIG_ENDIAN
#endif
#endif
#if defined(LITTLE_ENDIAN) || defined(_LITTLE_ENDIAN) \
  || defined(__LITTLE_ENDIAN)
#define LITTLEENDIAN 1
#elif defined(BIG_ENDIAN) || defined(_BIG_ENDIAN) || defined(__BIG_ENDIAN)
#undef LITTLEENDIAN
#else
#error "Cannot find LITTLE_ENDIAN or BIG_ENDIAN symbol defined."
#error "Please add to compilation options"
#endif


void mousse::writeFuns::swapWord(label& word32)
{
  char* mem = reinterpret_cast<char*>(&word32);
  char a = mem[0];
  mem[0] = mem[3];
  mem[3] = a;
  a = mem[1];
  mem[1] = mem[2];
  mem[2] = a;
}


void mousse::writeFuns::swapWords(const label nWords, label* words32)
{
  for (label i = 0; i < nWords; i++) {
    swapWord(words32[i]);
  }
}


void mousse::writeFuns::write
(
  std::ostream& os,
  const bool binary,
  List<floatScalar>& fField
)
{
  if (binary) {
#ifdef LITTLEENDIAN
    swapWords(fField.size(), reinterpret_cast<label*>(fField.begin()));
#endif
    os.write
    (
      reinterpret_cast<char*>(fField.begin()),
      fField.size()*sizeof(float)
    );
    os << std::endl;
  } else {
    FOR_ALL(fField, i) {
      os << fField[i];
      if (i > 0 && (i % 10) == 0) {
        os << std::endl;
      } else {
        os << ' ';
      }
    }
    os << std::endl;
  }
}


void mousse::writeFuns::write
(
  std::ostream& os,
  const bool binary,
  DynamicList<floatScalar>& fField
)
{
  List<floatScalar>& fld = fField.shrink();
  write(os, binary, fld);
}


void mousse::writeFuns::write
(
  std::ostream& os,
  const bool binary,
  labelList& elems
)
{
  if (binary) {
#ifdef LITTLEENDIAN
    swapWords(elems.size(), reinterpret_cast<label*>(elems.begin()));
#endif
    os.write
    (
      reinterpret_cast<char*>(elems.begin()),
      elems.size()*sizeof(label)
    );
    os << std::endl;
  } else {
    FOR_ALL(elems, i) {
      os << elems[i];
      if (i > 0 && (i % 10) == 0) {
        os << std::endl;
      } else {
        os << ' ';
      }
    }
    os << std::endl;
  }
}


void mousse::writeFuns::write
(
  std::ostream& os,
  const bool binary,
  DynamicList<label>& elems
)
{
  labelList& fld = elems.shrink();
  write(os, binary, fld);
}


void mousse::writeFuns::writeHeader
(
  std::ostream& os,
  const bool binary,
  const std::string& title
)
{
  os << "# vtk DataFile Version 2.0" << std::endl << title << std::endl;
  os << ((binary) ? "BINARY" : "ASCII") << std::endl;
}


void mousse::writeFuns::writeCellDataHeader
(
  std::ostream& os,
  const label nCells,
  const label nFields
)
{
  os << "CELL_DATA " << nCells << std::endl
    << "FIELD attributes " << nFields << std::endl;
}


void mousse::writeFuns::writePointDataHeader
(
  std::ostream& os,
  const label nPoints,
  const label nFields
)
{
  os << "POINT_DATA  " << nPoints << std::endl
    << "FIELD attributes " << nFields << std::endl;
}


void mousse::writeFuns::insert(const scalar src, DynamicList<floatScalar>& dest)
{
  dest.append(float(src));
}


void mousse::writeFuns::insert(const vector& src, DynamicList<floatScalar>& dest)
{
  for (direction cmpt = 0; cmpt < vector::nComponents; ++cmpt) {
    dest.append(float(src[cmpt]));
  }
}


void mousse::writeFuns::insert
(
  const sphericalTensor& src,
  DynamicList<floatScalar>& dest
)
{
  for (direction cmpt = 0; cmpt < sphericalTensor::nComponents; ++cmpt) {
    dest.append(float(src[cmpt]));
  }
}


void mousse::writeFuns::insert
(
  const symmTensor& src,
  DynamicList<floatScalar>& dest
)
{
  dest.append(float(src.xx()));
  dest.append(float(src.yy()));
  dest.append(float(src.zz()));
  dest.append(float(src.xy()));
  dest.append(float(src.yz()));
  dest.append(float(src.xz()));
}


void mousse::writeFuns::insert(const tensor& src, DynamicList<floatScalar>& dest)
{
  for (direction cmpt = 0; cmpt < tensor::nComponents; ++cmpt) {
    dest.append(float(src[cmpt]));
  }
}


void mousse::writeFuns::insert(const labelList& src, DynamicList<label>& dest)
{
  dest.append(src);
}
