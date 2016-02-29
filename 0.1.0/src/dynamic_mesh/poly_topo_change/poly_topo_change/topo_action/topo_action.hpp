#ifndef DYNAMIC_MESH_POLY_TOPO_CHANGE_POLY_TOPO_CHANGE_TOPO_ACTION_TOPO_ACTION_HPP_
#define DYNAMIC_MESH_POLY_TOPO_CHANGE_POLY_TOPO_CHANGE_TOPO_ACTION_TOPO_ACTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::topoAction
// Description
//   A virtual base class for topological actions
// SourceFiles
#include "type_info.hpp"
#include "auto_ptr.hpp"
namespace mousse
{
class topoAction
{
public:
  // Static data members
    //- Runtime type information
    TYPE_NAME("topoAction");
  // Constructors
    //- Construct null
    topoAction()
    {}
    //- Construct and return a clone
    virtual autoPtr<topoAction> clone() const = 0;
  //- Destructor
  virtual ~topoAction()
  {}
};
}  // namespace mousse
#endif
