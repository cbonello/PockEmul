#ifndef KEYB1401_H
#define KEYB1401_H


BYTE scandef_pc1401[] = {
//+0		+1			+2			+3			+4			+5			+6			+7
K_SIGN,		'8',		'2',		'5',		K_CAL,		'q',		'a',		'z',
'.',		'9',		'3',		'6',		K_BASIC,	'w',		's',		'x',
'+',		'/',		'-',		'*',		K_CTRL,		'e',		'd',		'c',
')',		'(',		K_SQR,		K_ROOT,		K_POT,		K_EXP,		K_XM,		'=',
K_STAT,		K_1X,		K_LOG,		K_LN,		K_DEG,		K_HEX,		NUL,		K_MPLUS,
K_CCE,		K_FSE,		K_TAN,		K_COS,		K_SIN,		K_HYP,		K_SHT,		K_RM,
NUL,		NUL,		NUL,		NUL,		NUL,		NUL,		NUL,		NUL,
NUL,		NUL,		NUL,		NUL,		NUL,		NUL,		NUL,		NUL,

NUL,		'7',		'1',		'4',		K_DA,		'r',		'f',		'v',
NUL,		NUL,		',',		'p',		K_UA,		't',		'g',		'b',
NUL,		NUL,		NUL,		'o',		K_LA,		'y',		'h',		'n',
NUL,		NUL,		NUL,		NUL,		K_RA,		'u',		'j',		'm',
NUL,		NUL,		NUL,		NUL,		NUL,		'i',		'k',		' ',
NUL,		NUL,		NUL,		NUL,		NUL,		NUL,		'l',		K_RET,
NUL,		NUL,		NUL,		NUL,		NUL,		NUL,		NUL,		'0',

};


TransMap KeyMap1401[]={
1,	"  -         ",	'-',	537,	181,	2,		//OK
2,	"  (         ",	'(',	552,	77,		1,		//OK
3,	"  )         ",	')',	589,	77,		1,		//OK
4,	"  *         ",	'*',	537,	144,	2,		//OK
5,	"  ,         ",	',',	361,	189,	1,		//OK
6,	"  .         ",	'.',	492,	217,	2,		//OK
7,	"  /         ",	'/',	537,	107,	2,		//OK
8,	"  :         ",	':',	0,		0,		1,		//OK
9,	"  ;         ",	';',	0,		0,		1,		//OK
10,	"  +         ",	'+',	537,	217,	2,		//OK
11,	" CLS        ",	K_CCE,	589,	22,		1,		//OK
12,	" DEF        ",	K_CTRL,	136,	126,	1,		//OK
13,	" DEL        ",	K_DEL,	0,		0,		1,
14,	" Down Arrow ",	K_DA,	174,	126,	1,		//OK	
15,	" ENTER      ",	K_RET,	323,	220,	3,		//OK
16,	" Equal      ",	'=',	582,	217,	2,		//OK
17,	" INS        ",	K_INS,	0,		0,		1,
18,	" Left Arrow ",	K_LA ,	249,	126,	1,		//OK
19,	" MODE       ",	K_MOD,	0,		0,		1,
20,	" On/Brk     ",	K_BRK,	94,		126,	1,		//OK	
21,	" On/Off     ",	K_OF ,	24,		55,		4,		//OK
22,	" Rigth Arrow",	K_RA ,	286,	126,	1,		//OK	
23,	" SHIFT1     ",	K_SHT,	361,	126,	1,		//OK	
24,	" SHIFT2     ",	K_SHT,	0,		0,		1,
25,	" SML        ",	K_SML,	0,		0,		1,
26,	" Space      ",	' ',	286,	220,	1,		//OK
27,	" Up Arrow   ",	K_UA ,	212,	126,	1,		//OK	
28,	"0           ",	'0',	402,	217,	2,		//OK
29,	"1           ",	'1',	402,	181,	2,		//OK
30,	"2           ",	'2',	447,	181,	2,		//OK
31,	"3           ",	'3',	492,	181,	2,		//OK
32,	"4           ",	'4',	402,	144,	2,		//OK
33,	"5           ",	'5',	447,	144,	2,		//OK
34,	"6           ",	'6',	492,	144,	2,		//OK
35,	"7           ",	'7',	402,	107,	2,		//OK
36,	"8           ",	'8',	447,	107,	2,		//OK
37,	"9           ",	'9',	492,	107,	2,		//OK
38,	"A           ",	'a',	24,		189,	1,		//OK
39,	"B           ",	'b',	174,	220,	1,		//OK
40,	"C           ",	'c',	94,		220,	1,		//OK
41,	"D           ",	'd',	94,		189,	1,		//OK
42,	"E           ",	'e',	94,		157,	1,		//OK
43,	"F           ",	'f',	136,	189,	1,		//OK
44,	"G           ",	'g',	174,	189,	1,		//OK
45,	"H           ",	'h',	212,	189,	1,		//OK
46,	"I           ",	'i',	286,	157,	1,		//OK
47,	"J           ",	'j',	249,	189,	1,		//OK
48,	"K           ",	'k',	286,	189,	1,		//OK
49,	"L           ",	'l',	323,	189,	1,		//OK
50,	"M           ",	'm',	249,	220,	1,		//OK
51,	"N           ",	'n',	212,	220,	1,		//OK
52,	"O           ",	'o',	323,	157,	1,		//OK
53,	"P           ",	'p',	361,	157,	1,		//OK
54,	"Q           ",	'q',	24,		157,	1,		//OK
55,	"R           ",	'r',	136,	157,	1,		//OK
56,	"S           ",	's',	62,		189,	1,		//OK
57,	"T           ",	't',	174,	157,	1,		//OK
58,	"U           ",	'u',	249,	157,	1,		//OK
59,	"V           ",	'v',	136,	220,	1,		//OK
60,	"W           ",	'w',	62,		157,	1,		//OK
61,	"X           ",	'x',	62,		220,	1,		//OK
62,	"Y           ",	'y',	212,	157,	1,		//OK
63,	"Z           ",	'z',	24,		220,	1,		//OK
64, "CAL         ", K_CAL,	24,		126,	1,		//OK
65, "BASIC       ", K_BASIC,62,		126,	1,		//OK
66,	"+/-         ",	K_SIGN,	447,	217,	2,		//OK
67,	"x^2         ",	K_SQR,	514,	77,		1,		//OK
68, "SQRT(x)     ",	K_ROOT,	477,	77,		1,		//OK
69,	"x^y         ",	K_POT,	440,	77,		1,		//OK
70,	"Exp         ",	K_EXP,	402,	77,		1,		//OK
71,	"X:M         ",	K_XM,	582,	107,	2,		//OK
72,	"Stat        ",	K_STAT,	589,	50,		1,		//OK
73,	"1/x         ",	K_1X,	552,	50,		1,		//OK
74,	"Log         ",	K_LOG,	514,	50,		1,		//OK
75,	"Ln          ",	K_LN,	477,	50,		1,		//OK
76,	"Deg         ",	K_DEG,	440,	50,		1,		//OK
77,	"Hex         ",	K_HEX,	402,	50,		1,		//OK
78,	"M+          ",	K_MPLUS,582,	181,	2,		//OK
79,	"FSE         ",	K_FSE,	552,	22,		1,		//OK
80,	"Tan         ",	K_TAN,	514,	22,		1,		//OK
81,	"Cos         ",	K_COS,	477,	22,		1,		//OK
82,	"Sin         ",	K_SIN,	440,	22,		1,		//OK
83,	"Hyp         ",	K_HYP,	402,	22,		1,		//OK
84,	"RM          ",	K_RM,	582,	144,	2,		//OK
};
int KeyMap1401Lenght = 84;

#endif
