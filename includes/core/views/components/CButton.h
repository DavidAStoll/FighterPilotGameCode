/*
 * CButton.h
 *
 *  Created on: Jul 14, 2012
 *      Author: dstoll
 */

#ifndef CBUTTON_H_
#define CBUTTON_H_

#include "includes/core/utility/CTouchEventHandler.h"
#include "includes/core/views/components/CBasicComponent.h"

class CButton : public CBasicComponent
{
	public:

	enum TButtonState
	{
		EStateDefault,
		EStateSelected,
		EStateDisabled
	};

	struct SButtonArguments
	{
		TPoint Location;
		TSize SizeOfHitbox;
		SGraphicsEngineTextureId* TextureIdDefault;
		SGraphicsEngineTextureId* TextureIdSelected;
		SGraphicsEngineTextureId* TextureIdDisabled;
		TSize SizeOfTexture;
		CString* Label;
		SFont* FontForLabel;
		TColour ColourForLabelDefault;
		TColour ColourForLabelSelected;
		TColour ColourForLabelDisabled;
	};

	virtual ~CButton();
	static CButton* New(const SButtonArguments& aArg);
	//Assigns new default and selected TextureIds for the Button
	void AssignTextureIds(SGraphicsEngineTextureId* aTextureIdDefault, SGraphicsEngineTextureId* aTextureIdSelected, SGraphicsEngineTextureId* TextureIdDisabled);
	//checks if the button has been touched or released
	virtual TBool CheckIfHit(const STouchEventLocationConverted& aTouchEvent);
	//Draws the Button its current state
	virtual void Draw();
	//return the Hitbox size of the Button
	TSize GetSizeOfHitBox();
	//returns weither the button is in default, selected or disabled
	CButton::TButtonState GetState();
	//Resets the button its default state, that is not touched
	void ResetButton();
	//changes the Colour for the Font of this button
	void SetColourForLabel(const TColour& aColourDefault, const TColour& aColourSelected, const TColour& aColourDisabled);
	//changes the label for the button, if label is NULL, no lable will be displayed
	void SetLabel(const CString* aLabel);
	//changes the State of the radio button
	void SetState(CButton::TButtonState aState);

protected:
	CButton(const SButtonArguments& aArg);


	CString* iButtonLabel;
	TSize iSizeOfHitBox;
	TInt iIdOfFinger;
	SFont iFontForLabel;
	TButtonState iState;
	SGraphicsEngineTextureId* iTextureIdSelected;
	SGraphicsEngineTextureId* iTextureIdDisabled;
	TColour iColourForLabelDefault;
	TColour iColourForLabelSelected;
	TColour iColourForLabelDisabled;
};

#endif /* CBUTTON_H_ */
