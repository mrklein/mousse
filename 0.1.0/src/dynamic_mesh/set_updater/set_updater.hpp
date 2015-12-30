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
#ifndef set_updater_hpp_
#define set_updater_hpp_
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
    //- Disallow default bitwise copy construct
    setUpdater(const setUpdater&);
    //- Disallow default bitwise assignment
    void operator=(const setUpdater&);
public:
  //- Runtime type information
  TypeName("setUpdater");
  // Constructors
    //- Construct from dictionary
    setUpdater
    (
      const word& name,
      const dictionary& dict,
      const label index,
      const polyTopoChanger& mme
    );
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
