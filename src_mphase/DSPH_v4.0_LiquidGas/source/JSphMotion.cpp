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

#include "JSphMotion.h"
#include "JMotion.h"
#include "JXml.h"

using namespace std;
//==============================================================================
/// Constructor.
//==============================================================================
JSphMotion::JSphMotion(){
  Mot=new JMotion();
}

//==============================================================================
/// Destructor.
//==============================================================================
JSphMotion::~JSphMotion(){
  delete Mot;
}

//==============================================================================
/// Initialization of variables.
//==============================================================================
void JSphMotion::Reset(){
  Mot->Reset();
}

//==============================================================================
/// Initialization of configuration and returns number of moving objects.
//==============================================================================
unsigned JSphMotion::Init(JXml *jxml,const std::string &path,const std::string &dirdata){
  Mot->ReadXml(jxml,path,false);
  Mot->Prepare();
  Mot->SetDirIn(dirdata);
  int numobjects=Mot->GetMaxRef()+1;
  return(unsigned(numobjects));
}

//==============================================================================
/// Processes next time interval and returns true if there are active motions.
//==============================================================================
bool JSphMotion::ProcesTime(double timestep,double dt){
  return(Mot->ProcesTime(timestep,dt));
}

//==============================================================================
/// Returns the number of performed movements.
//==============================================================================
unsigned JSphMotion::GetMovCount()const{
  return(Mot->GetMovCount());
}

//==============================================================================
/// Returns data of the motion of an object.
//==============================================================================
bool JSphMotion::GetMov(unsigned mov,unsigned &ref,tfloat3 &mvsimple,tmatrix4f &mvmatrix)const{
  JMatrix4d aux;
  tdouble3 mvsimpled;
  bool ret=Mot->GetMov(mov,ref,mvsimpled,aux);
  mvsimple=ToTFloat3(mvsimpled);
  mvmatrix=aux.GetMatrix4f();
  return(ret);
}

//==============================================================================
/// Returns data of the motion of an object.
//==============================================================================
bool JSphMotion::GetMov(unsigned mov,unsigned &ref,tdouble3 &mvsimple,tmatrix4d &mvmatrix)const{
  JMatrix4d aux;
  bool ret=Mot->GetMov(mov,ref,mvsimple,aux);
  mvmatrix=aux.GetMatrix4d();
  return(ret);
}

//==============================================================================
/// Returns the number of finished movements.
//==============================================================================
unsigned JSphMotion::GetStopCount()const{
  return(Mot->GetStopCount());
}

//==============================================================================
/// Returns the reference of the stopped object.
//==============================================================================
unsigned JSphMotion::GetStopRef(unsigned mov)const{
  return(Mot->GetStopRef(mov));
}


