#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_EXTERNAL_COUPLED_MIXED_FV_PATCH_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_EXTERNAL_COUPLED_MIXED_FV_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::externalCoupledMixedFvPatchField
// Group
//   grpGenericBoundaryConditions grpCoupledBoundaryConditions
// Description
//   This boundary condition provides an interface to an external application.
//   Values are transferred as plain text files, where mousse data is written
//   as:
//     # Patch: <patch name>
//     <magSf1> <value1> <surfaceNormalGradient1>
//     <magSf2> <value2> <surfaceNormalGradient2>
//     <magSf3> <value3> <surfaceNormalGradient3>
//     ...
//     <magSfN> <valueN> <surfaceNormalGradientN>
//   and received as the constituent pieces of the `mixed' condition, i.e.
//     # Patch: <patch name>
//     <value1> <gradient1> <valueFracion1>
//     <value2> <gradient2> <valueFracion2>
//     <value3> <gradient3> <valueFracion3>
//     ...
//     <valueN> <gradientN> <valueFracionN>
//   Data is sent/received as a single file for all patches from the directory
//     $MOUSSE_CASE/<commsDir>
//   At start-up, the boundary creates a lock file, i.e..
//     mousse.lock
//   ... to signal the external source to wait.  During the boundary condition
//   update, boundary values are written to file, e.g.
//     <fileName>.out
//   The lock file is then removed, instructing the external source to take
//   control of the program execution.  When ready, the external program
//   should create the return values, e.g. to file
//     <fileName>.in
//   ... and then re-instate the lock file.  The boundary condition will then
//   read the return values, and pass program execution back to mousse.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     commsDir     | communications directory   | yes         |
//     fileName     | transfer file name      | yes         |
//     waitInterval | interval [s] between file checks | no | 1
//     timeOut      | time after which error invoked [s] |no |100*waitInterval
//     calcFrequency | calculation frequency  | no          | 1
//     initByExternal | external app to initialises values  | yes |
//     log          | log program control     | no          | no
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            externalCoupled;
//     commsDir        "$MOUSSE_CASE/comms";
//     fileName        data;
//     calcFrequency   1;
//     initByExternal  yes;
//   }
//   \endverbatim
// SeeAlso
//   mixedFvPatchField

#include "mixed_fv_patch_fields.hpp"
#include "ofstream.hpp"
#include "geometric_fields.hpp"


namespace mousse {

class IFstream;


template<class Type>
class externalCoupledMixedFvPatchField
:
  public mixedFvPatchField<Type>
{
private:

  // Private data

    //- Convenience typedefs
    typedef externalCoupledMixedFvPatchField<Type> patchType;
    typedef GeometricField<Type, fvPatchField, volMesh> volFieldType;

    //- Path to communications directory
    fileName commsDir_;

    //- Name of data file
    word fName_;

    //- Interval time between checking for return data [s]
    label waitInterval_;

    //- Time out time [s]
    label timeOut_;

    //- Calculation frequency
    label calcFrequency_;

    //- Flag to indicate values are initialised by external application
    bool initByExternal_;

    //- Log flag
    bool log_;

    //- Master patch flag - controls when to pause/resume execution
    //  Note: only valid when collate option is selected
    bool master_;

    //- Offsets in data file to start reading at correct position
    List<List<label>> offsets_;

    //- Initialised flag
    bool initialised_;

    //- List of coupled patch IDs
    List<label> coupledPatchIDs_;

  // Private Member Functions

    //- Initialise
    void initialise(const fileName& transferFile);

    //- Set the master flag when collate option is selected
    void setMaster(const labelList& patchIDs);

    //- Return the file path to the base communications directory
    fileName baseDir() const;

    //- Write the geometry to the comms dir
    void writeGeometry(OFstream& osPoints, OFstream& osFaces) const;

    //- Return the file path to the lock file
    fileName lockFile() const;

    //- Create lock file
    void createLockFile() const;

    //- Remove lock file
    void removeLockFile() const;

    //- Wait for response from external source
    void startWait() const;

    //- Wait for response from external source
    void wait() const;

    //- Initialise input stream for reading
    void initialiseRead(IFstream& is) const;

protected:

  // Protected Member Functions

    //- Read data from external source
    virtual void readData(const fileName& transferFile);

    //- Write data for external source - calls transferData
    virtual void writeData(const fileName& transferFile) const;

    //- Write header to transfer file
    virtual void writeHeader(OFstream& os) const;

public:

  //- Runtime type information
  TYPE_NAME("externalCoupled");

  //- Name of lock file
  static word lockName;

  //- Name of patch key, e.g. '# Patch:' when looking for start of patch data
  static string patchKey;

  // Constructors

    //- Construct from patch and internal field
    externalCoupledMixedFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );

    //- Construct from patch, internal field and dictionary
    externalCoupledMixedFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );

    //- Construct by mapping given externalCoupledMixedFvPatchField
    //  onto a new patch
    externalCoupledMixedFvPatchField
    (
      const externalCoupledMixedFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );

    //- Construct as copy
    externalCoupledMixedFvPatchField
    (
      const externalCoupledMixedFvPatchField&
    );

    //- Construct and return a clone
    virtual tmp<fvPatchField<Type>> clone() const
    {
      return tmp<fvPatchField<Type>>
      {
        new externalCoupledMixedFvPatchField<Type>{*this}
      };
    }

    //- Construct as copy setting internal field reference
    externalCoupledMixedFvPatchField
    (
      const externalCoupledMixedFvPatchField&,
      const DimensionedField<Type, volMesh>&
    );

    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchField<Type>> clone
    (
      const DimensionedField<Type, volMesh>& iF
    ) const
    {
      return tmp<fvPatchField<Type>>
      {
        new externalCoupledMixedFvPatchField<Type>{*this, iF}
      };
    }

  //- Destructor
  virtual ~externalCoupledMixedFvPatchField();

  // Member functions

    // Access

      //- Return the log flag
      bool log() const
      {
        return log_;
      }

      //- Return the master flag
      bool master() const
      {
        return master_;
      }

      //- Return the master flag
      bool& master()
      {
        return master_;
      }

    // Evaluation functions

      //- Evaluate the patch field
      virtual void evaluate
      (
        const Pstream::commsTypes commsType=Pstream::blocking
      );

      //- Transfer data for external source
      virtual void transferData(OFstream& os) const;

    //- Write the geometry to the comms dir
    void writeGeometry() const;

    //- Write
    virtual void write(Ostream&) const;

};

}  // namespace mousse

#include "external_coupled_mixed_fv_patch_field.ipp"

#endif
