// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::option
// Description
//   Finite volume options abtract base class.  Provides a base set of controls,
//   e.g.
//     type            scalarExplicitSource    // source type
//     active          on;                     // on/off switch
// Note:
//   On evaluation, source/sink options are to be added to the equation R.H.S.
// SourceFiles
//   fv_option.cpp
//   fv_option_io.cpp
#ifndef fv_option_hpp_
#define fv_option_hpp_
#include "fv_matrices_fwd.hpp"
#include "vol_fields_fwd.hpp"
#include "dictionary.hpp"
#include "switch.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class fvMesh;
namespace fv
{
class option
{
protected:
  // Protected data
    //- Source name
    const word name_;
    //- Model type
    const word modelType_;
    //- Reference to the mesh database
    const fvMesh& mesh_;
    //- Top level source dictionary
    dictionary dict_;
    //- Dictionary containing source coefficients
    dictionary coeffs_;
    //- Source active flag
    Switch active_;
    //- Field names to apply source to - populated by derived models
    wordList fieldNames_;
    //- Applied flag list - corresponds to each fieldNames_ entry
    List<bool> applied_;
public:
  //- Runtime type information
  TypeName("option");
  // Declare run-time constructor selection table
    declareRunTimeSelectionTable
    (
      autoPtr,
      option,
      dictionary,
      (
        const word& name,
        const word& modelType,
        const dictionary& dict,
        const fvMesh& mesh
      ),
      (name, modelType, dict, mesh)
    );
  // Constructors
    //- Construct from components
    option
    (
      const word& name,
      const word& modelType,
      const dictionary& dict,
      const fvMesh& mesh
    );
    //- Return clone
    autoPtr<option> clone() const
    {
      notImplemented("autoPtr<option> clone() const");
      return autoPtr<option>(NULL);
    }
    //- Return pointer to new fvOption object created
    //  on the freestore from an Istream
    class iNew
    {
      //- Reference to the mesh
      const fvMesh& mesh_;
      const word& name_;
    public:
      iNew
      (
        const fvMesh& mesh,
        const word& name
      )
      :
        mesh_(mesh),
        name_(name)
      {}
      autoPtr<option> operator()(Istream& is) const
      {
        //const word name(is);
        const dictionary dict(is);
        return autoPtr<option>
        (
          option::New(name_, dict, mesh_)
        );
      }
    };
  // Selectors
    //- Return a reference to the selected fvOption model
    static autoPtr<option> New
    (
      const word& name,
      const dictionary& dict,
      const fvMesh& mesh
    );
  //- Destructor
  virtual ~option();
  // Member Functions
    // Access
      //- Return const access to the source name
      inline const word& name() const;
      //- Return const access to the mesh database
      inline const fvMesh& mesh() const;
      //- Return dictionary
      inline const dictionary& coeffs() const;
      //- Return const access to the source active flag
      inline bool active() const;
      //- Set the applied flag to true for field index fieldI
      inline void setApplied(const label fieldI);
    // Edit
      //- Return access to the source active flag
      inline Switch& active();
    // Checks
      //- Is the source active?
      virtual bool isActive();
      //- Return index of field name if found in fieldNames list
      virtual label applyToField(const word& fieldName) const;
      //- Check that the source has been applied
      virtual void checkApplied() const;
    // Evaluation
      // Explicit and implicit sources
        virtual void addSup
        (
          fvMatrix<scalar>& eqn,
          const label fieldI
        );
        virtual void addSup
        (
          fvMatrix<vector>& eqn,
          const label fieldI
        );
        virtual void addSup
        (
          fvMatrix<symmTensor>& eqn,
          const label fieldI
        );
        virtual void addSup
        (
          fvMatrix<sphericalTensor>& eqn,
          const label fieldI
        );
        virtual void addSup
        (
          fvMatrix<tensor>& eqn,
          const label fieldI
        );
      // Explicit and implicit sources for compressible equations
        virtual void addSup
        (
          const volScalarField& rho,
          fvMatrix<scalar>& eqn,
          const label fieldI
        );
        virtual void addSup
        (
          const volScalarField& rho,
          fvMatrix<vector>& eqn,
          const label fieldI
        );
        virtual void addSup
        (
          const volScalarField& rho,
          fvMatrix<symmTensor>& eqn,
          const label fieldI
        );
        virtual void addSup
        (
          const volScalarField& rho,
          fvMatrix<sphericalTensor>& eqn,
          const label fieldI
        );
        virtual void addSup
        (
          const volScalarField& rho,
          fvMatrix<tensor>& eqn,
          const label fieldI
        );
      // Explicit and implicit sources for phase equations
        virtual void addSup
        (
          const volScalarField& alpha,
          const volScalarField& rho,
          fvMatrix<scalar>& eqn,
          const label fieldI
        );
        virtual void addSup
        (
          const volScalarField& alpha,
          const volScalarField& rho,
          fvMatrix<vector>& eqn,
          const label fieldI
        );
        virtual void addSup
        (
          const volScalarField& alpha,
          const volScalarField& rho,
          fvMatrix<symmTensor>& eqn,
          const label fieldI
        );
        virtual void addSup
        (
          const volScalarField& alpha,
          const volScalarField& rho,
          fvMatrix<sphericalTensor>& eqn,
          const label fieldI
        );
        virtual void addSup
        (
          const volScalarField& alpha,
          const volScalarField& rho,
          fvMatrix<tensor>& eqn,
          const label fieldI
        );
      // Constraints
        virtual void constrain
        (
          fvMatrix<scalar>& eqn,
          const label fieldI
        );
        virtual void constrain
        (
          fvMatrix<vector>& eqn,
          const label fieldI
        );
        virtual void constrain
        (
          fvMatrix<sphericalTensor>& eqn,
          const label fieldI
        );
        virtual void constrain
        (
          fvMatrix<symmTensor>& eqn,
          const label fieldI
        );
        virtual void constrain
        (
          fvMatrix<tensor>& eqn,
          const label fieldI
        );
      // Correction
        virtual void correct(volScalarField& field);
        virtual void correct(volVectorField& field);
        virtual void correct(volSphericalTensorField& field);
        virtual void correct(volSymmTensorField& field);
        virtual void correct(volTensorField& field);
    // IO
      //- Write the source header information
      virtual void writeHeader(Ostream&) const;
      //- Write the source footer information
      virtual void writeFooter(Ostream&) const;
      //- Write the source properties
      virtual void writeData(Ostream&) const;
      //- Read source dictionary
      virtual bool read(const dictionary& dict);
};
}  // namespace fv
}  // namespace mousse
#include "fv_option_i.hpp"
#endif
