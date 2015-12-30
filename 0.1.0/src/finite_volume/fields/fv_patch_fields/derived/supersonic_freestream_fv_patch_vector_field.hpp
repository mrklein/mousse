// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::supersonicFreestreamFvPatchVectorField
// Group
//   grpInletBoundaryConditions grpOutletBoundaryConditions
// Description
//   This boundary condition provides a supersonic free-stream condition.
//   - supersonic outflow is vented according to ???
//   - supersonic inflow is assumed to occur according to the Prandtl-Meyer
//    expansion process.
//   - subsonic outflow is applied via a zero-gradient condition from inside
//    the domain.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     TName        | Temperature field name  | no          | T
//     pName        | Pressure field name     | no          | p
//     psiName      | Compressibility field name | no       | thermo:psi
//     UInf         | free-stream velocity    | yes         |
//     pInf         | free-stream pressure    | yes         |
//     TInf         | free-stream temperature | yes         |
//     gamma        | heat capacity ratio (cp/Cv) | yes     |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            supersonicFreestream;
//     UInf            500;
//     pInf            1e4;
//     TInf            265;
//     gamma           1.4;
//   }
//   \endverbatim
// Note
//   This boundary condition is ill-posed if the free-stream flow is normal
//   to the boundary.
// SourceFiles
//   supersonic_freestream_fv_patch_vector_field.cpp
#ifndef supersonicFreestreamFvPatchVectorFields_H
#define supersonicFreestreamFvPatchVectorFields_H
#include "fv_patch_fields.hpp"
#include "mixed_fv_patch_fields.hpp"
namespace mousse
{
class supersonicFreestreamFvPatchVectorField
:
  public mixedFvPatchVectorField
{
  // Private data
    //- Name of temperature field, default = "T"
    word TName_;
    //- Name of pressure field, default = "p"
    word pName_;
    //- Name of compressibility field field, default = "thermo:psi"
    word psiName_;
    //- Velocity of the free stream
    vector UInf_;
    //- Pressure of the free stream
    scalar pInf_;
    //- Temperature of the free stream
    scalar TInf_;
    //- Heat capacity ratio
    scalar gamma_;
public:
  //- Runtime type information
  TypeName("supersonicFreestream");
  // Constructors
    //- Construct from patch and internal field
    supersonicFreestreamFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    supersonicFreestreamFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given supersonicFreestreamFvPatchVectorField
    //  onto a new patch
    supersonicFreestreamFvPatchVectorField
    (
      const supersonicFreestreamFvPatchVectorField&,
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    supersonicFreestreamFvPatchVectorField
    (
      const supersonicFreestreamFvPatchVectorField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchVectorField> clone() const
    {
      return tmp<fvPatchVectorField>
      (
        new supersonicFreestreamFvPatchVectorField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    supersonicFreestreamFvPatchVectorField
    (
      const supersonicFreestreamFvPatchVectorField&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchVectorField> clone
    (
      const DimensionedField<vector, volMesh>& iF
    ) const
    {
      return tmp<fvPatchVectorField>
      (
        new supersonicFreestreamFvPatchVectorField(*this, iF)
      );
    }
  // Member functions
    // Access
      //- Return the velocity at infinity
      const vector& UInf() const
      {
        return UInf_;
      }
      //- Return reference to the velocity at infinity to allow adjustment
      vector& UInf()
      {
        return UInf_;
      }
      //- Return the pressure at infinity
      scalar pInf() const
      {
        return pInf_;
      }
      //- Return reference to the pressure at infinity to allow adjustment
      scalar& pInf()
      {
        return pInf_;
      }
      //- Return the temperature at infinity
      scalar TInf() const
      {
        return TInf_;
      }
      //- Return reference to the temperature at infinity
      //  to allow adjustment
      scalar& TInf()
      {
        return TInf_;
      }
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
