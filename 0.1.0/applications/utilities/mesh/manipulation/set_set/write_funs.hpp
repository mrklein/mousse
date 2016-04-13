#ifndef UTILITIES_MESH_MANIPULATION_SET_SET_WRITE_FUNS_HPP_
#define UTILITIES_MESH_MANIPULATION_SET_SET_WRITE_FUNS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::writeFuns
// Description
//   Various functions for collecting and writing binary data.
//   The LITTLE_ENDIAN is based on 32bit words.
//   It is not clear how 64bit labels should be handled, currently they are
//   split into two 32bit words and swapWord applied to these two.
//   writeFuns should be a namespace rather than a class.
// SourceFiles
//   write_funs.cpp

#include "label_list.hpp"
#include "float_scalar.hpp"
#include "ofstream.hpp"
#include "dynamic_list.hpp"
#include "point.hpp"

namespace mousse
{
class writeFuns
{
  // Private member functions
    //- Swap halves of word
    static void swapWord(int32_t& word32);
    //- Swap halves of word
    static void swapWords(const label nWords, int32_t* words32);
public:
  //- Write floats ascii or binary.
  //  If binary optionally in-place swaps argument
  static void write(std::ostream&, const bool, DynamicList<floatScalar>&);
  //- Write labels ascii or binary.
  //  If binary optionally in-place swaps argument
  static void write(std::ostream&, const bool, DynamicList<label>&);
  //- Write floats ascii or binary.
  //  If binary optionally in-place swaps argument
  static void write(std::ostream&, const bool, List<floatScalar>&);
  //- Write labels ascii or binary.
  //  If binary optionally in-place swaps argument
  static void write(std::ostream&, const bool, labelList&);
  //- Append point to given DynamicList
  static void insert(const point&, DynamicList<floatScalar>& dest);
  //- Append elements of labelList to given DynamicList
  static void insert(const labelList&, DynamicList<label>&);
  //- Append elements of scalarList to given DynamicList
  static void insert(const List<scalar>&, DynamicList<floatScalar>&);
  //- Append elements of scalarList to given DynamicList using map
  static void insert
  (
    const labelList& map,
    const List<scalar>& source,
    DynamicList<floatScalar>&
  );
  //- Append points to given DynamicList of floats
  static void insert(const List<point>& source, DynamicList<floatScalar>&);
  //- Append points to given DynamicList of floats using map
  static void insert
  (
    const labelList& map,
    const List<point>& source,
    DynamicList<floatScalar>&
  );
};
}  // namespace mousse
#endif
