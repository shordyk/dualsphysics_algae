//HEAD_DSPH
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

/// \file JSphGpuSingle.h \brief Declares the class \ref JSphGpuSingle.

#ifndef _JSphGpuSingle_
#define _JSphGpuSingle_

#include "DualSphDef.h"
#include "JSphGpu.h"
#include <string>

class JCellDivGpuSingle;
class JPartsLoad4;

//##############################################################################
//# JSphGpuSingle
//##############################################################################
/// \brief Defines the attributes and functions used only in Single-GPU implementation.

class JSphGpuSingle : public JSphGpu
{
protected:
  JCellDivGpuSingle* CellDivSingle;
  JPartsLoad4* PartsLoaded;

  llong GetAllocMemoryCpu()const;  
  llong GetAllocMemoryGpu()const;  
  llong GetMemoryGpuNp()const;
  llong GetMemoryGpuNct()const;
  void UpdateMaxValues();
  void LoadConfig(JCfgRun *cfg);
  void LoadCaseParticles();
  void ConfigDomain();

  void ResizeParticlesSize(unsigned newsize,float oversize,bool updatedivide);
  void RunPeriodic();
  void RunCellDivide(bool updateperiodic);

  void Interaction_Forces(TpInter tinter);
//�����������������������������������������������������������������������//MP
  void Interaction_ForcesMP(TpInter tinter);
  void RunShifting_MP(double dt);
  double ComputeRhopMaxMP(float *auxmem);
//�����������������������������������������������������������������������//MP
  double ComputeAceMax(float *auxmem);

  double ComputeStep(){ return(TStep==STEP_Verlet? ComputeStep_Ver(): ComputeStep_Sym()); }

  double ComputeStep_Ver();
  double ComputeStep_Sym();

  void RunFloating(double dt,bool predictor);

  void SaveData();
//�����������������������������������������������������������������������//MP
  void SaveDataMP();
//�����������������������������������������������������������������������//MP
  void FinishRun(bool stop);

public:
  JSphGpuSingle();
  ~JSphGpuSingle();
  void Run(std::string appname,JCfgRun *cfg,JLog2 *log);
};

#endif


