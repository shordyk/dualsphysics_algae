//HEAD_DSCODES
/*
 <DUALSPHYSICS>  Copyright (c) 2018 by Dr Jose M. Dominguez et al. (see http://dual.sphysics.org/index.php/developers/). 

 EPHYSLAB Environmental Physics Laboratory, Universidade de Vigo, Ourense, Spain.
 School of Mechanical, Aerospace and Civil Engineering, University of Manchester, Manchester, U.K.

 This file is part of DualSPHysics. 

 DualSPHysics is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License 
 as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.
 
 DualSPHysics is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details. 

 You should have received a copy of the GNU Lesser General Public License along with DualSPHysics. If not, see <http://www.gnu.org/licenses/>. 
*/

//:#############################################################################
//:# Cambios:
//:# =========
//:# - Nuevo metodo ReadElementStr() para leer un atributo string de un elemento
//:#   directamente. (14-10-2010)
//:# - En metodos con datos int3, float3 y double3 se a�adio la opcion de 
//:#   indicar el nombre de los atributos (por defecto son x,y,z). (02-05-2011)
//:# - Nuevo metodo ReadElementInt3(). (26-10-2011)
//:# - Traduccion de comentarios al ingles. (10-02-2012)
//:# - Nuevo metodo AddElement(). (13-12-2013)
//:# - Nuevo metodo ExistsElement(). (20-12-2013)
//:# - Uso de valores por defecto en ReadElementStr, ReadElementUnsigned, 
//:#   ReadElementInt, ReadElementFloat y ReadElementDouble. (20-12-2013)
//:# - Nuevo metodo ReadElementBool() y correccion en ReadElementInt(). (02-01-2014)
//:# - Nuevo metodo CheckElementNames() para comprobar elementos validos. (21-05-2014)
//:# - Convierte codigos de mayor y menor a simbolos. (20-01-2016)
//:# - Pasa varios parametros de lectura a const. (25-01-2016)
//:# - Ahora GetAttributeUnsigned/GetAttributeUint permiten todo el rango de unsigned. (06-05-2016)
//:# - Nuevos metodos para lectura y escritura de matrices. (29-11-2017)
//:# - Nuevo metodo CountElement() para contar numero de apariciones de un elemento. (24-01-2018)
//:# - Nuevos metodos CheckElementAttributes() y CheckAttributes(). (15-02-2018)
//:# - Se elimino CountElement() porque ya existia CountElements(). (15-02-2018)
//:#############################################################################

/// \file JXml.h \brief Declares the class \ref JXml.

#ifndef _JXml_
#define _JXml_

#include "TypesDef.h"
#include "JObject.h"
#include "tinyxml.h"
#include <string>

//##############################################################################
//# JXml
//##############################################################################
/// \brief Class that helps to manage the XML document using library TinyXML.

class JXml : protected JObject
{
public:
  TiXmlDocument* Doc;       ///<Pointer at the xml document.
  std::string FileReading;  ///<File to read the xml docuemnt.

  //==============================================================================
  /// Returns date and time of the system in text format (dd-mm-yyyy hh:mm:ss)
  //==============================================================================
  static std::string GetDateTime();

  //==============================================================================
  /// Returns the requested node and creates it if necessary.
  /// \param path Path of the requested node.
  /// \param createpath Allows to create the path if does not exist,
  /// otherwise returns \a NULL.
  //==============================================================================
  TiXmlNode* GetNode(const std::string &path,bool createpath=false);

  //==============================================================================
  /// Returns the root node.
  //==============================================================================
  TiXmlNode* GetNodeRoot(){ return(Doc); }

  //==============================================================================
  /// Returns the requested node and if does not exist an exception is thrown.
  /// \throw The requested node does not exist.
  //==============================================================================
  TiXmlNode* GetNodeError(const std::string &path);

  //==============================================================================
  /// Returns the first requested element of a node TiXmlNode.
  /// \param node Xml node where the reach is performed.
  /// \param name Name of filtered elements (no filter using "").
  /// \param optional If it does not exist,
  /// returns \a NULL instead of throwing an exception.
  /// \throw JException The requested element does not exist...
  //==============================================================================
  TiXmlElement* GetFirstElement(const TiXmlNode* node,const std::string &name,bool optional=false)const;

  //==============================================================================
  /// Returns the next requested element of a node TiXmlNode.
  /// \param node Xml node where the reach is performed.
  /// \param name Name of filtered elements (no filter using "").
  /// \param optional If it does not exist,
  /// returns \a NULL instead of throwing an exception.
  /// \throw JException The requested element does not exist...
  //==============================================================================
  TiXmlElement* GetNextElement(TiXmlNode* node,const std::string &name,bool optional=false)const;

  //==============================================================================
  /// Removes the requested node.
  /// \param path Path of the requested node.
  //==============================================================================
  void RemoveNode(const std::string &path);

  //==============================================================================
  /// Returns the filename of the current xml with row of the requested node.
  //==============================================================================
  std::string ErrGetFileRow(const TiXmlNode* node)const;

  //==============================================================================
  /// Throws an exception with the xml node and the name of the element.
  /// \param node Xml node of the error.
  /// \param element Name of the element where the error appears.
  /// \param missing Error because it does not exist.
  /// \throw JException Error in element...
  //==============================================================================
  void ErrReadElement(const TiXmlNode* node,const std::string &element,bool missing,std::string errortext)const;
  void ErrReadElement(const TiXmlNode* node,const std::string &element,bool missing)const;

  //==============================================================================
  /// Throws an exception with the xml element and the name of the attribute.
  /// \param ele Xml element of the error.
  /// \param atrib Name of the attribute where the error appears.
  /// \param missing Error because it does not exist.
  /// \throw JException Error in element...
  //==============================================================================
  void ErrReadAtrib(const TiXmlElement* ele,const std::string &atrib,bool missing,std::string errortext="")const;

  //-Checking child element of the element.

  //==============================================================================
  /// Checks if the requested child element of an element already exists.
  /// \param ele Xml element of the error.
  /// \param name Name of the requested element.
  //==============================================================================
  bool ExistsElement(const TiXmlElement* ele,const std::string &name)const{ return(ele->FirstChildElement(name.c_str())!=NULL); }

  //==============================================================================
  /// Returns the number of elements with a requested name of a node TiXmlNode.
  /// \param node Xml node where the reach is performed.
  /// \param name Name of filtered elements (no filter using "").
  //==============================================================================
  unsigned CountElements(const TiXmlNode* node,const std::string &name)const;

  //==============================================================================
  /// Throws an exception if there are unknown or repeated elements.
  /// \param lis Xml element to check.
  /// \param names List of valid names (separated by spaces).
  /// \param checkrepeated Checks if there are repeated elements.
  //==============================================================================
  void CheckElementNames(TiXmlElement* lis,bool checkrepeated,std::string names)const;

  //==============================================================================
  /// Checks if some or several attributes appers in the element. Returns number
  /// of found attribute (1...n), 0 none found and -1 several found.
  /// \param ele Xml element of the error.
  /// \param names Names of the requested attributes separated by by spaces.
  /// \param checkmanyatt Throw exception if several attributes exist.
  /// \param checkmanyele Throw exception if several elements exist.
  //==============================================================================
  int CheckElementAttributes(const TiXmlElement* ele,const std::string &name,std::string attnames,bool checkmanyatt,bool checkmanyele)const;


  //-Checking attributes of the element.

  //==============================================================================
  /// Checks if the requested attribute of an element already exists.
  /// \param ele Xml element of the error.
  /// \param name Name of the requested attribute.
  //==============================================================================
  bool ExistsAttribute(const TiXmlElement* ele,const std::string &name)const;

  //==============================================================================
  /// Checks if some or several attributes appers in the element. Returns number
  /// of found attribute (1...n), 0 none found and -1 several found.
  /// \param ele Xml element of the error.
  /// \param names Names of the requested attributes separated by by spaces.
  /// \param checkmanyatt Throw exception if several attributes exist.
  //==============================================================================
  int CheckAttributes(const TiXmlElement* ele,std::string names,bool checkmanyatt)const;


  //-Reading attributes of the element.

  //==============================================================================
  /// Checks and returns a value of type string of an xml element.
  /// \param ele Xml element.
  /// \param name Name of the requested attribute.
  /// \param optional If it does not exist,
  /// returns \a valdef instead of throwing an exception.
  /// \param valdef Value by default if it does not exist and \a optional was activated. 
  /// \throw JException The requested attribute does not exist...
  //==============================================================================
  std::string GetAttributeStr(const TiXmlElement* ele,const std::string &name,bool optional=false,const std::string &valdef="")const;

  //==============================================================================
  /// Checks and returns a value of type bool of an xml element, 
  /// valid values in xml text are: \a '0', \a '1', \a 'false' and \a 'true'.
  /// \param ele Xml element.
  /// \param optional If it does not exist,
  /// returns \a valdef instead of throwing an exception.
  /// \param valdef Value by default if it does not exist and \a optional was activated. 
  /// \throw JException The requested attribute does not exist...
  //==============================================================================
  bool GetAttributeBool(const TiXmlElement* ele,const std::string &name,bool optional=false,bool valdef=false)const;

  //==============================================================================
  /// Checks and returns a value of type byte of an xml element that must be (0-225).
  /// \param ele Xml element.
  /// \param optional If it does not exist,
  /// returns \a valdef instead of throwing an exception.
  /// \param valdef Value by default if it does not exist and \a optional was activated. 
  /// \throw JException The requested attribute does not exist...
  //==============================================================================
  byte GetAttributeByte(TiXmlElement* ele,const std::string &name,bool optional=false,byte valdef=0)const;

  //==============================================================================
  /// Checks and returns a value of type word of an xml element that must be (0-65535).
  /// \param ele Xml element.
  /// \param optional If it does not exist,
  /// returns \a valdef instead of throwing an exception.
  /// \param valdef Value by default if it does not exist and \a optional was activated. 
  /// \throw JException The requested attribute does not exist...
  //==============================================================================
  word GetAttributeWord(TiXmlElement* ele,const std::string &name,bool optional=false,word valdef=0)const;

  //==============================================================================
  /// Checks and returns a value of type unsigned of an xml element that must be positive.
  /// \param ele Xml element.
  /// \param name Name of the requested attribute.
  /// \param optional If it does not exist,
  /// returns \a valdef instead of throwing an exception.
  /// \param valdef Value by default if it does not exist and \a optional was activated. 
  /// \throw JException The requested attribute does not exist...
  //==============================================================================
  unsigned GetAttributeUnsigned(TiXmlElement* ele,const std::string &name,bool optional=false,unsigned valdef=0)const;

  //==============================================================================
  /// Checks and returns a value of type unsigned of an xml element that must be positive.
  /// \param ele Xml element.
  /// \param name Name of the requested attribute.
  /// \param optional If it does not exist,
  /// returns \a valdef instead of throwing an exception.
  /// \param valdef Value by default if it does not exist and \a optional was activated. 
  /// \throw JException The requested attribute does not exist...
  //==============================================================================
  unsigned GetAttributeUint(TiXmlElement* ele,const std::string &name,bool optional=false,unsigned valdef=0)const{ return(GetAttributeUnsigned(ele,name,optional,valdef)); }

  //==============================================================================
  /// Checks and returns a value of type int of an xml element. 
  /// \param ele Xml element.
  /// \param name Name of the requested attribute.
  /// \param optional If it does not exist,
  /// returns \a valdef instead of throwing an exception.
  /// \param valdef Value by default if it does not exist and \a optional was activated. 
  /// \throw JException The requested attribute does not exist...
  //==============================================================================
  int GetAttributeInt(TiXmlElement* ele,const std::string &name,bool optional=false,int valdef=0)const;

  //==============================================================================
  /// Checks and returns a value of type double of an xml element. 
  /// \param ele Xml element.
  /// \param name Name of the requested attribute.
  /// \param optional If it does not exist,
  /// returns \a valdef instead of throwing an exception.
  /// \param valdef Value by default if it does not exist and \a optional was activated. 
  /// \throw JException The requested attribute does not exist...
  //==============================================================================
  double GetAttributeDouble(TiXmlElement* ele,const std::string &name,bool optional=false,double valdef=0)const;

  //==============================================================================
  /// Calls \ref GetAttributeDouble() with the same parameters.
  //==============================================================================
  float GetAttributeFloat(TiXmlElement* ele,const std::string &name,bool optional=false,float valdef=0)const{ return(float(GetAttributeDouble(ele,name,optional,valdef))); }
  
  //==============================================================================
  /// Checks and returns value of type int3 of the xml element.
  /// \param ele Xml element to read.
  /// \throw JException Format not valid for the requested type...
  //==============================================================================
  tint3 GetAttributeInt3(TiXmlElement* ele)const{ return(TInt3(GetAttributeInt(ele,"x"),GetAttributeInt(ele,"y"),GetAttributeInt(ele,"z"))); }
  
  //==============================================================================
  /// Checks and returns value of type uint3 of the xml element.
  /// \param ele Xml element to read.
  /// \throw JException Format not valid for the requested type...
  //==============================================================================
  tuint3 GetAttributeUint3(TiXmlElement* ele)const{ return(TUint3(GetAttributeUint(ele,"x"),GetAttributeUint(ele,"y"),GetAttributeUint(ele,"z"))); }
  
  //==============================================================================
  /// Calls \ref GetAttributeDouble3() with the same parameters.
  //==============================================================================
  tfloat3 GetAttributeFloat3(TiXmlElement* ele,const char* name1="x",const char* name2="y",const char* name3="z")const{ tdouble3 v=GetAttributeDouble3(ele,name1,name2,name3); return(TFloat3(float(v.x),float(v.y),float(v.z))); }
  
  //==============================================================================
  /// Checks and returns value of type double3 of the xml element.
  /// \param ele Xml element to read.
  /// \param name1 Name of the first attribute (x by default).
  /// \param name2 Name of the second attribute (y by default).
  /// \param name3 Name of the third attribute (z by default).
  /// \throw JException Format not valid for the requested type...
  //==============================================================================
  tdouble3 GetAttributeDouble3(TiXmlElement* ele,const char* name1="x",const char* name2="y",const char* name3="z")const{ return(TDouble3(GetAttributeDouble(ele,name1),GetAttributeDouble(ele,name2),GetAttributeDouble(ele,name3))); }


  //- Reading complete nodes.

  //==============================================================================
  /// Checks and returns value of type string of the first xml element 
  /// of a node with a given name.
  /// \param name Name of the element to be reached.
  /// \param node Xml node where the element is reached.
  /// \param attrib Name of the requested attribute.
  /// \param optional If it does not exist,
  /// \param valdef Value by default if it does not exist and \a optional was activated. 
  /// \throw JException The requested element or attribute does not exist...
  //==============================================================================
  std::string ReadElementStr(TiXmlNode* node,const std::string &name,const std::string &attrib,bool optional=false,const std::string &valdef="")const{
    TiXmlElement* ele=GetFirstElement(node,name,optional); 
    return(ele? GetAttributeStr(ele,attrib,optional,valdef): valdef);
  }
  
  //==============================================================================
  /// Calls \ref ReadElementDouble3() with the same parameters.
  //==============================================================================
  tfloat3 ReadElementFloat3(TiXmlNode* node,const std::string &name)const{ return(ToTFloat3(ReadElementDouble3(node,name))); }
  
  //==============================================================================
  /// Checks and returns value of type double3 of the first xml element 
  /// of a node with a given name.
  /// \param name Name of the element to be reached.
  /// \param node Xml node where the element is reached.
  /// \throw JException Element is not found...
  /// \throw JException Format not valid for the requested type...
  //==============================================================================
  tdouble3 ReadElementDouble3(TiXmlNode* node,const std::string &name)const{ return(GetAttributeDouble3(GetFirstElement(node,name))); }
  
  //==============================================================================
  /// Checks and returns value of type int3 of the first xml element 
  /// of a node with a given name.
  /// \param name Name of the element to be reached.
  /// \param node Xml node where the element is reached.
  /// \throw JException Element is not found...
  /// \throw JException Format not valid for the requested type...
  //==============================================================================
  tint3 ReadElementInt3(TiXmlNode* node,const std::string &name)const{ return(GetAttributeInt3(GetFirstElement(node,name))); }
  
  //==============================================================================
  /// Calls \ref ReadElementDouble() with the same parameters.
  //==============================================================================
  float ReadElementFloat(TiXmlNode* node,const std::string &name,const std::string &attrib,bool optional=false,float valdef=0)const{ 
    return(float(ReadElementDouble(node,name,attrib,optional,valdef)));
  }
  
  //==============================================================================
  /// Checks and returns value of type double of the first xml element 
  /// of a node with a given name.
  /// \param name Name of the element to be reached.
  /// \param node Xml node where the element is reached.
  /// \param attrib Name of the requested attribute.
  /// \param optional If it does not exist,
  /// \param valdef Value by default if it does not exist and \a optional was activated. 
  /// \throw JException Element is not found...
  /// \throw JException Format not valid for the requested type...
  //==============================================================================
  double ReadElementDouble(TiXmlNode* node,const std::string &name,const std::string &attrib,bool optional=false,double valdef=0)const{ 
    TiXmlElement* ele=GetFirstElement(node,name,optional); 
    return(ele? GetAttributeDouble(ele,attrib,optional,valdef): valdef);
  }

  //==============================================================================
  /// Checks and returns value of type unsigned of the first xml element 
  /// of a node with a given name.
  /// \param name Name of the element to be reached.
  /// \param node Xml node where the element is reached.
  /// \param attrib Name of the requested attribute.
  /// \param optional If it does not exist,
  /// \param valdef Value by default if it does not exist and \a optional was activated. 
  /// \throw JException Element is not found...
  /// \throw JException Format not valid for the requested type...
  //==============================================================================
  unsigned ReadElementUnsigned(TiXmlNode* node,const std::string &name,const std::string &attrib,bool optional=false,unsigned valdef=0)const{ 
    TiXmlElement* ele=GetFirstElement(node,name,optional); 
    return(ele? GetAttributeUnsigned(ele,attrib,optional,valdef): valdef);
  }
  
  //==============================================================================
  /// Checks and returns value of type int of the first xml element 
  /// of a node with a given name.
  /// \param name Name of the element to be reached.
  /// \param node Xml node where the element is reached.
  /// \param optional If it does not exist,
  /// \param valdef Value by default if it does not exist and \a optional was activated. 
  /// \throw JException Element is not found...
  /// \throw JException Format not valid for the requested type...
  //==============================================================================
  int ReadElementInt(TiXmlNode* node,const std::string &name,const std::string &attrib,bool optional=false,int valdef=0)const{ 
    TiXmlElement* ele=GetFirstElement(node,name,optional); 
    return(ele? GetAttributeInt(ele,attrib,optional,valdef): valdef);
  }
  
  //==============================================================================
  /// Checks and returns value of type bool of the first xml element 
  /// of a node with a given name.
  /// \param name Name of the element to be reached.
  /// \param node Xml node where the element is reached.
  /// \param optional If it does not exist,
  /// \param valdef Value by default if it does not exist and \a optional was activated. 
  /// \throw JException Element is not found...
  /// \throw JException Format not valid for the requested type...
  //==============================================================================
  bool ReadElementBool(TiXmlNode* node,const std::string &name,const std::string &attrib,bool optional=false,bool valdef=0)const{ 
    TiXmlElement* ele=GetFirstElement(node,name,optional); 
    return(ele? GetAttributeBool(ele,attrib,optional,valdef): valdef);
  }
  
  //==============================================================================
  /// Checks and returns value of type tmatrix3d of the first xml element 
  /// of a node with a given name.
  /// \param node Xml node where the element is reached.
  /// \param name Name of the element to be reached.
  /// \param optionalvalues If some value does not exist, valdef is used.
  /// \param valdef Value by default if some value does not exist and \a optional was activated. 
  /// \throw JException Size of data is not enough...
  /// \throw JException Element is not found...
  /// \throw JException Values missing or any value is not valid...
  //==============================================================================
  tmatrix3d ReadElementMatrix3d(TiXmlNode* node,const std::string &name,bool optionalvalues=false,double valdef=0)const{
    tmatrix3d mat;
    ReadMatrixDouble(node,name,3,3,sizeof(mat)/sizeof(valdef),(double*)&mat,optionalvalues,valdef);
    return(mat);
  }

  //- Reading arrays of complete nodes.

  //==============================================================================
  /// Loads a list of xml elements of type tfloat3 in an array 
  /// and returns the number of loaded elements.
  /// \param node Xml node the list of values is loaded from. 
  /// \param name Name of the elements of type tfloat3.
  /// \param vec Array where loaded values are stored.
  /// \param count Maximum number of elements to load.
  /// \param readcount If activated, throws an exception 
  /// if there are less elements than the requested ones.
  /// \throw JException Values missing or any value is not valid...
  //==============================================================================
  unsigned ReadArrayFloat3(TiXmlNode* node,const std::string &name,tfloat3 *vec,unsigned count,bool readcount=true)const;

  //==============================================================================
  /// Loads a list of xml elements of type double3 in an array 
  /// and returns the number of loaded elements.
  /// \param node Xml node the list of values is loaded from. 
  /// \param name Name of the elements of type double3.
  /// \param vec Array where loaded values are stored.
  /// \param count Maximum number of elements to load.
  /// \param readcount If activated, throws an exception 
  /// if there are less elements than the requested ones.
  /// \throw JException Values missing or any value is not valid...
  //==============================================================================
  unsigned ReadArrayDouble3(TiXmlNode* node,const std::string &name,tdouble3 *vec,unsigned count,bool readcount=true)const;

  //==============================================================================
  /// Loads a list of xml elements of type int3 in an array 
  /// and returns the number of loaded elements.
  /// \param node Xml node the list of values is loaded from. 
  /// \param name Name of the elements of type int3.
  /// \param vec Array where loaded values are stored.
  /// \param count Maximum number of elements to load.
  /// \param readcount If activated, throws an exception 
  /// if there are less elements than the requested ones.
  /// \throw JException Values missing or any value is not valid...
  //==============================================================================
  unsigned ReadArrayInt3(TiXmlNode* node,const std::string &name,tint3 *vec,unsigned count,bool readcount=true)const;

  //==============================================================================
  /// Loads a matrix from xml and returns the number of loaded elements.
  /// \param node Xml node the list of values is loaded from. 
  /// \param name Name of the elements of type double.
  /// \param nrow Number of rows (the maximum allowed is 9).
  /// \param ncol Number of cols (the maximum allowed is 9).
  /// \param data Memory pointer where loaded values are stored.
  /// \param count Available memory size of data pointer.
  /// \param optionalvalues If some value does not exist, valdef is used.
  /// \param valdef Value by default if some value does not exist and \a optional was activated. 
  /// \throw JException Element is not found...
  /// \throw JException Values missing or any value is not valid...
  //==============================================================================
  unsigned ReadMatrixDouble(TiXmlNode* node,const std::string &name,unsigned nrows,unsigned ncols,unsigned ndata,double *data,bool optionalvalues=false,double valdef=0)const;
  

  //-Conversion to text.

  //==============================================================================
  /// Returns a value type bool in text format (\a 'true' or \a 'false').
  //==============================================================================
  static std::string ToStr(bool v){ return(v? "true": "false"); }

  //==============================================================================
  /// Returns a value of type double in text according to the requested format.
  /// \param v Real value.
  /// \param fmt Format to be converted into text (used by printf()).
  //==============================================================================
  static std::string ToStr(double v,const char* fmt="%g");


  //-Insertion of attributes in node.

  //==============================================================================
  /// Adds attribute of type string to an xml element.
  /// \param ele Xml element.
  /// \param attrib Name of the attribute.
  /// \param v Value of the attribute.
  //==============================================================================
  static void AddAttribute(TiXmlElement* ele,const std::string &attrib,const std::string &v);

  //==============================================================================
  /// Adds attribute of type char[] to an xml element.
  /// \param ele Xml element.
  /// \param attrib Name of the attribute.
  /// \param v Value of the attribute.
  //==============================================================================
  static void AddAttribute(TiXmlElement* ele,const char *attrib,const char *v){ AddAttribute(ele,std::string(attrib),std::string(v)); }

  //==============================================================================
  /// Adds attribute of type bool to an xml element.
  /// \param ele Xml element.
  /// \param attrib Name of the attribute.
  /// \param v Value of the attribute.
  //==============================================================================
  static void AddAttribute(TiXmlElement* ele,const std::string &attrib,bool v);

  //==============================================================================
  /// Adds attribute of type int to an xml element.
  /// \param ele Xml element.
  /// \param attrib Name of the attribute.
  /// \param v Value of the attribute.
  //==============================================================================
  static void AddAttribute(TiXmlElement* ele,const std::string &attrib,int v);

  //==============================================================================
  /// Adds attribute of type double to an xml element.
  /// \param ele Xml element.
  /// \param attrib Name of the attribute.
  /// \param v Value of the attribute.
  /// \param fmt Format to be converted into text (used by printf()).
  //==============================================================================
  static void AddAttribute(TiXmlElement* ele,const std::string &attrib,double v,const char* fmt="%g");

  //==============================================================================
  /// Adds attributes (\a x,\a y,\a z) of type tfloat3 to the xml element.
  /// \param ele Xml element to add.
  /// \param v Value of type tfloat3.
  /// \param name1 Name of the first attribute (x by default).
  /// \param name2 Name of the second attribute (y by default).
  /// \param name3 Name of the third attribute (z by default).
  /// \param fmt Format to be converted to text (used by printf()).
  //==============================================================================
  static void AddAttribute(TiXmlElement* ele,tfloat3 v,const char* name1="x",const char* name2="y",const char* name3="z",const char* fmt="%g"){ AddAttribute(ele,TDouble3(v.x,v.y,v.z),name1,name2,name3,fmt); }

  //==============================================================================
  /// Adds attributes (\a x,\a y,\a z) of type double3 to the xml element.
  /// \param ele Xml element to add.
  /// \param v Value of type double3.
  /// \param name1 Name of the first attribute (x by default).
  /// \param name2 Name of the second attribute (y by default).
  /// \param name3 Name of the third attribute (z by default).
  /// \param fmt Format to be converted to text (used by printf()).
  //==============================================================================
  static void AddAttribute(TiXmlElement* ele,tdouble3 v,const char* name1="x",const char* name2="y",const char* name3="z",const char* fmt="%g"){ AddAttribute(ele,name1,v.x,fmt); AddAttribute(ele,name2,v.y,fmt); AddAttribute(ele,name3,v.z,fmt); }

  //==============================================================================
  /// Adds attribute of type unsigned to the xml element.
  /// \param ele Xml element to add.
  /// \param attrib Name of the attribute.
  /// \param v Value of the attribute.
  //==============================================================================
  static void AddAttribute(TiXmlElement* ele,const std::string &attrib,unsigned v){ AddAttribute(ele,attrib,int(v)); }


  //-Node creation.

  //==============================================================================
  /// Creates and returns an element starting from a value of type int3.
  /// \param name Name of the element.
  /// \param v Value of the element.
  /// \param name1 Name of the first attribute (x by default).
  /// \param name2 Name of the second attribute (y by default).
  /// \param name3 Name of the third attribute (z by default).
  //==============================================================================
  static TiXmlElement MakeElementInt3(const std::string &name,const tint3 &v,const char* name1="x",const char* name2="y",const char* name3="z");

  //==============================================================================
  /// Creates and returns an element starting from a value of type double3.
  /// \param name Name of the element.
  /// \param v Value of the element.
  /// \param name1 Name of the first attribute (x by default).
  /// \param name2 Name of the second attribute (y by default).
  /// \param name3 Name of the third attribute (z by default).
  //==============================================================================
  static TiXmlElement MakeElementDouble3(const std::string &name,const tdouble3 &v,const char* name1="x",const char* name2="y",const char* name3="z");

  //==============================================================================
  /// Creates and returns an element starting from a value of type tfloat3.
  /// \param name Name of the element.
  /// \param v Value of the element.
  /// \param name1 Name of the first attribute (x by default).
  /// \param name2 Name of the second attribute (y by default).
  /// \param name3 Name of the third attribute (z by default).
  //==============================================================================
  static TiXmlElement MakeElementFloat3(const std::string &name,const tfloat3 &v,const char* name1="x",const char* name2="y",const char* name3="z"){ return(MakeElementDouble3(name,TDouble3(v.x,v.y,v.z),name1,name2,name3)); }

  //==============================================================================
  /// Creates and returns an element starting from a value of type double3.
  /// \param name Name of the element.
  /// \param nrow Number of rows (the maximum allowed is 9).
  /// \param ncol Number of cols (the maximum allowed is 9).
  /// \param values Values of the matrix.
  //==============================================================================
  static TiXmlElement MakeElementMatrixDouble(const std::string &name,unsigned nrows,unsigned ncols,const double* values);
  
  //==============================================================================
  /// Creates and returns an element with attribute of type double.
  /// \param name Name of the element.
  /// \param attrib Name of the attribute.
  /// \param v Value of the attribute.
  /// \param fmt Format to be converted into text (used by printf()).
  //==============================================================================   
  static TiXmlElement MakeElementAttrib(const std::string &name,const std::string &attrib,double v,const char* fmt="%g"); 

  //==============================================================================
  /// Creates and returns an element with attribute of type int.
  /// \param name Name of the element.
  /// \param attrib Name of the attribute.
  /// \param v Value of the attribute.
  //==============================================================================
  static TiXmlElement MakeElementAttrib(const std::string &name,const std::string &attrib,int v);

  //==============================================================================
  /// Creates and returns an element with attribute of type string.
  /// \param name Name of the element.
  /// \param attrib Name of the attribute.
  /// \param v Value of the attribute.
  //==============================================================================
  static TiXmlElement MakeElementAttrib(const std::string &name,const std::string &attrib,const std::string &v);

  //==============================================================================
  /// Creates and returns an element with an attribute of type unsigned.
  /// \param name Name of the element.
  /// \param attrib Name of the attribute.
  /// \param v Value of the attribute.
  //==============================================================================
  static TiXmlElement MakeElementAttrib(const std::string &name,const std::string &attrib,unsigned v){ return(MakeElementAttrib(name,attrib,v));}


  //-Insertion of new element in node.

  //==============================================================================
  /// Adds a new element of type int3 to the node and returns the element.
  /// \param node Xml node to which the element node is created.
  /// \param name Name of the element.
  /// \param v Value of the element.
  /// \param name1 Name of the first attribute (x by default).
  /// \param name2 Name of the second attribute (y by default).
  /// \param name3 Name of the third attribute (z by default).
  //==============================================================================
  static TiXmlElement* AddElementInt3(TiXmlNode* node,const std::string &name,const tint3 &v,const char* name1="x",const char* name2="y",const char* name3="z"){ return(node->InsertEndChild(MakeElementInt3(name,v,name1,name2,name3))->ToElement()); }
  
  //==============================================================================
  /// Calls \ref AddElementDouble3() with the same parameters.
  //==============================================================================
  static TiXmlElement* AddElementFloat3(TiXmlNode* node,const std::string &name,const tfloat3 &v,const char* name1="x",const char* name2="y",const char* name3="z"){ return(AddElementDouble3(node,name,TDouble3(v.x,v.y,v.z),name1,name2,name3)); }
  
  //==============================================================================
  /// Adds a new element of type double3 to the node and returns the element.
  /// \param node Xml node to which the element node is created.
  /// \param name Name of the element.
  /// \param v Value of the element.
  /// \param name1 Name of the first attribute (x by default).
  /// \param name2 Name of the second attribute (y by default).
  /// \param name3 Name of the third attribute (z by default).
  //==============================================================================
  static TiXmlElement* AddElementDouble3(TiXmlNode* node,const std::string &name,const tdouble3 &v,const char* name1="x",const char* name2="y",const char* name3="z"){ return(node->InsertEndChild(MakeElementDouble3(name,v,name1,name2,name3))->ToElement()); }
  
  //==============================================================================
  /// Adds a new element of type double3 to the node and returns the element.
  /// \param node Xml node to which the element node is created.
  /// \param name Name of the element.
  /// \param nrow Number of rows.
  /// \param ncol Number of cols.
  /// \param values Values of the matrix.
  //==============================================================================
  static TiXmlElement* AddElementMatrixDouble(TiXmlNode* node,const std::string &name
    ,unsigned nrows,unsigned ncols,const double* values)
  { 
    return(node->InsertEndChild(MakeElementMatrixDouble(name,nrows,ncols,values))->ToElement());
  }
  
  //==============================================================================
  /// Adds a new element of type tmatrix3d to the node and returns the element.
  /// \param node Xml node to which the element node is created.
  /// \param name Name of the element.
  /// \param v Value of the element.
  //==============================================================================
  static TiXmlElement* AddElementMatrix3d(TiXmlNode* node,const std::string &name,const tmatrix3d &v){ 
    return(AddElementMatrixDouble(node,name,3,3,(const double*)&v));
  }
  
  //==============================================================================
  /// Adds a new element of type double to the node and returns the element.
  /// \param node Xml node to which the element node is created.
  /// \param name Name of the element.
  /// \param attrib Name of the attribute.
  /// \param v Value of the element.
  /// \param fmt Format to be converted to text (used by printf()).
  //==============================================================================
  static TiXmlElement* AddElementAttrib(TiXmlNode* node,const std::string &name,const std::string &attrib,double v,const char* fmt="%g"){ return(node->InsertEndChild(MakeElementAttrib(name,attrib,v,fmt))->ToElement()); }
  
  //==============================================================================
  /// Adds a new element of type unsigned to the node and returns the element.
  /// \param node Xml node to which the element node is created.
  /// \param name Name of the element.
  /// \param attrib Name of the attribute.
  /// \param v Value of the element.
  //==============================================================================
  static TiXmlElement* AddElementAttrib(TiXmlNode* node,const std::string &name,const std::string &attrib,unsigned v){ return(node->InsertEndChild(MakeElementAttrib(name,attrib,v))->ToElement()); }
  
  //==============================================================================
  /// Adds a new element of type int to the node and returns the element.
  /// \param node Xml node to which the element node is created.
  /// \param name Name of the element.
  /// \param attrib Name of the attribute.
  /// \param v Value of the element.
  //==============================================================================
  static TiXmlElement* AddElementAttrib(TiXmlNode* node,const std::string &name,const std::string &attrib,int v){ return(node->InsertEndChild(MakeElementAttrib(name,attrib,v))->ToElement()); }
  
  //==============================================================================
  /// Adds a new element of type string to the node and returns the element.
  /// \param node Xml node to which the element node is created.
  /// \param name Name of the element.
  /// \param attrib Name of the attribute.
  /// \param v Value of the element..
  //==============================================================================
  static TiXmlElement* AddElementAttrib(TiXmlNode* node,const std::string &name,const std::string &attrib,const std::string &v){ return(node->InsertEndChild(MakeElementAttrib(name,attrib,v))->ToElement()); }
  
  //==============================================================================
  /// Adds a new element to the node and returns the element.
  /// \param node Xml node to which the element node is created.
  /// \param name Name of the element.
  //==============================================================================
  static TiXmlElement* AddElement(TiXmlNode* node,const std::string &name){ TiXmlElement item(name.c_str()); return(node->InsertEndChild(item)->ToElement()); }

public:
  //==============================================================================
  /// Constructor of objects.
  //==============================================================================
  JXml();
  
  //==============================================================================
  /// Destructor of objects.
  //==============================================================================
  ~JXml();

  //==============================================================================
  /// Reinitialises the object state, recovering its configuration by default.
  //==============================================================================
  void Reset();

  //==============================================================================
  /// Stores the xml document in a file, including in the main node 
  /// atributtes with the name of the application that generates it and when.
  /// \param fname Filename.
  /// \param app Name of the application that generates it
  /// \param date Stores date and time of creation.
  /// \throw JException Problems with file access...
  //==============================================================================
  void SaveFile(const std::string &fname,const std::string &app="",bool date=false);

  //==============================================================================
  /// Creates an xml document of a file.
  /// \param fname Filename.
  /// \throw JException Problems with file access...
  //==============================================================================
  void LoadFile(const std::string &fname);

  //==============================================================================
  /// Correct symbols in file.
  /// \param fname Filename.
  /// \throw JException Problems with file access...
  //==============================================================================
  void CorrectFile(const std::string &fname);

};

#endif


