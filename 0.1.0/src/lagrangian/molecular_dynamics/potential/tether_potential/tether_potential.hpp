// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::tetherPotential
// Description
// SourceFiles
//   tether_potential.cpp
//   tether_potential_new.cpp
#ifndef tether_potential_hpp_
#define tether_potential_hpp_
#include "iodictionary.hpp"
#include "type_info.hpp"
#include "run_time_selection_tables.hpp"
#include "auto_ptr.hpp"
#include "vector.hpp"
namespace mousse
{
class tetherPotential
{
protected:
  // Protected data
    word name_;
    dictionary tetherPotentialProperties_;
  // Private Member Functions
    //- Disallow copy construct
    tetherPotential(const tetherPotential&);
    //- Disallow default bitwise assignment
    void operator=(const tetherPotential&);
public:
  //- Runtime type information
  TypeName("tetherPotential");
  // Declare run-time constructor selection table
    declareRunTimeSelectionTable
    (
      autoPtr,
      tetherPotential,
      dictionary,
      (
        const word& name,
        const dictionary& tetherPotentialProperties
      ),
      (name, tetherPotentialProperties)
    );
  // Selectors
    //- Return a reference to the selected viscosity model
    static autoPtr<tetherPotential> New
    (
      const word& name,
      const dictionary& tetherPotentialProperties
    );
  // Constructors
    //- Construct from components
    tetherPotential
    (
      const word& name,
      const dictionary& tetherPotentialProperties
    );
  //- Destructor
  virtual ~tetherPotential()
  {}
  // Member Functions
    virtual scalar energy (const vector r) const = 0;
    virtual vector force (const vector r) const = 0;
    const dictionary& tetherPotentialProperties() const
    {
      return tetherPotentialProperties_;
    }
    //- Read tetherPotential dictionary
    virtual bool read(const dictionary& tetherPotentialProperties) = 0;
};
}  // namespace mousse
#endif
