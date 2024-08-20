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

/// \file JSpaceCtes.cpp \brief Implements the class \ref JSpaceCtes.

#include "JSpaceCtes.h"
#include "JXml.h"
#include <cmath>
#include <algorithm>

//##############################################################################
//# JSpaceCtes
//##############################################################################
//==============================================================================
/// Compute constants using parameters nonzero.
//==============================================================================
JSpaceCtes::StConstants JSpaceCtes::CalcConstans(StConstants cte){
  const double dim=(cte.data2d? 2: 3);
  const double dpvol=(cte.data2d? cte.dp*cte.dp: cte.dp*cte.dp*cte.dp);
  const double mg=sqrt((cte.gravity.x*cte.gravity.x)+(cte.gravity.y*cte.gravity.y)+(cte.gravity.z*cte.gravity.z));
  if(cte.coefh)cte.coefhdp=cte.coefh*sqrt(dim);
  else cte.coefh=cte.coefhdp/sqrt(dim);
  if(!cte.cteh)cte.cteh=cte.coefh*sqrt(dim)*cte.dp;
  if(!cte.massbound)cte.massbound=dpvol*cte.rhop0;
  if(!cte.massfluid)cte.massfluid=dpvol*cte.rhop0;
  if(!cte.speedsystem)cte.speedsystem=sqrt(mg*cte.hswl);
  if(!cte.speedsound)cte.speedsound=std::max(cte.coefsound*cte.speedsystem,10.*sqrt(mg*cte.hswl));
  if(!cte.cteb)cte.cteb=cte.speedsound*cte.speedsound*cte.rhop0/cte.gamma;
  return(cte);
}

//==============================================================================
/// Constructor.
//==============================================================================
JSpaceCtes::JSpaceCtes(){
  ClassName="JSpaceCtes";
  Reset();
}

//==============================================================================
/// Initialisation of variables.
//==============================================================================
void JSpaceCtes::Reset(){
  SetLatticeBound(true);
  SetLatticeFluid(true);
  Gravity=TDouble3(0);
  CFLnumber=0; 
  HSwlAuto=true; HSwl=0;
  SpeedSystemAuto=true; SpeedSystem=0;
  CoefSound=0; 
  SpeedSoundAuto=true; SpeedSound=0;
  CoefH=CoefHdp=0; Gamma=0; Rhop0=0;
  Eps=0; EpsDefined=false;
  HAuto=BAuto=MassBoundAuto=MassFluidAuto=true;
  H=B=MassBound=MassFluid=0;
  Dp=0;
}

//==============================================================================
/// Loads values by default.
//==============================================================================
void JSpaceCtes::LoadDefault(){
  Reset();
  SetLatticeBound(true);
  SetLatticeFluid(true);
  SetGravity(TDouble3(0,0,-9.81));
  SetCFLnumber(0.2);
  SetHSwlAuto(true);  SetHSwl(0);
  SetSpeedSystemAuto(true);  SetSpeedSystem(0);
  SetCoefSound(10);
  SetSpeedSoundAuto(true);  SetSpeedSound(0);
  SetCoefH(0.866025);
  SetGamma(7);
  SetRhop0(1000);
  SetEps(0);
  SetHAuto(true);  SetH(0);
  SetBAuto(true);  SetB(0);
  SetMassBoundAuto(true);  SetMassBound(0);
  SetMassFluidAuto(true);  SetMassFluid(0);
}

//==============================================================================
/// Reads constants auto for definition of the case of xml node.
//==============================================================================
void JSpaceCtes::ReadXmlElementAuto(JXml *sxml,TiXmlElement* node,bool optional,std::string name,double &value,bool &valueauto){
  TiXmlElement* xele=sxml->GetFirstElement(node,name,optional);
  if(xele){
    value=sxml->GetAttributeDouble(xele,"value");
    valueauto=sxml->GetAttributeBool(xele,"auto");
  }
}

//==============================================================================
/// Reads constants for definition of the case of xml node.
//==============================================================================
void JSpaceCtes::ReadXmlDef(JXml *sxml,TiXmlElement* node){
  const char met[]="ReadXmlDef";
  TiXmlElement* lattice=sxml->GetFirstElement(node,"lattice");
  SetLatticeBound(sxml->GetAttributeInt(lattice,"bound")==1);
  SetLatticeFluid(sxml->GetAttributeInt(lattice,"fluid")==1);
  SetGravity(sxml->ReadElementDouble3(node,"gravity"));
  SetCFLnumber(sxml->ReadElementDouble(node,"cflnumber","value"));
  ReadXmlElementAuto(sxml,node,false,"hswl",HSwl,HSwlAuto);
  ReadXmlElementAuto(sxml,node,true,"speedsystem",SpeedSystem,SpeedSystemAuto);
  SetCoefSound(sxml->ReadElementDouble(node,"coefsound","value"));
  ReadXmlElementAuto(sxml,node,true,"speedsound",SpeedSound,SpeedSoundAuto);
  double ch=sxml->ReadElementDouble(node,"coefh","value",true,0);
  if(!ch)ch=sxml->ReadElementDouble(node,"coefficient","value",true,0);
  double chdp=sxml->ReadElementDouble(node,"hdp","value",true,0);
  if(!ch && !chdp)ch=sxml->ReadElementDouble(node,"coefh","value");
  if(ch!=0 && chdp!=0)RunException(met,"Only one constant must be defined (coefh or hdp).",sxml->ErrGetFileRow(node));
  if(ch)SetCoefH(ch); 
  if(chdp)SetCoefHdp(chdp);
  SetGamma(sxml->ReadElementDouble(node,"gamma","value"));
  SetRhop0(sxml->ReadElementDouble(node,"rhop0","value"));
  EpsDefined=sxml->ExistsElement(node,"eps");
  SetEps(sxml->ReadElementDouble(node,"eps","value",true,0));
  ReadXmlElementAuto(sxml,node,true,"h",H,HAuto);
  ReadXmlElementAuto(sxml,node,true,"b",B,BAuto);
  ReadXmlElementAuto(sxml,node,true,"massbound",MassBound,MassBoundAuto);
  ReadXmlElementAuto(sxml,node,true,"massfluid",MassFluid,MassFluidAuto);
}

//==============================================================================
/// Writes constants auto for definition of the case of xml node.
//==============================================================================
void JSpaceCtes::WriteXmlElementAuto(JXml *sxml,TiXmlElement* node,std::string name,double value,bool valueauto,std::string comment,std::string unitscomment)const{
  TiXmlElement xele(name.c_str());
  JXml::AddAttribute(&xele,"value",value); 
  JXml::AddAttribute(&xele,"auto",valueauto);
  if(!comment.empty())JXml::AddAttribute(&xele,"comment",comment);
  if(!unitscomment.empty())JXml::AddAttribute(&xele,"units_comment",unitscomment);

  node->InsertEndChild(xele);
}

//==============================================================================
/// Writes constants with comment.
//==============================================================================
void JSpaceCtes::WriteXmlElementComment(TiXmlElement* ele,std::string comment,std::string unitscomment)const{
  if(!comment.empty())JXml::AddAttribute(ele,"comment",comment);
  if(!unitscomment.empty())JXml::AddAttribute(ele,"units_comment",unitscomment);
}

//==============================================================================
/// Writes constants for definition of the case of xml node.
//==============================================================================
void JSpaceCtes::WriteXmlDef(JXml *sxml,TiXmlElement* node)const{
  TiXmlElement lattice("lattice");
  JXml::AddAttribute(&lattice,"bound",GetLatticeBound());
  JXml::AddAttribute(&lattice,"fluid",GetLatticeFluid());
  node->InsertEndChild(lattice);
  WriteXmlElementComment(JXml::AddElementDouble3(node,"gravity",GetGravity()),"Gravitational acceleration","m/s^2");
  WriteXmlElementComment(JXml::AddElementAttrib(node,"rhop0","value",GetRhop0()),"Reference density of the fluid","kg/m^3");
  WriteXmlElementAuto(sxml,node,"hswl",GetHSwl(),GetHSwlAuto(),"Maximum still water level to calculate speedofsound using coefsound","metres (m)");
  WriteXmlElementComment(JXml::AddElementAttrib(node,"gamma","value",GetGamma()),"Polytropic constant for water used in the state equation");
  WriteXmlElementAuto(sxml,node,"speedsystem",GetSpeedSystem(),GetSpeedSystemAuto(),"Maximum system speed (by default the dam-break propagation is used)");
  WriteXmlElementComment(JXml::AddElementAttrib(node,"coefsound","value",GetCoefSound()),"Coefficient to multiply speedsystem");
  WriteXmlElementAuto(sxml,node,"speedsound",GetSpeedSound(),GetSpeedSoundAuto(),"Speed of sound to use in the simulation (by default speedofsound=coefsound*speedsystem)");
  if(GetCoefH()||!GetCoefHdp())WriteXmlElementComment(JXml::AddElementAttrib(node,"coefh","value",GetCoefH()),"Coefficient to calculate the smoothing length (h=coefh*sqrt(3*dp^2) in 3D)");
  if(GetCoefHdp())WriteXmlElementComment(JXml::AddElementAttrib(node,"hdp","value",GetCoefHdp()),"Coefficient to calculate the smoothing length (hdp=h/dp)");
  WriteXmlElementComment(JXml::AddElementAttrib(node,"cflnumber","value",GetCFLnumber()),"Coefficient to multiply dt");
  WriteXmlElementAuto(sxml,node,"h",GetH(),GetHAuto(),"","metres (m)");
  WriteXmlElementAuto(sxml,node,"b",GetB(),GetBAuto(),"","metres (m)");
  WriteXmlElementAuto(sxml,node,"massbound",GetMassBound(),GetMassBoundAuto(),"","kg");
  WriteXmlElementAuto(sxml,node,"massfluid",GetMassFluid(),GetMassFluidAuto(),"","kg");

}

//==============================================================================
/// Reads constants for execution of the case of xml node.
//==============================================================================
void JSpaceCtes::ReadXmlRun(JXml *sxml,TiXmlElement* node){
  SetGravity(sxml->ReadElementDouble3(node,"gravity"));
  SetCFLnumber(sxml->ReadElementDouble(node,"cflnumber","value"));
  SetGamma(sxml->ReadElementDouble(node,"gamma","value"));
  SetRhop0(sxml->ReadElementDouble(node,"rhop0","value"));
  SetEps(sxml->ReadElementDouble(node,"eps","value",true,0));
  SetDp(sxml->ReadElementDouble(node,"dp","value"));
  SetH(sxml->ReadElementDouble(node,"h","value"));
  SetB(sxml->ReadElementDouble(node,"b","value"));
  SetMassBound(sxml->ReadElementDouble(node,"massbound","value"));
  SetMassFluid(sxml->ReadElementDouble(node,"massfluid","value"));
}

//�����������������������������������������������������������������������//MP
//==============================================================================
/// Reads constants to determine the lattice used. Needed for Setting the particle mass
/// for multi-phaase simulations -��//MP
//==============================================================================
void JSpaceCtes::ReadXmlRunLattMP(JXml *sxml,TiXmlElement* node){
	SetLatticeBound(sxml->ReadElementInt(node,"lattice","bound")==1);
	SetLatticeFluid(sxml->ReadElementInt(node,"lattice","fluid")==1);
}
//�����������������������������������������������������������������������//MP

//==============================================================================
/// Writes constants for execution of the case of xml node.
//==============================================================================
void JSpaceCtes::WriteXmlRun(JXml *sxml,TiXmlElement* node)const{
  WriteXmlElementComment(JXml::AddElementDouble3(node,"gravity",GetGravity()),"","m/s^2");
  WriteXmlElementComment(JXml::AddElementAttrib(node,"cflnumber","value",GetCFLnumber()));
  WriteXmlElementComment(JXml::AddElementAttrib(node,"gamma","value",GetGamma()));
  WriteXmlElementComment(JXml::AddElementAttrib(node,"rhop0","value",GetRhop0()),"","kg/m^3");
  if(EpsDefined)WriteXmlElementComment(JXml::AddElementAttrib(node,"eps","value",GetEps()),"","m/s^2");
  WriteXmlElementComment(JXml::AddElementAttrib(node,"dp","value",GetDp()),"","metres (m)");
  WriteXmlElementComment(JXml::AddElementAttrib(node,"h","value",GetH(),"%.10E"),"","metres (m)");
  WriteXmlElementComment(JXml::AddElementAttrib(node,"b","value",GetB(),"%.10E"),"","metres (m)");
  WriteXmlElementComment(JXml::AddElementAttrib(node,"massbound","value",GetMassBound(),"%.10E"),"","kg");
  WriteXmlElementComment(JXml::AddElementAttrib(node,"massfluid","value",GetMassFluid(),"%.10E"),"","kg");
  //JXml::AddElementDouble3(node,"gravity",GetGravity());
  //JXml::AddElementAttrib(node,"cflnumber","value",GetCFLnumber());
  //JXml::AddElementAttrib(node,"gamma","value",GetGamma());
  //JXml::AddElementAttrib(node,"rhop0","value",GetRhop0());
  //if(EpsDefined)JXml::AddElementAttrib(node,"eps","value",GetEps());
  //JXml::AddElementAttrib(node,"dp","value",GetDp());
  //JXml::AddElementAttrib(node,"h","value",GetH(),"%.10E");
  //JXml::AddElementAttrib(node,"b","value",GetB(),"%.10E");
  //JXml::AddElementAttrib(node,"massbound","value",GetMassBound(),"%.10E");
  //JXml::AddElementAttrib(node,"massfluid","value",GetMassFluid(),"%.10E");
}

//==============================================================================
/// Loads constants for execution of the case of xml node.
//==============================================================================
void JSpaceCtes::LoadXmlDef(JXml *sxml,const std::string &place){
  Reset();
  TiXmlNode* node=sxml->GetNode(place,false);
  if(!node)RunException("LoadXmlDef",std::string("The item is not found \'")+place+"\'.");
  ReadXmlDef(sxml,node->ToElement());
}

//==============================================================================
/// Stores constants for execution of the case of xml node.
//==============================================================================
void JSpaceCtes::SaveXmlDef(JXml *sxml,const std::string &place)const{
  WriteXmlDef(sxml,sxml->GetNode(place,true)->ToElement());
}

//==============================================================================
/// Loads constants for execution of the case of xml node.
//==============================================================================
void JSpaceCtes::LoadXmlRun(JXml *sxml,const std::string &place){
  Reset();
  TiXmlNode* node=sxml->GetNode(place,false);
  if(!node)RunException("LoadXmlRun",std::string("The item is not found \'")+place+"\'.");
  ReadXmlRun(sxml,node->ToElement());
}

//�����������������������������������������������������������������������//MP
//==============================================================================
/// Loads constants to determine the lattice used. Needed for Setting the particle mass
/// for multi-phaase simulations -��//MP
//==============================================================================
void JSpaceCtes::LoadXmlRunLattMP(JXml *sxml,const std::string &place){
  TiXmlNode* node=sxml->GetNode(place,false);
  if(!node)RunException("LoadXmlRunLattMP",std::string("The item is not found \'")+place+"\'.");
  ReadXmlRunLattMP(sxml,node->ToElement());
}
//�����������������������������������������������������������������������//MP

//==============================================================================
/// Stores constants for execution of the case of xml node.
//==============================================================================
void JSpaceCtes::SaveXmlRun(JXml *sxml,const std::string &place)const{
  WriteXmlRun(sxml,sxml->GetNode(place,true)->ToElement());
}




