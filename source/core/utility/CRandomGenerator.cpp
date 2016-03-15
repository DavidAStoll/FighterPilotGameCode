/*
 * CRandomGenerator.cpp
 *
 *  Created on: Aug 10, 2012
 *      Author: dstoll
 */

#include "includes/core/utility/CRandomGenerator.h"
#include "includes/core/utility/CSystem.h"

CRandomGenerator::CRandomGenerator()
{
	iIndex = 0;
}

CRandomGenerator::~CRandomGenerator()
{
	// TODO Auto-generated destructor stub
}

CRandomGenerator* CRandomGenerator::New(CFramesPerSecondCounter* aFramesPerSecondCounter)
{
	CRandomGenerator* lSelf = new CRandomGenerator();
	lSelf->Construct(aFramesPerSecondCounter);
	return lSelf;
}

void CRandomGenerator::Construct(CFramesPerSecondCounter* aFramesPerSecondCounter)
{
	//start off at a relative random index
	iIndex = CSystem::GetCurrentTimeInMicroSecondsNative() % CRANDOM_GENERATOR_NUMBER_OF_RANDOM_NUMBERS;

	iRandomOffsetValueArray[0] = TIntFloat::Convert(0.520);
	iRandomOffsetValueArray[1] = TIntFloat::Convert(0.300);
	iRandomOffsetValueArray[2] = TIntFloat::Convert(0.695);
	iRandomOffsetValueArray[3] = TIntFloat::Convert(0.509);
	iRandomOffsetValueArray[4] = TIntFloat::Convert(0.332);
	iRandomOffsetValueArray[5] = TIntFloat::Convert(0.599);
	iRandomOffsetValueArray[6] = TIntFloat::Convert(0.798);
	iRandomOffsetValueArray[7] = TIntFloat::Convert(0.467);
	iRandomOffsetValueArray[8] = TIntFloat::Convert(0.460);
	iRandomOffsetValueArray[9] = TIntFloat::Convert(0.214);

	iRandomOffsetValueArray[10] = TIntFloat::Convert(0.217);
	iRandomOffsetValueArray[11] = TIntFloat::Convert(0.740);
	iRandomOffsetValueArray[12] = TIntFloat::Convert(0.905);
	iRandomOffsetValueArray[13] = TIntFloat::Convert(0.146);
	iRandomOffsetValueArray[14] = TIntFloat::Convert(0.687);
	iRandomOffsetValueArray[15] = TIntFloat::Convert(0.065);
	iRandomOffsetValueArray[16] = TIntFloat::Convert(0.666);
	iRandomOffsetValueArray[17] = TIntFloat::Convert(0.129);
	iRandomOffsetValueArray[18] = TIntFloat::Convert(0.473);
	iRandomOffsetValueArray[19] = TIntFloat::Convert(0.706);

	iRandomOffsetValueArray[20] = TIntFloat::Convert(0.477);
	iRandomOffsetValueArray[21] = TIntFloat::Convert(0.428);
	iRandomOffsetValueArray[22] = TIntFloat::Convert(0.311);
	iRandomOffsetValueArray[23] = TIntFloat::Convert(0.663);
	iRandomOffsetValueArray[24] = TIntFloat::Convert(0.606);
	iRandomOffsetValueArray[25] = TIntFloat::Convert(0.044);
	iRandomOffsetValueArray[26] = TIntFloat::Convert(0.730);
	iRandomOffsetValueArray[27] = TIntFloat::Convert(0.937);
	iRandomOffsetValueArray[28] = TIntFloat::Convert(0.050);
	iRandomOffsetValueArray[29] = TIntFloat::Convert(0.975);

	iRandomOffsetValueArray[30] = TIntFloat::Convert(0.200);
	iRandomOffsetValueArray[31] = TIntFloat::Convert(0.527);
	iRandomOffsetValueArray[32] = TIntFloat::Convert(0.546);
	iRandomOffsetValueArray[33] = TIntFloat::Convert(0.958);
	iRandomOffsetValueArray[34] = TIntFloat::Convert(0.168);
	iRandomOffsetValueArray[35] = TIntFloat::Convert(0.623);
	iRandomOffsetValueArray[36] = TIntFloat::Convert(0.257);
	iRandomOffsetValueArray[37] = TIntFloat::Convert(0.823);
	iRandomOffsetValueArray[38] = TIntFloat::Convert(0.125);
	iRandomOffsetValueArray[39] = TIntFloat::Convert(0.986);

	iRandomOffsetValueArray[40] = TIntFloat::Convert(0.751);
	iRandomOffsetValueArray[41] = TIntFloat::Convert(0.873);
	iRandomOffsetValueArray[42] = TIntFloat::Convert(0.242);
	iRandomOffsetValueArray[43] = TIntFloat::Convert(0.399);
	iRandomOffsetValueArray[44] = TIntFloat::Convert(0.196);
	iRandomOffsetValueArray[45] = TIntFloat::Convert(0.805);
	iRandomOffsetValueArray[46] = TIntFloat::Convert(0.712);
	iRandomOffsetValueArray[47] = TIntFloat::Convert(0.723);
	iRandomOffsetValueArray[48] = TIntFloat::Convert(0.691);
	iRandomOffsetValueArray[49] = TIntFloat::Convert(0.787);

	iRandomOffsetValueArray[50] = TIntFloat::Convert(0.499);
	iRandomOffsetValueArray[51] = TIntFloat::Convert(0.364);
	iRandomOffsetValueArray[52] = TIntFloat::Convert(0.503);
	iRandomOffsetValueArray[53] = TIntFloat::Convert(0.086);
	iRandomOffsetValueArray[54] = TIntFloat::Convert(0.602);
	iRandomOffsetValueArray[55] = TIntFloat::Convert(0.321);
	iRandomOffsetValueArray[56] = TIntFloat::Convert(0.631);
	iRandomOffsetValueArray[57] = TIntFloat::Convert(0.702);
	iRandomOffsetValueArray[58] = TIntFloat::Convert(0.755);
	iRandomOffsetValueArray[59] = TIntFloat::Convert(0.595);

	iRandomOffsetValueArray[60] = TIntFloat::Convert(0.076);
	iRandomOffsetValueArray[61] = TIntFloat::Convert(0.634);
	iRandomOffsetValueArray[62] = TIntFloat::Convert(0.225);
	iRandomOffsetValueArray[63] = TIntFloat::Convert(0.919);
	iRandomOffsetValueArray[64] = TIntFloat::Convert(0.837);
	iRandomOffsetValueArray[65] = TIntFloat::Convert(0.616);
	iRandomOffsetValueArray[66] = TIntFloat::Convert(0.012);
	iRandomOffsetValueArray[67] = TIntFloat::Convert(0.826);
	iRandomOffsetValueArray[68] = TIntFloat::Convert(0.648);
	iRandomOffsetValueArray[69] = TIntFloat::Convert(0.915);

	iRandomOffsetValueArray[70] = TIntFloat::Convert(0.114);
	iRandomOffsetValueArray[71] = TIntFloat::Convert(0.783);
	iRandomOffsetValueArray[72] = TIntFloat::Convert(0.777);
	iRandomOffsetValueArray[73] = TIntFloat::Convert(0.531);
	iRandomOffsetValueArray[74] = TIntFloat::Convert(0.268);
	iRandomOffsetValueArray[75] = TIntFloat::Convert(0.791);
	iRandomOffsetValueArray[76] = TIntFloat::Convert(0.221);
	iRandomOffsetValueArray[77] = TIntFloat::Convert(0.463);
	iRandomOffsetValueArray[78] = TIntFloat::Convert(0.738);
	iRandomOffsetValueArray[79] = TIntFloat::Convert(0.381);

	iRandomOffsetValueArray[80] = TIntFloat::Convert(0.716);
	iRandomOffsetValueArray[81] = TIntFloat::Convert(0.445);
	iRandomOffsetValueArray[82] = TIntFloat::Convert(0.524);
	iRandomOffsetValueArray[83] = TIntFloat::Convert(0.022);
	iRandomOffsetValueArray[84] = TIntFloat::Convert(0.794);
	iRandomOffsetValueArray[85] = TIntFloat::Convert(0.744);
	iRandomOffsetValueArray[86] = TIntFloat::Convert(0.627);
	iRandomOffsetValueArray[87] = TIntFloat::Convert(0.979);
	iRandomOffsetValueArray[88] = TIntFloat::Convert(0.922);
	iRandomOffsetValueArray[89] = TIntFloat::Convert(0.360);

	iRandomOffsetValueArray[90] = TIntFloat::Convert(0.781);
	iRandomOffsetValueArray[91] = TIntFloat::Convert(0.253);
	iRandomOffsetValueArray[92] = TIntFloat::Convert(0.367);
	iRandomOffsetValueArray[93] = TIntFloat::Convert(0.292);
	iRandomOffsetValueArray[94] = TIntFloat::Convert(0.516);
	iRandomOffsetValueArray[95] = TIntFloat::Convert(0.578);
	iRandomOffsetValueArray[96] = TIntFloat::Convert(0.862);
	iRandomOffsetValueArray[97] = TIntFloat::Convert(0.275);
	iRandomOffsetValueArray[98] = TIntFloat::Convert(0.303);
	iRandomOffsetValueArray[99] = TIntFloat::Convert(0.484);

	iRandomOffsetValueArray[100] = TIntFloat::Convert(0.674);
	iRandomOffsetValueArray[101] = TIntFloat::Convert(0.574);
	iRandomOffsetValueArray[102] = TIntFloat::Convert(0.140);
	iRandomOffsetValueArray[103] = TIntFloat::Convert(0.441);
	iRandomOffsetValueArray[104] = TIntFloat::Convert(0.802);
	iRandomOffsetValueArray[105] = TIntFloat::Convert(0.189);
	iRandomOffsetValueArray[106] = TIntFloat::Convert(0.559);
	iRandomOffsetValueArray[107] = TIntFloat::Convert(0.449);
	iRandomOffsetValueArray[108] = TIntFloat::Convert(0.247);
	iRandomOffsetValueArray[109] = TIntFloat::Convert(0.855);

	iRandomOffsetValueArray[110] = TIntFloat::Convert(0.029);
	iRandomOffsetValueArray[111] = TIntFloat::Convert(0.040);
	iRandomOffsetValueArray[112] = TIntFloat::Convert(0.007);
	iRandomOffsetValueArray[113] = TIntFloat::Convert(0.104);
	iRandomOffsetValueArray[114] = TIntFloat::Convert(0.815);
	iRandomOffsetValueArray[115] = TIntFloat::Convert(0.680);
	iRandomOffsetValueArray[116] = TIntFloat::Convert(0.819);
	iRandomOffsetValueArray[117] = TIntFloat::Convert(0.403);
	iRandomOffsetValueArray[118] = TIntFloat::Convert(0.652);
	iRandomOffsetValueArray[119] = TIntFloat::Convert(0.638);

	iRandomOffsetValueArray[120] = TIntFloat::Convert(0.947);
	iRandomOffsetValueArray[121] = TIntFloat::Convert(0.018);
	iRandomOffsetValueArray[122] = TIntFloat::Convert(0.072);
	iRandomOffsetValueArray[123] = TIntFloat::Convert(0.911);
	iRandomOffsetValueArray[124] = TIntFloat::Convert(0.392);
	iRandomOffsetValueArray[125] = TIntFloat::Convert(0.684);
	iRandomOffsetValueArray[126] = TIntFloat::Convert(0.542);
	iRandomOffsetValueArray[127] = TIntFloat::Convert(0.236);
	iRandomOffsetValueArray[128] = TIntFloat::Convert(0.887);
	iRandomOffsetValueArray[129] = TIntFloat::Convert(0.933);

	iRandomOffsetValueArray[130] = TIntFloat::Convert(0.328);
	iRandomOffsetValueArray[131] = TIntFloat::Convert(0.877);
	iRandomOffsetValueArray[132] = TIntFloat::Convert(0.965);
	iRandomOffsetValueArray[133] = TIntFloat::Convert(0.232);
	iRandomOffsetValueArray[134] = TIntFloat::Convert(0.431);
	iRandomOffsetValueArray[135] = TIntFloat::Convert(0.834);
	iRandomOffsetValueArray[136] = TIntFloat::Convert(0.093);
	iRandomOffsetValueArray[137] = TIntFloat::Convert(0.847);
	iRandomOffsetValueArray[138] = TIntFloat::Convert(0.584);
	iRandomOffsetValueArray[139] = TIntFloat::Convert(0.108);

	iRandomOffsetValueArray[140] = TIntFloat::Convert(0.537);
	iRandomOffsetValueArray[141] = TIntFloat::Convert(0.514);
	iRandomOffsetValueArray[142] = TIntFloat::Convert(0.054);
	iRandomOffsetValueArray[143] = TIntFloat::Convert(0.698);
	iRandomOffsetValueArray[144] = TIntFloat::Convert(0.033);
	iRandomOffsetValueArray[145] = TIntFloat::Convert(0.762);
	iRandomOffsetValueArray[146] = TIntFloat::Convert(0.841);
	iRandomOffsetValueArray[147] = TIntFloat::Convert(0.339);
	iRandomOffsetValueArray[148] = TIntFloat::Convert(0.845);
	iRandomOffsetValueArray[149] = TIntFloat::Convert(0.061);

	iRandomOffsetValueArray[150] = TIntFloat::Convert(0.943);
	iRandomOffsetValueArray[151] = TIntFloat::Convert(0.296);
	iRandomOffsetValueArray[152] = TIntFloat::Convert(0.238);
	iRandomOffsetValueArray[153] = TIntFloat::Convert(0.676);
	iRandomOffsetValueArray[154] = TIntFloat::Convert(0.097);
	iRandomOffsetValueArray[155] = TIntFloat::Convert(0.570);
	iRandomOffsetValueArray[156] = TIntFloat::Convert(0.417);
	iRandomOffsetValueArray[157] = TIntFloat::Convert(0.343);
	iRandomOffsetValueArray[158] = TIntFloat::Convert(0.567);
	iRandomOffsetValueArray[159] = TIntFloat::Convert(0.894);

	iRandomOffsetValueArray[160] = TIntFloat::Convert(0.178);
	iRandomOffsetValueArray[161] = TIntFloat::Convert(0.591);
	iRandomOffsetValueArray[162] = TIntFloat::Convert(0.353);
	iRandomOffsetValueArray[163] = TIntFloat::Convert(0.535);
	iRandomOffsetValueArray[164] = TIntFloat::Convert(0.990);
	iRandomOffsetValueArray[165] = TIntFloat::Convert(0.890);
	iRandomOffsetValueArray[166] = TIntFloat::Convert(0.456);
	iRandomOffsetValueArray[167] = TIntFloat::Convert(0.492);
	iRandomOffsetValueArray[168] = TIntFloat::Convert(0.118);
	iRandomOffsetValueArray[169] = TIntFloat::Convert(0.505);

	iRandomOffsetValueArray[170] = TIntFloat::Convert(0.610);
	iRandomOffsetValueArray[171] = TIntFloat::Convert(0.766);
	iRandomOffsetValueArray[172] = TIntFloat::Convert(0.563);
	iRandomOffsetValueArray[173] = TIntFloat::Convert(0.172);
	iRandomOffsetValueArray[174] = TIntFloat::Convert(0.345);
	iRandomOffsetValueArray[175] = TIntFloat::Convert(0.356);
	iRandomOffsetValueArray[176] = TIntFloat::Convert(0.324);
	iRandomOffsetValueArray[177] = TIntFloat::Convert(0.420);
	iRandomOffsetValueArray[178] = TIntFloat::Convert(0.866);
	iRandomOffsetValueArray[179] = TIntFloat::Convert(0.997);

	iRandomOffsetValueArray[180] = TIntFloat::Convert(0.136);
	iRandomOffsetValueArray[181] = TIntFloat::Convert(0.719);
	iRandomOffsetValueArray[182] = TIntFloat::Convert(0.969);
	iRandomOffsetValueArray[183] = TIntFloat::Convert(0.954);
	iRandomOffsetValueArray[184] = TIntFloat::Convert(0.264);
	iRandomOffsetValueArray[185] = TIntFloat::Convert(0.335);
	iRandomOffsetValueArray[186] = TIntFloat::Convert(0.388);
	iRandomOffsetValueArray[187] = TIntFloat::Convert(0.228);
	iRandomOffsetValueArray[188] = TIntFloat::Convert(0.708);
	iRandomOffsetValueArray[189] = TIntFloat::Convert(0.001);

	iRandomOffsetValueArray[190] = TIntFloat::Convert(0.858);
	iRandomOffsetValueArray[191] = TIntFloat::Convert(0.552);
	iRandomOffsetValueArray[192] = TIntFloat::Convert(0.204);
	iRandomOffsetValueArray[193] = TIntFloat::Convert(0.249);
	iRandomOffsetValueArray[194] = TIntFloat::Convert(0.644);
	iRandomOffsetValueArray[195] = TIntFloat::Convert(0.193);
	iRandomOffsetValueArray[196] = TIntFloat::Convert(0.281);
	iRandomOffsetValueArray[197] = TIntFloat::Convert(0.548);
	iRandomOffsetValueArray[198] = TIntFloat::Convert(0.481);
	iRandomOffsetValueArray[199] = TIntFloat::Convert(0.150);

	iRandomOffsetValueArray[200] = TIntFloat::Convert(0.409);
	iRandomOffsetValueArray[201] = TIntFloat::Convert(0.898);
	iRandomOffsetValueArray[202] = TIntFloat::Convert(0.901);
	iRandomOffsetValueArray[203] = TIntFloat::Convert(0.424);
	iRandomOffsetValueArray[204] = TIntFloat::Convert(0.588);
	iRandomOffsetValueArray[205] = TIntFloat::Convert(0.830);
	iRandomOffsetValueArray[206] = TIntFloat::Convert(0.371);
	iRandomOffsetValueArray[207] = TIntFloat::Convert(0.749);
	iRandomOffsetValueArray[208] = TIntFloat::Convert(0.349);
	iRandomOffsetValueArray[209] = TIntFloat::Convert(0.813);

	iRandomOffsetValueArray[210] = TIntFloat::Convert(0.157);
	iRandomOffsetValueArray[211] = TIntFloat::Convert(0.655);
	iRandomOffsetValueArray[212] = TIntFloat::Convert(0.161);
	iRandomOffsetValueArray[213] = TIntFloat::Convert(0.377);
	iRandomOffsetValueArray[214] = TIntFloat::Convert(0.260);
	iRandomOffsetValueArray[215] = TIntFloat::Convert(0.612);
	iRandomOffsetValueArray[216] = TIntFloat::Convert(0.289);
	iRandomOffsetValueArray[217] = TIntFloat::Convert(0.727);
	iRandomOffsetValueArray[218] = TIntFloat::Convert(0.413);
	iRandomOffsetValueArray[219] = TIntFloat::Convert(0.620);

	iRandomOffsetValueArray[220] = TIntFloat::Convert(0.734);
	iRandomOffsetValueArray[221] = TIntFloat::Convert(0.659);
	iRandomOffsetValueArray[222] = TIntFloat::Convert(0.883);
	iRandomOffsetValueArray[223] = TIntFloat::Convert(0.210);
	iRandomOffsetValueArray[224] = TIntFloat::Convert(0.495);
	iRandomOffsetValueArray[225] = TIntFloat::Convert(0.642);
	iRandomOffsetValueArray[226] = TIntFloat::Convert(0.670);
	iRandomOffsetValueArray[227] = TIntFloat::Convert(0.851);
	iRandomOffsetValueArray[228] = TIntFloat::Convert(0.307);
	iRandomOffsetValueArray[229] = TIntFloat::Convert(0.206);

	iRandomOffsetValueArray[230] = TIntFloat::Convert(0.772);
	iRandomOffsetValueArray[231] = TIntFloat::Convert(0.809);
	iRandomOffsetValueArray[232] = TIntFloat::Convert(0.435);
	iRandomOffsetValueArray[233] = TIntFloat::Convert(0.556);
	iRandomOffsetValueArray[234] = TIntFloat::Convert(0.926);
	iRandomOffsetValueArray[235] = TIntFloat::Convert(0.082);
	iRandomOffsetValueArray[236] = TIntFloat::Convert(0.879);
	iRandomOffsetValueArray[237] = TIntFloat::Convert(0.488);
	iRandomOffsetValueArray[238] = TIntFloat::Convert(0.396);
	iRandomOffsetValueArray[239] = TIntFloat::Convert(0.407);

	iRandomOffsetValueArray[240] = TIntFloat::Convert(0.375);
	iRandomOffsetValueArray[241] = TIntFloat::Convert(0.471);
	iRandomOffsetValueArray[242] = TIntFloat::Convert(0.182);
	iRandomOffsetValueArray[243] = TIntFloat::Convert(0.313);
	iRandomOffsetValueArray[244] = TIntFloat::Convert(0.452);
	iRandomOffsetValueArray[245] = TIntFloat::Convert(0.770);
	iRandomOffsetValueArray[246] = TIntFloat::Convert(0.285);
	iRandomOffsetValueArray[247] = TIntFloat::Convert(0.270);
	iRandomOffsetValueArray[248] = TIntFloat::Convert(0.580);
	iRandomOffsetValueArray[249] = TIntFloat::Convert(0.385);

	iRandomOffsetValueArray[250] = TIntFloat::Convert(0.439);
	iRandomOffsetValueArray[251] = TIntFloat::Convert(0.279);
	iRandomOffsetValueArray[252] = TIntFloat::Convert(0.759);
	iRandomOffsetValueArray[253] = TIntFloat::Convert(0.317);
	iRandomOffsetValueArray[254] = TIntFloat::Convert(0.909);
	iRandomOffsetValueArray[255] = TIntFloat::Convert(0.869);
	iRandomOffsetValueArray[256] = TIntFloat::Convert(0.262);
	iRandomOffsetValueArray[257] = TIntFloat::Convert(0.401);
	iRandomOffsetValueArray[258] = TIntFloat::Convert(0.985);
	iRandomOffsetValueArray[259] = TIntFloat::Convert(0.234);

	iRandomOffsetValueArray[260] = TIntFloat::Convert(0.220);
	iRandomOffsetValueArray[261] = TIntFloat::Convert(0.529);
	iRandomOffsetValueArray[262] = TIntFloat::Convert(0.600);
	iRandomOffsetValueArray[263] = TIntFloat::Convert(0.654);
	iRandomOffsetValueArray[264] = TIntFloat::Convert(0.493);
	iRandomOffsetValueArray[265] = TIntFloat::Convert(0.974);
	iRandomOffsetValueArray[266] = TIntFloat::Convert(0.266);
	iRandomOffsetValueArray[267] = TIntFloat::Convert(0.124);
	iRandomOffsetValueArray[268] = TIntFloat::Convert(0.818);
	iRandomOffsetValueArray[269] = TIntFloat::Convert(0.469);

	iRandomOffsetValueArray[270] = TIntFloat::Convert(0.515);
	iRandomOffsetValueArray[271] = TIntFloat::Convert(0.910);
	iRandomOffsetValueArray[272] = TIntFloat::Convert(0.459);
	iRandomOffsetValueArray[273] = TIntFloat::Convert(0.547);
	iRandomOffsetValueArray[274] = TIntFloat::Convert(0.814);
	iRandomOffsetValueArray[275] = TIntFloat::Convert(0.747);
	iRandomOffsetValueArray[276] = TIntFloat::Convert(0.416);
	iRandomOffsetValueArray[277] = TIntFloat::Convert(0.675);
	iRandomOffsetValueArray[278] = TIntFloat::Convert(0.164);
	iRandomOffsetValueArray[279] = TIntFloat::Convert(0.166);

	iRandomOffsetValueArray[280] = TIntFloat::Convert(0.690);
	iRandomOffsetValueArray[281] = TIntFloat::Convert(0.854);
	iRandomOffsetValueArray[282] = TIntFloat::Convert(0.096);
	iRandomOffsetValueArray[283] = TIntFloat::Convert(0.636);
	iRandomOffsetValueArray[284] = TIntFloat::Convert(0.014);
	iRandomOffsetValueArray[285] = TIntFloat::Convert(0.615);
	iRandomOffsetValueArray[286] = TIntFloat::Convert(0.078);
	iRandomOffsetValueArray[287] = TIntFloat::Convert(0.423);
	iRandomOffsetValueArray[288] = TIntFloat::Convert(0.921);
	iRandomOffsetValueArray[289] = TIntFloat::Convert(0.427);

	iRandomOffsetValueArray[290] = TIntFloat::Convert(0.643);
	iRandomOffsetValueArray[291] = TIntFloat::Convert(0.525);
	iRandomOffsetValueArray[292] = TIntFloat::Convert(0.878);
	iRandomOffsetValueArray[293] = TIntFloat::Convert(0.555);
	iRandomOffsetValueArray[294] = TIntFloat::Convert(0.993);
	iRandomOffsetValueArray[295] = TIntFloat::Convert(0.679);
	iRandomOffsetValueArray[296] = TIntFloat::Convert(0.886);
	iRandomOffsetValueArray[297] = TIntFloat::Convert(0.999);
	iRandomOffsetValueArray[298] = TIntFloat::Convert(0.925);
	iRandomOffsetValueArray[299] = TIntFloat::Convert(0.149);

	iRandomOffsetValueArray[300] = TIntFloat::Convert(0.476);
	iRandomOffsetValueArray[301] = TIntFloat::Convert(0.760);
	iRandomOffsetValueArray[302] = TIntFloat::Convert(0.907);
	iRandomOffsetValueArray[303] = TIntFloat::Convert(0.935);
	iRandomOffsetValueArray[304] = TIntFloat::Convert(0.117);
	iRandomOffsetValueArray[305] = TIntFloat::Convert(0.572);
	iRandomOffsetValueArray[306] = TIntFloat::Convert(0.472);
	iRandomOffsetValueArray[307] = TIntFloat::Convert(0.038);
	iRandomOffsetValueArray[308] = TIntFloat::Convert(0.074);
	iRandomOffsetValueArray[309] = TIntFloat::Convert(0.700);

	iRandomOffsetValueArray[310] = TIntFloat::Convert(0.822);
	iRandomOffsetValueArray[311] = TIntFloat::Convert(0.192);
	iRandomOffsetValueArray[312] = TIntFloat::Convert(0.348);
	iRandomOffsetValueArray[313] = TIntFloat::Convert(0.145);
	iRandomOffsetValueArray[314] = TIntFloat::Convert(0.754);
	iRandomOffsetValueArray[315] = TIntFloat::Convert(0.662);
	iRandomOffsetValueArray[316] = TIntFloat::Convert(0.672);
	iRandomOffsetValueArray[317] = TIntFloat::Convert(0.640);
	iRandomOffsetValueArray[318] = TIntFloat::Convert(0.736);
	iRandomOffsetValueArray[319] = TIntFloat::Convert(0.448);

	iRandomOffsetValueArray[320] = TIntFloat::Convert(0.579);
	iRandomOffsetValueArray[321] = TIntFloat::Convert(0.718);
	iRandomOffsetValueArray[322] = TIntFloat::Convert(0.035);
	iRandomOffsetValueArray[323] = TIntFloat::Convert(0.551);
	iRandomOffsetValueArray[324] = TIntFloat::Convert(0.536);
	iRandomOffsetValueArray[325] = TIntFloat::Convert(0.846);
	iRandomOffsetValueArray[326] = TIntFloat::Convert(0.651);
	iRandomOffsetValueArray[327] = TIntFloat::Convert(0.704);
	iRandomOffsetValueArray[328] = TIntFloat::Convert(0.544);
	iRandomOffsetValueArray[329] = TIntFloat::Convert(0.025);

	iRandomOffsetValueArray[330] = TIntFloat::Convert(0.583);
	iRandomOffsetValueArray[331] = TIntFloat::Convert(0.174);
	iRandomOffsetValueArray[332] = TIntFloat::Convert(0.134);
	iRandomOffsetValueArray[333] = TIntFloat::Convert(0.786);
	iRandomOffsetValueArray[334] = TIntFloat::Convert(0.565);
	iRandomOffsetValueArray[335] = TIntFloat::Convert(0.961);
	iRandomOffsetValueArray[336] = TIntFloat::Convert(0.775);
	iRandomOffsetValueArray[337] = TIntFloat::Convert(0.598);
	iRandomOffsetValueArray[338] = TIntFloat::Convert(0.865);
	iRandomOffsetValueArray[339] = TIntFloat::Convert(0.063);

	iRandomOffsetValueArray[340] = TIntFloat::Convert(0.732);
	iRandomOffsetValueArray[341] = TIntFloat::Convert(0.726);
	iRandomOffsetValueArray[342] = TIntFloat::Convert(0.480);
	iRandomOffsetValueArray[343] = TIntFloat::Convert(0.483);
	iRandomOffsetValueArray[344] = TIntFloat::Convert(0.006);
	iRandomOffsetValueArray[345] = TIntFloat::Convert(0.170);
	iRandomOffsetValueArray[346] = TIntFloat::Convert(0.412);
	iRandomOffsetValueArray[347] = TIntFloat::Convert(0.953);
	iRandomOffsetValueArray[348] = TIntFloat::Convert(0.331);
	iRandomOffsetValueArray[349] = TIntFloat::Convert(0.931);

	iRandomOffsetValueArray[350] = TIntFloat::Convert(0.395);
	iRandomOffsetValueArray[351] = TIntFloat::Convert(0.739);
	iRandomOffsetValueArray[352] = TIntFloat::Convert(0.971);
	iRandomOffsetValueArray[353] = TIntFloat::Convert(0.743);
	iRandomOffsetValueArray[354] = TIntFloat::Convert(0.694);
	iRandomOffsetValueArray[355] = TIntFloat::Convert(0.576);
	iRandomOffsetValueArray[356] = TIntFloat::Convert(0.929);
	iRandomOffsetValueArray[357] = TIntFloat::Convert(0.871);
	iRandomOffsetValueArray[358] = TIntFloat::Convert(0.309);
	iRandomOffsetValueArray[359] = TIntFloat::Convert(0.995);

	iRandomOffsetValueArray[360] = TIntFloat::Convert(0.202);
	iRandomOffsetValueArray[361] = TIntFloat::Convert(0.316);
	iRandomOffsetValueArray[362] = TIntFloat::Convert(0.241);
	iRandomOffsetValueArray[363] = TIntFloat::Convert(0.465);
	iRandomOffsetValueArray[364] = TIntFloat::Convert(0.792);
	iRandomOffsetValueArray[365] = TIntFloat::Convert(0.811);
	iRandomOffsetValueArray[366] = TIntFloat::Convert(0.224);
	iRandomOffsetValueArray[367] = TIntFloat::Convert(0.252);
	iRandomOffsetValueArray[368] = TIntFloat::Convert(0.433);
	iRandomOffsetValueArray[369] = TIntFloat::Convert(0.889);

	iRandomOffsetValueArray[370] = TIntFloat::Convert(0.523);
	iRandomOffsetValueArray[371] = TIntFloat::Convert(0.089);
	iRandomOffsetValueArray[372] = TIntFloat::Convert(0.391);
	iRandomOffsetValueArray[373] = TIntFloat::Convert(0.017);
	iRandomOffsetValueArray[374] = TIntFloat::Convert(0.138);
	iRandomOffsetValueArray[375] = TIntFloat::Convert(0.508);
	iRandomOffsetValueArray[376] = TIntFloat::Convert(0.664);
	iRandomOffsetValueArray[377] = TIntFloat::Convert(0.461);
	iRandomOffsetValueArray[378] = TIntFloat::Convert(0.070);
	iRandomOffsetValueArray[379] = TIntFloat::Convert(0.978);

	iRandomOffsetValueArray[380] = TIntFloat::Convert(0.989);
	iRandomOffsetValueArray[381] = TIntFloat::Convert(0.957);
	iRandomOffsetValueArray[382] = TIntFloat::Convert(0.053);
	iRandomOffsetValueArray[383] = TIntFloat::Convert(0.764);
	iRandomOffsetValueArray[384] = TIntFloat::Convert(0.630);
	iRandomOffsetValueArray[385] = TIntFloat::Convert(0.768);
	iRandomOffsetValueArray[386] = TIntFloat::Convert(0.352);
	iRandomOffsetValueArray[387] = TIntFloat::Convert(0.867);
	iRandomOffsetValueArray[388] = TIntFloat::Convert(0.587);
	iRandomOffsetValueArray[389] = TIntFloat::Convert(0.897);

	iRandomOffsetValueArray[390] = TIntFloat::Convert(0.967);
	iRandomOffsetValueArray[391] = TIntFloat::Convert(0.021);
	iRandomOffsetValueArray[392] = TIntFloat::Convert(0.861);
	iRandomOffsetValueArray[393] = TIntFloat::Convert(0.341);
	iRandomOffsetValueArray[394] = TIntFloat::Convert(0.899);
	iRandomOffsetValueArray[395] = TIntFloat::Convert(0.491);
	iRandomOffsetValueArray[396] = TIntFloat::Convert(0.185);
	iRandomOffsetValueArray[397] = TIntFloat::Convert(0.102);
	iRandomOffsetValueArray[398] = TIntFloat::Convert(0.882);
	iRandomOffsetValueArray[399] = TIntFloat::Convert(0.277);

	iRandomOffsetValueArray[400] = TIntFloat::Convert(0.091);
	iRandomOffsetValueArray[401] = TIntFloat::Convert(0.914);
	iRandomOffsetValueArray[402] = TIntFloat::Convert(0.181);
	iRandomOffsetValueArray[403] = TIntFloat::Convert(0.380);
	iRandomOffsetValueArray[404] = TIntFloat::Convert(0.049);
	iRandomOffsetValueArray[405] = TIntFloat::Convert(0.042);
	iRandomOffsetValueArray[406] = TIntFloat::Convert(0.796);
	iRandomOffsetValueArray[407] = TIntFloat::Convert(0.533);
	iRandomOffsetValueArray[408] = TIntFloat::Convert(0.057);
	iRandomOffsetValueArray[409] = TIntFloat::Convert(0.487);

	iRandomOffsetValueArray[410] = TIntFloat::Convert(0.728);
	iRandomOffsetValueArray[411] = TIntFloat::Convert(0.003);
	iRandomOffsetValueArray[412] = TIntFloat::Convert(0.647);
	iRandomOffsetValueArray[413] = TIntFloat::Convert(0.982);
	iRandomOffsetValueArray[414] = TIntFloat::Convert(0.711);
	iRandomOffsetValueArray[415] = TIntFloat::Convert(0.790);
	iRandomOffsetValueArray[416] = TIntFloat::Convert(0.288);
	iRandomOffsetValueArray[417] = TIntFloat::Convert(0.059);
	iRandomOffsetValueArray[418] = TIntFloat::Convert(0.010);
	iRandomOffsetValueArray[419] = TIntFloat::Convert(0.893);

	iRandomOffsetValueArray[420] = TIntFloat::Convert(0.245);
	iRandomOffsetValueArray[421] = TIntFloat::Convert(0.188);
	iRandomOffsetValueArray[422] = TIntFloat::Convert(0.626);
	iRandomOffsetValueArray[423] = TIntFloat::Convert(0.046);
	iRandomOffsetValueArray[424] = TIntFloat::Convert(0.519);
	iRandomOffsetValueArray[425] = TIntFloat::Convert(0.632);
	iRandomOffsetValueArray[426] = TIntFloat::Convert(0.557);
	iRandomOffsetValueArray[427] = TIntFloat::Convert(0.782);
	iRandomOffsetValueArray[428] = TIntFloat::Convert(0.843);
	iRandomOffsetValueArray[429] = TIntFloat::Convert(0.128);

	iRandomOffsetValueArray[430] = TIntFloat::Convert(0.540);
	iRandomOffsetValueArray[431] = TIntFloat::Convert(0.568);
	iRandomOffsetValueArray[432] = TIntFloat::Convert(0.750);
	iRandomOffsetValueArray[433] = TIntFloat::Convert(0.939);
	iRandomOffsetValueArray[434] = TIntFloat::Convert(0.839);
	iRandomOffsetValueArray[435] = TIntFloat::Convert(0.405);
	iRandomOffsetValueArray[436] = TIntFloat::Convert(0.707);
	iRandomOffsetValueArray[437] = TIntFloat::Convert(0.068);
	iRandomOffsetValueArray[438] = TIntFloat::Convert(0.455);
	iRandomOffsetValueArray[439] = TIntFloat::Convert(0.824);

	iRandomOffsetValueArray[440] = TIntFloat::Convert(0.715);
	iRandomOffsetValueArray[441] = TIntFloat::Convert(0.512);
	iRandomOffsetValueArray[442] = TIntFloat::Convert(0.121);
	iRandomOffsetValueArray[443] = TIntFloat::Convert(0.294);
	iRandomOffsetValueArray[444] = TIntFloat::Convert(0.305);
	iRandomOffsetValueArray[445] = TIntFloat::Convert(0.273);
	iRandomOffsetValueArray[446] = TIntFloat::Convert(0.369);
	iRandomOffsetValueArray[447] = TIntFloat::Convert(0.081);
	iRandomOffsetValueArray[448] = TIntFloat::Convert(0.946);
	iRandomOffsetValueArray[449] = TIntFloat::Convert(0.085);

	iRandomOffsetValueArray[450] = TIntFloat::Convert(0.668);
	iRandomOffsetValueArray[451] = TIntFloat::Convert(0.918);
	iRandomOffsetValueArray[452] = TIntFloat::Convert(0.903);
	iRandomOffsetValueArray[453] = TIntFloat::Convert(0.213);
	iRandomOffsetValueArray[454] = TIntFloat::Convert(0.284);
	iRandomOffsetValueArray[455] = TIntFloat::Convert(0.337);
	iRandomOffsetValueArray[456] = TIntFloat::Convert(0.177);
	iRandomOffsetValueArray[457] = TIntFloat::Convert(0.658);
	iRandomOffsetValueArray[458] = TIntFloat::Convert(0.950);
	iRandomOffsetValueArray[459] = TIntFloat::Convert(0.807);

	iRandomOffsetValueArray[460] = TIntFloat::Convert(0.501);
	iRandomOffsetValueArray[461] = TIntFloat::Convert(0.153);
	iRandomOffsetValueArray[462] = TIntFloat::Convert(0.198);
	iRandomOffsetValueArray[463] = TIntFloat::Convert(0.593);
	iRandomOffsetValueArray[464] = TIntFloat::Convert(0.142);
	iRandomOffsetValueArray[465] = TIntFloat::Convert(0.230);
	iRandomOffsetValueArray[466] = TIntFloat::Convert(0.497);
	iRandomOffsetValueArray[467] = TIntFloat::Convert(0.696);
	iRandomOffsetValueArray[468] = TIntFloat::Convert(0.100);
	iRandomOffsetValueArray[469] = TIntFloat::Convert(0.359);

	iRandomOffsetValueArray[470] = TIntFloat::Convert(0.113);
	iRandomOffsetValueArray[471] = TIntFloat::Convert(0.850);
	iRandomOffsetValueArray[472] = TIntFloat::Convert(0.373);
	iRandomOffsetValueArray[473] = TIntFloat::Convert(0.803);
	iRandomOffsetValueArray[474] = TIntFloat::Convert(0.779);
	iRandomOffsetValueArray[475] = TIntFloat::Convert(0.320);
	iRandomOffsetValueArray[476] = TIntFloat::Convert(0.963);
	iRandomOffsetValueArray[477] = TIntFloat::Convert(0.298);
	iRandomOffsetValueArray[478] = TIntFloat::Convert(0.027);
	iRandomOffsetValueArray[479] = TIntFloat::Convert(0.106);

	iRandomOffsetValueArray[480] = TIntFloat::Convert(0.604);
	iRandomOffsetValueArray[481] = TIntFloat::Convert(0.110);
	iRandomOffsetValueArray[482] = TIntFloat::Convert(0.326);
	iRandomOffsetValueArray[483] = TIntFloat::Convert(0.209);
	iRandomOffsetValueArray[484] = TIntFloat::Convert(0.561);
	iRandomOffsetValueArray[485] = TIntFloat::Convert(0.504);
	iRandomOffsetValueArray[486] = TIntFloat::Convert(0.942);
	iRandomOffsetValueArray[487] = TIntFloat::Convert(0.363);
	iRandomOffsetValueArray[488] = TIntFloat::Convert(0.835);
	iRandomOffsetValueArray[489] = TIntFloat::Convert(0.683);

	iRandomOffsetValueArray[490] = TIntFloat::Convert(0.608);
	iRandomOffsetValueArray[491] = TIntFloat::Convert(0.833);
	iRandomOffsetValueArray[492] = TIntFloat::Convert(0.160);
	iRandomOffsetValueArray[493] = TIntFloat::Convert(0.444);
	iRandomOffsetValueArray[494] = TIntFloat::Convert(0.856);
	iRandomOffsetValueArray[495] = TIntFloat::Convert(0.619);
	iRandomOffsetValueArray[496] = TIntFloat::Convert(0.800);
	iRandomOffsetValueArray[497] = TIntFloat::Convert(0.256);
	iRandomOffsetValueArray[498] = TIntFloat::Convert(0.156);
	iRandomOffsetValueArray[499] = TIntFloat::Convert(0.722);

	iRandomOffsetValueArray[500] = TIntFloat::Convert(0.758);
	iRandomOffsetValueArray[501] = TIntFloat::Convert(0.384);
	iRandomOffsetValueArray[502] = TIntFloat::Convert(0.771);
	iRandomOffsetValueArray[503] = TIntFloat::Convert(0.875);
	iRandomOffsetValueArray[504] = TIntFloat::Convert(0.031);
	iRandomOffsetValueArray[505] = TIntFloat::Convert(0.828);
	iRandomOffsetValueArray[506] = TIntFloat::Convert(0.437);
	iRandomOffsetValueArray[507] = TIntFloat::Convert(0.611);
	iRandomOffsetValueArray[508] = TIntFloat::Convert(0.621);
	iRandomOffsetValueArray[509] = TIntFloat::Convert(0.589);

	iRandomOffsetValueArray[510] = TIntFloat::Convert(0.686);
	iRandomOffsetValueArray[511] = TIntFloat::Convert(0.132);
	iRandomOffsetValueArray[512] = TIntFloat::Convert(0.624);
	iRandomOffsetValueArray[513] = TIntFloat::Convert(0.147);
	iRandomOffsetValueArray[514] = TIntFloat::Convert(0.553);
	iRandomOffsetValueArray[515] = TIntFloat::Convert(0.094);
	iRandomOffsetValueArray[516] = TIntFloat::Convert(0.880);
	iRandomOffsetValueArray[517] = TIntFloat::Convert(0.112);
	iRandomOffsetValueArray[518] = TIntFloat::Convert(0.884);
	iRandomOffsetValueArray[519] = TIntFloat::Convert(0.717);

	iRandomOffsetValueArray[520] = TIntFloat::Convert(0.450);
	iRandomOffsetValueArray[521] = TIntFloat::Convert(0.457);
	iRandomOffsetValueArray[522] = TIntFloat::Convert(0.382);
	iRandomOffsetValueArray[523] = TIntFloat::Convert(0.952);
	iRandomOffsetValueArray[524] = TIntFloat::Convert(0.365);
	iRandomOffsetValueArray[525] = TIntFloat::Convert(0.393);
	iRandomOffsetValueArray[526] = TIntFloat::Convert(0.158);
	iRandomOffsetValueArray[527] = TIntFloat::Convert(0.649);
	iRandomOffsetValueArray[528] = TIntFloat::Convert(0.211);
	iRandomOffsetValueArray[529] = TIntFloat::Convert(0.119);

	iRandomOffsetValueArray[530] = TIntFloat::Convert(0.130);
	iRandomOffsetValueArray[531] = TIntFloat::Convert(0.098);
	iRandomOffsetValueArray[532] = TIntFloat::Convert(0.194);
	iRandomOffsetValueArray[533] = TIntFloat::Convert(0.008);
	iRandomOffsetValueArray[534] = TIntFloat::Convert(0.162);
	iRandomOffsetValueArray[535] = TIntFloat::Convert(0.482);
	iRandomOffsetValueArray[536] = TIntFloat::Convert(0.243);
	iRandomOffsetValueArray[537] = TIntFloat::Convert(0.023);
	iRandomOffsetValueArray[538] = TIntFloat::Convert(0.418);
	iRandomOffsetValueArray[539] = TIntFloat::Convert(0.055);

	iRandomOffsetValueArray[540] = TIntFloat::Convert(0.322);
	iRandomOffsetValueArray[541] = TIntFloat::Convert(0.521);
	iRandomOffsetValueArray[542] = TIntFloat::Convert(0.190);
	iRandomOffsetValueArray[543] = TIntFloat::Convert(0.183);
	iRandomOffsetValueArray[544] = TIntFloat::Convert(0.628);
	iRandomOffsetValueArray[545] = TIntFloat::Convert(0.144);
	iRandomOffsetValueArray[546] = TIntFloat::Convert(0.788);
	iRandomOffsetValueArray[547] = TIntFloat::Convert(0.123);
	iRandomOffsetValueArray[548] = TIntFloat::Convert(0.852);
	iRandomOffsetValueArray[549] = TIntFloat::Convert(0.429);

	iRandomOffsetValueArray[550] = TIntFloat::Convert(0.151);
	iRandomOffsetValueArray[551] = TIntFloat::Convert(0.034);
	iRandomOffsetValueArray[552] = TIntFloat::Convert(0.386);
	iRandomOffsetValueArray[553] = TIntFloat::Convert(0.329);
	iRandomOffsetValueArray[554] = TIntFloat::Convert(0.187);
	iRandomOffsetValueArray[555] = TIntFloat::Convert(0.660);
	iRandomOffsetValueArray[556] = TIntFloat::Convert(0.773);
	iRandomOffsetValueArray[557] = TIntFloat::Convert(0.923);
	iRandomOffsetValueArray[558] = TIntFloat::Convert(0.984);
	iRandomOffsetValueArray[559] = TIntFloat::Convert(0.269);

	iRandomOffsetValueArray[560] = TIntFloat::Convert(0.681);
	iRandomOffsetValueArray[561] = TIntFloat::Convert(0.709);
	iRandomOffsetValueArray[562] = TIntFloat::Convert(0.891);
	iRandomOffsetValueArray[563] = TIntFloat::Convert(0.080);
	iRandomOffsetValueArray[564] = TIntFloat::Convert(0.980);
	iRandomOffsetValueArray[565] = TIntFloat::Convert(0.848);
	iRandomOffsetValueArray[566] = TIntFloat::Convert(0.208);
	iRandomOffsetValueArray[567] = TIntFloat::Convert(0.596);
	iRandomOffsetValueArray[568] = TIntFloat::Convert(0.653);
	iRandomOffsetValueArray[569] = TIntFloat::Convert(0.446);

	iRandomOffsetValueArray[570] = TIntFloat::Convert(0.414);
	iRandomOffsetValueArray[571] = TIntFloat::Convert(0.510);
	iRandomOffsetValueArray[572] = TIntFloat::Convert(0.222);
	iRandomOffsetValueArray[573] = TIntFloat::Convert(0.087);
	iRandomOffsetValueArray[574] = TIntFloat::Convert(0.226);
	iRandomOffsetValueArray[575] = TIntFloat::Convert(0.354);
	iRandomOffsetValueArray[576] = TIntFloat::Convert(0.425);
	iRandomOffsetValueArray[577] = TIntFloat::Convert(0.478);
	iRandomOffsetValueArray[578] = TIntFloat::Convert(0.318);
	iRandomOffsetValueArray[579] = TIntFloat::Convert(0.799);

	iRandomOffsetValueArray[580] = TIntFloat::Convert(0.948);
	iRandomOffsetValueArray[581] = TIntFloat::Convert(0.283);
	iRandomOffsetValueArray[582] = TIntFloat::Convert(0.254);
	iRandomOffsetValueArray[583] = TIntFloat::Convert(0.991);
	iRandomOffsetValueArray[584] = TIntFloat::Convert(0.944);
	iRandomOffsetValueArray[585] = TIntFloat::Convert(0.920);
	iRandomOffsetValueArray[586] = TIntFloat::Convert(0.745);
	iRandomOffsetValueArray[587] = TIntFloat::Convert(0.251);
	iRandomOffsetValueArray[588] = TIntFloat::Convert(0.350);
	iRandomOffsetValueArray[589] = TIntFloat::Convert(0.645);

	iRandomOffsetValueArray[590] = TIntFloat::Convert(0.083);
	iRandomOffsetValueArray[591] = TIntFloat::Convert(0.976);
	iRandomOffsetValueArray[592] = TIntFloat::Convert(0.973);
	iRandomOffsetValueArray[593] = TIntFloat::Convert(0.301);
	iRandomOffsetValueArray[594] = TIntFloat::Convert(0.585);
	iRandomOffsetValueArray[595] = TIntFloat::Convert(0.941);
	iRandomOffsetValueArray[596] = TIntFloat::Convert(0.397);
	iRandomOffsetValueArray[597] = TIntFloat::Convert(0.297);
	iRandomOffsetValueArray[598] = TIntFloat::Convert(0.863);
	iRandomOffsetValueArray[599] = TIntFloat::Convert(0.912);

	iRandomOffsetValueArray[600] = TIntFloat::Convert(0.016);
	iRandomOffsetValueArray[601] = TIntFloat::Convert(0.752);
	iRandomOffsetValueArray[602] = TIntFloat::Convert(0.827);
	iRandomOffsetValueArray[603] = TIntFloat::Convert(0.126);
	iRandomOffsetValueArray[604] = TIntFloat::Convert(0.361);
	iRandomOffsetValueArray[605] = TIntFloat::Convert(0.741);
	iRandomOffsetValueArray[606] = TIntFloat::Convert(0.115);
	iRandomOffsetValueArray[607] = TIntFloat::Convert(0.959);
	iRandomOffsetValueArray[608] = TIntFloat::Convert(0.656);
	iRandomOffsetValueArray[609] = TIntFloat::Convert(0.051);

	iRandomOffsetValueArray[610] = TIntFloat::Convert(0.688);
	iRandomOffsetValueArray[611] = TIntFloat::Convert(0.955);
	iRandomOffsetValueArray[612] = TIntFloat::Convert(0.888);
	iRandomOffsetValueArray[613] = TIntFloat::Convert(0.816);
	iRandomOffsetValueArray[614] = TIntFloat::Convert(0.831);
	iRandomOffsetValueArray[615] = TIntFloat::Convert(0.155);
	iRandomOffsetValueArray[616] = TIntFloat::Convert(0.756);
	iRandomOffsetValueArray[617] = TIntFloat::Convert(0.219);
	iRandomOffsetValueArray[618] = TIntFloat::Convert(0.564);
	iRandomOffsetValueArray[619] = TIntFloat::Convert(0.062);

	iRandomOffsetValueArray[620] = TIntFloat::Convert(0.784);
	iRandomOffsetValueArray[621] = TIntFloat::Convert(0.019);
	iRandomOffsetValueArray[622] = TIntFloat::Convert(0.820);
	iRandomOffsetValueArray[623] = TIntFloat::Convert(0.066);
	iRandomOffsetValueArray[624] = TIntFloat::Convert(0.290);
	iRandomOffsetValueArray[625] = TIntFloat::Convert(0.617);
	iRandomOffsetValueArray[626] = TIntFloat::Convert(0.048);
	iRandomOffsetValueArray[627] = TIntFloat::Convert(0.258);
	iRandomOffsetValueArray[628] = TIntFloat::Convert(0.713);
	iRandomOffsetValueArray[629] = TIntFloat::Convert(0.613);

	iRandomOffsetValueArray[630] = TIntFloat::Convert(0.179);
	iRandomOffsetValueArray[631] = TIntFloat::Convert(0.215);
	iRandomOffsetValueArray[632] = TIntFloat::Convert(0.333);
	iRandomOffsetValueArray[633] = TIntFloat::Convert(0.489);
	iRandomOffsetValueArray[634] = TIntFloat::Convert(0.286);
	iRandomOffsetValueArray[635] = TIntFloat::Convert(0.895);
	iRandomOffsetValueArray[636] = TIntFloat::Convert(0.720);
	iRandomOffsetValueArray[637] = TIntFloat::Convert(0.859);
	iRandomOffsetValueArray[638] = TIntFloat::Convert(0.176);
	iRandomOffsetValueArray[639] = TIntFloat::Convert(0.692);

	iRandomOffsetValueArray[640] = TIntFloat::Convert(0.677);
	iRandomOffsetValueArray[641] = TIntFloat::Convert(0.987);
	iRandomOffsetValueArray[642] = TIntFloat::Convert(0.685);
	iRandomOffsetValueArray[643] = TIntFloat::Convert(0.724);
	iRandomOffsetValueArray[644] = TIntFloat::Convert(0.315);
	iRandomOffsetValueArray[645] = TIntFloat::Convert(0.927);
	iRandomOffsetValueArray[646] = TIntFloat::Convert(0.916);
	iRandomOffsetValueArray[647] = TIntFloat::Convert(0.776);
	iRandomOffsetValueArray[648] = TIntFloat::Convert(0.075);
	iRandomOffsetValueArray[649] = TIntFloat::Convert(0.325);

	iRandomOffsetValueArray[650] = TIntFloat::Convert(0.310);
	iRandomOffsetValueArray[651] = TIntFloat::Convert(0.064);
	iRandomOffsetValueArray[652] = TIntFloat::Convert(0.357);
	iRandomOffsetValueArray[653] = TIntFloat::Convert(0.908);
	iRandomOffsetValueArray[654] = TIntFloat::Convert(0.605);
	iRandomOffsetValueArray[655] = TIntFloat::Convert(0.000);
	iRandomOffsetValueArray[656] = TIntFloat::Convert(0.549);
	iRandomOffsetValueArray[657] = TIntFloat::Convert(0.637);
	iRandomOffsetValueArray[658] = TIntFloat::Convert(0.904);
	iRandomOffsetValueArray[659] = TIntFloat::Convert(0.103);

	iRandomOffsetValueArray[660] = TIntFloat::Convert(0.506);
	iRandomOffsetValueArray[661] = TIntFloat::Convert(0.765);
	iRandomOffsetValueArray[662] = TIntFloat::Convert(0.780);
	iRandomOffsetValueArray[663] = TIntFloat::Convert(0.186);
	iRandomOffsetValueArray[664] = TIntFloat::Convert(0.370);
	iRandomOffsetValueArray[665] = TIntFloat::Convert(0.705);
	iRandomOffsetValueArray[666] = TIntFloat::Convert(0.434);
	iRandomOffsetValueArray[667] = TIntFloat::Convert(0.513);
	iRandomOffsetValueArray[668] = TIntFloat::Convert(0.011);
	iRandomOffsetValueArray[669] = TIntFloat::Convert(0.517);

	iRandomOffsetValueArray[670] = TIntFloat::Convert(0.733);
	iRandomOffsetValueArray[671] = TIntFloat::Convert(0.968);
	iRandomOffsetValueArray[672] = TIntFloat::Convert(0.769);
	iRandomOffsetValueArray[673] = TIntFloat::Convert(0.090);
	iRandomOffsetValueArray[674] = TIntFloat::Convert(0.015);
	iRandomOffsetValueArray[675] = TIntFloat::Convert(0.239);
	iRandomOffsetValueArray[676] = TIntFloat::Convert(0.566);
	iRandomOffsetValueArray[677] = TIntFloat::Convert(0.263);
	iRandomOffsetValueArray[678] = TIntFloat::Convert(0.207);
	iRandomOffsetValueArray[679] = TIntFloat::Convert(0.562);

	iRandomOffsetValueArray[680] = TIntFloat::Convert(0.282);
	iRandomOffsetValueArray[681] = TIntFloat::Convert(0.438);
	iRandomOffsetValueArray[682] = TIntFloat::Convert(0.235);
	iRandomOffsetValueArray[683] = TIntFloat::Convert(0.844);
	iRandomOffsetValueArray[684] = TIntFloat::Convert(0.028);
	iRandomOffsetValueArray[685] = TIntFloat::Convert(0.996);
	iRandomOffsetValueArray[686] = TIntFloat::Convert(0.092);
	iRandomOffsetValueArray[687] = TIntFloat::Convert(0.538);
	iRandomOffsetValueArray[688] = TIntFloat::Convert(0.669);
	iRandomOffsetValueArray[689] = TIntFloat::Convert(0.808);

	iRandomOffsetValueArray[690] = TIntFloat::Convert(0.641);
	iRandomOffsetValueArray[691] = TIntFloat::Convert(0.936);
	iRandomOffsetValueArray[692] = TIntFloat::Convert(0.060);
	iRandomOffsetValueArray[693] = TIntFloat::Convert(0.900);
	iRandomOffsetValueArray[694] = TIntFloat::Convert(0.673);
	iRandomOffsetValueArray[695] = TIntFloat::Convert(0.530);
	iRandomOffsetValueArray[696] = TIntFloat::Convert(0.876);
	iRandomOffsetValueArray[697] = TIntFloat::Convert(0.154);
	iRandomOffsetValueArray[698] = TIntFloat::Convert(0.573);
	iRandomOffsetValueArray[699] = TIntFloat::Convert(0.502);

	iRandomOffsetValueArray[700] = TIntFloat::Convert(0.043);
	iRandomOffsetValueArray[701] = TIntFloat::Convert(0.421);
	iRandomOffsetValueArray[702] = TIntFloat::Convert(0.485);
	iRandomOffsetValueArray[703] = TIntFloat::Convert(0.829);
	iRandomOffsetValueArray[704] = TIntFloat::Convert(0.327);
	iRandomOffsetValueArray[705] = TIntFloat::Convert(0.932);
	iRandomOffsetValueArray[706] = TIntFloat::Convert(0.406);
	iRandomOffsetValueArray[707] = TIntFloat::Convert(0.167);
	iRandomOffsetValueArray[708] = TIntFloat::Convert(0.314);
	iRandomOffsetValueArray[709] = TIntFloat::Convert(0.342);

	iRandomOffsetValueArray[710] = TIntFloat::Convert(0.107);
	iRandomOffsetValueArray[711] = TIntFloat::Convert(0.079);
	iRandomOffsetValueArray[712] = TIntFloat::Convert(0.047);
	iRandomOffsetValueArray[713] = TIntFloat::Convert(0.143);
	iRandomOffsetValueArray[714] = TIntFloat::Convert(0.442);
	iRandomOffsetValueArray[715] = TIntFloat::Convert(0.111);
	iRandomOffsetValueArray[716] = TIntFloat::Convert(0.951);
	iRandomOffsetValueArray[717] = TIntFloat::Convert(0.581);
	iRandomOffsetValueArray[718] = TIntFloat::Convert(0.541);
	iRandomOffsetValueArray[719] = TIntFloat::Convert(0.972);

	iRandomOffsetValueArray[720] = TIntFloat::Convert(0.004);
	iRandomOffsetValueArray[721] = TIntFloat::Convert(0.271);
	iRandomOffsetValueArray[722] = TIntFloat::Convert(0.470);
	iRandomOffsetValueArray[723] = TIntFloat::Convert(0.139);
	iRandomOffsetValueArray[724] = TIntFloat::Convert(0.577);
	iRandomOffsetValueArray[725] = TIntFloat::Convert(0.737);
	iRandomOffsetValueArray[726] = TIntFloat::Convert(0.338);
	iRandomOffsetValueArray[727] = TIntFloat::Convert(0.801);
	iRandomOffsetValueArray[728] = TIntFloat::Convert(0.378);
	iRandomOffsetValueArray[729] = TIntFloat::Convert(0.983);

	iRandomOffsetValueArray[730] = TIntFloat::Convert(0.278);
	iRandomOffsetValueArray[731] = TIntFloat::Convert(0.402);
	iRandomOffsetValueArray[732] = TIntFloat::Convert(0.609);
	iRandomOffsetValueArray[733] = TIntFloat::Convert(0.872);
	iRandomOffsetValueArray[734] = TIntFloat::Convert(0.199);
	iRandomOffsetValueArray[735] = TIntFloat::Convert(0.218);
	iRandomOffsetValueArray[736] = TIntFloat::Convert(0.840);
	iRandomOffsetValueArray[737] = TIntFloat::Convert(0.295);
	iRandomOffsetValueArray[738] = TIntFloat::Convert(0.797);
	iRandomOffsetValueArray[739] = TIntFloat::Convert(0.545);

	iRandomOffsetValueArray[740] = TIntFloat::Convert(0.071);
	iRandomOffsetValueArray[741] = TIntFloat::Convert(0.868);
	iRandomOffsetValueArray[742] = TIntFloat::Convert(0.171);
	iRandomOffsetValueArray[743] = TIntFloat::Convert(0.036);
	iRandomOffsetValueArray[744] = TIntFloat::Convert(0.175);
	iRandomOffsetValueArray[745] = TIntFloat::Convert(0.274);
	iRandomOffsetValueArray[746] = TIntFloat::Convert(0.374);
	iRandomOffsetValueArray[747] = TIntFloat::Convert(0.267);
	iRandomOffsetValueArray[748] = TIntFloat::Convert(0.748);
	iRandomOffsetValueArray[749] = TIntFloat::Convert(0.306);

	iRandomOffsetValueArray[750] = TIntFloat::Convert(0.592);
	iRandomOffsetValueArray[751] = TIntFloat::Convert(0.498);
	iRandomOffsetValueArray[752] = TIntFloat::Convert(0.203);
	iRandomOffsetValueArray[753] = TIntFloat::Convert(0.940);
	iRandomOffsetValueArray[754] = TIntFloat::Convert(0.135);
	iRandomOffsetValueArray[755] = TIntFloat::Convert(0.410);
	iRandomOffsetValueArray[756] = TIntFloat::Convert(0.389);
	iRandomOffsetValueArray[757] = TIntFloat::Convert(0.466);
	iRandomOffsetValueArray[758] = TIntFloat::Convert(0.299);
	iRandomOffsetValueArray[759] = TIntFloat::Convert(0.594);

	iRandomOffsetValueArray[760] = TIntFloat::Convert(0.032);
	iRandomOffsetValueArray[761] = TIntFloat::Convert(0.453);
	iRandomOffsetValueArray[762] = TIntFloat::Convert(0.039);
	iRandomOffsetValueArray[763] = TIntFloat::Convert(0.964);
	iRandomOffsetValueArray[764] = TIntFloat::Convert(0.250);
	iRandomOffsetValueArray[765] = TIntFloat::Convert(0.534);
	iRandomOffsetValueArray[766] = TIntFloat::Convert(0.346);
	iRandomOffsetValueArray[767] = TIntFloat::Convert(0.246);
	iRandomOffsetValueArray[768] = TIntFloat::Convert(0.812);
	iRandomOffsetValueArray[769] = TIntFloat::Convert(0.474);

	iRandomOffsetValueArray[770] = TIntFloat::Convert(0.231);
	iRandomOffsetValueArray[771] = TIntFloat::Convert(0.122);
	iRandomOffsetValueArray[772] = TIntFloat::Convert(0.701);
	iRandomOffsetValueArray[773] = TIntFloat::Convert(0.607);
	iRandomOffsetValueArray[774] = TIntFloat::Convert(0.789);
	iRandomOffsetValueArray[775] = TIntFloat::Convert(0.244);
	iRandomOffsetValueArray[776] = TIntFloat::Convert(0.710);
	iRandomOffsetValueArray[777] = TIntFloat::Convert(0.746);
	iRandomOffsetValueArray[778] = TIntFloat::Convert(0.372);
	iRandomOffsetValueArray[779] = TIntFloat::Convert(0.494);

	iRandomOffsetValueArray[780] = TIntFloat::Convert(0.864);
	iRandomOffsetValueArray[781] = TIntFloat::Convert(0.020);
	iRandomOffsetValueArray[782] = TIntFloat::Convert(0.817);
	iRandomOffsetValueArray[783] = TIntFloat::Convert(0.426);
	iRandomOffsetValueArray[784] = TIntFloat::Convert(0.334);
	iRandomOffsetValueArray[785] = TIntFloat::Convert(0.344);
	iRandomOffsetValueArray[786] = TIntFloat::Convert(0.312);
	iRandomOffsetValueArray[787] = TIntFloat::Convert(0.120);
	iRandomOffsetValueArray[788] = TIntFloat::Convert(0.390);
	iRandomOffsetValueArray[789] = TIntFloat::Convert(0.223);

	iRandomOffsetValueArray[790] = TIntFloat::Convert(0.518);
	iRandomOffsetValueArray[791] = TIntFloat::Convert(0.323);
	iRandomOffsetValueArray[792] = TIntFloat::Convert(0.376);
	iRandomOffsetValueArray[793] = TIntFloat::Convert(0.216);
	iRandomOffsetValueArray[794] = TIntFloat::Convert(0.697);
	iRandomOffsetValueArray[795] = TIntFloat::Convert(0.255);
	iRandomOffsetValueArray[796] = TIntFloat::Convert(0.806);
	iRandomOffsetValueArray[797] = TIntFloat::Convert(0.458);
	iRandomOffsetValueArray[798] = TIntFloat::Convert(0.633);
	iRandomOffsetValueArray[799] = TIntFloat::Convert(0.447);

	iRandomOffsetValueArray[800] = TIntFloat::Convert(0.404);
	iRandomOffsetValueArray[801] = TIntFloat::Convert(0.398);
	iRandomOffsetValueArray[802] = TIntFloat::Convert(0.152);
	iRandomOffsetValueArray[803] = TIntFloat::Convert(0.678);
	iRandomOffsetValueArray[804] = TIntFloat::Convert(0.842);
	iRandomOffsetValueArray[805] = TIntFloat::Convert(0.084);
	iRandomOffsetValueArray[806] = TIntFloat::Convert(0.625);
	iRandomOffsetValueArray[807] = TIntFloat::Convert(0.603);
	iRandomOffsetValueArray[808] = TIntFloat::Convert(0.067);
	iRandomOffsetValueArray[809] = TIntFloat::Convert(0.411);

	iRandomOffsetValueArray[810] = TIntFloat::Convert(0.415);
	iRandomOffsetValueArray[811] = TIntFloat::Convert(0.366);
	iRandomOffsetValueArray[812] = TIntFloat::Convert(0.248);
	iRandomOffsetValueArray[813] = TIntFloat::Convert(0.601);
	iRandomOffsetValueArray[814] = TIntFloat::Convert(0.543);
	iRandomOffsetValueArray[815] = TIntFloat::Convert(0.981);
	iRandomOffsetValueArray[816] = TIntFloat::Convert(0.667);
	iRandomOffsetValueArray[817] = TIntFloat::Convert(0.874);
	iRandomOffsetValueArray[818] = TIntFloat::Convert(0.988);
	iRandomOffsetValueArray[819] = TIntFloat::Convert(0.913);

	iRandomOffsetValueArray[820] = TIntFloat::Convert(0.137);
	iRandomOffsetValueArray[821] = TIntFloat::Convert(0.464);
	iRandomOffsetValueArray[822] = TIntFloat::Convert(0.896);
	iRandomOffsetValueArray[823] = TIntFloat::Convert(0.924);
	iRandomOffsetValueArray[824] = TIntFloat::Convert(0.105);
	iRandomOffsetValueArray[825] = TIntFloat::Convert(0.195);
	iRandomOffsetValueArray[826] = TIntFloat::Convert(0.761);
	iRandomOffsetValueArray[827] = TIntFloat::Convert(0.689);
	iRandomOffsetValueArray[828] = TIntFloat::Convert(0.810);
	iRandomOffsetValueArray[829] = TIntFloat::Convert(0.180);

	iRandomOffsetValueArray[830] = TIntFloat::Convert(0.336);
	iRandomOffsetValueArray[831] = TIntFloat::Convert(0.133);
	iRandomOffsetValueArray[832] = TIntFloat::Convert(0.742);
	iRandomOffsetValueArray[833] = TIntFloat::Convert(0.650);
	iRandomOffsetValueArray[834] = TIntFloat::Convert(0.661);
	iRandomOffsetValueArray[835] = TIntFloat::Convert(0.629);
	iRandomOffsetValueArray[836] = TIntFloat::Convert(0.725);
	iRandomOffsetValueArray[837] = TIntFloat::Convert(0.436);
	iRandomOffsetValueArray[838] = TIntFloat::Convert(0.302);
	iRandomOffsetValueArray[839] = TIntFloat::Convert(0.024);

	iRandomOffsetValueArray[840] = TIntFloat::Convert(0.539);
	iRandomOffsetValueArray[841] = TIntFloat::Convert(0.259);
	iRandomOffsetValueArray[842] = TIntFloat::Convert(0.569);
	iRandomOffsetValueArray[843] = TIntFloat::Convert(0.639);
	iRandomOffsetValueArray[844] = TIntFloat::Convert(0.693);
	iRandomOffsetValueArray[845] = TIntFloat::Convert(0.013);
	iRandomOffsetValueArray[846] = TIntFloat::Convert(0.571);
	iRandomOffsetValueArray[847] = TIntFloat::Convert(0.163);
	iRandomOffsetValueArray[848] = TIntFloat::Convert(0.857);
	iRandomOffsetValueArray[849] = TIntFloat::Convert(0.774);

	iRandomOffsetValueArray[850] = TIntFloat::Convert(0.554);
	iRandomOffsetValueArray[851] = TIntFloat::Convert(0.949);
	iRandomOffsetValueArray[852] = TIntFloat::Convert(0.586);
	iRandomOffsetValueArray[853] = TIntFloat::Convert(0.853);
	iRandomOffsetValueArray[854] = TIntFloat::Convert(0.052);
	iRandomOffsetValueArray[855] = TIntFloat::Convert(0.721);
	iRandomOffsetValueArray[856] = TIntFloat::Convert(0.714);
	iRandomOffsetValueArray[857] = TIntFloat::Convert(0.729);
	iRandomOffsetValueArray[858] = TIntFloat::Convert(0.159);
	iRandomOffsetValueArray[859] = TIntFloat::Convert(0.400);

	iRandomOffsetValueArray[860] = TIntFloat::Convert(0.319);
	iRandomOffsetValueArray[861] = TIntFloat::Convert(0.383);
	iRandomOffsetValueArray[862] = TIntFloat::Convert(0.462);
	iRandomOffsetValueArray[863] = TIntFloat::Convert(0.960);
	iRandomOffsetValueArray[864] = TIntFloat::Convert(0.682);
	iRandomOffsetValueArray[865] = TIntFloat::Convert(0.917);
	iRandomOffsetValueArray[866] = TIntFloat::Convert(0.860);
	iRandomOffsetValueArray[867] = TIntFloat::Convert(0.191);
	iRandomOffsetValueArray[868] = TIntFloat::Convert(0.304);
	iRandomOffsetValueArray[869] = TIntFloat::Convert(0.454);

	iRandomOffsetValueArray[870] = TIntFloat::Convert(0.212);
	iRandomOffsetValueArray[871] = TIntFloat::Convert(0.240);
	iRandomOffsetValueArray[872] = TIntFloat::Convert(0.422);
	iRandomOffsetValueArray[873] = TIntFloat::Convert(0.511);
	iRandomOffsetValueArray[874] = TIntFloat::Convert(0.077);
	iRandomOffsetValueArray[875] = TIntFloat::Convert(0.379);
	iRandomOffsetValueArray[876] = TIntFloat::Convert(0.127);
	iRandomOffsetValueArray[877] = TIntFloat::Convert(0.387);
	iRandomOffsetValueArray[878] = TIntFloat::Convert(0.184);
	iRandomOffsetValueArray[879] = TIntFloat::Convert(0.793);

	iRandomOffsetValueArray[880] = TIntFloat::Convert(0.977);
	iRandomOffsetValueArray[881] = TIntFloat::Convert(0.945);
	iRandomOffsetValueArray[882] = TIntFloat::Convert(0.041);
	iRandomOffsetValueArray[883] = TIntFloat::Convert(0.753);
	iRandomOffsetValueArray[884] = TIntFloat::Convert(0.618);
	iRandomOffsetValueArray[885] = TIntFloat::Convert(0.757);
	iRandomOffsetValueArray[886] = TIntFloat::Convert(0.340);
	iRandomOffsetValueArray[887] = TIntFloat::Convert(0.590);
	iRandomOffsetValueArray[888] = TIntFloat::Convert(0.575);
	iRandomOffsetValueArray[889] = TIntFloat::Convert(0.885);

	iRandomOffsetValueArray[890] = TIntFloat::Convert(0.956);
	iRandomOffsetValueArray[891] = TIntFloat::Convert(0.009);
	iRandomOffsetValueArray[892] = TIntFloat::Convert(0.849);
	iRandomOffsetValueArray[893] = TIntFloat::Convert(0.622);
	iRandomOffsetValueArray[894] = TIntFloat::Convert(0.479);
	iRandomOffsetValueArray[895] = TIntFloat::Convert(0.825);
	iRandomOffsetValueArray[896] = TIntFloat::Convert(0.870);
	iRandomOffsetValueArray[897] = TIntFloat::Convert(0.902);
	iRandomOffsetValueArray[898] = TIntFloat::Convert(0.169);
	iRandomOffsetValueArray[899] = TIntFloat::Convert(0.368);

	iRandomOffsetValueArray[900] = TIntFloat::Convert(0.785);
	iRandomOffsetValueArray[901] = TIntFloat::Convert(0.522);
	iRandomOffsetValueArray[902] = TIntFloat::Convert(0.045);
	iRandomOffsetValueArray[903] = TIntFloat::Convert(0.475);
	iRandomOffsetValueArray[904] = TIntFloat::Convert(0.451);
	iRandomOffsetValueArray[905] = TIntFloat::Convert(0.992);
	iRandomOffsetValueArray[906] = TIntFloat::Convert(0.635);
	iRandomOffsetValueArray[907] = TIntFloat::Convert(0.699);
	iRandomOffsetValueArray[908] = TIntFloat::Convert(0.778);
	iRandomOffsetValueArray[909] = TIntFloat::Convert(0.276);

	iRandomOffsetValueArray[910] = TIntFloat::Convert(0.881);
	iRandomOffsetValueArray[911] = TIntFloat::Convert(0.614);
	iRandomOffsetValueArray[912] = TIntFloat::Convert(0.507);
	iRandomOffsetValueArray[913] = TIntFloat::Convert(0.355);
	iRandomOffsetValueArray[914] = TIntFloat::Convert(0.280);
	iRandomOffsetValueArray[915] = TIntFloat::Convert(0.832);
	iRandomOffsetValueArray[916] = TIntFloat::Convert(0.116);
	iRandomOffsetValueArray[917] = TIntFloat::Convert(0.291);
	iRandomOffsetValueArray[918] = TIntFloat::Convert(0.928);
	iRandomOffsetValueArray[919] = TIntFloat::Convert(0.394);

	iRandomOffsetValueArray[920] = TIntFloat::Convert(0.430);
	iRandomOffsetValueArray[921] = TIntFloat::Convert(0.056);
	iRandomOffsetValueArray[922] = TIntFloat::Convert(0.443);
	iRandomOffsetValueArray[923] = TIntFloat::Convert(0.109);
	iRandomOffsetValueArray[924] = TIntFloat::Convert(0.358);
	iRandomOffsetValueArray[925] = TIntFloat::Convert(0.804);
	iRandomOffsetValueArray[926] = TIntFloat::Convert(0.934);
	iRandomOffsetValueArray[927] = TIntFloat::Convert(0.073);
	iRandomOffsetValueArray[928] = TIntFloat::Convert(0.657);
	iRandomOffsetValueArray[929] = TIntFloat::Convert(0.906);

	iRandomOffsetValueArray[930] = TIntFloat::Convert(0.892);
	iRandomOffsetValueArray[931] = TIntFloat::Convert(0.272);
	iRandomOffsetValueArray[932] = TIntFloat::Convert(0.165);
	iRandomOffsetValueArray[933] = TIntFloat::Convert(0.646);
	iRandomOffsetValueArray[934] = TIntFloat::Convert(0.490);
	iRandomOffsetValueArray[935] = TIntFloat::Convert(0.141);
	iRandomOffsetValueArray[936] = TIntFloat::Convert(0.582);
	iRandomOffsetValueArray[937] = TIntFloat::Convert(0.131);
	iRandomOffsetValueArray[938] = TIntFloat::Convert(0.486);
	iRandomOffsetValueArray[939] = TIntFloat::Convert(0.419);

	iRandomOffsetValueArray[940] = TIntFloat::Convert(0.088);
	iRandomOffsetValueArray[941] = TIntFloat::Convert(0.347);
	iRandomOffsetValueArray[942] = TIntFloat::Convert(0.836);
	iRandomOffsetValueArray[943] = TIntFloat::Convert(0.838);
	iRandomOffsetValueArray[944] = TIntFloat::Convert(0.362);
	iRandomOffsetValueArray[945] = TIntFloat::Convert(0.526);
	iRandomOffsetValueArray[946] = TIntFloat::Convert(0.308);
	iRandomOffsetValueArray[947] = TIntFloat::Convert(0.287);
	iRandomOffsetValueArray[948] = TIntFloat::Convert(0.095);
	iRandomOffsetValueArray[949] = TIntFloat::Convert(0.099);

	iRandomOffsetValueArray[950] = TIntFloat::Convert(0.197);
	iRandomOffsetValueArray[951] = TIntFloat::Convert(0.550);
	iRandomOffsetValueArray[952] = TIntFloat::Convert(0.227);
	iRandomOffsetValueArray[953] = TIntFloat::Convert(0.665);
	iRandomOffsetValueArray[954] = TIntFloat::Convert(0.351);
	iRandomOffsetValueArray[955] = TIntFloat::Convert(0.558);
	iRandomOffsetValueArray[956] = TIntFloat::Convert(0.671);
	iRandomOffsetValueArray[957] = TIntFloat::Convert(0.597);
	iRandomOffsetValueArray[958] = TIntFloat::Convert(0.821);
	iRandomOffsetValueArray[959] = TIntFloat::Convert(0.148);

	iRandomOffsetValueArray[960] = TIntFloat::Convert(0.432);
	iRandomOffsetValueArray[961] = TIntFloat::Convert(0.037);
	iRandomOffsetValueArray[962] = TIntFloat::Convert(0.069);
	iRandomOffsetValueArray[963] = TIntFloat::Convert(0.101);
	iRandomOffsetValueArray[964] = TIntFloat::Convert(0.930);
	iRandomOffsetValueArray[965] = TIntFloat::Convert(0.962);
	iRandomOffsetValueArray[966] = TIntFloat::Convert(0.005);
	iRandomOffsetValueArray[967] = TIntFloat::Convert(0.440);
	iRandomOffsetValueArray[968] = TIntFloat::Convert(0.767);
	iRandomOffsetValueArray[969] = TIntFloat::Convert(0.265);

	iRandomOffsetValueArray[970] = TIntFloat::Convert(0.408);
	iRandomOffsetValueArray[971] = TIntFloat::Convert(0.233);
	iRandomOffsetValueArray[972] = TIntFloat::Convert(0.735);
	iRandomOffsetValueArray[973] = TIntFloat::Convert(0.205);
	iRandomOffsetValueArray[974] = TIntFloat::Convert(0.237);
	iRandomOffsetValueArray[975] = TIntFloat::Convert(0.532);
	iRandomOffsetValueArray[976] = TIntFloat::Convert(0.970);
	iRandomOffsetValueArray[977] = TIntFloat::Convert(0.002);
	iRandomOffsetValueArray[978] = TIntFloat::Convert(0.938);
	iRandomOffsetValueArray[979] = TIntFloat::Convert(0.703);

	iRandomOffsetValueArray[980] = TIntFloat::Convert(0.201);
	iRandomOffsetValueArray[981] = TIntFloat::Convert(0.500);
	iRandomOffsetValueArray[982] = TIntFloat::Convert(0.173);
	iRandomOffsetValueArray[983] = TIntFloat::Convert(0.966);
	iRandomOffsetValueArray[984] = TIntFloat::Convert(0.030);
	iRandomOffsetValueArray[985] = TIntFloat::Convert(0.998);
	iRandomOffsetValueArray[986] = TIntFloat::Convert(0.468);
	iRandomOffsetValueArray[987] = TIntFloat::Convert(0.229);
	iRandomOffsetValueArray[988] = TIntFloat::Convert(0.994);
	iRandomOffsetValueArray[989] = TIntFloat::Convert(0.026);

	iRandomOffsetValueArray[990] = TIntFloat::Convert(0.496);
	iRandomOffsetValueArray[991] = TIntFloat::Convert(0.058);
	iRandomOffsetValueArray[992] = TIntFloat::Convert(0.261);
	iRandomOffsetValueArray[993] = TIntFloat::Convert(0.795);
	iRandomOffsetValueArray[994] = TIntFloat::Convert(0.731);
	iRandomOffsetValueArray[995] = TIntFloat::Convert(0.528);
	iRandomOffsetValueArray[996] = TIntFloat::Convert(0.560);
	iRandomOffsetValueArray[997] = TIntFloat::Convert(0.763);
	iRandomOffsetValueArray[998] = TIntFloat::Convert(0.293);
	iRandomOffsetValueArray[999] = TIntFloat::Convert(0.326);

}


//---------------------------- functions ---------------------//

TInt CRandomGenerator::GetRandomNumberTInt(TInt aLowerBound, TInt aUpperBound)
{
  //find what extra value we want to add to the LowerBound, will never exceed Upperbound
  TInt lReturnValue = (iRandomOffsetValueArray[iIndex] * (aUpperBound-aLowerBound)).GetIntInBaseInt();
 //check if we need to go back to the front of the array
  iIndex++;
  if(iIndex == CRANDOM_GENERATOR_NUMBER_OF_RANDOM_NUMBERS)
  	iIndex=0;

  //add LowerBound Value, and convert from TIntFloat format to TINt
  lReturnValue = lReturnValue + aLowerBound;
  return lReturnValue;
}

TIntFloat CRandomGenerator::GetRandomNumberTIntFloat(TInt aLowerBound, TInt aUpperBound)
{
  //find what extra value we want to add to the LowerBound, will never exceed Upperbound
  TIntFloat lReturnValue = iRandomOffsetValueArray[iIndex] * (aUpperBound-aLowerBound);
  //check if we need to go back to the front of the array
  iIndex++;
  if(iIndex == CRANDOM_GENERATOR_NUMBER_OF_RANDOM_NUMBERS)
  	iIndex=0;

  //add LowerBound Value, and convert lowerBound to TIntFloat format
  lReturnValue = lReturnValue + aLowerBound;
  return lReturnValue;
}

