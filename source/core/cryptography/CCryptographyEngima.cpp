/*
 * CCryptographyEngima.cpp
 *
 *  Created on: Nov 6, 2012
 *      Author: dstoll
 */

#include "includes/core/cryptography/CCryptographyEngima.h"
#include "includes/core/utility/CSystem.h"

CCryptographyEngima::CCryptographyEngima(TUint aNumberOfWheels)
{
	iNumberOfWheels = aNumberOfWheels;
}

CCryptographyEngima::~CCryptographyEngima()
{
	// TODO Auto-generated destructor stub
}

CCryptographyEngima* CCryptographyEngima::New(TUint aNumberOfWheels)
{
	CCryptographyEngima* lSelf = new CCryptographyEngima(aNumberOfWheels);
	lSelf->Construct();
	return lSelf;
}

void CCryptographyEngima::Construct()
{
	iCodeWheelOffsets = new TInt [iNumberOfWheels];

	//need to initialize code values here
	iCodeValues[0] = 240;
	iCodeValues[1] = 59;
	iCodeValues[2] = 22;
	iCodeValues[3] = 200;
	iCodeValues[4] = 111;
	iCodeValues[5] = 123;
	iCodeValues[6] = 224;
	iCodeValues[7] = 108;
	iCodeValues[8] = 131;
	iCodeValues[9] = 199;

	iCodeValues[10] = 250;
	iCodeValues[11] = 97;
	iCodeValues[12] = 164;
	iCodeValues[13] = 101;
	iCodeValues[14] = 33;
	iCodeValues[15] = 167;
	iCodeValues[16] = 21;
	iCodeValues[17] = 15;
	iCodeValues[18] = 154;
	iCodeValues[19] = 62;

	iCodeValues[20] = 148;
	iCodeValues[21] = 79;
	iCodeValues[22] = 99;
	iCodeValues[23] = 226;
	iCodeValues[24] = 100;
	iCodeValues[25] = 155;
	iCodeValues[26] = 125;
	iCodeValues[27] = 216;
	iCodeValues[28] = 201;
	iCodeValues[29] = 57;

	iCodeValues[30] = 165;
	iCodeValues[31] = 30;
	iCodeValues[32] = 247;
	iCodeValues[33] = 227;
	iCodeValues[34] = 29;
	iCodeValues[35] = 113;
	iCodeValues[36] = 185;
	iCodeValues[37] = 35;
	iCodeValues[38] = 230;
	iCodeValues[39] = 137;

	iCodeValues[40] = 112;
	iCodeValues[41] = 1;
	iCodeValues[42] = 10;
	iCodeValues[43] = 170;
	iCodeValues[44] = 13;
	iCodeValues[45] = 40;
	iCodeValues[46] = 80;
	iCodeValues[47] = 28;
	iCodeValues[48] = 184;
	iCodeValues[49] = 228;

	iCodeValues[50] = 231;
	iCodeValues[51] = 223;
	iCodeValues[52] = 105;
	iCodeValues[53] = 139;
	iCodeValues[54] = 174;
	iCodeValues[55] = 68;
	iCodeValues[56] = 132;
	iCodeValues[57] = 128;
	iCodeValues[58] = 207;
	iCodeValues[59] = 225;

	iCodeValues[60] = 239;
	iCodeValues[61] = 198;
	iCodeValues[62] = 65;
	iCodeValues[63] = 140;
	iCodeValues[64] = 103;
	iCodeValues[65] = 25;
	iCodeValues[66] = 192;
	iCodeValues[67] = 204;
	iCodeValues[68] = 49;
	iCodeValues[69] = 189;

	iCodeValues[70] = 212;
	iCodeValues[71] = 24;
	iCodeValues[72] = 7;
	iCodeValues[73] = 178;
	iCodeValues[74] = 245;
	iCodeValues[75] = 114;
	iCodeValues[76] = 248;
	iCodeValues[77] = 34;
	iCodeValues[78] = 96;
	iCodeValues[79] = 235;

	iCodeValues[80] = 75;
	iCodeValues[81] = 229;
	iCodeValues[82] = 92;
	iCodeValues[83] = 180;
	iCodeValues[84] = 51;
	iCodeValues[85] = 181;
	iCodeValues[86] = 236;
	iCodeValues[87] = 206;
	iCodeValues[88] = 41;
	iCodeValues[89] = 214;

	iCodeValues[90] = 70;
	iCodeValues[91] = 246;
	iCodeValues[92] = 43;
	iCodeValues[93] = 72;
	iCodeValues[94] = 52;
	iCodeValues[95] = 110;
	iCodeValues[96] = 194;
	iCodeValues[97] = 48;
	iCodeValues[98] = 55;
	iCodeValues[99] = 102;

	iCodeValues[100] = 218;
	iCodeValues[101] = 193;
	iCodeValues[102] = 82;
	iCodeValues[103] = 91;
	iCodeValues[104] = 251;
	iCodeValues[105] = 26;
	iCodeValues[106] = 121;
	iCodeValues[107] = 161;
	iCodeValues[108] = 109;
	iCodeValues[109] = 9;

	iCodeValues[110] = 241;
	iCodeValues[111] = 244;
	iCodeValues[112] = 4;
	iCodeValues[113] = 186;
	iCodeValues[114] = 220;
	iCodeValues[115] = 255;
	iCodeValues[116] = 81;
	iCodeValues[117] = 213;
	iCodeValues[118] = 209;
	iCodeValues[119] = 32;

	iCodeValues[120] = 238;
	iCodeValues[121] = 252;
	iCodeValues[122] = 211;
	iCodeValues[123] = 78;
	iCodeValues[124] = 221;
	iCodeValues[125] = 116;
	iCodeValues[126] = 106;
	iCodeValues[127] = 17;
	iCodeValues[128] = 217;
	iCodeValues[129] = 14;

	iCodeValues[130] = 37;
	iCodeValues[131] = 88;
	iCodeValues[132] = 3;
	iCodeValues[133] = 2;
	iCodeValues[134] = 195;
	iCodeValues[135] = 73;
	iCodeValues[136] = 115;
	iCodeValues[137] = 177;
	iCodeValues[138] = 60;
	iCodeValues[139] = 156;

	iCodeValues[140] = 54;
	iCodeValues[141] = 173;
	iCodeValues[142] = 5;
	iCodeValues[143] = 142;
	iCodeValues[144] = 129;
	iCodeValues[145] = 175;
	iCodeValues[146] = 31;
	iCodeValues[147] = 71;
	iCodeValues[148] = 159;
	iCodeValues[149] = 16;

	iCodeValues[150] = 63;
	iCodeValues[151] = 179;
	iCodeValues[152] = 86;
	iCodeValues[153] = 243;
	iCodeValues[154] = 122;
	iCodeValues[155] = 202;
	iCodeValues[156] = 205;
	iCodeValues[157] = 197;
	iCodeValues[158] = 147;
	iCodeValues[159] = 42;

	iCodeValues[160] = 172;
	iCodeValues[161] = 39;
	iCodeValues[162] = 182;
	iCodeValues[163] = 77;
	iCodeValues[164] = 234;
	iCodeValues[165] = 23;
	iCodeValues[166] = 254;
	iCodeValues[167] = 219;
	iCodeValues[168] = 222;
	iCodeValues[169] = 76;

	iCodeValues[170] = 138;
	iCodeValues[171] = 117;
	iCodeValues[172] = 203;
	iCodeValues[173] = 134;
	iCodeValues[174] = 210;
	iCodeValues[175] = 0;
	iCodeValues[176] = 85;
	iCodeValues[177] = 94;
	iCodeValues[178] = 152;
	iCodeValues[179] = 168;

	iCodeValues[180] = 90;
	iCodeValues[181] = 144;
	iCodeValues[182] = 56;
	iCodeValues[183] = 133;
	iCodeValues[184] = 163;
	iCodeValues[185] = 135;
	iCodeValues[186] = 83;
	iCodeValues[187] = 27;
	iCodeValues[188] = 46;
	iCodeValues[189] = 187;

	iCodeValues[190] = 183;
	iCodeValues[191] = 6;
	iCodeValues[192] = 38;
	iCodeValues[193] = 253;
	iCodeValues[194] = 120;
	iCodeValues[195] = 158;
	iCodeValues[196] = 104;
	iCodeValues[197] = 11;
	iCodeValues[198] = 130;
	iCodeValues[199] = 233;

	iCodeValues[200] = 44;
	iCodeValues[201] = 237;
	iCodeValues[202] = 169;
	iCodeValues[203] = 47;
	iCodeValues[204] = 157;
	iCodeValues[205] = 151;
	iCodeValues[206] = 215;
	iCodeValues[207] = 95;
	iCodeValues[208] = 45;
	iCodeValues[209] = 166;

	iCodeValues[210] = 127;
	iCodeValues[211] = 107;
	iCodeValues[212] = 249;
	iCodeValues[213] = 171;
	iCodeValues[214] = 146;
	iCodeValues[215] = 50;
	iCodeValues[216] = 149;
	iCodeValues[217] = 176;
	iCodeValues[218] = 64;
	iCodeValues[219] = 19;

	iCodeValues[220] = 12;
	iCodeValues[221] = 8;
	iCodeValues[222] = 87;
	iCodeValues[223] = 119;
	iCodeValues[224] = 20;
	iCodeValues[225] = 84;
	iCodeValues[226] = 69;
	iCodeValues[227] = 160;
	iCodeValues[228] = 143;
	iCodeValues[229] = 58;

	iCodeValues[230] = 232;
	iCodeValues[231] = 61;
	iCodeValues[232] = 126;
	iCodeValues[233] = 208;
	iCodeValues[234] = 188;
	iCodeValues[235] = 74;
	iCodeValues[236] = 191;
	iCodeValues[237] = 98;
	iCodeValues[238] = 162;
	iCodeValues[239] = 145;

	iCodeValues[240] = 124;
	iCodeValues[241] = 66;
	iCodeValues[242] = 93;
	iCodeValues[243] = 89;
	iCodeValues[244] = 118;
	iCodeValues[245] = 242;
	iCodeValues[246] = 153;
	iCodeValues[247] = 150;
	iCodeValues[248] = 196;
	iCodeValues[249] = 36;

	iCodeValues[250] = 190;
	iCodeValues[251] = 53;
	iCodeValues[252] = 141;
	iCodeValues[253] = 18;
	iCodeValues[254] = 136;
	iCodeValues[255] = 67;
}

//------------------ functions ----------------------------//

CPointerArray<TInt>* CCryptographyEngima::EncodeData(CPointerArray<TInt>& aDataToEndcode)
{
	CPointerArray<TInt>* lEncodedData = CPointerArray<TInt>::New();

	//initialize Codewheels Offset
	TInt lInitialCodeValueOffset = CSystem::GetCurrentTimeInMicroSecondsNative() % 256;
	iInitialOffset = lInitialCodeValueOffset;
	for(TInt lIndex = 0; lIndex < iNumberOfWheels; lIndex++)
	{
		iCodeWheelOffsets[lIndex] = (lInitialCodeValueOffset * (lIndex + 1)) % 256; //keep it in range
	}
	//need to add initial CodeValueOffset to allow proper decoding
	lEncodedData->Append(new TInt(lInitialCodeValueOffset));

	for(TInt lIndex = 0; lIndex < aDataToEndcode.GetCount(); lIndex++)
	{
		TInt lValueToEncode = *aDataToEndcode.Get(lIndex);

		TUint8 l1ByteValue = (TUint8) (lValueToEncode >> 24) ;
		TUint8 l2ByteValue = (TUint8) (lValueToEncode >> 16) ;
		TUint8 l3ByteValue = (TUint8) (lValueToEncode >> 8) ;
		TUint8 l4ByteValue = (TUint8) (lValueToEncode >> 0) ;

		l1ByteValue = EncodeByte(l1ByteValue);
		l2ByteValue = EncodeByte(l2ByteValue);
		l3ByteValue = EncodeByte(l3ByteValue);
		l4ByteValue = EncodeByte(l4ByteValue);

		TInt lEncodedIntValue = (l1ByteValue << 24) + (l2ByteValue << 16) + (l3ByteValue << 8) + (l4ByteValue << 0);

		lEncodedData->Append(new TInt(lEncodedIntValue));
	}

	return lEncodedData;
}

CPointerArray<TInt>* CCryptographyEngima::DecodeData(CPointerArray<TInt>& aDataToDecode)
{
	CPointerArray<TInt>* lDecodedData = CPointerArray<TInt>::New();

	//initialize Codewheels Offset
	TInt lInitialCodeValueOffset = *aDataToDecode.Get(0); //first value is always the wheel offset
	for(TInt lIndex = 0; lIndex < iNumberOfWheels; lIndex++)
	{
		iCodeWheelOffsets[lIndex] = (lInitialCodeValueOffset * (lIndex + 1)) % 256; //keep it in range
	}

	for(TInt lIndex = 1; lIndex < aDataToDecode.GetCount(); lIndex++)
	{
		TInt lValueToDecode = *aDataToDecode.Get(lIndex);

		TUint8 l1ByteValue = (TUint8) (lValueToDecode >> 24) ;
		TUint8 l2ByteValue = (TUint8) (lValueToDecode >> 16) ;
		TUint8 l3ByteValue = (TUint8) (lValueToDecode >> 8) ;
		TUint8 l4ByteValue = (TUint8) (lValueToDecode >> 0) ;

		l1ByteValue = DecodeByte(l1ByteValue);
		l2ByteValue = DecodeByte(l2ByteValue);
		l3ByteValue = DecodeByte(l3ByteValue);
		l4ByteValue = DecodeByte(l4ByteValue);

		TInt lDecodedIntValue = (l1ByteValue << 24) + (l2ByteValue << 16) + (l3ByteValue << 8) + (l4ByteValue << 0);

		lDecodedData->Append(new TInt(lDecodedIntValue));
	}

	return lDecodedData;
}

TUint8 CCryptographyEngima::EncodeByte(TUint8 aValue)
{
	TUint8 lEncodedByte;

	iCodeWheelOffsets[0] = (aValue + iCodeWheelOffsets[0]) % 256; //need to update first wheel

	//go through each wheel and keep carrying forward the CodeValue from the previous to modify the WheelOffset of the next wheel
	for(TInt lIndex = 1; lIndex < iNumberOfWheels; lIndex++)
	{
		TUint8 lOffsetFromPreviousWheel = iCodeWheelOffsets[lIndex - 1];
		iCodeWheelOffsets[lIndex] = (iCodeValues[lOffsetFromPreviousWheel] + iCodeWheelOffsets[lIndex]) % 256;
	}

	TUint8 lOffset = iCodeWheelOffsets[iNumberOfWheels - 1];
	lEncodedByte = iCodeValues[lOffset];

	return lEncodedByte;
}

TUint8 CCryptographyEngima::DecodeByte(TUint8 aValue)
{
	TUint8 lDecodedByte;
	TUint8 lOffsetForThisWheel = 0;
	TUint8 lOffsetForPreviousWheel = 0;

	//need to find out what index corresponds to the CodeValue
	lOffsetForThisWheel = GetOffsetValueForCodeValue(aValue);

	for(TInt lIndex = iNumberOfWheels - 1; 0 < lIndex; lIndex--)
	{
		//find out what offset the previous wheel must have given us to reach our current offset
		if(lOffsetForThisWheel < iCodeWheelOffsets[lIndex])
		{
			//means we looped around
			lOffsetForPreviousWheel = lOffsetForThisWheel + (256 - iCodeWheelOffsets[lIndex]);
			lOffsetForPreviousWheel = GetOffsetValueForCodeValue(lOffsetForPreviousWheel); //since it is just a CodeValue, need to get the actual Offset
		}
		else
		{
			lOffsetForPreviousWheel = lOffsetForThisWheel - iCodeWheelOffsets[lIndex];
			lOffsetForPreviousWheel = GetOffsetValueForCodeValue(lOffsetForPreviousWheel); //since it is just a CodeValue, need to get the actual Offset
		}

		//update current wheel Offset Location
		iCodeWheelOffsets[lIndex] = lOffsetForThisWheel;
		lOffsetForThisWheel = lOffsetForPreviousWheel; //for next iteration
	}

	//find out what offset the previous wheel must have given us to reach our current offset
	if(lOffsetForThisWheel < iCodeWheelOffsets[0])
	{
		//means we looped around
		lOffsetForPreviousWheel = lOffsetForThisWheel + (256 - iCodeWheelOffsets[0]);
	}
	else
	{
		lOffsetForPreviousWheel = lOffsetForThisWheel - iCodeWheelOffsets[0];
	}
	iCodeWheelOffsets[0] = lOffsetForThisWheel;

	lDecodedByte = lOffsetForPreviousWheel; //reached the final Offset Value, which was given by the value of the Byte to be encoded
	return lDecodedByte;
}

TUint8 CCryptographyEngima::GetOffsetValueForCodeValue(TUint8 aCodeValue)
{
	TUint8 lOffsetValue = 0;

	//need to find out what index corresponds to the CodeValue
	for(TInt lIndex = 0; lIndex < 256; lIndex++)
	{
		if(iCodeValues[lIndex] == aCodeValue)
		{
			lOffsetValue = lIndex;
			break;
		}
	}

	return lOffsetValue;
}

