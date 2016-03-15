/*
 * CBasicTransition.cpp
 *
 *  Created on: Jul 11, 2012
 *      Author: dstoll
 */

#include "includes/core/views/transitions/CBasicTransition.h"
#include "includes/core/CGame.h"

#define BASIC_TRANSITION_FRAMES_PER_SECOND_DURING_TRANSITION 60

CBasicTransition::CBasicTransition(CBasicView** aViewObserver, CBasicView* aOldView, CBasicView* aNewView, TFloat aDurationInSeconds)
:CBasicView()
{
	iViewObserver = aViewObserver;
	iNewView = aNewView;
	iOldView = aOldView;
	iUpdateCounter = 0;
	iDeleteOldView = true;
	(*iViewObserver) = this; //observer will now always reference this transition
	//need to find out how many frames we have for the transition
	iUpdatesUntilComplete = aDurationInSeconds * BASIC_TRANSITION_FRAMES_PER_SECOND_DURING_TRANSITION;
	//create new View and Old View images
	BasicConstruct();
}

CBasicTransition::~CBasicTransition()
{
	if (iOldViewTextureObject != NULL)
	{
		delete iOldViewTextureObject;
		iOldViewTextureObject = NULL;
	}
	if (iNewViewTextureObject != NULL)
	{
		delete iNewViewTextureObject;
		iNewViewTextureObject = NULL;
	}
	if (iOldViewTextureId.GraphicsEngineId != 0)
	{
		glDeleteTextures(1, &iOldViewTextureId.OpenGlId); //need to delete outside the normal deletion method, since it is not cached
	}
	if (iNewViewTextureId.GraphicsEngineId != 0)
	{
		glDeleteTextures(1, &iNewViewTextureId.OpenGlId);
	}

	//signal graphics engine that it can now reclaim the memory used for the transition
	CGame::Game->iMemoryUsageTracker->UpdateGPUUsedForTextures(-iKiloBytesUsedForTransitionOnGPU);
}

void  CBasicTransition::BasicConstruct()
{
	//Tell both Views that they are undergoing a transition
	iOldView->SetIsDoingTransition(true);
	iNewView->SetIsDoingTransition(true);

	//run logic code for the new view, so it gets into the right state
	iNewView->RunLogicCode();

	//need it so we can reset it after the transition is done
	iFramesPerSecondBeforeTransition = CGame::Game->iFramesPerSecondCounter->GetFramesPerSecond();
	CGame::Game->iFramesPerSecondCounter->SetFramesPerSecond(BASIC_TRANSITION_FRAMES_PER_SECOND_DURING_TRANSITION);
	//create textures for the Old and New View
	TSize lSizeOfScreenInPixels = iGraphicsEngine->GetRealDimensionsOfScreenInPixels();
	TInt lWidthOfScreenTexture = CMath::GetNextGreatestMemberOfPower(lSizeOfScreenInPixels.iWidth, 2);
	TInt lHeightOfScreenTexture = CMath::GetNextGreatestMemberOfPower(lSizeOfScreenInPixels.iHeight, 2);

	//need to make copies of the old and New View
	iOldViewTextureId = CGame::Game->iGraphicsEngine->CreateGraphicsTextureId();
	iNewViewTextureId = CGame::Game->iGraphicsEngine->CreateGraphicsTextureId();

	//need to clean up the GPU
	iGraphicsEngine->RemoveTexturesFromGPUThatHaveBeenCached();
	iKiloBytesUsedForTransitionOnGPU = lWidthOfScreenTexture * lHeightOfScreenTexture * 4;
	//signal graphics engine that it needs to reserve some memory on the GPU for the transition textures
	CGame::Game->iMemoryUsageTracker->UpdateGPUUsedForTextures(iKiloBytesUsedForTransitionOnGPU);

	glEnable(GL_TEXTURE_2D);
	//create texture for OldView
	glBindTexture(GL_TEXTURE_2D, iOldViewTextureId.OpenGlId);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, lWidthOfScreenTexture, lHeightOfScreenTexture, 0, GL_RGBA, GL_UNSIGNED_BYTE,
			NULL);
	iOldViewTextureId.MaxS = TIntFloat::Convert((TFloat) lSizeOfScreenInPixels.iWidth / lWidthOfScreenTexture);
	iOldViewTextureId.MaxT = TIntFloat::Convert((TFloat) lSizeOfScreenInPixels.iHeight / lHeightOfScreenTexture);

	//create Texture for NewView
	glBindTexture(GL_TEXTURE_2D, iNewViewTextureId.OpenGlId);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, lWidthOfScreenTexture, lHeightOfScreenTexture, 0, GL_RGBA, GL_UNSIGNED_BYTE,
			NULL);
	iNewViewTextureId.MaxS = TIntFloat::Convert((TFloat) lSizeOfScreenInPixels.iWidth / lWidthOfScreenTexture);
	iNewViewTextureId.MaxT = TIntFloat::Convert((TFloat) lSizeOfScreenInPixels.iHeight / lHeightOfScreenTexture);


	iGraphicsEngine->SetClearColour(KColourBlack);
	iGraphicsEngine->ClearScreen();

	//New View
	iNewView->ChildViewDrawMethod();
	iGraphicsEngine->FlushBatchedTexturesToScreen();
	//copy into texture
	glBindTexture(GL_TEXTURE_2D, iNewViewTextureId.OpenGlId);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, lSizeOfScreenInPixels.iWidth, lSizeOfScreenInPixels.iHeight);


	iGraphicsEngine->SetClearColour(KColourBlack);
	iGraphicsEngine->ClearScreen();

	//save views in the allocated textures
	//Old View
	iOldView->ChildViewDrawMethod();
	iGraphicsEngine->FlushBatchedTexturesToScreen();
	//copy into texture
	glBindTexture(GL_TEXTURE_2D, iOldViewTextureId.OpenGlId);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, lSizeOfScreenInPixels.iWidth, lSizeOfScreenInPixels.iHeight);

	//create texture Object
	iOldViewTextureObject = CTextureObject::New(TPoint(0, 0), &iOldViewTextureId, 1024, 660, 0, 0, 1, 0, 1);
	iOldViewTextureObject->ReflectOverXAxis();
	iOldViewTextureObject->ChangeXCoordinate(-TIntFloat::Convert(512));
	iOldViewTextureObject->ChangeYCoordinate(-TIntFloat::Convert(330));
	iNewViewTextureObject = CTextureObject::New(TPoint(0, 0), &iNewViewTextureId, 1024, 660, 0, 0, 1, 0, 1);
	iNewViewTextureObject->ReflectOverXAxis();
	iNewViewTextureObject->ChangeXCoordinate(-TIntFloat::Convert(512));
	iNewViewTextureObject->ChangeYCoordinate(-TIntFloat::Convert(330));
}

void CBasicTransition::SetDeleteOldView(TBool aValue)
{
	iDeleteOldView = aValue;
}

void CBasicTransition::ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent)
{
	return; //nothing needs to be done
}

void CBasicTransition::ChildViewDrawMethod()
{
	//need to setup the texture environment
	TFloat lAlphaColour[] = {1, 1, 1, 1};
	glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE); //tell texture environment to use texture Environment colour and texture
	glTexEnvx(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);
	glTexEnvx(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_REPLACE);
	glTexEnvx(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_TEXTURE );
	glTexEnvx(GL_TEXTURE_ENV, GL_SRC0_ALPHA, GL_CONSTANT); //we want to draw the entire view without any holes, can happen at edges of textures
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, lAlphaColour); //set texture environment colour

	iGraphicsEngine->SetClearColour(KColourBlack);
	iGraphicsEngine->ClearScreen();

	//reset matrix to identity
	iGraphicsEngine->SetMatrixMode(GL_MODELVIEW);
	iGraphicsEngine->LoadIdentityMatrix();
	iGraphicsEngine->PushMatrix(); //push Modification to matrix so we can remove them easily to avoid messing up any other drawing code

	//move to center of screen, thus to center transition image, since Textue Object is centered around Origin 0,0,0
	iGraphicsEngine->TranslateMatrix(TIntFloat::Convert(512), TIntFloat::Convert(330), TIntFloat::Convert(0));

	//draw the Actuall transition, depends on child
	DrawTransition();

	//removes modification from Matrix
	iGraphicsEngine->PopMatrix();

	iUpdateCounter++;
	//going to check if the transition is done
	if(iUpdateCounter >= iUpdatesUntilComplete)
	{
		//Tell both Views that they are undergoing a transition
		iOldView->SetIsDoingTransition(false);
		iNewView->SetIsDoingTransition(false);

		//set back Frames per Second to the original value
		CGame::Game->iFramesPerSecondCounter->SetFramesPerSecond(iFramesPerSecondBeforeTransition);
		//remove cached fonts since new view might not need them
		iGraphicsFont->RemoveAllCharacterFontMappings();

		//transition has to be done
		(*iViewObserver) = iNewView; //reset the pointer so that the Observer uses the new view from now on

		//clean up
		if(iDeleteOldView)
		{
			delete iOldView; //we are the last reference, so we need to clean it up
		}
		delete this; // transition object is not needed anymore either
	}

	//reset the texture environment
	glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvx(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
	glTexEnvx(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_TEXTURE);
	glTexEnvx(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_PRIMARY_COLOR);
	glTexEnvx(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);
	glTexEnvx(GL_TEXTURE_ENV, GL_SRC0_ALPHA, GL_TEXTURE);
	glTexEnvx(GL_TEXTURE_ENV, GL_SRC1_ALPHA,  GL_PRIMARY_COLOR);
}

