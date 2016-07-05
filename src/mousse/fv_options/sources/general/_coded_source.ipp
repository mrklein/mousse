// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_coded_source.hpp"
#include "fv_mesh.hpp"
#include "fv_matrices.hpp"
#include "dynamic_code.hpp"
#include "dynamic_code_context.hpp"


// Private Member Functions 
template<class Type>
void mousse::fv::CodedSource<Type>::prepare
(
  dynamicCode& dynCode,
  const dynamicCodeContext& context
) const
{
  word sourceType(pTraits<Type>::typeName);
  // Set additional rewrite rules
  dynCode.setFilterVariable("typeName", redirectType_);
  dynCode.setFilterVariable("TemplateType", sourceType);
  dynCode.setFilterVariable("SourceType", sourceType + "Source");
  //dynCode.removeFilterVariable("code");
  dynCode.setFilterVariable("codeCorrect", codeCorrect_);
  dynCode.setFilterVariable("codeAddSup", codeAddSup_);
  dynCode.setFilterVariable("codeSetValue", codeSetValue_);
  // compile filtered C template
  dynCode.addCompileFile("codedFvOptionTemplate.C");
  // copy filtered H template
  dynCode.addCopyFile("codedFvOptionTemplate.H");
  // define Make/options
  dynCode.setMakeOptions
    (
      "EXE_INC = -g \\\n"
      "-I$(LIB_SRC)/fvOptions/lnInclude \\\n"
      "-I$(LIB_SRC)/finiteVolume/lnInclude \\\n"
      "-I$(LIB_SRC)/meshTools/lnInclude \\\n"
      "-I$(LIB_SRC)/sampling/lnInclude \\\n"
      + context.options()
      + "\n\nLIB_LIBS = \\\n"
      + "    -lmeshTools \\\n"
      + "    -lfvOptions \\\n"
      + "    -lsampling \\\n"
      + "    -lfiniteVolume \\\n"
      + context.libs()
    );
}


template<class Type>
mousse::dlLibraryTable& mousse::fv::CodedSource<Type>::libs() const
{
  return const_cast<Time&>(mesh_.time()).libs();
}


template<class Type>
mousse::string mousse::fv::CodedSource<Type>::description() const
{
  return "fvOption:: " + name_;
}


template<class Type>
void mousse::fv::CodedSource<Type>::clearRedirect() const
{
  redirectFvOptionPtr_.clear();
}


template<class Type>
const mousse::dictionary& mousse::fv::CodedSource<Type>::codeDict() const
{
  return coeffs_;
}


// Constructors 
template<class Type>
mousse::fv::CodedSource<Type>::CodedSource
(
  const word& name,
  const word& modelType,
  const dictionary& dict,
  const fvMesh& mesh
)
:
  cellSetOption{name, modelType, dict, mesh}
{
  read(dict);
}


// Member Functions 
template<class Type>
mousse::fv::option& mousse::fv::CodedSource<Type>::redirectFvOption() const
{
  if (!redirectFvOptionPtr_.valid()) {
    dictionary constructDict(dict_);
    constructDict.set("type", redirectType_);
    redirectFvOptionPtr_ =
      option::New
      (
        redirectType_,
        constructDict,
        mesh_
      );
  }
  return redirectFvOptionPtr_();
}


template<class Type>
void mousse::fv::CodedSource<Type>::correct
(
  GeometricField<Type, fvPatchField, volMesh>& field
)
{
  if (debug) {
    Info << "CodedSource<"<< pTraits<Type>::typeName
      << ">::correct for source " << name_ << endl;
  }
  updateLibrary(redirectType_);
  redirectFvOption().correct(field);
}


template<class Type>
void mousse::fv::CodedSource<Type>::addSup
(
  fvMatrix<Type>& eqn,
  const label fieldI
)
{
  if (debug) {
    Info << "CodedSource<"<< pTraits<Type>::typeName
      << ">::addSup for source " << name_ << endl;
  }
  updateLibrary(redirectType_);
  redirectFvOption().addSup(eqn, fieldI);
}


template<class Type>
void mousse::fv::CodedSource<Type>::addSup
(
  const volScalarField& rho,
  fvMatrix<Type>& eqn,
  const label fieldI
)
{
  if (debug) {
    Info << "CodedSource<"<< pTraits<Type>::typeName
      << ">::addSup for source " << name_ << endl;
  }
  updateLibrary(redirectType_);
  redirectFvOption().addSup(rho, eqn, fieldI);
}


template<class Type>
void mousse::fv::CodedSource<Type>::constrain
(
  fvMatrix<Type>& eqn,
  const label fieldI
)
{
  if (debug) {
    Info << "CodedSource<"<< pTraits<Type>::typeName
      << ">::constrain for source " << name_ << endl;
  }
  updateLibrary(redirectType_);
  redirectFvOption().constrain(eqn, fieldI);
}

