#ifndef THERMOPHYSICAL_MODELS_RADIATION_DERIVED_FV_PATCH_FIELDS_RADIATION_COUPLED_BASE_HPP_
#define THERMOPHYSICAL_MODELS_RADIATION_DERIVED_FV_PATCH_FIELDS_RADIATION_COUPLED_BASE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   radiationCoupledBase
// Description
//   Common functions to emissivity. It gets supplied from lookup into a
//   dictionary or calculated by the solidThermo:
//   - 'lookup' : Read the patch emissivity field from the dictionary
//   - 'solidRadiation' : Use the emissivity field mapped from the adjacent solid

#include "scalar_field.hpp"
#include "named_enum.hpp"
#include "fv_patch.hpp"
#include "fv_patch_field_mapper.hpp"


namespace mousse {

class radiationCoupledBase
{
public:
  //- Type of supplied emissivity
  enum emissivityMethodType
  {
    SOLIDRADIATION,
    LOOKUP
  };
private:
  // Private data
    static const NamedEnum<emissivityMethodType, 2> emissivityMethodTypeNames_;
    //- Underlying patch
    const fvPatch& patch_;
protected:
  // Protected data
    //- How to get emissivity
    const emissivityMethodType method_;
    //- Emissivity
    // Cached locally when is read from dictionary (lookup mode)
    scalarField emissivity_;
public:
  //- Runtime type information
  TYPE_NAME("radiationCoupledBase");
  // Constructors
    //- Construct from patch, emissivity mode and emissivity
    radiationCoupledBase
    (
      const fvPatch& patch,
      const word& calculationMethod,
      const scalarField& emissivity
    );
    //- Construct from patch, emissivity mode and emissivity and mapper
    radiationCoupledBase
    (
      const fvPatch& patch,
      const word& calculationMethod,
      const scalarField& emissivity,
      const fvPatchFieldMapper& mapper
    );
    //- Construct from patch and dictionary
    radiationCoupledBase
    (
      const fvPatch& patch,
      const dictionary& dict
    );
  //- Destructor
    virtual ~radiationCoupledBase();
  // Member functions
    // Access
      //- Method to obtain emissivity
      word emissivityMethod() const
      {
        return emissivityMethodTypeNames_[method_];
      }
      //- Calculate corresponding emissivity field
      scalarField emissivity() const;
    // Mapping functions
      //- Map (and resize as needed) from self given a mapping object
      virtual void autoMap
      (
        const fvPatchFieldMapper&
      );
      //- Reverse map the given fvPatchField onto this fvPatchField
      virtual void rmap
      (
        const fvPatchScalarField&,
        const labelList&
      );
    //- Write
    void write(Ostream&) const;
};

}  // namespace mousse

#endif

