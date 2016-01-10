// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef face_list_fwd_hpp_
#define face_list_fwd_hpp_
#include "list.hpp"
#include "sub_list.hpp"
namespace mousse
{
  class face;
  typedef UList<face> faceUList;
  typedef List<face> faceList;
  typedef SubList<face> faceSubList;
  typedef List<faceList> faceListList;
  // same as faceUList:
  typedef UList<face> unallocFaceList;
}
#endif
