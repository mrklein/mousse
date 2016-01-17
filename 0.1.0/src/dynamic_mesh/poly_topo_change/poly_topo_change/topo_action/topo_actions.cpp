// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "topo_action.hpp"
#include "poly_add_point.hpp"
#include "poly_add_face.hpp"
#include "poly_add_cell.hpp"
#include "poly_modify_point.hpp"
#include "poly_modify_face.hpp"
#include "poly_modify_cell.hpp"
#include "poly_remove_point.hpp"
#include "poly_remove_face.hpp"
#include "poly_remove_cell.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(topoAction, 0);
  DEFINE_TYPE_NAME_AND_DEBUG(polyAddPoint, 0);
  DEFINE_TYPE_NAME_AND_DEBUG(polyModifyPoint, 0);
  DEFINE_TYPE_NAME_AND_DEBUG(polyRemovePoint, 0);
  DEFINE_TYPE_NAME_AND_DEBUG(polyAddFace, 0);
  DEFINE_TYPE_NAME_AND_DEBUG(polyModifyFace, 0);
  DEFINE_TYPE_NAME_AND_DEBUG(polyRemoveFace, 0);
  DEFINE_TYPE_NAME_AND_DEBUG(polyAddCell, 0);
  DEFINE_TYPE_NAME_AND_DEBUG(polyModifyCell, 0);
  DEFINE_TYPE_NAME_AND_DEBUG(polyRemoveCell, 0);
}
