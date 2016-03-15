/*
 * CRadioButtonGroup.cpp
 *
 *  Created on: Aug 19, 2012
 *      Author: dstoll
 */

#include "includes/core/views/components/CRadioButtonGroup.h"

CRadioButtonGroup::CRadioButtonGroup()
{
	iRadioButtons = NULL;
}

CRadioButtonGroup::~CRadioButtonGroup()
{
	if(iRadioButtons != NULL)
	{
		for(TInt lIndex = 0; lIndex < iRadioButtons->GetCount(); lIndex++)
		{
			if(iRadioButtons->Get(lIndex)->RadioButton != NULL)
			{
				delete iRadioButtons->Get(lIndex)->RadioButton;
				iRadioButtons->Get(lIndex)->RadioButton = NULL;
			}
		}

		iRadioButtons->ClearAndDestroy();
		delete iRadioButtons;
		iRadioButtons = NULL;
	}
}

CRadioButtonGroup* CRadioButtonGroup::New()
{
	CRadioButtonGroup* lSelf = new CRadioButtonGroup();
	lSelf->Construct();
	return lSelf;
}

void CRadioButtonGroup::Construct()
{
	iRadioButtons = CPointerArray<SRadioButtonEntry>::New();
}

//--------------------- functions ------------------//

void CRadioButtonGroup::AddRadioButton(CRadioButton* aButton, TInt aValueIfSelected)
{
	SRadioButtonEntry lEntry;
	lEntry.RadioButton = aButton;
	lEntry.aValueIfSelected = aValueIfSelected;

	iRadioButtons->Append(new SRadioButtonEntry(lEntry));
}

TBool CRadioButtonGroup::CheckIfHit(const STouchEventLocationConverted& aTouchEvent)
{
	for(TInt lIndex = 0; lIndex < iRadioButtons->GetCount(); lIndex++)
	{
		if(iRadioButtons->Get(lIndex)->RadioButton->CheckIfHit(aTouchEvent))
		{
			//need to deselect all pervious Buttons that are in the select state
			for(TInt lDeselectIndex = 0; lDeselectIndex < iRadioButtons->GetCount(); lDeselectIndex++)
			{
				if(iRadioButtons->Get(lDeselectIndex)->RadioButton->GetState() == CRadioButton::EStateSelected)
				{
					iRadioButtons->Get(lDeselectIndex)->RadioButton->SetState(CRadioButton::EStateDefault);
				}
			}

			//set this button to be selected
			iRadioButtons->Get(lIndex)->RadioButton->SetState(CRadioButton::EStateSelected);
			return true;
		}
	}

	//none of the buttons have been selected
	return false;
}

void CRadioButtonGroup::Draw()
{
	for(TInt lIndex = 0; lIndex < iRadioButtons->GetCount(); lIndex++)
	{
		iRadioButtons->Get(lIndex)->RadioButton->Draw();
	}
}

TInt CRadioButtonGroup::GetCount()
{
	return iRadioButtons->GetCount();
}

CRadioButton* CRadioButtonGroup::GetRadioButton(TInt aIndex)
{
	return iRadioButtons->Get(aIndex)->RadioButton;
}

TInt CRadioButtonGroup::GetValueOfSelectedButton()
{
	for(TInt lIndex = 0; lIndex < iRadioButtons->GetCount(); lIndex++)
	{
		if(iRadioButtons->Get(lIndex)->RadioButton->GetState() == CRadioButton::EStateSelected)
		{
			return iRadioButtons->Get(lIndex)->aValueIfSelected;
		}
	}

	return -1;
}

