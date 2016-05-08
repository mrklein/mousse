// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "coded_function_object.hpp"
#include "vol_fields.hpp"
#include "dictionary.hpp"
#include "time.hpp"
#include "sha1_digest.hpp"
#include "dynamic_code.hpp"
#include "dynamic_code_context.hpp"
#include "string_ops.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(codedFunctionObject, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  functionObject,
  codedFunctionObject,
  dictionary
);

}


// Private Member Functions 
void mousse::codedFunctionObject::prepare
(
  dynamicCode& dynCode,
  const dynamicCodeContext& context
) const
{
  // Set additional rewrite rules
  dynCode.setFilterVariable("typeName", redirectType_);
  dynCode.setFilterVariable("codeRead", codeRead_);
  dynCode.setFilterVariable("codeExecute", codeExecute_);
  dynCode.setFilterVariable("codeEnd", codeEnd_);
  dynCode.setFilterVariable("codeData", codeData_);
  dynCode.setFilterVariable("codeTimeSet", codeTimeSet_);
  // compile filtered cpp template
  dynCode.addCompileFile("function_object_template.cpp");
  dynCode.addCompileFile("filter_function_object_template.cpp");
  // copy filtered hpp template
  dynCode.addCopyFile("filter_function_object_template.hpp");
  dynCode.addCopyFile("function_object_template.hpp");
  dynCode.addCopyFile("io_function_object_template.hpp");
  // define Make/options
  dynCode.setMakeOptions
    (
      "EXE_INC = -g \\\n"
      "-I$(LIB_SRC)/finite_volume/_lninclude \\\n"
      "-I$(LIB_SRC)/mesh_tools/_lninclude \\\n"
      + context.options()
      + "\n\nLIB_LIBS = \\\n"
      + "    -lmousse_core \\\n"
      + "    -lmousse_finite_volume \\\n"
      + "    -lmousse_mesh_tools \\\n"
      + context.libs()
    );
}


mousse::dlLibraryTable& mousse::codedFunctionObject::libs() const
{
  return const_cast<Time&>(time_).libs();
}


mousse::string mousse::codedFunctionObject::description() const
{
  return "functionObject " + name();
}


void mousse::codedFunctionObject::clearRedirect() const
{
  redirectFunctionObjectPtr_.clear();
}


const mousse::dictionary& mousse::codedFunctionObject::codeDict() const
{
  return dict_;
}


// Constructors 
mousse::codedFunctionObject::codedFunctionObject
(
  const word& name,
  const Time& time,
  const dictionary& dict,
  bool readNow
)
:
  functionObject(name),
  codedBase(),
  time_(time),
  dict_(dict)
{
  if (readNow) {
    read(dict_);
  }
}


// Destructor 
mousse::codedFunctionObject::~codedFunctionObject()
{}


// Member Functions 
mousse::functionObject&
mousse::codedFunctionObject::redirectFunctionObject() const
{
  if (!redirectFunctionObjectPtr_.valid()) {
    dictionary constructDict{dict_};
    constructDict.set("type", redirectType_);
    redirectFunctionObjectPtr_ =
      functionObject::New
      (
        redirectType_,
        time_,
        constructDict
      );
  }
  return redirectFunctionObjectPtr_();
}


bool mousse::codedFunctionObject::start()
{
  updateLibrary(redirectType_);
  return redirectFunctionObject().start();
}


bool mousse::codedFunctionObject::execute(const bool forceWrite)
{
  updateLibrary(redirectType_);
  return redirectFunctionObject().execute(forceWrite);
}


bool mousse::codedFunctionObject::end()
{
  updateLibrary(redirectType_);
  return redirectFunctionObject().end();
}


bool mousse::codedFunctionObject::timeSet()
{
  updateLibrary(redirectType_);
  return redirectFunctionObject().timeSet();
}


bool mousse::codedFunctionObject::read(const dictionary& dict)
{
  dict.lookup("redirectType") >> redirectType_;
  const entry* dataPtr =
    dict.lookupEntryPtr
    (
      "codeData",
      false,
      false
    );
  if (dataPtr) {
    codeData_ = stringOps::trim(dataPtr->stream());
    stringOps::inplaceExpand(codeData_, dict);
    dynamicCodeContext::addLineDirective
      (
        codeData_,
        dataPtr->startLineNumber(),
        dict.name()
      );
  }
  const entry* readPtr =
    dict.lookupEntryPtr
    (
      "codeRead",
      false,
      false
    );
  if (readPtr) {
    codeRead_ = stringOps::trim(readPtr->stream());
    stringOps::inplaceExpand(codeRead_, dict);
    dynamicCodeContext::addLineDirective
      (
        codeRead_,
        readPtr->startLineNumber(),
        dict.name()
      );
  }
  const entry* execPtr =
    dict.lookupEntryPtr
    (
      "codeExecute",
      false,
      false
    );
  if (execPtr) {
    codeExecute_ = stringOps::trim(execPtr->stream());
    stringOps::inplaceExpand(codeExecute_, dict);
    dynamicCodeContext::addLineDirective
      (
        codeExecute_,
        execPtr->startLineNumber(),
        dict.name()
      );
  }
  const entry* endPtr =
    dict.lookupEntryPtr
    (
      "codeEnd",
      false,
      false
    );
  if (endPtr) {
    codeEnd_ = stringOps::trim(endPtr->stream());
    stringOps::inplaceExpand(codeEnd_, dict);
    dynamicCodeContext::addLineDirective
      (
        codeEnd_,
        endPtr->startLineNumber(),
        dict.name()
      );
  }
  const entry* timeSetPtr =
    dict.lookupEntryPtr
    (
      "codeTimeSet",
      false,
      false
    );
  if (timeSetPtr) {
    codeTimeSet_ = stringOps::trim(timeSetPtr->stream());
    stringOps::inplaceExpand(codeTimeSet_, dict);
    dynamicCodeContext::addLineDirective
      (
        codeTimeSet_,
        timeSetPtr->startLineNumber(),
        dict.name()
      );
  }
  updateLibrary(redirectType_);
  return redirectFunctionObject().read(dict);
}


void mousse::codedFunctionObject::updateMesh(const mapPolyMesh&)
{}


void mousse::codedFunctionObject::movePoints(const polyMesh&)
{}

