/*
 * CAnimationPlayer.cpp
 *
 *  Created on: Jun 11, 2012
 *      Author: dstoll
 */

#include "includes/core/graphic/CAnimationPlayer.h"
#include "includes/core/CGame.h"

CAnimationPlayer::CAnimationPlayer(CTextureObject* aTextureObject,SAnimationRequest aDefaultRequest)
	{
	iTextureObject=aTextureObject;
	iCurrentFrame=0;
	iCurrentImage=0;
	iTotalFramesCalled =0;
	iCurrentRequest=NULL;
	iDefaultRequest=aDefaultRequest;
	}

CAnimationPlayer::~CAnimationPlayer()
	{
	  if(iCurrentRequest)
	    {
	      delete iCurrentRequest;
	    }
	}

CAnimationPlayer* CAnimationPlayer::New(CTextureObject* aTextureObject,SAnimationRequest aDefaultRequest)
	{
	CAnimationPlayer* lSelf = new CAnimationPlayer(aTextureObject,aDefaultRequest);
	lSelf->Construct();
	return lSelf;
	}

void CAnimationPlayer::Construct()
	{
		iCurrentRequest = new SAnimationRequest(iDefaultRequest); //use default animation as current animation
	}
//----------------------- functions ---------------------------

void CAnimationPlayer::AssignDefaultAnimation(SAnimationRequest aDefaultAnimation)
{
  iDefaultRequest=aDefaultAnimation;
}

void CAnimationPlayer::CancelCurrentAnimation()
{
  iCurrentFrame=0;
  iCurrentImage=0;
  if(iCurrentRequest) delete iCurrentRequest;
  iCurrentRequest = new SAnimationRequest(iDefaultRequest); //use default animation as current animation
}

TInt CAnimationPlayer::GetCurrentImageInAnimation()
{
  return iCurrentImage;
}

void CAnimationPlayer::PlayAnimation(SAnimationRequest* aRequest)
{
  //reset all settings to start playing this animation
  iCurrentFrame=0;
  iCurrentImage=0;
  if(iCurrentRequest) delete iCurrentRequest;
  iCurrentRequest=aRequest;
}

void CAnimationPlayer::Update()
{
  if(iCurrentFrame==0)
    {
      //tell the texture object what image to display
      iTextureObject->AssignTexture(iCurrentRequest->TextureId);
      TInt lWidthPixelsStart = iCurrentImage * iCurrentRequest->Width;
      TInt lWidthPixelsEnd = (iCurrentImage + 1) * iCurrentRequest->Width;

      //find out if we need to start on a new line
      if(lWidthPixelsEnd > GRAPHICS_DEFAULT_MAX_TEXTURE_PIXEL_SIZE)
      {
      	//should work if LowerT is less than UpperT, which is hopefully always the case
      	iCurrentRequest->LowerT += iCurrentRequest->UpperT;
      	iCurrentRequest->UpperT += iCurrentRequest->UpperT;

      	//need to start from the beginning of the image
      	iCurrentRequest->TotalImages -= iCurrentImage;
      	iCurrentImage = 0;
        lWidthPixelsStart = 0;
        lWidthPixelsEnd = iCurrentRequest->Width;
      }

      //update S and T values
      iTextureObject->SetNewSandTValues
      (
          (TFloat)(lWidthPixelsStart) / (iCurrentRequest->TotalLength),//lower value
          (TFloat)(lWidthPixelsEnd) / (iCurrentRequest->TotalLength),//upper value
          iCurrentRequest->LowerT, iCurrentRequest->UpperT
      );
    }

  //update what frame we are on right now
  iCurrentFrame++;
  iTotalFramesCalled++;

  if(iCurrentFrame == iCurrentRequest->FramesUntilSwitch)
    {
      iCurrentFrame=0;
      iCurrentImage++;
      if(iCurrentImage == iCurrentRequest->TotalImages)
        CancelCurrentAnimation();//this animation is complete, return to default animation
    }
}

void CAnimationPlayer::SaveOnDisk(CFileWriteStream &aOutputStream)
{
  aOutputStream.WriteInt32(iCurrentImage);
  aOutputStream.WriteInt32(iCurrentFrame);

  //Save default request
	//Write down String location
	const CString* lReuploadEntry = CGame::Game->iGraphicsEngine->GetImageIdentifier(*iDefaultRequest.TextureId);
	aOutputStream.WriteString(lReuploadEntry->GetData());

  aOutputStream.WriteInt32(iDefaultRequest.FramesUntilSwitch);
  aOutputStream.WriteInt32(iDefaultRequest.TotalImages);
  aOutputStream.WriteInt32(iDefaultRequest.TotalLength);
  aOutputStream.WriteInt32(iDefaultRequest.Width);
  aOutputStream.WriteIntFloat(TIntFloat::Convert((iDefaultRequest.LowerT)));
  aOutputStream.WriteIntFloat(TIntFloat::Convert(iDefaultRequest.UpperT));

	//save current request if exists
	if (iCurrentRequest != NULL)
	{
		//signal that we have a current request
		aOutputStream.WriteInt32(1);

		//Write down String location
		const CString* lCurrentReuploadEntry= CGame::Game->iGraphicsEngine->GetImageIdentifier(*iCurrentRequest->TextureId);
		aOutputStream.WriteString(lCurrentReuploadEntry->GetData());

		aOutputStream.WriteInt32(iCurrentRequest->FramesUntilSwitch);
		aOutputStream.WriteInt32(iCurrentRequest->TotalImages);
		aOutputStream.WriteInt32(iCurrentRequest->TotalLength);
		aOutputStream.WriteInt32(iCurrentRequest->Width);
		aOutputStream.WriteIntFloat(TIntFloat::Convert(iCurrentRequest->LowerT));
		aOutputStream.WriteIntFloat(TIntFloat::Convert(iCurrentRequest->UpperT));
	}
	else
	{
		aOutputStream.WriteInt32(0);
	}
}

void CAnimationPlayer::LoadFromDisk(CFileReadStream &aInputStream,CTextureObject* aTextureObject)
{
  iTextureObject = aTextureObject;
  iCurrentImage = aInputStream.ReadInt32();
  iCurrentFrame = aInputStream.ReadInt32();

  //load up the default request
  //get TextureId
  CString* lFileLocation = new CString();
  aInputStream.ReadString(lFileLocation);
  iDefaultRequest.TextureId = CGame::Game->iGraphicsEngine->GetTextureId(lFileLocation->GetData()); // need to convert back float
  delete lFileLocation;

  iDefaultRequest.FramesUntilSwitch = aInputStream.ReadInt32();
  iDefaultRequest.TotalImages = aInputStream.ReadInt32();
  iDefaultRequest.TotalLength = aInputStream.ReadInt32();
	iDefaultRequest.Width = aInputStream.ReadInt32();
	iDefaultRequest.LowerT =  aInputStream.ReadIntFloat().GetAsDouble();
	iDefaultRequest.UpperT =  aInputStream.ReadIntFloat().GetAsDouble();

  //check if a currentRequest was  pending during that time
	TInt lCurrentRequestPending = aInputStream.ReadInt32();
	if (lCurrentRequestPending == 1)
	{
		//need to scan in the current request as well
		if (iCurrentRequest)
			delete iCurrentRequest;
		iCurrentRequest = new SAnimationRequest();

		//Get TextureId
	  CString* lFileLocationCurrent = new CString();
	  aInputStream.ReadString(lFileLocationCurrent);
	  iCurrentRequest->TextureId = CGame::Game->iGraphicsEngine->GetTextureId(lFileLocationCurrent->GetData()); // need to convert back float
	  delete lFileLocationCurrent;

		iCurrentRequest->FramesUntilSwitch = aInputStream.ReadInt32();
		iCurrentRequest->TotalImages = aInputStream.ReadInt32();
		iCurrentRequest->TotalLength = aInputStream.ReadInt32();
		iCurrentRequest->Width = aInputStream.ReadInt32();
		iCurrentRequest->LowerT = (aInputStream.ReadIntFloat().GetAsDouble());
		iCurrentRequest->UpperT = (aInputStream.ReadIntFloat().GetAsDouble());
	}
}

