#ifndef REGION_COUPLED_DERIVED_FV_PATCH_FIELDS_ENERGY_REGION_COUPLED_ENERGY_REGION_COUPLED_FV_PATCH_SCALAR_FIELD_HPP_
#define REGION_COUPLED_DERIVED_FV_PATCH_FIELDS_ENERGY_REGION_COUPLED_ENERGY_REGION_COUPLED_FV_PATCH_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::energyRegionCoupledFvPatchScalarField
// Description
//   Energy region coupled implicit boundary condition.
//   The fvPatch is treated as uncoupled from the delta point of view.
//   In the mesh the fvPatch is an interface and is incorporated
//   into the matrix implicitly.

#include "region_coupled_base_fv_patch.hpp"
#include "ldu_interface_field.hpp"
#include "fv_patch_field.hpp"
#include "named_enum.hpp"
#include "basic_thermo.hpp"
#include "coupled_fv_patch_field.hpp"


namespace mousse {

class energyRegionCoupledFvPatchScalarField
:
  public coupledFvPatchField<scalar>
{
public:
    enum kappaMethodType
    {
      SOLID,
      FLUID,
      UNDEFINED
    };
private:
  // Private data
    //- Local reference to region couple patch
    const regionCoupledBaseFvPatch& regionCoupledPatch_;
    //- Methof to extract kappa
    static const NamedEnum<kappaMethodType, 3> methodTypeNames_;
    //- How to get K
    mutable kappaMethodType method_;
    //- AutoPtr to nbr thermo
    mutable const basicThermo* nbrThermoPtr_;
    //- AutoPtr to my thermo
    mutable const basicThermo* thermoPtr_;
  // Private functions
    //- Local weight for this coupled field
    tmp<scalarField> weights() const;
    //- Return nbr temperature internal field
    tmp<scalarField> patchNeighbourTemperatureField() const;
    //- Return local temperature internal field
    tmp<scalarField> patchInternalTemperatureField() const;
    //- Return kappa
    tmp<scalarField> kappa() const;
    //- Set method
    void setMethod() const;
public:
  //- Runtime type information
  TYPE_NAME("compressible::energyRegionCoupled");
  // Constructors
    //- Construct from patch and internal field
    energyRegionCoupledFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    energyRegionCoupledFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given energyRegionCoupledFvPatchScalarField
    // onto a new patch
    energyRegionCoupledFvPatchScalarField
    (
      const energyRegionCoupledFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    energyRegionCoupledFvPatchScalarField
    (
      const energyRegionCoupledFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<scalar>> clone() const
    {
      return
        tmp<fvPatchField<scalar>>
        {
          new energyRegionCoupledFvPatchScalarField{*this}
        };
    }
    //- Construct as copy setting internal field reference
    energyRegionCoupledFvPatchScalarField
    (
      const energyRegionCoupledFvPatchScalarField&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchField<scalar>> clone
    (
      const DimensionedField<scalar, volMesh>& iF
    ) const
    {
      return
        tmp<fvPatchField<scalar>>
        {
          new energyRegionCoupledFvPatchScalarField{*this, iF}
        };
    }
  //- Destructor
  virtual ~energyRegionCoupledFvPatchScalarField()
  {}
  // Member functions
    // Access
      //- Method to obtain K
      word kappaMethod() const { return methodTypeNames_[method_]; }
    // Evaluation functions
      //- Return neighbour coupled internal cell data
      virtual tmp<scalarField> patchNeighbourField() const;
      //- Return patch-normal gradient
      virtual tmp<scalarField> snGrad() const;
      //- Return patch-normal gradient
      //  Note: the deltaCoeffs supplied are not used
      virtual tmp<scalarField> snGrad
      (
        const scalarField& deltaCoeffs
      ) const;
      //- Evaluate the patch field
      virtual void evaluate
      (
        const Pstream::commsTypes commsType
      );
    // Coupled interface functionality
      //- Update result field based on interface functionality
      virtual void updateInterfaceMatrix
      (
        Field<scalar>& result,
        const scalarField& psiInternal,
        const scalarField& coeffs,
        const direction cmpt,
        const Pstream::commsTypes commsType
      ) const;
      //- Update result field based on interface functionality
      virtual void updateInterfaceMatrix
      (
        Field<scalar>&,
        const Field<scalar>&,
        const scalarField&,
        const Pstream::commsTypes commsType
      ) const;
      //- Return the interface type
      virtual const word& interfaceFieldType() const
      {
        return regionCoupledPatch_.regionCoupleType();
      }
    //- Write
    virtual void write(Ostream&) const;
};

}  // namespace mousse

#endif

