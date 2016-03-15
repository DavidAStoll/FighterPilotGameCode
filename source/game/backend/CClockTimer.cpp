/*
 ============================================================================
 Name		: ClockTimer.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CClockTimer implementation
 ============================================================================
 */

#include "includes/game/backend/CClockTimer.h"
#include "includes/game/CFighterPilotThePacificWar.h"

#define CLOCKTIMER_ZERO_PREFIX "0"
#define CLOCKTIMER_MINUTE_SECOND_DIVIDER ":"
#define CLOCKTIMER_FONT_SIZE 40

CClockTimer::CClockTimer(TInt aMinutes, TInt aSeconds,TPoint aPosition,TColour aDrawColor)
	{
  iCurrentSeconds=aSeconds;
  iCurrentMinutes=aMinutes;
  iPosition = aPosition;
  iDrawColor = aDrawColor;
	}

CClockTimer::~CClockTimer()
	{
	}

CClockTimer* CClockTimer::New(TInt aMinutes, TInt aSeconds,TPoint aPosition,TColour aDrawColor)
	{
	CClockTimer* self = new CClockTimer(aMinutes,aSeconds,aPosition,aDrawColor);
	self->ConstructL();
	return self;
	}

void CClockTimer::ConstructL()
	{
		iFont = CGame::Game->iGraphicsFont->CreateFont(CLOCKTIMER_FONT_SIZE);
	}

//--------------------- functions ----------------------//

void CClockTimer::Draw()
{
  if(iCurrentSeconds != 0 || iCurrentMinutes !=0) //otherwise don't draw timer
    {
  	CString* lStringTime = new CString();

    if(iCurrentMinutes<10)
      {
    	lStringTime->Append(CLOCKTIMER_ZERO_PREFIX);
    	lStringTime->AppendNum(iCurrentMinutes); // should only be 1 digit
      }
    else
    	lStringTime->AppendNum(iCurrentMinutes);

    lStringTime->Append(CLOCKTIMER_MINUTE_SECOND_DIVIDER);

    if(iCurrentSeconds<10)
      {
    	lStringTime->Append(CLOCKTIMER_ZERO_PREFIX);
    	lStringTime->AppendNum(iCurrentSeconds); // should only be 1 digit
      }
    else
    	lStringTime->AppendNum(iCurrentSeconds);


    CGame::Game->iGraphicsFont->SetColour(iDrawColor);
    CGame::Game->iGraphicsFont->SetFont(iFont);
    CGame::Game->iGraphicsFont->DrawStringToScreenHorizontally(iPosition, lStringTime);

    delete lStringTime;
    }
}

void CClockTimer::DecreaseTimerByMinute()
{
  if(iCurrentMinutes == 0)
    iCurrentSeconds=0;// set it to 0, no matter how many seconds are left since it can only be equal or less than to 1 minute
  else
    iCurrentMinutes--;
}

void CClockTimer::DecreaseTimerBySecond()
{
  iCurrentSeconds--;
  if(iCurrentSeconds<0 && iCurrentMinutes>0)
    {
    iCurrentMinutes--;
    iCurrentSeconds=59;
    }
  else if(iCurrentSeconds<0 && iCurrentMinutes == 0)
    {
    iCurrentSeconds =0;
    }
}
void CClockTimer::SetNewTime(TInt aMinutes, TInt aSeconds)
{
  iCurrentMinutes = aMinutes;
  iCurrentSeconds = aSeconds;
}

void CClockTimer::SaveOnDisk(CFileWriteStream &aOutputStream)
{
  aOutputStream.WriteInt32(iCurrentSeconds);
  aOutputStream.WriteInt32(iCurrentMinutes);
  aOutputStream.WriteInt32(iPosition.iX);aOutputStream.WriteInt32(iPosition.iY);
  aOutputStream.WriteInt32(iDrawColor);

}

void CClockTimer::LoadFromDisk(CFileReadStream &aReadStream)
{
  iCurrentSeconds = aReadStream.ReadInt32();
  iCurrentMinutes = aReadStream.ReadInt32();
  iPosition.iX = aReadStream.ReadInt32(); iPosition.iY = aReadStream.ReadInt32();
  iDrawColor = aReadStream.ReadInt32();
}
