#ifndef CORE_MESHES_POLY_MESH_ZONES_FACE_ZONE_HPP_
#define CORE_MESHES_POLY_MESH_ZONES_FACE_ZONE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::faceZone
// Description
//   A subset of mesh faces organised as a primitive patch.
//   For quick check whether a face belongs to the zone use the lookup
//   mechanism in faceZoneMesh, where all the zoned faces are registered
//   with their zone number.
// SourceFiles
//   face_zone.cpp
//   face_zone_new.cpp


#include "zone.hpp"
#include "face_zone_mesh_fwd.hpp"
#include "bool_list.hpp"
#include "primitive_face_patch.hpp"

namespace mousse
{

class mapPolyMesh;
// Forward declaration of friend functions and operators
class faceZone;
Ostream& operator<<(Ostream&, const faceZone&);

class faceZone
:
  public zone
{

  // Private data

    //- The name associated with the zone-labels dictionary entry
    static const word labelsName_;

protected:

  // Protected data

    //- Flip map for all faces in the zone.  Set to true if the
    //  face needs to be flipped to achieve the correct orientation.
    boolList flipMap_;

    //- Reference to zone list
    const faceZoneMesh& zoneMesh_;

    // Demand-driven private data

      //- Primitive patch made out of correctly flipped faces
      mutable primitiveFacePatch* patchPtr_;

      //- Master cell layer
      mutable labelList* masterCellsPtr_;

      //- Slave cell layer
      mutable labelList* slaveCellsPtr_;

      //- Global edge addressing
      mutable labelList* mePtr_;

private:
  // Private Member Functions

    //- Build primitive patch
    void calcFaceZonePatch() const;

    //- Return map of local face indices
    const Map<label>& faceLookupMap() const;

    //- Calculate master and slave face layer
    void calcCellLayers() const;

    //- Check addressing
    void checkAddressing() const;

public:

  // Static data members

    //- The name associated with the zone-labels dictionary entry
    static const char * const labelsName;

  //- Runtime type information
  TYPE_NAME("faceZone");

  // Declare run-time constructor selection tables
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      faceZone,
      dictionary,
      (
        const word& name,
        const dictionary& dict,
        const label index,
        const faceZoneMesh& zm
      ),
      (name, dict, index, zm)
    );
  // Constructors

    //- Construct from components
    faceZone
    (
      const word& name,
      const labelUList& addr,
      const boolList& fm,
      const label index,
      const faceZoneMesh& zm
    );

    //- Construct from components, transferring contents
    faceZone
    (
      const word& name,
      const Xfer<labelList>& addr,
      const Xfer<boolList>& fm,
      const label index,
      const faceZoneMesh&
    );

    //- Construct from dictionary
    faceZone
    (
      const word& name,
      const dictionary&,
      const label index,
      const faceZoneMesh&
    );

    //- Construct given the original zone and resetting the
    //  face list and zone mesh information
    faceZone
    (
      const faceZone&,
      const labelUList& addr,
      const boolList& fm,
      const label index,
      const faceZoneMesh&
    );

    //- Construct given the original zone, resetting the
    //  face list and zone mesh information
    faceZone
    (
      const faceZone&,
      const Xfer<labelList>& addr,
      const Xfer<boolList>& fm,
      const label index,
      const faceZoneMesh&
    );

    //- Construct and return a clone, resetting the zone mesh
    virtual autoPtr<faceZone> clone(const faceZoneMesh& zm) const
    {
      return autoPtr<faceZone>
      {
        new faceZone{*this, *this, flipMap(), index(), zm}
      };
    }

    //- Construct and return a clone, resetting the face list
    //  and zone mesh
    virtual autoPtr<faceZone> clone
    (
      const labelUList& addr,
      const boolList& fm,
      const label index,
      const faceZoneMesh& zm
    ) const
    {
      return autoPtr<faceZone>
      {
        new faceZone{*this, addr, fm, index, zm}
      };
    }

    //- Disallow default bitwise copy construct
    faceZone(const faceZone&) = delete;

    //- Disallow default bitwise assignment
    faceZone& operator=(const faceZone&) = delete;

  // Selectors

    //- Return a pointer to a new face zone
    //  created on freestore from dictionary
    static autoPtr<faceZone> New
    (
      const word& name,
      const dictionary&,
      const label index,
      const faceZoneMesh&
    );

  //- Destructor
  virtual ~faceZone();

  // Member Functions

    //- Return face flip map
    const boolList& flipMap() const
    {
      return flipMap_;
    }

    //- Helper function to re-direct to zone::localID(...)
    label whichFace(const label globalCellID) const;

    //- Return reference to primitive patch
    const primitiveFacePatch& operator()() const;

    //- Return zoneMesh reference
    const faceZoneMesh& zoneMesh() const;

    // Addressing into mesh

      //- Return labels of master cells (cells next to the master face
      //  zone in the prescribed direction)
      const labelList& masterCells() const;

      //- Return labels of slave cells
      const labelList& slaveCells() const;

      //- Return global edge index for local edges
      const labelList& meshEdges() const;

    //- Clear addressing
    virtual void clearAddressing();

    //- Reset addressing and flip map (clearing demand-driven data)
    virtual void resetAddressing(const labelUList&, const boolList&);

    //- Check zone definition. Return true if in error.
    virtual bool checkDefinition(const bool report = false) const;

    //- Check whether all procs have faces synchronised. Return
    //  true if in error.
    virtual bool checkParallelSync(const bool report = false) const;

    //- Correct patch after moving points
    virtual void movePoints(const pointField&);

    //- Update for changes in topology
    virtual void updateMesh(const mapPolyMesh&);

    //- Write
    virtual void write(Ostream&) const;

    //- Write dictionary
    virtual void writeDict(Ostream&) const;

  // I-O

    //- Ostream Operator
    friend Ostream& operator<<(Ostream&, const faceZone&);

};

}  // namespace mousse
#endif
