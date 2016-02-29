#ifndef DYNAMIC_MESH_SET_UPDATER_SET_UPDATER_HPP_
#define DYNAMIC_MESH_SET_UPDATER_SET_UPDATER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::setUpdater
// Description
//   Keeps cell/face/vertex sets uptodate. Both the ones loaded and the ones
//   on disk.
// SourceFiles
//   set_updater.cpp
//   set_updater_templates.cpp
#include "poly_mesh_modifier.hpp"
namespace mousse
{
class setUpdater
:
  public polyMeshModifier
{
  // Private Member Functions
    //- Updates all sets
    template<class Type>
    void updateSets(const mapPolyMesh& morphMap) const;
public:
  //- Runtime type information
  TYPE_NAME("setUpdater");
  // Constructors
    //- Construct from dictionary
    setUpdater
    (
      const word& name,
      const dictionary& dict,
      const label index,
      const polyTopoChanger& mme
    );
    //- Disallow default bitwise copy construct
    setUpdater(const setUpdater&) = delete;
    //- Disallow default bitwise assignment
    setUpdater& operator=(const setUpdater&) = delete;
  //- Destructor
  virtual ~setUpdater();
  // Member Functions
    //- Check for topology change
    virtual bool changeTopology() const;
    //- Insert the layer addition/removal instructions
    //  into the topological change
    virtual void setRefinement(polyTopoChange&) const;
    //- Modify motion points to comply with the topological change
    virtual void modifyMotionPoints(pointField& motionPoints) const;
    //- Force recalculation of locally stored data on topological change
    virtual void updateMesh(const mapPolyMesh&);
    //- Write
    virtual void write(Ostream&) const;
    //- Write dictionary
    virtual void writeDict(Ostream&) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "set_updater_templates.cpp"
#endif
#endif
