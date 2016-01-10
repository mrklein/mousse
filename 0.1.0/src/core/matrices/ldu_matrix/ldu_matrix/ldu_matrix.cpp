// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ldu_matrix.hpp"
#include "iostreams.hpp"
#include "switch.hpp"

// Static Data Members
namespace mousse {
DEFINE_TYPE_NAME_AND_DEBUG(lduMatrix, 1);
}

mousse::lduMatrix::lduMatrix(const lduMesh& mesh)
:
  lduMesh_{mesh},
  lowerPtr_{NULL},
  diagPtr_{NULL},
  upperPtr_{NULL}
{}


mousse::lduMatrix::lduMatrix(const lduMatrix& A)
:
  lduMesh_{A.lduMesh_},
  lowerPtr_{NULL},
  diagPtr_{NULL},
  upperPtr_{NULL}
{
  if (A.lowerPtr_)
  {
    lowerPtr_ = new scalarField(*(A.lowerPtr_));
  }
  if (A.diagPtr_)
  {
    diagPtr_ = new scalarField(*(A.diagPtr_));
  }
  if (A.upperPtr_)
  {
    upperPtr_ = new scalarField(*(A.upperPtr_));
  }
}


mousse::lduMatrix::lduMatrix(lduMatrix& A, bool reUse)
:
  lduMesh_{A.lduMesh_},
  lowerPtr_{NULL},
  diagPtr_{NULL},
  upperPtr_{NULL}
{
  if (reUse)
  {
    if (A.lowerPtr_)
    {
      lowerPtr_ = A.lowerPtr_;
      A.lowerPtr_ = NULL;
    }
    if (A.diagPtr_)
    {
      diagPtr_ = A.diagPtr_;
      A.diagPtr_ = NULL;
    }
    if (A.upperPtr_)
    {
      upperPtr_ = A.upperPtr_;
      A.upperPtr_ = NULL;
    }
  }
  else
  {
    if (A.lowerPtr_)
    {
      lowerPtr_ = new scalarField(*(A.lowerPtr_));
    }
    if (A.diagPtr_)
    {
      diagPtr_ = new scalarField(*(A.diagPtr_));
    }
    if (A.upperPtr_)
    {
      upperPtr_ = new scalarField(*(A.upperPtr_));
    }
  }
}


mousse::lduMatrix::lduMatrix(const lduMesh& mesh, Istream& is)
:
  lduMesh_{mesh},
  lowerPtr_{NULL},
  diagPtr_{NULL},
  upperPtr_{NULL}
{
  Switch hasLow(is);
  Switch hasDiag(is);
  Switch hasUp(is);
  if (hasLow)
  {
    lowerPtr_ = new scalarField(is);
  }
  if (hasDiag)
  {
    diagPtr_ = new scalarField(is);
  }
  if (hasUp)
  {
    upperPtr_ = new scalarField{is};
  }
}


mousse::lduMatrix::~lduMatrix()
{
  if (lowerPtr_)
  {
    delete lowerPtr_;
  }
  if (diagPtr_)
  {
    delete diagPtr_;
  }
  if (upperPtr_)
  {
    delete upperPtr_;
  }
}


mousse::scalarField& mousse::lduMatrix::lower()
{
  if (!lowerPtr_)
  {
    if (upperPtr_)
    {
      lowerPtr_ = new scalarField{*upperPtr_};
    }
    else
    {
      lowerPtr_ = new scalarField{lduAddr().lowerAddr().size(), 0.0};
    }
  }
  return *lowerPtr_;
}


mousse::scalarField& mousse::lduMatrix::diag()
{
  if (!diagPtr_)
  {
    diagPtr_ = new scalarField{lduAddr().size(), 0.0};
  }
  return *diagPtr_;
}


mousse::scalarField& mousse::lduMatrix::upper()
{
  if (!upperPtr_)
  {
    if (lowerPtr_)
    {
      upperPtr_ = new scalarField{*lowerPtr_};
    }
    else
    {
      upperPtr_ = new scalarField{lduAddr().lowerAddr().size(), 0.0};
    }
  }
  return *upperPtr_;
}


mousse::scalarField& mousse::lduMatrix::lower(const label nCoeffs)
{
  if (!lowerPtr_)
  {
    if (upperPtr_)
    {
      lowerPtr_ = new scalarField(*upperPtr_);
    }
    else
    {
      lowerPtr_ = new scalarField(nCoeffs, 0.0);
    }
  }
  return *lowerPtr_;
}


mousse::scalarField& mousse::lduMatrix::diag(const label size)
{
  if (!diagPtr_)
  {
    diagPtr_ = new scalarField(size, 0.0);
  }
  return *diagPtr_;
}


mousse::scalarField& mousse::lduMatrix::upper(const label nCoeffs)
{
  if (!upperPtr_)
  {
    if (lowerPtr_)
    {
      upperPtr_ = new scalarField{*lowerPtr_};
    }
    else
    {
      upperPtr_ = new scalarField{nCoeffs, 0.0};
    }
  }
  return *upperPtr_;
}


const mousse::scalarField& mousse::lduMatrix::lower() const
{
  if (!lowerPtr_ && !upperPtr_)
  {
    FATAL_ERROR_IN("lduMatrix::lower() const")
      << "lowerPtr_ or upperPtr_ unallocated"
      << abort(FatalError);
  }
  if (lowerPtr_)
  {
    return *lowerPtr_;
  }
  else
  {
    return *upperPtr_;
  }
}


const mousse::scalarField& mousse::lduMatrix::diag() const
{
  if (!diagPtr_)
  {
    FATAL_ERROR_IN("const scalarField& lduMatrix::diag() const")
      << "diagPtr_ unallocated"
      << abort(FatalError);
  }
  return *diagPtr_;
}


const mousse::scalarField& mousse::lduMatrix::upper() const
{
  if (!lowerPtr_ && !upperPtr_)
  {
    FATAL_ERROR_IN("lduMatrix::upper() const")
      << "lowerPtr_ or upperPtr_ unallocated"
      << abort(FatalError);
  }
  if (upperPtr_)
  {
    return *upperPtr_;
  }
  else
  {
    return *lowerPtr_;
  }
}


// Friend Operators 
mousse::Ostream& mousse::operator<<(Ostream& os, const lduMatrix& ldum)
{
  Switch hasLow = ldum.hasLower();
  Switch hasDiag = ldum.hasDiag();
  Switch hasUp = ldum.hasUpper();
  os  << hasLow << token::SPACE << hasDiag << token::SPACE
    << hasUp << token::SPACE;
  if (hasLow)
  {
    os  << ldum.lower();
  }
  if (hasDiag)
  {
    os  << ldum.diag();
  }
  if (hasUp)
  {
    os  << ldum.upper();
  }
  os.check("Ostream& operator<<(Ostream&, const lduMatrix&");
  return os;
}


mousse::Ostream& mousse::operator<<(Ostream& os, const InfoProxy<lduMatrix>& ip)
{
  const lduMatrix& ldum = ip.t_;
  Switch hasLow = ldum.hasLower();
  Switch hasDiag = ldum.hasDiag();
  Switch hasUp = ldum.hasUpper();
  os  << "Lower:" << hasLow
    << " Diag:" << hasDiag
    << " Upper:" << hasUp << endl;
  if (hasLow)
  {
    os  << "lower:" << ldum.lower().size() << endl;
  }
  if (hasDiag)
  {
    os  << "diag :" << ldum.diag().size() << endl;
  }
  if (hasUp)
  {
    os  << "upper:" << ldum.upper().size() << endl;
  }
  os.check("Ostream& operator<<(Ostream&, const lduMatrix&");
  return os;
}
