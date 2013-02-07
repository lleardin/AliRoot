#ifndef ALIITSUSIMULATIONPIX_H
#define ALIITSUSIMULATIONPIX_H

/* Copyright(c) 2007-2009, ALICE Experiment at CERN, All rights reserved. *
* See cxx source for full Copyright notice                               */

////////////////////////////////////////////////////////////
// Simulation class for upgrade pixels                    //
////////////////////////////////////////////////////////////

#include <TObjArray.h>
#include "AliITSUSimulation.h"
#include "AliITSUSegmentationPix.h"

class TH1F;
class AliITSUModule;
class AliITSUSimuParam;
class AliParamList;

//-------------------------------------------------------------------

class AliITSUSimulationPix : public AliITSUSimulation {
public:
  enum {kCellX1,kCellX2,kCellZ1,kCellZ2,kCellYDepth,kNDtSpread}; // data used for ch. spread integral calc.
  //
  // charge spread functions defined
  enum {kSpreadFunGauss2D,                  // single gaussian in 2D, SpreadFunGauss2D
	kSpreadFunDoubleGauss2D,            // double gaussian in 2D, SpreadFunDoubleGauss2D
	kNSpreadFuns
  };
  // fist kParamStart entried of spread fun params are reserved for common parameters
  enum {kSpreadFunParamNXoffs,               // number of pixels to consider +- from injection point (in X)
	kSpreadFunParamNZoffs,               // number of pixels to consider +- from injection point (in Z)
	kParamStart
  };
  // elements of the SpreadFunGauss2D parameterization (offsetted by kParamStart)
  enum {kG1MeanX=kParamStart,kG1SigX,kG1MeanZ,kG1SigZ,kNG1Par};
  // elements of the SpreadFunDoubleGauss2D parameterization (offsetted by kParamStart)
  enum {kG2MeanX0=kParamStart,kG2SigX0,kG2MeanZ0,kG2SigZ0,kG2MeanX1,kG2SigX1,kG2MeanZ1,kG2SigZ1,kG2ScaleG2,kNG2Par};
  //
  AliITSUSimulationPix();
  AliITSUSimulationPix(AliITSUSimuParam* sim,AliITSUSensMap* map);
  virtual ~AliITSUSimulationPix();
  AliITSUSimulationPix(const AliITSUSimulationPix &source); 
  AliITSUSimulationPix& operator=(const AliITSUSimulationPix &s);
  void Init();
  //
  void FinishSDigitiseModule();
  void DigitiseModule();
  //
  void SDigitiseModule();
  void WriteSDigits();
  void Hits2SDigits();
  void Hits2SDigitsFast();
  void AddNoisyPixels();   
  void RemoveDeadPixels();
  void FrompListToDigits();
  Int_t CreateNoisyDigits(Int_t minID,Int_t maxID,double probNoisy, double noise, double base);
  Bool_t SetTanLorAngle(Double_t WeightHole=1.0);
  Double_t GetTanLorAngle() const {return fTanLorAng;};
  //
  // For backwards compatibility
  void SDigitsToDigits(){ FinishSDigitiseModule();}
  
  // This sets fStrobe flag and allows generating the strobe and applying it to select hits 
  void SetStrobeGeneration(Bool_t b=kFALSE) {fStrobe=b;};
  virtual void GenerateStrobePhase();
  //
  Double_t SpreadFunDoubleGauss2D(const Double_t *dtIn);
  Double_t SpreadFunGauss2D(const Double_t *dtIn);
  //
  virtual void SetResponseParam(AliParamList* resp);
  //
 private:
  void SpreadCharge2D(Double_t x0,Double_t z0, Double_t dy, Int_t ix0,Int_t iz0,
		      Double_t el, Int_t tID, Int_t hID);
  //
  void SetCoupling(AliITSUSDigit* old,Int_t ntrack,Int_t idhit);     // "New" coupling routine  Tiziano Virgili
  void SetCouplingOld(AliITSUSDigit* old,Int_t ntrack,Int_t idhit);  // "Old" coupling routine  Rocco Caliandro
  //   
 protected:
   Double_t      fTanLorAng;    //! Tangent of the Lorentz Angle (weighted average for hole and electrons)
   Bool_t        fStrobe;       // kTRUE if readout strobe with proper phase applied to select hits
   Int_t         fStrobeLenght; // Strobe signal lenght in units of 25 ns
   Double_t      fStrobePhase;  // The phase of the strobe signal with respect to the trigger
   //   
   Double_t (AliITSUSimulationPix::*fSpreadFun)(const Double_t *dtIn); //! pointer on current spread function

   ClassDef(AliITSUSimulationPix,1)  // Simulation of pixel clusters
 };
#endif 
