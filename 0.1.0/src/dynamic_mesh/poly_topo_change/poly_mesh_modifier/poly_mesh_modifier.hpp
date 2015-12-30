// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::polyMeshModifier
// Description
//   Virtual base class for mesh modifiers.
// SourceFiles
//   poly_mesh_modifier.cpp
#ifndef poly_mesh_modifier_hpp_
#define poly_mesh_modifier_hpp_
#include "label.hpp"
#include "word.hpp"
#include "switch.hpp"
#include "type_info.hpp"
#include "run_time_selection_tables.hpp"
#include "auto_ptr.hpp"
#include "point_field.hpp"
#include "face_list.hpp"
#include "cell_list.hpp"
namespace mousse
{
// Forward declaration of classes
class polyTopoChanger;
class polyTopoChange;
class mapPolyMesh;
class polyMeshModifier
{
  // Private data
    //- Name of modifier
    word name_;
    //- Index of modifier
    label index_;
    //- Reference to morph engine
    const polyTopoChanger& topoChanger_;
    //- Activation switch
    mutable Switch active_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    polyMeshModifier(const polyMeshModifier&);
    //- Disallow default bitwise assignment
    void operator=(const polyMeshModifier&);
public:
  // Static data members
    //- Runtime type information
    TypeName("meshModifier");
  // Declare run-time constructor selection tables
    declareRunTimeSelectionTable
    (
      autoPtr,
      polyMeshModifier,
      dictionary,
      (
        const word& name,
        const dictionary& dict,
        const label index,
        const polyTopoChanger& mme
      ),
      (name, dict, index, mme)
    );
  // Constructors
    //- Construct from components
    polyMeshModifier
    (
      const word& name,
      const label index,
      const polyTopoChanger& mme,
      const bool act
    );
  // Selectors
    //- Select constructed from dictionary
    static autoPtr<polyMeshModifier> New
    (
      const word& name,
      const dictionary& dict,
      const label index,
      const polyTopoChanger& mme
    );
  //- Destructor
  virtual ~polyMeshModifier();
  // Member Functions
    //- Return name of this modifier
    const word& name() const
    {
      return name_;
    }
    //- Return the index of this modifier
    label index() const
    {
      return index_;
    }
    //- Return reference to morph engine
    const polyTopoChanger& topoChanger() const;
    //- Check for topology change
    virtual bool changeTopology() const = 0;
    //- Insert the topological change instructions
    virtual void setRefinement(polyTopoChange&) const = 0;
    //- Modify motion points to comply with the topological change
    virtual void modifyMotionPoints(pointField& motionPoints) const = 0;
    //- Force recalculation of locally stored data on topological change
    virtual void updateMesh(const mapPolyMesh&) = 0;
    // Activation and deactivation
      const Switch& active() const
      {
        return active_;
      }
      //- Activate mesh modifier
      void enable() const
      {
        active_ = true;
      }
      //- Activate mesh modifier
      void disable() const
      {
        active_ = false;
      }
    //- Write
    virtual void write(Ostream&) const = 0;
    //- Write dictionary
    virtual void writeDict(Ostream&) const = 0;
  // Ostream Operator
    friend Ostream& operator<<(Ostream&, const polyMeshModifier&);
};
}  // namespace mousse
#endif
