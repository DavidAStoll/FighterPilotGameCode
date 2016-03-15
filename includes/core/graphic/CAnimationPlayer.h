/*
 * CAnimationPlayer.h
 *
 *  Created on: Jun 11, 2012
 *      Author: dstoll
 */

#ifndef CANIMATIONPLAYER_H_
#define CANIMATIONPLAYER_H_

#include "includes/core/graphic/CTextureObject.h"
#include "includes/core/utility/CFileStream.h"

// CLASS DECLARATION

/**
 *  CAnimationPlayer
 *  This class, will Play animations stored in a single texture object that contains the entire animation sequence.
 *  Each image will have the same size and start from left to right. Going to the next image after certain number of frames
 *  after the animation has been complete it will return to a default animation to play.
 */

struct SAnimationRequest
{
  SGraphicsEngineTextureId* TextureId;//which texture handle contains the sequence
  TInt FramesUntilSwitch;//how many frames will be waited until we switch to the next image
  TInt TotalImages; // how many images are contained in the sequence
  TInt Width;//width of each image
  TInt TotalLength;
  TFloat LowerT;
  TFloat UpperT;
};

class CAnimationPlayer
	{
public:

	~CAnimationPlayer();
	static CAnimationPlayer* New(CTextureObject* aTextureObject,SAnimationRequest aDefaultRequest);

	void AssignDefaultAnimation(SAnimationRequest aDefaultAnimation);
	void CancelCurrentAnimation();//stops the current animations and starts playing the default one
	TInt GetCurrentImageInAnimation();
	void PlayAnimation(SAnimationRequest* aRequest);
	void Update();//updates the state of the animaiton object, which might result in changing in what image the Texture object will display
	void SaveOnDisk(CFileWriteStream &aOutputStream);
	void LoadFromDisk(CFileReadStream &aInputStream,CTextureObject* aTextureObject);

private:

	CAnimationPlayer(CTextureObject* aTextureObject,SAnimationRequest aDefaultRequest);
	void Construct();

	TInt iCurrentImage;//indexes what image of the animation is currently played
	TInt iCurrentFrame;
	TInt iTotalFramesCalled;
	CTextureObject* iTextureObject;
	SAnimationRequest iDefaultRequest;//returns to playing this request after currentRequest has been completed
	SAnimationRequest* iCurrentRequest;// points to the current request and when completed will delete it

	};


#endif /* CANIMATIONPLAYER_H_ */
