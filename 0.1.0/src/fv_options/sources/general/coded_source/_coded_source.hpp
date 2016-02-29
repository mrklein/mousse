#ifndef FV_OPTIONS_SOURCES_GENERAL_CODED_SOURCE_TCODED_SOURCE_HPP_
#define FV_OPTIONS_SOURCES_GENERAL_CODED_SOURCE_TCODED_SOURCE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::codedSource
// Description
//  Constructs on-the-fly fvOption source
//   The hook functions take the following arguments:
//   codeCorrect
//   (
//     GeometricField<Type, fvPatchField, volMesh>& field
//   )
//   codeAddSup
//   (
//     fvMatrix<Type}>& eqn,
//     const label fieldI
//   )
//   constrain
//   (
//     fvMatrix<Type}>& eqn,
//     const label fieldI
//   )
//   where :
//     field is the field in fieldNames
//     eqn is the fvMatrix
//   \heading Source usage
//   Example usage in controlDict:
//   \verbatim
//   energySource
//   {
//     type            scalarCodedSource;
//     active          yes;
//     scalarCodedSourceCoeffs
//     {
//       selectionMode   all;
//       fieldNames      (h);
//       redirectType    sourceTime;
//       codeInclude
//       #{
//       #};
//       codeCorrect
//       #{
//         Pout<< "**codeCorrect**" << endl;
//       #};
//       codeAddSup
//       #{
//         const Time& time = mesh().time();
//         const scalarField& V = mesh_.V();
//         scalarField& heSource = eqn.source();
//         heSource -= 0.1*sqr(time.value())*V;
//       #};
//       codeSetValue
//       #{
//         Pout<< "**codeSetValue**" << endl;
//       #};
//       // Dummy entry. Make dependent on above to trigger recompilation
//       code
//       #{
//         $codeInclude
//         $codeCorrect
//         $codeAddSup
//         $codeSetValue
//       #};
//     }
//     sourceTimeCoeffs
//     {
//       // Dummy entry
//     }
//   }
//   \endverbatim
// SourceFiles
//   _coded_source.cpp
#include "cell_set_option.hpp"
#include "coded_base.hpp"
namespace mousse
{
namespace fv
{
template<class Type>
class CodedSource
:
  public cellSetOption,
  public codedBase
{
protected:
  // Protected data
    word redirectType_;
    string codeCorrect_;
    string codeAddSup_;
    string codeSetValue_;
    //- Underlying functionObject
    mutable autoPtr<option> redirectFvOptionPtr_;
  // Protected Member Functions
    //- Get the loaded dynamic libraries
    virtual dlLibraryTable& libs() const;
    //- Adapt the context for the current object
    virtual void prepare(dynamicCode&, const dynamicCodeContext&) const;
    // Return a description (type + name) for the output
    virtual string description() const;
    // Clear any redirected objects
    virtual void clearRedirect() const;
    // Get the dictionary to initialize the codeContext
    virtual const dictionary& codeDict() const;
public:
  //- Runtime type information
  TYPE_NAME("coded");
  // Constructors
    //- Construct from components
    CodedSource
    (
      const word& name,
      const word& modelType,
      const dictionary& dict,
      const fvMesh& mesh
    );
  // Member Functions
    //- Dynamically compiled fvOption
    option& redirectFvOption() const;
    // Evaluation
      //- Correct field
      virtual void correct
      (
        GeometricField<Type, fvPatchField, volMesh>&
      );
      //- Explicit and implicit matrix contributions
      virtual void addSup
      (
        fvMatrix<Type>& eqn,
        const label fieldI
      );
      //- Explicit and implicit matrix contributions
      //  to compressible equation
      virtual void addSup
      (
        const volScalarField& rho,
        fvMatrix<Type>& eqn,
        const label fieldI
      );
      //- Set value
      virtual void constrain
      (
        fvMatrix<Type>& eqn,
        const label fieldI
      );
    // IO
      //- Read source dictionary
      virtual bool read(const dictionary& dict);
};
}  // namespace fv
}  // namespace mousse
#ifdef NoRepository
#   include "_coded_source.cpp"
#   include "_coded_source_io.cpp"
#endif
#endif
