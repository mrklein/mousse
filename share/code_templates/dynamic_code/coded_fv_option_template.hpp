// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   coded_fv_option_template.cpp
#ifndef coded_fv_option_template_hpp_
#define coded_fv_option_template_hpp_
#include "fv_option.hpp"
namespace mousse
{
namespace fv
{
class ${typeName}FvOption${SourceType}
:
  public option
{
public:
  //- Information about the SHA1 of the code itself
  static const char* const SHA1sum;
  //- Runtime type information
  TypeName("${typeName}");
  // Constructors
    //- Construct from patch and internal field
    ${typeName}FvOption${SourceType}
    (
      const word& name,
      const word& modelType,
      const dictionary& dict,
      const fvMesh& mesh
    );
  //- Destructor
  virtual ~${typeName}FvOption${SourceType}();
  // Member functions
      //- Correct field
      virtual void correct
      (
        GeometricField<${TemplateType}, fvPatchField, volMesh>&
      );
      //- Explicit and implicit matrix contributions
      virtual void addSup
      (
        fvMatrix<${TemplateType}>& eqn,
        const label fieldI
      );
      //- Explicit and implicit matrix contributions for compressible
      //  equations
      virtual void addSup
      (
        const volScalarField& rho,
        fvMatrix<${TemplateType}>& eqn,
        const label fieldI
      );
      //- Set value
      virtual void setValue
      (
        fvMatrix<${TemplateType}>& eqn,
        const label fieldI
      );
};
}  // namespace mousse
}  // namespace fv
#endif
