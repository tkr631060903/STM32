#include "includes.h"

Gui_Control_Type_Def GuiCtrl[90]={

{0,	TYPE_UNDEFINED,	NULL,	NULL,	0,	0,	0,	0,	0,	0,	0x0000,	0x0000,	NULL,	"---"},

{1,	TYPE_PANEL,	PANEL,	NULL,	0,	0,	800,	480,	0,	1,	VAL_BLACK,	0xE5E4,	Panel31Callback,	NULL,"Panel31"},

{2,	TYPE_PICTURE,	PANEL,	PANEL_PICTURE,	0,	0,	800,	480,	0,	1,	VAL_BLACK,	0xF79E,	NULL,	NULL,"1337300"},

{3,	TYPE_PICTURE,	PANEL,	PANEL_PICTURE_4,	260,	38,	399,	88,	0,	1,	VAL_BLACK,	0xF79E,	Panel31ChooseWeightUnitCallback,	NULL,"2105300"},

{4,	TYPE_PICTURE,	PANEL,	PANEL_PICTURE_5,	640,	38,	779,	88,	0,	1,	VAL_BLACK,	0xF79E,	Panel31ChooseHeightUnitCallback,	NULL,"2133100"},

{5,	TYPE_PICTURE,	PANEL,	PANEL_PICTURE_8,	162,	103,	195,	134,	0,	1,	VAL_BLACK,	0xF79E,	NULL,	NULL,"2160900"},

{6,	TYPE_PICTURE,	PANEL,	PANEL_PICTURE_9,	176,	194,	532,	473,	0,	1,	VAL_BLACK,	0xF79E,	NULL,	NULL,"2162946"},

{7,	TYPE_PANEL,	PANEL_2,	NULL,	0,	0,	800,	480,	0,	1,	VAL_BLACK,	0xE5E4,	Panel4Callback,	NULL,"Panel4"},

{8,	TYPE_PICTURE,	PANEL_2,	PANEL_2_PICTURE,	0,	0,	800,	480,	0,	1,	VAL_BLACK,	0xF79E,	NULL,	NULL,"3842906"},

{9,	TYPE_PICTURE,	PANEL_2,	PANEL_2_PICTURE_15,	637,	402,	797,	476,	0,	1,	VAL_BLACK,	VAL_TRANSPARENT,	Panel4HomepageCallback,	NULL,"0"},

{10,	TYPE_PICTURE,	PANEL_2,	PANEL_2_PICTURE_2,	663,	54,	730,	121,	0,	1,	VAL_BLACK,	VAL_TRANSPARENT,	Panel4CloseCallback,	NULL,"0"},

{11,	TYPE_PANEL,	PANEL_3,	NULL,	0,	0,	800,	480,	0,	1,	VAL_BLACK,	0xE5E4,	Panel2Callback,	NULL,"Panel2"},

{12,	TYPE_PICTURE,	PANEL_3,	PANEL_3_PICTURE,	0,	0,	800,	480,	0,	1,	VAL_BLACK,	0xF79E,	NULL,	NULL,"1024"},

{13,	TYPE_PICTURE,	PANEL_3,	PANEL_3_PICTURE_2,	263,	395,	538,	476,	0,	1,	VAL_BLACK,	VAL_TRANSPARENT,	Panel2OKCallback,	NULL,"0"},

{14,	TYPE_PICTURE,	PANEL_3,	PANEL_3_PICTURE_8,	697,	287,	777,	353,	0,	1,	VAL_BLACK,	VAL_TRANSPARENT,	Panel2AgeChangedCallback,	NULL,"0"},

{15,	TYPE_PICTURE,	PANEL_3,	PANEL_3_PICTURE_9,	24,	108,	239,	369,	0,	1,	VAL_BLACK,	0xF79E,	Panel2ChooseSexMaleCallback,	NULL,"769024"},

{16,	TYPE_PICTURE,	PANEL_3,	PANEL_3_PICTURE_10,	262,	108,	477,	369,	0,	1,	VAL_BLACK,	0xF79E,	Panel2ChooseSexFemaleCallback,	NULL,"993484"},

{17,	TYPE_PICTURE,	PANEL_3,	PANEL_3_PICTURE_7,	700,	125,	780,	191,	0,	1,	VAL_BLACK,	VAL_TRANSPARENT,	Panel2AgeChangedCallback,	NULL,"0"},

{18,	TYPE_PICTURE,	PANEL_3,	PANEL_3_PICTURE_11,	657,	410,	791,	457,	0,	1,	VAL_BLACK,	VAL_TRANSPARENT,	NULL,	NULL,"1217944"},

{19,	TYPE_PICTURE,	PANEL_3,	PANEL_3_PICTURE_12,	525,	125,	658,	184,	0,	1,	VAL_BLACK,	VAL_TRANSPARENT,	Panel2AthleteSelectCallback,	NULL,"1243136"},

{20,	TYPE_PICTURE,	PANEL_3,	PANEL_3_PICTURE_13,	525,	206,	658,	265,	0,	1,	VAL_BLACK,	VAL_TRANSPARENT,	Panel2AthleteSelectCallback,	NULL,"1274524"},

{21,	TYPE_PICTURE,	PANEL_3,	PANEL_3_PICTURE_14,	525,	290,	658,	349,	0,	1,	VAL_BLACK,	VAL_TRANSPARENT,	Panel2AthleteSelectCallback,	NULL,"1305912"},

{22,	TYPE_PICTURE,	PANEL_3,	PANEL_3_PICTURE_15,	638,	395,	799,	473,	0,	1,	VAL_BLACK,	VAL_TRANSPARENT,	Panel2LanguageSelectionCallback,	NULL,"0"},

{23,	TYPE_PANEL,	PANEL_4,	NULL,	0,	0,	800,	480,	0,	1,	VAL_BLACK,	0xE5E4,	Panel1Callback,	NULL,"Panel1"},

{24,	TYPE_PICTURE,	PANEL_4,	PANEL_4_PICTURE,	259,	107,	541,	312,	0,	1,	VAL_BLACK,	0xF79E,	NULL,	NULL,"13012478"},

{25,	TYPE_PICTURE,	PANEL_4,	PANEL_4_PICTURE_2,	247,	322,	554,	372,	0,	1,	VAL_BLACK,	0xF79E,	NULL,	NULL,"14168678"},

{26,	TYPE_PANEL,	PANEL_5,	NULL,	0,	0,	593,	336,	0,	1,	VAL_BLACK,	VAL_WHITE,	NULL,	NULL,"Panel6"},

{27,	TYPE_PICTURE,	PANEL_5,	PANEL_5_PICTURE_4,	0,	0,	594,	336,	0,	1,	VAL_BLACK,	0xF79E,	NULL,	NULL,"5425986"},

{28,	TYPE_PICTURE,	PANEL_5,	PANEL_5_PICTURE_2,	107,	175,	474,	214,	0,	1,	VAL_BLACK,	0xF79E,	NULL,	NULL,"5825154"},

{29,	TYPE_PICTURE,	PANEL_5,	PANEL_5_PICTURE_3,	537,	0,	590,	53,	0,	1,	VAL_BLACK,	VAL_TRANSPARENT,	Panel6CloseCallback,	NULL,"0"},

{30,	TYPE_PANEL,	PANEL_6,	NULL,	0,	0,	800,	480,	0,	1,	VAL_BLACK,	0xE5E4,	Panel34Callback,	NULL,"Panel34"},

{31,	TYPE_PICTURE,	PANEL_6,	PANEL_6_PICTURE_16,	0,	0,	800,	480,	0,	1,	VAL_BLACK,	0xF79E,	NULL,	NULL,"2761122"},

{32,	TYPE_PICTURE,	PANEL_6,	PANEL_6_PICTURE_4,	260,	38,	399,	88,	0,	1,	VAL_BLACK,	0xF79E,	Panel34ChooseWeightUnitCallback,	NULL,"2105300"},

{33,	TYPE_PICTURE,	PANEL_6,	PANEL_6_PICTURE_5,	640,	38,	779,	88,	0,	1,	VAL_BLACK,	0xF79E,	Panel34ChooseHeightUnitCallback,	NULL,"2133100"},

{34,	TYPE_PICTURE,	PANEL_6,	PANEL_6_PICTURE_8,	162,	103,	195,	134,	0,	1,	VAL_BLACK,	0xF79E,	NULL,	NULL,"2160900"},

{35,	TYPE_PICTURE,	PANEL_6,	PANEL_6_PICTURE_10,	470,	184,	573,	286,	0,	1,	VAL_BLACK,	0xF79E,	NULL,	NULL,"3529122"},

{36,	TYPE_PICTURE,	PANEL_6,	PANEL_6_PICTURE_11,	322,	352,	569,	371,	0,	1,	VAL_BLACK,	0xF79E,	NULL,	NULL,"3613170"},

{37,	TYPE_PICTURE,	PANEL_6,	PANEL_6_PICTURE_13,	147,	388,	288,	478,	0,	1,	VAL_BLACK,	VAL_TRANSPARENT,	Panel34PrintCallback,	NULL,"0"},

{38,	TYPE_PICTURE,	PANEL_6,	PANEL_6_PICTURE_14,	3,	394,	145,	478,	0,	1,	VAL_BLACK,	VAL_TRANSPARENT,	Panel34ScanCallback,	NULL,"0"},

{39,	TYPE_PICTURE,	PANEL_6,	PANEL_6_PICTURE_15,	638,	388,	799,	477,	0,	1,	VAL_BLACK,	VAL_TRANSPARENT,	Panel34HomepageCallback,	NULL,"0"},

{40,	TYPE_PICTURE,	PANEL_6,	PANEL_6_PICTURE,	596,	229,	764,	372,	0,	1,	VAL_BLACK,	0xF79E,	NULL,	NULL,"3650714"},

{41,	TYPE_PANEL,	PANEL_7,	NULL,	0,	0,	800,	480,	0,	1,	VAL_BLACK,	0xE5E4,	Panel5Callback,	NULL,"Panel5"},

{42,	TYPE_PICTURE,	PANEL_7,	PANEL_7_PICTURE_12,	0,	0,	800,	480,	0,	1,	VAL_BLACK,	0xF79E,	NULL,	NULL,"4610906"},

{43,	TYPE_PICTURE,	PANEL_7,	PANEL_7_PICTURE_3,	272,	404,	526,	464,	0,	1,	VAL_BLACK,	0xF79E,	Panel5SaveCallback,	NULL,"5395506"},

{44,	TYPE_PICTURE,	PANEL_7,	PANEL_7_PICTURE_8,	542,	129,	625,	154,	0,	1,	VAL_BLACK,	0xF79E,	NULL,	NULL,"5378906"},

{45,	TYPE_PICTURE,	PANEL_7,	PANEL_7_PICTURE_9,	542,	195,	625,	220,	0,	1,	VAL_BLACK,	0xF79E,	NULL,	NULL,"5383056"},

{46,	TYPE_PICTURE,	PANEL_7,	PANEL_7_PICTURE_10,	542,	262,	625,	287,	0,	1,	VAL_BLACK,	0xF79E,	NULL,	NULL,"5387206"},

{47,	TYPE_PICTURE,	PANEL_7,	PANEL_7_PICTURE_11,	542,	331,	625,	356,	0,	1,	VAL_BLACK,	0xF79E,	NULL,	NULL,"5391356"},

{48,	TYPE_PANEL,	PANEL_8,	NULL,	0,	0,	602,	425,	0,	1,	VAL_BLACK,	VAL_LT_GRAY,	Panel7Callback,	NULL,"Panel7"},

{49,	TYPE_STRING,	PANEL_8,	PANEL_8_STRING,	110,	9,	523,	35,	0,	1,	VAL_BLACK,	VAL_WHITE,	NULL,	NULL,""},

{50,	TYPE_COMMANDBUTTON,	PANEL_8,	PANEL_8_COMMANDBUTTON,	436,	54,	551,	84,	0,	1,	VAL_BLACK,	0xF79E,	Panel7HeightCalibOKCallback,	NULL,"Exec/????????"},

{51,	TYPE_TEXTMSG,	PANEL_8,	PANEL_8_TEXTMSG,	12,	12,	103,	32,	0,	1,	VAL_BLACK,	VAL_LT_GRAY,	NULL,	NULL,"Status/????"},

{52,	TYPE_STRING,	PANEL_8,	PANEL_8_STRING_2,	262,	55,	313,	81,	0,	1,	VAL_BLACK,	VAL_WHITE,	NULL,	NULL,""},

{53,	TYPE_TEXTMSG,	PANEL_8,	PANEL_8_TEXTMSG_2,	317,	58,	336,	78,	0,	1,	VAL_BLACK,	VAL_LT_GRAY,	NULL,	NULL,"cm"},

{54,	TYPE_COMMANDBUTTON,	PANEL_8,	PANEL_8_COMMANDBUTTON_2,	212,	54,	252,	84,	0,	1,	VAL_BLACK,	0xF79E,	Panel7HeightCalibCallback,	NULL,"-"},

{55,	TYPE_COMMANDBUTTON,	PANEL_8,	PANEL_8_COMMANDBUTTON_3,	342,	54,	382,	84,	0,	1,	VAL_BLACK,	0xF79E,	Panel7HeightCalibCallback,	NULL,"+"},

{56,	TYPE_TEXTMSG,	PANEL_8,	PANEL_8_TEXTMSG_3,	10,	58,	141,	78,	0,	1,	VAL_BLACK,	VAL_LT_GRAY,	NULL,	NULL,"H-Calib/????????"},

{57,	TYPE_COMMANDBUTTON,	PANEL_8,	PANEL_8_COMMANDBUTTON_4,	25,	94,	193,	124,	0,	1,	VAL_BLACK,	0xF79E,	Panel7EnterWeightCalibCallback,	NULL,"W-Calib/????????"},

{58,	TYPE_COMMANDBUTTON,	PANEL_8,	PANEL_8_COMMANDBUTTON_5,	267,	94,	374,	124,	0,	1,	VAL_BLACK,	0xF79E,	Panel7SetLanguageCallback,	NULL,"Chinese/????"},

{59,	TYPE_COMMANDBUTTON,	PANEL_8,	PANEL_8_COMMANDBUTTON_6,	372,	94,	479,	124,	0,	1,	VAL_BLACK,	0xF79E,	Panel7SetLanguageCallback,	NULL,"English/????"},

{60,	TYPE_TEXTMSG,	PANEL_8,	PANEL_8_TEXTMSG_4,	23,	141,	146,	161,	0,	1,	VAL_BLACK,	VAL_LT_GRAY,	NULL,	NULL,"DefAge/????????"},

{61,	TYPE_STRING,	PANEL_8,	PANEL_8_STRING_3,	245,	138,	296,	164,	0,	1,	VAL_BLACK,	VAL_WHITE,	NULL,	NULL,""},

{62,	TYPE_COMMANDBUTTON,	PANEL_8,	PANEL_8_COMMANDBUTTON_8,	195,	137,	235,	167,	0,	1,	VAL_BLACK,	0xF79E,	Panel7DefaultAgeCallback,	NULL,"-"},

{63,	TYPE_COMMANDBUTTON,	PANEL_8,	PANEL_8_COMMANDBUTTON_7,	305,	137,	345,	167,	0,	1,	VAL_BLACK,	0xF79E,	Panel7DefaultAgeCallback,	NULL,"+"},

{64,	TYPE_TEXTMSG,	PANEL_8,	PANEL_8_TEXTMSG_5,	23,	188,	146,	208,	0,	1,	VAL_BLACK,	VAL_LT_GRAY,	NULL,	NULL,"DefSex/????????"},

{65,	TYPE_COMMANDBUTTON,	PANEL_8,	PANEL_8_COMMANDBUTTON_10,	195,	184,	268,	214,	0,	1,	VAL_BLACK,	0xF79E,	Panel7DefaultSexCallback,	NULL,"Men/??"},

{66,	TYPE_COMMANDBUTTON,	PANEL_8,	PANEL_8_COMMANDBUTTON_9,	272,	184,	358,	214,	0,	1,	VAL_BLACK,	0xF79E,	Panel7DefaultSexCallback,	NULL,"Women/??"},

{67,	TYPE_COMMANDBUTTON,	PANEL_8,	PANEL_8_COMMANDBUTTON_11,	545,	0,	607,	57,	0,	1,	VAL_BLACK,	0xF79E,	Panel7CloseCallback,	NULL,"X"},

{68,	TYPE_COMMANDBUTTON,	PANEL_8,	PANEL_8_COMMANDBUTTON_12,	168,	54,	208,	84,	0,	1,	VAL_BLACK,	0xF79E,	Panel7HeightCalibCallback,	NULL,"--"},

{69,	TYPE_COMMANDBUTTON,	PANEL_8,	PANEL_8_COMMANDBUTTON_13,	385,	54,	425,	84,	0,	1,	VAL_BLACK,	0xF79E,	Panel7HeightCalibCallback,	NULL,"++"},

{70,	TYPE_COMMANDBUTTON,	PANEL_8,	PANEL_8_COMMANDBUTTON_14,	370,	177,	550,	207,	0,	1,	VAL_BLACK,	0xF79E,	Panel7UserModeCallback,	NULL,"Admin/??????????"},

{71,	TYPE_COMMANDBUTTON,	PANEL_8,	PANEL_8_COMMANDBUTTON_15,	370,	217,	550,	247,	0,	1,	VAL_BLACK,	0xF79E,	Panel7UserModeCallback,	NULL,"UserMode/????????"},

{72,	TYPE_COMMANDBUTTON,	PANEL_8,	PANEL_8_COMMANDBUTTON_16,	20,	249,	151,	283,	0,	1,	VAL_BLACK,	0xF79E,	Panel7SetBTModeCallback,	NULL,"BLE ON/??????"},

{73,	TYPE_COMMANDBUTTON,	PANEL_8,	PANEL_8_COMMANDBUTTON_17,	20,	289,	150,	319,	0,	1,	VAL_BLACK,	0xF79E,	Panel7SetBTModeCallback,	NULL,"BLE OFF/??????"},

{74,	TYPE_COMMANDBUTTON,	PANEL_8,	PANEL_8_COMMANDBUTTON_19,	161,	250,	292,	280,	0,	1,	VAL_BLACK,	0xF79E,	Panel7SetAudioModeCallback,	NULL,"Audio On/??????"},

{75,	TYPE_COMMANDBUTTON,	PANEL_8,	PANEL_8_COMMANDBUTTON_18,	161,	290,	300,	320,	0,	1,	VAL_BLACK,	0xF79E,	Panel7SetAudioModeCallback,	NULL,"Audio Off/??????"},

{76,	TYPE_COMMANDBUTTON,	PANEL_8,	PANEL_8_COMMANDBUTTON_20,	412,	137,	592,	167,	0,	1,	VAL_BLACK,	0xF79E,	Panel7UserModeCallback,	NULL,"Maintain/????????"},

{77,	TYPE_COMMANDBUTTON,	PANEL_8,	PANEL_8_COMMANDBUTTON_21,	307,	255,	457,	285,	0,	1,	VAL_BLACK,	0xF79E,	Panel7VolumeSetCallback,	NULL,"VolumeHigh/??????"},

{78,	TYPE_COMMANDBUTTON,	PANEL_8,	PANEL_8_COMMANDBUTTON_22,	307,	289,	457,	319,	0,	1,	VAL_BLACK,	0xF79E,	Panel7VolumeSetCallback,	NULL,"VolumeLow/??????"},

{79,	TYPE_COMMANDBUTTON,	PANEL_8,	PANEL_8_COMMANDBUTTON_23,	378,	332,	493,	362,	0,	1,	VAL_BLACK,	0xF79E,	Panel7WeightOffsetSaveCallback,	NULL,"Save/????"},

{80,	TYPE_STRING,	PANEL_8,	PANEL_8_STRING_4,	224,	333,	275,	359,	0,	1,	VAL_BLACK,	VAL_WHITE,	NULL,	NULL,""},

{81,	TYPE_TEXTMSG,	PANEL_8,	PANEL_8_TEXTMSG_6,	279,	336,	298,	356,	0,	1,	VAL_BLACK,	VAL_LT_GRAY,	NULL,	NULL,"kg"},

{82,	TYPE_COMMANDBUTTON,	PANEL_8,	PANEL_8_COMMANDBUTTON_27,	174,	332,	214,	362,	0,	1,	VAL_BLACK,	0xF79E,	Panel7WeightOffsetAdjustCallback,	NULL,"-"},

{83,	TYPE_COMMANDBUTTON,	PANEL_8,	PANEL_8_COMMANDBUTTON_25,	304,	332,	344,	362,	0,	1,	VAL_BLACK,	0xF79E,	Panel7WeightOffsetAdjustCallback,	NULL,"+"},

{84,	TYPE_TEXTMSG,	PANEL_8,	PANEL_8_TEXTMSG_7,	12,	336,	151,	356,	0,	1,	VAL_BLACK,	VAL_LT_GRAY,	NULL,	NULL,"W-offset/????????"},

{85,	TYPE_PANEL,	PANEL_9,	NULL,	0,	0,	444,	259,	0,	1,	VAL_BLACK,	0xB596,	NULL,	NULL,"Panel61"},

{86,	TYPE_TEXTMSG,	PANEL_9,	PANEL_9_TEXTMSG,	37,	81,	96,	101,	0,	1,	VAL_BLACK,	0xB596,	NULL,	NULL,"Content"},

{87,	TYPE_COMMANDBUTTON,	PANEL_9,	PANEL_9_COMMANDBUTTON,	49,	187,	156,	238,	0,	1,	VAL_BLACK,	0xF79E,	Panel61OKCallback,	NULL,"__OK"},

{88,	TYPE_TEXTMSG,	PANEL_9,	PANEL_9_TEXTMSG_2,	8,	8,	51,	28,	0,	1,	VAL_BLACK,	0xB596,	NULL,	NULL,"Title"},

{89,	TYPE_COMMANDBUTTON,	PANEL_9,	PANEL_9_COMMANDBUTTON_2,	269,	187,	376,	238,	0,	1,	VAL_BLACK,	0xF79E,	Panel61CancelCallback,	NULL,"Cancel"},

};
