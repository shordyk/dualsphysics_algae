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

#ifndef _JMotionMov_
#define _JMotionMov_

#include "TypesDef.h"
#include "JObject.h"

class JXml;
class TiXmlNode;

//==============================================================================
//##############################################################################
//==============================================================================
class JMotionMov : protected JObject
{
public:
  typedef enum{ Null=0,Nulo,CircularSinusoidal,RotationSinusoidal,RectilinearSinusoidal,CircularAce,Circular,RotationFile,RotationAce,Rotation,RectilinearFile,RectilinearAce,Rectilinear,Wait }TpMotionMov; 

  const unsigned Id;
  const unsigned NextId;
  const double Time;
  const bool TypeBlock;
  const TpMotionMov Type;
  JMotionMov* NextMov;

  JMotionMov(const char* classname,bool typeblock,TpMotionMov type,unsigned id,unsigned nextid,double time):TypeBlock(typeblock),Type(type),Id(id),NextId(nextid),Time(time){
    ClassName=classname;
    NextMov=NULL;
  }
  void SetNextMov(JMotionMov* nextmov){ NextMov=nextmov; }
  virtual void WriteXml(TiXmlNode* node)const=0;
  //virtual ~JMotionMov(){  printf("del JMotionMov\n");  }
  //void Reset();
};

//==============================================================================
//##############################################################################
//==============================================================================
class JMotionMovBlock : public JMotionMov
{
public:
  JMotionMovBlock(const char* classname,TpMotionMov type,unsigned id,unsigned nextid,double time): JMotionMov(classname,true,type,id,nextid,time) {}
};

//==============================================================================
//##############################################################################
//==============================================================================
class JMotionMovPart : public JMotionMov
{
public:
  JMotionMovPart(const char* classname,TpMotionMov type,unsigned id,unsigned nextid,double time): JMotionMov(classname,false,type,id,nextid,time) {}
};

//==============================================================================
//##############################################################################
//==============================================================================
class JMotionMovWait : public JMotionMovBlock
{
public:
  JMotionMovWait(unsigned id,unsigned nextid,double time):JMotionMovBlock("JMotionMovWait",Wait,id,nextid,time){}
  void WriteXml(TiXmlNode* node)const;
};

//==============================================================================
//##############################################################################
//==============================================================================
class JMotionMovRect : public JMotionMovBlock
{
public:
  const tdouble3 Vel;
  JMotionMovRect(unsigned id,unsigned nextid,double time,const tdouble3 &vel):JMotionMovBlock("JMotionMovRect",Rectilinear,id,nextid,time),Vel(vel){}
  void WriteXml(TiXmlNode* node)const;
};

//==============================================================================
//##############################################################################
//==============================================================================
class JMotionMovRectAce : public JMotionMovBlock
{
public:
  const tdouble3 Ace;
  const tdouble3 Vel;
  const bool VelPrev;
  JMotionMovRectAce(unsigned id,unsigned nextid,double time,const tdouble3 &ace,const tdouble3 &vel,bool velprev):JMotionMovBlock("JMotionMovRectAce",RectilinearAce,id,nextid,time),Ace(ace),Vel(vel),VelPrev(velprev){}
  void WriteXml(TiXmlNode* node)const;
};


//==============================================================================
//##############################################################################
//==============================================================================
class JMotionAxis
{
public:
  tdouble3 P1;
  tdouble3 P2;
  JMotionAxis(const tdouble3 &p1,const tdouble3 &p2):P1(p1),P2(p2){}
  bool Equals(const tdouble3 &p1,const tdouble3 &p2)const{ return(P1.x==p1.x&&P1.y==p1.y&&P1.z==p1.z&&P2.x==p2.x&&P2.y==p2.y&&P2.z==p2.z); }
};

//==============================================================================
//##############################################################################
//==============================================================================
class JMotionMovRot : public JMotionMovPart
{
public:
  const bool AngDegrees;
  const JMotionAxis* Axis;
  const double VelAng;  //-Siempre en grados/s.
  JMotionMovRot(unsigned id,unsigned nextid,double time,bool angdegrees,const JMotionAxis* axis,const double &velang):JMotionMovPart("JMotionMovRot",Rotation,id,nextid,time),AngDegrees(angdegrees),Axis(axis),VelAng(velang){}
  void WriteXml(TiXmlNode* node)const;
};

//==============================================================================
//##############################################################################
//==============================================================================
class JMotionMovRotAce : public JMotionMovPart
{
public:
  const bool AngDegrees;
  const JMotionAxis* Axis;
  const double VelAng;  //-Siempre en grados/s.
  const double AceAng;  //-Siempre en grados/s^2.
  const bool VelPrev;

  JMotionMovRotAce(unsigned id,unsigned nextid,double time,bool angdegrees,const JMotionAxis* axis,const double &aceang,const double &velang,bool velprev):JMotionMovPart("JMotionMovRotAce",RotationAce,id,nextid,time),AngDegrees(angdegrees),Axis(axis),AceAng(aceang),VelAng(velang),VelPrev(velprev){}
  void WriteXml(TiXmlNode* node)const;
};

//==============================================================================
//##############################################################################
//==============================================================================
class JMotionMovCir : public JMotionMovBlock
{
public:
  const bool AngDegrees;
  const JMotionAxis* Axis;
  const JMotionAxis* Ref;
  const double VelAng;  //-Siempre en grados/s.
  JMotionMovCir(unsigned id,unsigned nextid,double time,bool angdegrees,const JMotionAxis* axis,const JMotionAxis* ref,const double &velang):JMotionMovBlock("JMotionMovCir",Circular,id,nextid,time),AngDegrees(angdegrees),Axis(axis),Ref(ref),VelAng(velang){}
  void WriteXml(TiXmlNode* node)const;
};

//==============================================================================
//##############################################################################
//==============================================================================
class JMotionMovCirAce : public JMotionMovBlock
{
public:
  const bool AngDegrees;
  const JMotionAxis* Axis;
  const JMotionAxis* Ref;
  const double VelAng;  //-Siempre en grados/s.
  const double AceAng;  //-Siempre en grados/s^2.
  const bool VelPrev;
  JMotionMovCirAce(unsigned id,unsigned nextid,double time,bool angdegrees,const JMotionAxis* axis,const JMotionAxis* ref,const double &aceang,const double &velang,bool velprev):JMotionMovBlock("JMotionMovCirAce",CircularAce,id,nextid,time),AngDegrees(angdegrees),Axis(axis),Ref(ref),AceAng(aceang),VelAng(velang),VelPrev(velprev){}
  void WriteXml(TiXmlNode* node)const;
};

//==============================================================================
//##############################################################################
//==============================================================================
class JMotionMovRectSinu : public JMotionMovBlock
{
public:
  const bool AngDegrees;
  const tdouble3 Freq;  //-Frequency, siempre en 1/s.
  const tdouble3 Ampl;  //-Amplitude, siempre en metros.
  const tdouble3 Phase; //-Siempre en radianes.
  const bool PhasePrev;
  JMotionMovRectSinu(unsigned id,unsigned nextid,double time,bool angdegrees,const tdouble3 &freq,const tdouble3 &ampl,const tdouble3 &phase,bool phaseprev):JMotionMovBlock("JMotionMovRectSinu",RectilinearSinusoidal,id,nextid,time),AngDegrees(angdegrees),Freq(freq),Ampl(ampl),Phase(phase),PhasePrev(phaseprev){}
  void WriteXml(TiXmlNode* node)const;
};

//==============================================================================
//##############################################################################
//==============================================================================
class JMotionMovRotSinu : public JMotionMovPart
{
public:
  const bool AngDegrees;
  const JMotionAxis* Axis;
  const double Freq;  //-Frequency, siempre en 1/s.
  const double Ampl;  //-Amplitude, siempre en grados.
  const double Phase; //-Siempre en radianes.
  const bool PhasePrev;
  JMotionMovRotSinu(unsigned id,unsigned nextid,double time,bool angdegrees,const JMotionAxis *axis,double freq,double ampl,double phase,bool phaseprev):JMotionMovPart("JMotionMovRotSinu",RotationSinusoidal,id,nextid,time),AngDegrees(angdegrees),Axis(axis),Freq(freq),Ampl(ampl),Phase(phase),PhasePrev(phaseprev){}
  void WriteXml(TiXmlNode* node)const;
};

//==============================================================================
//##############################################################################
//==============================================================================
class JMotionMovCirSinu : public JMotionMovBlock
{
public:
  const bool AngDegrees; //-Independientemente de AngDegrees, todos los valores se guardan en grados.
  const JMotionAxis* Axis;
  const JMotionAxis* Ref;
  const double Freq;  //-Frequency, siempre en 1/s.
  const double Ampl;  //-Amplitude, siempre en grados.
  const double Phase; //-Siempre en radianes.
  const bool PhasePrev;
  JMotionMovCirSinu(unsigned id,unsigned nextid,double time,bool angdegrees,const JMotionAxis* axis,const JMotionAxis* ref,double freq,double ampl,double phase,bool phaseprev):JMotionMovBlock("JMotionMovCirSinu",CircularSinusoidal,id,nextid,time),AngDegrees(angdegrees),Axis(axis),Ref(ref),Freq(freq),Ampl(ampl),Phase(phase),PhasePrev(phaseprev){}
  void WriteXml(TiXmlNode* node)const;
};

//==============================================================================
//##############################################################################
//==============================================================================
class JMotionMovRectFile : public JMotionMovBlock
{
public:
  const std::string File;
  const int Fields;
  const int FieldTime;
  const int FieldX;
  const int FieldY;
  const int FieldZ;
  JMotionMovRectFile(unsigned id,unsigned nextid,double time,const std::string &file,int fields,int fieldtime,int fieldx,int fieldy,int fieldz):JMotionMovBlock("JMotionMovRectFile",RectilinearFile,id,nextid,time),File(file),Fields(fields),FieldTime(fieldtime),FieldX(fieldx),FieldY(fieldy),FieldZ(fieldz){}
  void WriteXml(TiXmlNode* node)const;
};

//==============================================================================
//##############################################################################
//==============================================================================
class JMotionMovRotFile : public JMotionMovPart
{
public:
  const bool AngDegrees;
  const JMotionAxis* Axis;
  const std::string File;
  JMotionMovRotFile(unsigned id,unsigned nextid,double time,bool angdegrees,const JMotionAxis* axis,const std::string &file):JMotionMovPart("JMotionMovRotFile",RotationFile,id,nextid,time),AngDegrees(angdegrees),Axis(axis),File(file){}
  void WriteXml(TiXmlNode* node)const;
};

//==============================================================================
//##############################################################################
//==============================================================================
class JMotionMovNull : public JMotionMovBlock
{
public:
  JMotionMovNull(unsigned id):JMotionMovBlock("JMotionMovNull",Nulo,id,0,0){}
  void WriteXml(TiXmlNode* node)const;
};

#endif

