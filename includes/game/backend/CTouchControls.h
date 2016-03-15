/*
 * CTouchControls.h
 *
 *  Created on: Aug 22, 2012
 *      Author: dstoll
 */

#ifndef CTOUCHCONTROLS_H_
#define CTOUCHCONTROLS_H_

#include "includes/core/standardLibrary/StandardLibrary.h"
#include "includes/core/CGame.h"

enum TGameControlType
{
	EGameControlTypeUp,
	EGameControlTypeDown,
	EGameControlTypeLeft,
	EGameControlTypeRight,
	EGameControlTypeIncreaseSpeed,
	EGameControlTypeDecreaseSpeed,
	EGameControlTypeFirePrimary,
	EGameControlTypeFireSecondary,
	EGameControlTypePauseMenu,
	EGameControlTypeZoomIn,
	EGameControlTypeZoomOut
};

class CTouchControls
{
	enum TButtonState
	{
		EDefault,
		EPushed,
		EDisabled
	};

	struct STouchButton
	{
		CTextureObject* TextureObject;
		SGraphicsEngineTextureId* TextureIdDefault;
		SGraphicsEngineTextureId* TextureIdPushed;
		SGraphicsEngineTextureId* TextureIdDisabled;
		TRect HitBox;
		TButtonState ButtonState;
		TGameControlType ControlType;
		TInt FramesSincePushed;
		TInt FingerId;
		TBool OutsideHud;
		CString* ButtonLabel;
	};

public:
	static CTouchControls* New(TBool aUseButtonIcons);
	virtual ~CTouchControls();
	//draws all the Touch Buttons with their current state
	void Draw();
	//draw Secondary Touch Buttons, these buttons will be drawn outside the normal HUD
	void DrawButtonsOutsideHud();
	//should be called if a new TouchEvent occurred
	void HandleTouchEvent(const STouchEventLocationConverted& aTouchEvent);
	//returns how many frames it has been since this control has been pushed, return -1 if it has not been pushed
	TInt GetFramesSincePushed(TGameControlType aControlType);
	//changes if it uses Icons or Text for the buttons
	void SetUseIconButtons(TBool aUseIconButtons);
	//does some booking, should run every game frame
	void Update();
	//Loads All Images needed to display the TouchControls
	static void InitilizeTouchControls();
	//Unloads All images need to display the TouchControls
	static void RemoveTouchControls();


protected:
	CTouchControls(TBool aUseButtonIcons);
	void Construct();
	//Checks if Certain game buttons need to be disabled or enabled
	void CheckIfButtonsNeedToBeDisabled();
	//returns a reference to the TouchButton Data
	STouchButton* GetTouchButton(TGameControlType aControlType);
	//resets a button to its default state, doesn't change state if State is disabeled though
	void ResetButton(STouchButton* aButton);

	CPointerArray<STouchButton>* iTouchButtons;

	SFont iFontForButtons;
	TBool iUseIconButtons;
};

#endif /* CTOUCHCONTROLS_H_ */
