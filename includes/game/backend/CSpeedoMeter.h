/*
 * CSpeedoMeter.h
 *
 *  Created on: Aug 23, 2012
 *      Author: dstoll
 */

#ifndef CSPEEDOMETER_H_
#define CSPEEDOMETER_H_

#include "includes/core/standardLibrary/StandardLibrary.h"
#include "includes/core/graphic/CTextureObject.h"

#define SPEEDOMETER_WIDTH 100
#define SPEEDOMETER_HEIGHT_FOR_TEXTURE_OBJECT 190
#define SPEEDOMETER_HEIGHT_TEXTURE 128 //for the entire texture
#define SPEEDOMETER_ACTUAL_HEIGHT 74

#define SPEEDOMETER_X_OFFSET_ON_SCREEN (954 - SPEEDOMETER_WIDTH)
#define SPEEDOMETER_Y_OFFSET_ON_SCREEN (40)

class CSpeedoMeter
{
public:

	~CSpeedoMeter();

	static CSpeedoMeter* New(TIntFloat* aObjectAcceleration,TIntFloat aMaxAcceleration);
	static void InitilizeSpeedoMeter();
	static void RemoveSpeedoMeter();
	void Draw();// draws it at the right bottom corner of the screen
	void UpdateObjectProperties(TIntFloat *aObjectAcceleration,TIntFloat aMaxAcceleration);

private:

	CSpeedoMeter(TIntFloat* aObjectAcceleration,TIntFloat aMaxAcceleration);
	void ConstructL();
	static void LoadSpeedoMeterTextures();
	static void UnloadSpeedoMeterTextures();

	TIntFloat* iCurrentAcceleration;
	TIntFloat iMaxAcceleration;
	CTextureObject* iTextureObject;

};

#endif /* CSPEEDOMETER_H_ */
