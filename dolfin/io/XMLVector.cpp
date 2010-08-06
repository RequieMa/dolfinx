// Copyright (C) 2002-2006 Anders Logg and Ola Skavhaug.
// Licensed under the GNU LGPL Version 2.1.
//
// Modified by Garth N. Wells, 2009.
//
// First added:  2009-03-06
// Last changed: 2009-06-15


#include <dolfin/common/Array.h>
#include <dolfin/log/dolfin_log.h>
#include <dolfin/la/GenericVector.h>
#include "XMLIndent.h"
#include "XMLVector.h"

using namespace dolfin;

//-----------------------------------------------------------------------------
XMLVector::XMLVector(GenericVector& vector, XMLFile& parser)
  : XMLHandler(parser), x(vector), state(OUTSIDE), values(0)
{
  // Do nothing
}
//-----------------------------------------------------------------------------
XMLVector::~XMLVector()
{
  // Do nothing
}
//-----------------------------------------------------------------------------
void XMLVector::start_element(const xmlChar *name, const xmlChar **attrs)
{
  switch ( state )
  {
  case OUTSIDE:
    if ( xmlStrcasecmp(name, (xmlChar *) "vector") == 0 )
      read_vector_tag(name, attrs);
    break;
  case INSIDE_VECTOR:
    if ( xmlStrcasecmp(name, (xmlChar *) "array") == 0 )
      read_array_tag(name, attrs);
    break;
  default:
    break;
  }
}
//-----------------------------------------------------------------------------
void XMLVector::end_element(const xmlChar *name)
{
  switch ( state )
  {
  case INSIDE_VECTOR:
    if ( xmlStrcasecmp(name, (xmlChar *) "vector") == 0 )
    {
      end_vector();
      state = DONE;
      release();
    }
    break;
  default:
    break;
  }
}
//-----------------------------------------------------------------------------
void XMLVector::write(const GenericVector& vector, std::ostream& outfile,
                      uint indentation_level)
{
  XMLIndent indent(indentation_level);

  // Write vector header
  outfile << indent() << "<vector>" << std::endl;

  const std::pair<uint, uint> range = vector.local_range();
  const uint n0 = range.first;
  const uint size = range.second - range.first;

  // Get data
  Array<double> vector_values(size);
  vector.get_local(vector_values);

  // Write array
  ++indent;
  XMLArray::write(vector_values, n0, outfile, indent.level());
  --indent;

  // Write vector footer
  outfile << indent() << "</vector>" << std::endl;
}
//-----------------------------------------------------------------------------
void XMLVector::read_vector_tag(const xmlChar *name, const xmlChar **attrs)
{
  state = INSIDE_VECTOR;
}
//-----------------------------------------------------------------------------
void XMLVector::read_array_tag(const xmlChar *name, const xmlChar **attrs)
{
  xml_array.reset(new XMLArray(values, parser));
  xml_array->read_array_tag(name, attrs);
  xml_array->handle();
}
//-----------------------------------------------------------------------------
void XMLVector::end_vector()
{
  assert(xml_array);

  // Resize vector
  x.resize(xml_array->global_size);

  // Set values
  x.set(&values[0], xml_array->element_index.size(), &(xml_array->element_index)[0]);
  x.apply("insert");
}
//-----------------------------------------------------------------------------
