/*
 * CRadioButtonGroup.h
 *
 *  Created on: Aug 19, 2012
 *      Author: dstoll
 */

#ifndef CRADIOBUTTONGROUP_H_
#define CRADIOBUTTONGROUP_H_

#include "includes/core/standardLibrary/StandardLibrary.h"
#include "includes/core/views/components/CRadioButton.h"

class CRadioButtonGroup
{
public:

	struct SRadioButtonEntry
	{
		CRadioButton* RadioButton;
		TInt aValueIfSelected;
	};

	virtual ~CRadioButtonGroup();
	static CRadioButtonGroup* New();

	//adds a new RadioButton to this group, group also takes ownership of Button
	void AddRadioButton(CRadioButton* aButton, TInt aValueIfSelected);
	//performs a CheckIfHit function call with all RadioButtons in its group, it makes sure that only one Button is selected at all time
	TBool CheckIfHit(const STouchEventLocationConverted& aTouchEvent);
	//draws all Radio Buttons in its group
	void Draw();
	//returns how many radion buttons are in the group
	TInt GetCount();
	//returns the RadioButton at the given Index
	CRadioButton* GetRadioButton(TInt aIndex);
	//returns the SelectedValue of the Button that is currently selected, return -1 if none is selected
	TInt GetValueOfSelectedButton();


protected:
	CRadioButtonGroup();
	void Construct();

	CPointerArray<SRadioButtonEntry>* iRadioButtons;
};

#endif /* CRADIOBUTTONGROUP_H_ */
