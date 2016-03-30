#ifndef CORE_MESHES_MESH_SHAPES_FACE_LIST_FWD_HPP_
#define CORE_MESHES_MESH_SHAPES_FACE_LIST_FWD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "list.hpp"
#include "sub_list.hpp"


namespace mousse {

class face;
typedef UList<face> faceUList;
typedef List<face> faceList;
typedef SubList<face> faceSubList;
typedef List<faceList> faceListList;
// same as faceUList:
typedef UList<face> unallocFaceList;

}

#endif
