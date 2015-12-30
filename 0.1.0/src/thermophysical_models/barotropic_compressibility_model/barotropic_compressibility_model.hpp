// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::barotropicCompressibilityModel
// Description
//   Abstract class for barotropic compressibility models
// SourceFiles
//   barotropic_compressibility_model.cpp
//   newbarotropic_compressibility_model.cpp
#ifndef barotropic_compressibility_model_hpp_
#define barotropic_compressibility_model_hpp_
#include "iodictionary.hpp"
#include "type_info.hpp"
#include "run_time_selection_tables.hpp"
#include "vol_fields.hpp"
#include "dimensioned_scalar.hpp"
#include "auto_ptr.hpp"
namespace mousse
{
class barotropicCompressibilityModel
{
protected:
  // Protected data
    dictionary compressibilityProperties_;
    volScalarField psi_;
    const volScalarField& gamma_;
  // Private Member Functions
    //- Disallow copy construct
    barotropicCompressibilityModel(const barotropicCompressibilityModel&);
    //- Disallow default bitwise assignment
    void operator=(const barotropicCompressibilityModel&);
public:
  //- Runtime type information
  TypeName("barotropicCompressibilityModel");
  // Declare run-time constructor selection table
    declareRunTimeSelectionTable
    (
      autoPtr,
      barotropicCompressibilityModel,
      dictionary,
      (
        const dictionary& compressibilityProperties,
        const volScalarField& gamma,
        const word& psiName
      ),
      (compressibilityProperties, gamma, psiName)
    );
  // Selectors
    //- Return a reference to the selected compressibility model
    static autoPtr<barotropicCompressibilityModel> New
    (
      const dictionary& compressibilityProperties,
      const volScalarField& gamma,
      const word& psiName = "psi"
    );
  // Constructors
    //- Construct from components
    barotropicCompressibilityModel
    (
      const dictionary& compressibilityProperties,
      const volScalarField& gamma,
      const word& psiName = "psi"
    );
  //- Destructor
  virtual ~barotropicCompressibilityModel()
  {}
  // Member Functions
    //- Return the phase transport properties dictionary
    const dictionary& compressibilityProperties() const
    {
      return compressibilityProperties_;
    }
    //- Return the compressibility
    const volScalarField& psi() const
    {
      return psi_;
    }
    //- Correct the compressibility
    virtual void correct() = 0;
    //- Read compressibilityProperties dictionary
    virtual bool read(const dictionary& compressibilityProperties) = 0;
};
}  // namespace mousse
#endif
