// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::optionList
// Description
//   List of finite volume options
// SourceFile
//   optionList.C
#ifndef fv_option_list_hpp_
#define fv_option_list_hpp_
#include "fv_option.hpp"
#include "ptr_list.hpp"
#include "geometric_field.hpp"
#include "fv_patch_field.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
namespace fv
{
  class optionList;
}
Ostream& operator<<(Ostream& os, const fv::optionList& options);
namespace fv
{
class optionList
:
  public PtrList<option>
{
protected:
  // Protected data
    //- Reference to the mesh database
    const fvMesh& mesh_;
    //- Time index to check that all defined sources have been applied
    label checkTimeIndex_;
  // Protected Member Functions
    //- Return the "options" sub-dictionary if present otherwise return dict
    const dictionary& optionsDict(const dictionary& dict) const;
    //- Read options dictionary
    bool readOptions(const dictionary& dict);
    //- Check that all sources have been applied
    void checkApplied() const;
public:
  //- Runtime type information
  TYPE_NAME("optionList");
  // Constructors
    //- Construct null
    optionList(const fvMesh& mesh);
    //- Construct from mesh and dictionary
    optionList(const fvMesh& mesh, const dictionary& dict);
    //- Disallow default bitwise copy construct
    optionList(const optionList&) = delete;
    //- Disallow default bitwise assignment
    optionList& operator=(const optionList&) = delete;
  //- Destructor
  virtual ~optionList()
  {}
  // Member Functions
    //- Reset the source list
    void reset(const dictionary& dict);
    // Sources
      //- Return source for equation
      template<class Type>
      tmp<fvMatrix<Type> > operator()
      (
        GeometricField<Type, fvPatchField, volMesh>& field
      );
      //- Return source for equation with specified name
      template<class Type>
      tmp<fvMatrix<Type> > operator()
      (
        GeometricField<Type, fvPatchField, volMesh>& field,
        const word& fieldName
      );
      //- Return source for equation
      template<class Type>
      tmp<fvMatrix<Type> > operator()
      (
        const volScalarField& rho,
        GeometricField<Type, fvPatchField, volMesh>& field
      );
      //- Return source for equation with specified name
      template<class Type>
      tmp<fvMatrix<Type> > operator()
      (
        const volScalarField& rho,
        GeometricField<Type, fvPatchField, volMesh>& field,
        const word& fieldName
      );
      //- Return source for equation
      template<class Type>
      tmp<fvMatrix<Type> > operator()
      (
        const volScalarField& alpha,
        const volScalarField& rho,
        GeometricField<Type, fvPatchField, volMesh>& field
      );
      //- Return source for equation with specified name
      template<class Type>
      tmp<fvMatrix<Type> > operator()
      (
        const volScalarField& alpha,
        const volScalarField& rho,
        GeometricField<Type, fvPatchField, volMesh>& field,
        const word& fieldName
      );
    // Constraints
      //- Apply constraints to equation
      template<class Type>
      void constrain(fvMatrix<Type>& eqn);
    // Correction
      //- Apply correction to field
      template<class Type>
      void correct(GeometricField<Type, fvPatchField, volMesh>& field);
    // IO
      //- Read dictionary
      virtual bool read(const dictionary& dict);
      //- Write data to Ostream
      virtual bool writeData(Ostream& os) const;
      //- Ostream operator
      friend Ostream& operator<<
      (
        Ostream& os,
        const optionList& options
      );
};
}  // namespace fv
}  // namespace mousse
#ifdef NoRepository
  #include "fv_option_list_templates.cpp"
#endif
#endif
