#ifndef ALITRDMATRIX_h
#define ALITRDMATRIX_h
/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

#include <TObject.h>
#include <TObjArray.h>
#include <TH2.h>
#include <TH3.h>
#include <TStyle.h>
#include <TCanvas.h>

#include "AliTRDpixel.h"

///////////////////////////////////////////////////////
//  Stores the pixel-information of one TRD chamber  //
///////////////////////////////////////////////////////

class AliTRDmatrix : public TObject {

public:

  AliTRDmatrix();
  AliTRDmatrix(Int_t nRow, Int_t nCol, Int_t nTime, Int_t iSec, Int_t iCha, Int_t iPla);
  AliTRDmatrix(AliTRDmatrix &m);
  virtual ~AliTRDmatrix();

  virtual void         AddSignal(Int_t iRow, Int_t iCol, Int_t iTime, Float_t signal);
  virtual Bool_t       AddTrack(Int_t iRow, Int_t iCol, Int_t iTime, Int_t track);

  virtual void         Copy(AliTRDmatrix &m);
  virtual void         Draw(Option_t *opt = " ");
  virtual void         DrawRow(Int_t iRow);
  virtual void         DrawCol(Int_t iCol);
  virtual void         DrawTime(Int_t iTime);
  virtual void         ProjRow();  
  virtual void         ProjCol();  
  virtual void         ProjTime();  

  virtual void         SetSignal(Int_t iRow, Int_t iCol, Int_t iTime, Float_t signal);
  virtual void         SetTrack(Int_t iRow, Int_t iCol, Int_t iTime
                              , Int_t iTrack, Int_t track);

  virtual Float_t      GetSignal(Int_t iRow, Int_t iCol, Int_t iTime);
  virtual Int_t        GetTrack(Int_t iRow, Int_t iCol, Int_t iTime, Int_t iTrack);

  virtual Int_t        GetSector()  { return fSector;  };
  virtual Int_t        GetChamber() { return fChamber; };
  virtual Int_t        GetPlane()   { return fPlane;   };

  inline  AliTRDmatrix &operator=(AliTRDmatrix &m);

protected:

  virtual Int_t        GetIndex(Int_t iRow, Int_t iCol, Int_t iTime);
  virtual AliTRDpixel *GetPixel(Int_t iRow, Int_t iCol, Int_t iTime);

  Int_t         fRow;            // Number of pad-rows
  Int_t         fCol;            // Number of pad-columns
  Int_t         fTime;           // Number of time buckets
  Int_t         fPixel;          // Number of pixels
  Int_t         fSector;         // Sector number
  Int_t         fChamber;        // Chamber number
  Int_t         fPlane;          // Plane number
  TObjArray    *fPixelArray;     // Array of pixels

  ClassDef(AliTRDmatrix,1)       // The TRD detector matrix for one readout chamber

};

//_____________________________________________________________________________
AliTRDmatrix &AliTRDmatrix::operator=(AliTRDmatrix &m)
{
  //
  // Assignment operator
  //

  if (this != &m) m.Copy(*this);
  return *this;

}

#endif
