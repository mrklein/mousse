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
  defineTypeNameAndDebug(topoAction, 0);
  defineTypeNameAndDebug(polyAddPoint, 0);
  defineTypeNameAndDebug(polyModifyPoint, 0);
  defineTypeNameAndDebug(polyRemovePoint, 0);
  defineTypeNameAndDebug(polyAddFace, 0);
  defineTypeNameAndDebug(polyModifyFace, 0);
  defineTypeNameAndDebug(polyRemoveFace, 0);
  defineTypeNameAndDebug(polyAddCell, 0);
  defineTypeNameAndDebug(polyModifyCell, 0);
  defineTypeNameAndDebug(polyRemoveCell, 0);
}
