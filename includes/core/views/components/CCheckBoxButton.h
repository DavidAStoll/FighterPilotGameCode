/*
 * CCheckBoxButton.h
 *
 *  Created on: Sep 15, 2012
 *      Author: dstoll
 */

#ifndef CCHECKBOXBUTTON_H_
#define CCHECKBOXBUTTON_H_

#include "includes/core/utility/CTouchEventHandler.h"
#include "includes/core/views/components/CBasicComponent.h"

class CCheckBoxButton : public CBasicComponent
{
public:

	enum TCheckBoxButtonState
	{
		EStateDefault,
		EStateSelected,
		EStateDisabled
	};

	struct SCheckBoxButtonArguments
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

	virtual ~CCheckBoxButton();
	static CCheckBoxButton* New(const SCheckBoxButtonArguments& aArg);
	//Assigns new default and selected TextureIds for the Button
	void AssignTextureIds(SGraphicsEngineTextureId* aTextureIdDefault, SGraphicsEngineTextureId* aTextureIdSelected, SGraphicsEngineTextureId* aTextureIdDisabled);
	//checks if the button has been touched or released
	virtual TBool CheckIfHit(const STouchEventLocationConverted& aTouchEvent);
	//Draws the Button its current state
	virtual void Draw();
	//returns weither the button is in default, selected or disabled
	CCheckBoxButton::TCheckBoxButtonState GetState();
	//Resets the button its default state, that is not Selected
	void ResetButton();
	//changes the Colour for the Font of this button
	void SetColourForLabel(const TColour& aColourDefault, const TColour& aColourSelected, const TColour& aColourDisabled);
	//changes the State of the radio button
	void SetState(CCheckBoxButton::TCheckBoxButtonState aState);

protected:

	CCheckBoxButton(const SCheckBoxButtonArguments& aArg);

	SGraphicsEngineTextureId* iTextureIdSelected;
	SGraphicsEngineTextureId* iTextureIdDisabled;
	CString* iButtonLabel;
	TSize iSizeOfHitBox;
	TCheckBoxButtonState iState;
	SFont iFontForLabel;
	TColour iColourForLabelDefault;
	TColour iColourForLabelSelected;
	TColour iColourForLabelDisabled;
};

#endif /* CCHECKBOXBUTTON_H_ */
