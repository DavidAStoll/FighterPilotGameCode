/*
 * CRadioButton.h
 *
 *  Created on: Aug 19, 2012
 *      Author: dstoll
 */

#ifndef CRADIOBUTTON_H_
#define CRADIOBUTTON_H_

#include "includes/core/utility/CTouchEventHandler.h"
#include "includes/core/views/components/CBasicComponent.h"

class CRadioButton : public CBasicComponent
{

public:

	enum TRadioButtonState
	{
		EStateDefault,
		EStateSelected,
		EStateDisabled
	};

	struct SRadioButtonArguments
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

	virtual ~CRadioButton();
	static CRadioButton* New(const SRadioButtonArguments& aArg);
	//Assigns new default and selected TextureIds for the Button
	void AssignTextureIds(SGraphicsEngineTextureId* aTextureIdDefault, SGraphicsEngineTextureId* aTextureIdSelected, SGraphicsEngineTextureId* aTextureIdDisabled);
	//checks if the button has been touched or released
	virtual TBool CheckIfHit(const STouchEventLocationConverted& aTouchEvent);
	//Draws the Button its current state
	virtual void Draw();
	//returns weither the button is in default, selected or disabled
	CRadioButton::TRadioButtonState GetState();
	//Resets the button its default state, that is not Selected
	void ResetButton();
	//changes the Colour for the Font of this button
	void SetColourForLabel(const TColour& aColourDefault, const TColour& aColourSelected, const TColour& aColourDisabled);
	//changes the State of the radio button
	void SetState(CRadioButton::TRadioButtonState aState);

protected:

	CRadioButton(const SRadioButtonArguments& aArg);

	SGraphicsEngineTextureId* iTextureIdSelected;
	SGraphicsEngineTextureId* iTextureIdDisabled;
	CString* iButtonLabel;
	TSize iSizeOfHitBox;
	TRadioButtonState iState;
	SFont iFontForLabel;
	TColour iColourForLabelDefault;
	TColour iColourForLabelSelected;
	TColour iColourForLabelDisabled;
};

#endif /* CRADIOBUTTON_H_ */
