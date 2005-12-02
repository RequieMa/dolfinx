// Copyright (C) 2003-2005 Johan Hoffman, Johan Jansson and Anders Logg.
// Licensed under the GNU GPL Version 2.
//
// Modified by Garth N. Wells 2005.
//
// First added:  2003-11-28
// Last changed: 2005-12-01

#ifndef __FUNCTION_H
#define __FUNCTION_H

#include <dolfin/constants.h>
#include <dolfin/Variable.h>
#include <dolfin/TimeDependent.h>
#include <dolfin/FunctionPointer.h>
#include <dolfin/GenericFunction.h>

namespace dolfin
{
  class Point;
  class Vertex;
  class Cell;
  class Mesh;
  class Vector;
  class AffineMap;
  class FiniteElement;

  /// This class represents a function that can be evaluated on a
  /// mesh. The actual representation of the function can vary, but
  /// the typical representation is in terms of a mesh, a vector of
  /// degrees of freedom and a finite element that determines the
  /// distribution of degrees of freedom on the mesh.
  ///
  /// It is also possible to have user-defined functions, either by
  /// overloading the eval function of this class or by giving a
  /// function (pointer) that returns the value of the function.

  class Function : public Variable, public TimeDependent
  {
  public:
    
    /// Create user-defined function (evaluation operator must be overloaded)
    Function(uint vectordim = 1);

    /// Create user-defined function from the given function pointer
    Function(FunctionPointer fp, uint vectordim = 1);
    
    /// Create discrete function (mesh and finite element chosen automatically)
    Function(Vector& x);

    /// Create discrete function (finite element chosen automatically)
    Function(Vector& x, Mesh& mesh);

    /// Create discrete function
    Function(Vector& x, Mesh& mesh, FiniteElement& element);

    /// Copy constructor
    Function(const Function& f);

    /// Destructor
    virtual ~Function();

    /// Evaluate function at given point (must be implemented for user-defined function)
    virtual real eval(const Point& p, uint i = 0) { return (*f)(p, i); }

    /// Evaluate function at given point
    inline real operator() (const Point& p, uint i = 0) { return (*f)(p, i); }

    /// Evaluate function at given vertex
    inline real operator() (const Vertex& vertex, uint i = 0) { return (*f)(vertex, i); }

    /// Pick sub function (of mixed function) or component of vector-valued function
    Function operator[] (const uint i);

    /// Assignment operator
    const Function& operator= (const Function& f);

    /// Compute interpolation of function onto local finite element space
    void interpolate(real coefficients[], AffineMap& map, FiniteElement& element);
    
    /// Return vector dimension of function
    inline uint vectordim() const { return f->vectordim(); }

    /// Return vector associated with function (if any)
    inline Vector& vector() { return f->vector(); }

    /// Return mesh associated with function (if any)
    inline Mesh& mesh() { return f->mesh(); }

    /// Return element associated with function (if any)
    inline FiniteElement& element() { return f->element(); }

    /// Attach vector to function
    inline void attach(Vector& x) { f->attach(x); }

    /// Attach mesh to function
    inline void attach(Mesh& mesh) { f->attach(mesh); }

    /// Attach finite element to function
    inline void attach(FiniteElement& element) { f->attach(element); }

    /// Return current type of function
    enum Type { user, functionpointer, discrete };
    inline Type type() const { return _type; } 

  protected:

    /// Return current cell (can be called by user-defined function during assembly)
    Cell& cell();

  private:
    
    // Pointer to current implementation (letter base class)
    GenericFunction* f;

    // Current function type
    Type _type;

    // Pointer to current cell
    Cell* _cell;

  };


}

#endif
