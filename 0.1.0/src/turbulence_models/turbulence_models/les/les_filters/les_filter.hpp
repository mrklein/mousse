// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::LESfilter
// Description
//   Abstract class for LES filters
// SourceFiles
//   les_filter.cpp
//   new_filter.cpp
#ifndef les_filter_hpp_
#define les_filter_hpp_
#include "vol_fields.hpp"
#include "type_info.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class fvMesh;
class LESfilter
{
  // Private data
    const fvMesh& mesh_;
  // Private Member Functions
    // Disallow default bitwise copy construct and assignment
    LESfilter(const LESfilter&);
    void operator=(const LESfilter&);
public:
  //- Runtime type information
  TypeName("LESfilter");
  // Declare run-time constructor selection table
    declareRunTimeSelectionTable
    (
      autoPtr,
      LESfilter,
      dictionary,
      (
        const fvMesh& mesh,
        const dictionary& LESfilterDict
      ),
      (mesh, LESfilterDict)
    );
  // Constructors
    //- Construct from components
    LESfilter(const fvMesh& mesh)
    :
      mesh_(mesh)
    {}
  // Selectors
    //- Return a reference to the selected LES filter
    static autoPtr<LESfilter> New
    (
      const fvMesh&,
      const dictionary&,
      const word& filterDictName="filter"
    );
  //- Destructor
  virtual ~LESfilter()
  {}
  // Member Functions
    //- Return mesh reference
    const fvMesh& mesh() const
    {
      return mesh_;
    }
    //- Read the LESfilter dictionary
    virtual void read(const dictionary&) = 0;
  // Member Operators
    virtual tmp<volScalarField> operator()
    (
      const tmp<volScalarField>&
    ) const = 0;
    virtual tmp<volVectorField> operator()
    (
      const tmp<volVectorField>&
    ) const = 0;
    virtual tmp<volSymmTensorField> operator()
    (
      const tmp<volSymmTensorField>&
    ) const = 0;
    virtual tmp<volTensorField> operator()
    (
      const tmp<volTensorField>&
    ) const = 0;
};
}  // namespace mousse
#endif
