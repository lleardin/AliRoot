/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/*
$Log$
Revision 1.3  2000/06/07 16:27:01  cblume
Try to remove compiler warnings on Sun and HP

Revision 1.2  2000/05/08 16:17:27  cblume
Merge TRD-develop

Revision 1.1.2.1  2000/05/08 14:44:01  cblume
Add new class AliTRDdigitsManager

*/

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Manages the digits and the track dictionary in the form of               //
//  AliTRDdataArray objects.                                                 //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "AliRun.h"

#include "AliTRDdigitsManager.h"
#include "AliTRDconst.h"

ClassImp(AliTRDdigitsManager)

//_____________________________________________________________________________
AliTRDdigitsManager::AliTRDdigitsManager():TObject()
{
  //
  // Default constructor
  //

  fIsRaw = kFALSE;

  fDigits = new AliTRDsegmentArray("AliTRDdataArrayI",kNdet);

  for (Int_t iDict = 0; iDict < kNDict; iDict++) {
    fDictionary[iDict] = new AliTRDsegmentArray("AliTRDdataArrayI",kNdet);
  }

}

//_____________________________________________________________________________
AliTRDdigitsManager::AliTRDdigitsManager(AliTRDdigitsManager &m)
{
  //
  // AliTRDdigitsManager copy constructor
  //

  m.Copy(*this);

}

//_____________________________________________________________________________
AliTRDdigitsManager::~AliTRDdigitsManager()
{
  //
  // AliTRDdigitsManager destructor
  //

  if (fDigits) {
    fDigits->Delete();
    delete fDigits;
  }

  for (Int_t iDict = 0; iDict < kNDict; iDict++) {
    fDictionary[iDict]->Delete();
    delete fDictionary[iDict];
  }

}

//_____________________________________________________________________________
void AliTRDdigitsManager::Copy(AliTRDdigitsManager &m)
{
  //
  // Copy function
  //

  m.fIsRaw = fIsRaw;

  TObject::Copy(m);

}

//_____________________________________________________________________________
void AliTRDdigitsManager::SetRaw()
{

  fIsRaw = kTRUE;

  fDigits->SetBit(kRawDigit);
  
}

//_____________________________________________________________________________
Bool_t AliTRDdigitsManager::MakeBranch()
{
  //
  // Creates the branches for the digits and the dictionary in the digits tree
  //

  Int_t buffersize = 64000;

  Bool_t status = kTRUE;

  if (gAlice->TreeD()) {

    // Make the branch for the digits
    if (fDigits) {
      const AliTRDdataArrayI *kDigits = 
           (AliTRDdataArrayI *) fDigits->At(0);
      if (kDigits) {
        gAlice->TreeD()->Branch("TRDdigits",kDigits->IsA()->GetName()
                                           ,&kDigits,buffersize,1);
        printf("AliTRDdigitsManager::MakeBranch -- ");
        printf("Making branch TRDdigits\n");
      }
      else {
        status = kFALSE;
      }
    }
    else {
      status = kFALSE;
    }

    // Make the branches for the dictionaries
    for (Int_t iDict = 0; iDict < kNDict; iDict++) {

      Char_t branchname[15];
      sprintf(branchname,"TRDdictionary%d",iDict);
      if (fDictionary[iDict]) {
        const AliTRDdataArrayI *kDictionary = 
             (AliTRDdataArrayI *) fDictionary[iDict]->At(0);
        if (kDictionary) {
          gAlice->TreeD()->Branch(branchname,kDictionary->IsA()->GetName()
                                            ,&kDictionary,buffersize,1);
          printf("AliTRDdigitsManager::MakeBranch -- ");
          printf("Making branch %s\n",branchname);
	}
        else {
          status = kFALSE;
	}
      }
      else {
        status = kFALSE;
      }
    }

  }
  else {
    status = kFALSE;
  }

  return status;

}

//_____________________________________________________________________________
Bool_t AliTRDdigitsManager::ReadDigits()
{
  //
  // Reads the digit information from the input file
  //

  Bool_t status = kTRUE;

  status = fDigits->LoadArray("TRDdigits");

  for (Int_t iDict = 0; iDict < kNDict; iDict++) {
    Char_t branchname[15];
    sprintf(branchname,"TRDdictionary%d",iDict);
    status = fDictionary[iDict]->LoadArray(branchname);
  }  

  if (fDigits->TestBit(kRawDigit)) {
    fIsRaw = kTRUE;
  }
  else {
    fIsRaw = kFALSE;
  }

  return kTRUE;

}

//_____________________________________________________________________________
Bool_t AliTRDdigitsManager::WriteDigits()
{
  //
  // Writes out the TRD-digits and the dictionaries
  //

  // Create the branches
  if (!(gAlice->TreeD()->GetBranch("TRDdigits"))) { 
    if (!MakeBranch()) return kFALSE;
  }

  // Store the contents of the segment array in the tree
  if (!fDigits->StoreArray("TRDdigits")) {
    printf("AliTRDdigitsManager::WriteDigits -- ");
    printf("Error while storing digits in branch TRDdigits\n");
    return kFALSE;
  }
  for (Int_t iDict = 0; iDict < kNDict; iDict++) {
    Char_t branchname[15];
    sprintf(branchname,"TRDdictionary%d",iDict);
    if (!fDictionary[iDict]->StoreArray(branchname)) {
      printf("AliTRDdigitsManager::WriteDigits -- ");
      printf("Error while storing dictionary in branch %s\n",branchname);
      return kFALSE;
    }
  }

  return kTRUE;

}

//_____________________________________________________________________________
AliTRDdigit *AliTRDdigitsManager::GetDigit(Int_t row, Int_t col
                                         , Int_t time, Int_t det)
{
  // 
  // Creates a single digit object 
  //

  Int_t digits[4];
  Int_t amp[1];

  digits[0] = det;
  digits[1] = row;
  digits[2] = col;
  digits[3] = time;

  amp[0]    = GetDigits(det)->GetData(row,col,time);
  
  return (new AliTRDdigit(fIsRaw,digits,amp));

}

//_____________________________________________________________________________
Int_t AliTRDdigitsManager::GetTrack(Int_t track
                                  , Int_t row, Int_t col, Int_t time
                                  , Int_t det)
{
  // 
  // Returns the MC-track numbers from the dictionary.
  //

  if ((track < 0) || (track >= kNDict)) {
    TObject::Error("GetTracks"
                  ,"track %d out of bounds (size: %d, this: 0x%08x)"
                  ,track,kNDict,this);
    return -1;
  }

  // Array contains index+1 to allow data compression
  return (GetDictionary(det,track)->GetData(row,col,time) - 1);

}

