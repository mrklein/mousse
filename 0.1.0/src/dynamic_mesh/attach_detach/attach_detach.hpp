#ifndef DYNAMIC_MESH_ATTACH_DETACH_ATTACH_DETACH_HPP_
#define DYNAMIC_MESH_ATTACH_DETACH_ATTACH_DETACH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::attachDetach
// Description
//   Attach/detach boundary mesh modifier.  This modifier takes a set of
//   internal faces and converts them into boundary faces and vice versa
//   based on the given activation switch.
//   The patch is oriented using the flip map in the face zone.  The
//   oriented faces are put into the master patch and their mirror
//   images into the slave.
// SourceFiles
//   attach_detach.cpp
//   attach_interface.cpp
//   detach_interface.cpp
//   attach_detach_point_match_map.cpp
#include "poly_mesh_modifier.hpp"
#include "poly_patch_id.hpp"
#include "zone_i_ds.hpp"
namespace mousse
{
class attachDetach
:
  public polyMeshModifier
{
  // Data types
    //- State of the modifier
    enum modifierState
    {
      UNKNOWN,
      ATTACHED,
      DETACHED
    };
  // Private data
    //- Master face zone ID
    faceZoneID faceZoneID_;
    //- Master patch ID.  Holds faces with original orientation
    polyPatchID masterPatchID_;
    //- Slave patch ID.  Holds mirrored faces
    polyPatchID slavePatchID_;
    //- List of trigger times
    scalarField triggerTimes_;
    //- Use manual trigger
    Switch manualTrigger_;
    //- Trigger time index
    mutable label triggerIndex_;
    //- State of the modifier
    mutable modifierState state_;
    //- Attach/detach trigger
    mutable bool trigger_;
    // Private addressing data.  Created on topology change
      //- Map of matching points
      mutable Map<label>* pointMatchMapPtr_;
  // Private Member Functions
    //- Check validity of construction data
    void checkDefinition();
    // Topological changes
      //- Attach interface
      void attachInterface(polyTopoChange&) const;
      //- Detach interface
      void detachInterface(polyTopoChange&) const;
      //- Calculate point match addressing
      void calcPointMatchMap() const;
      //- Return point match map
      const Map<label>& pointMatchMap() const;
      //- Clear addressing
      void clearAddressing() const;
  // Static data members
    //- Relative vertex position tolerance
    static const scalar positionDifference_;
public:
  //- Runtime type information
  TYPE_NAME("attachDetach");
  // Constructors
    //- Construct from components
    attachDetach
    (
      const word& name,
      const label index,
      const polyTopoChanger& mme,
      const word& faceZoneName,
      const word& masterPatchName,
      const word& slavePatchName,
      const scalarField& triggerTimes,
      const bool manualTrigger = false
    );
    //- Construct from dictionary
    attachDetach
    (
      const word& name,
      const dictionary& dict,
      const label index,
      const polyTopoChanger& mesh
    );
    //- Disallow default bitwise copy construct
    attachDetach(const attachDetach&) = delete;
    //- Disallow default bitwise assignment
    attachDetach& operator=(const attachDetach&) = delete;
  //- Destructor
  virtual ~attachDetach();
  // Member Functions
    //- Return master patch ID
    const polyPatchID& masterPatchID() const
    {
      return masterPatchID_;
    }
    //- Return slave patch ID
    const polyPatchID& slavePatchID() const
    {
      return slavePatchID_;
    }
    //- Is the interface attached?
    bool attached() const
    {
      return state_ == ATTACHED;
    }
    const Switch& manualTrigger() const
    {
      return manualTrigger_;
    }
    // Manually set attach.  Use only with manual trigger
    bool setAttach() const;
    // Manually set detach.  Use only with manual trigger
    bool setDetach() const;
    //- Check for topology change
    virtual bool changeTopology() const;
    //- Insert the layer addition/removal instructions
    //  into the topological change
    virtual void setRefinement(polyTopoChange&) const;
    //- Modify motion points to comply with the topological change
    virtual void modifyMotionPoints(pointField& motionPoints) const;
    //- Force recalculation of locally stored data on topological change
    virtual void updateMesh(const mapPolyMesh&);
    //- Get reference to trigger times
    const scalarField& triggerTimes() const
    {
      return triggerTimes_;
    }
    //- Write
    virtual void write(Ostream&) const;
    //- Write dictionary
    virtual void writeDict(Ostream&) const;
};
}  // namespace mousse
#endif
