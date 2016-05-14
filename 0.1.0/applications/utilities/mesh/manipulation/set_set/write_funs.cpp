// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "write_funs.hpp"
#if defined(__mips)
#include <standards.h>
#include <sys/endian.h>
#endif
#if defined(LITTLE_ENDIAN) \
  || defined(_LITTLE_ENDIAN) \
  || defined(__LITTLE_ENDIAN)
#   define LITTLEENDIAN 1
#elif defined(BIG_ENDIAN) || defined(_BIG_ENDIAN) || defined(__BIG_ENDIAN)
#   undef LITTLEENDIAN
#else
#   error "Cannot find LITTLE_ENDIAN or BIG_ENDIAN symbol defined."
#   error "Please add to compilation options"
#endif


void mousse::writeFuns::swapWord(int32_t& word32)
{
  char* mem = reinterpret_cast<char*>(&word32);
  char a = mem[0];
  mem[0] = mem[3];
  mem[3] = a;
  a = mem[1];
  mem[1] = mem[2];
  mem[2] = a;
}


void mousse::writeFuns::swapWords(const label nWords, int32_t* words32)
{
  for (label i=0; i<nWords; i++) {
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
    swapWords(fField.size(), reinterpret_cast<int32_t*>(fField.begin()));
    #endif
    os.write
    (
      reinterpret_cast<char*>(fField.begin()),
      fField.size()*sizeof(float)
    );
    os << std::endl;
  } else {
    FOR_ALL(fField, i) {
      os << fField[i] << ' ';
      if (i > 0 && (i % 10) == 0) {
        os << std::endl;
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
    swapWords
    (
      (sizeof(label)/4)*elems.size(),
      reinterpret_cast<int32_t*>(elems.begin())
    );
    #endif
    os.write
    (
      reinterpret_cast<char*>(elems.begin()),
      elems.size()*sizeof(label)
    );
    os << std::endl;
  } else {
    FOR_ALL(elems, i) {
      os << elems[i] << ' ';
      if (i > 0 && (i % 10) == 0) {
        os << std::endl;
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


void mousse::writeFuns::insert(const point& pt, DynamicList<floatScalar>& dest)
{
  dest.append(float(pt.x()));
  dest.append(float(pt.y()));
  dest.append(float(pt.z()));
}


void mousse::writeFuns::insert(const labelList& source, DynamicList<label>& dest)
{
  FOR_ALL(source, i) {
    dest.append(source[i]);
  }
}


void mousse::writeFuns::insert
(
  const List<scalar>& source,
  DynamicList<floatScalar>& dest
)
{
  FOR_ALL(source, i) {
    dest.append(float(source[i]));
  }
}


void mousse::writeFuns::insert
(
  const labelList& map,
  const List<scalar>& source,
  DynamicList<floatScalar>& dest
)
{
  FOR_ALL(map, i) {
    dest.append(float(source[map[i]]));
  }
}


void mousse::writeFuns::insert
(
  const List<point>& source,
  DynamicList<floatScalar>& dest
)
{
  FOR_ALL(source, i) {
   insert(source[i], dest);
  }
}


void mousse::writeFuns::insert
(
  const labelList& map,
  const List<point>& source,
  DynamicList<floatScalar>& dest
)
{
  FOR_ALL(map, i) {
   insert(source[map[i]], dest);
  }
}

