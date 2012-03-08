
#line 1 "libswanson/s0-parser.c.rl"
/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2012, RedJack, LLC.
 * All rights reserved.
 *
 * Please see the LICENSE.txt file in this distribution for license
 * details.
 * ----------------------------------------------------------------------
 */

#include <libcork/core.h>

#include "swanson/ast.h"
#include "swanson/ds.h"
#include "swanson/metamodel.h"
#include "swanson/state.h"

static const char  digit_values[256] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1,
    -1, -1, -1, -1, -1, -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1
};


#line 116 "libswanson/s0-parser.c.rl"



#line 43 "libswanson/s0-parser.c"
static const unsigned char _s0_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 61, 64
};

static const int s0_start = 177;
static const int s0_first_final = 177;
static const int s0_error = 0;

static const int s0_en_main = 177;


#line 119 "libswanson/s0-parser.c.rl"

struct swan_ast *
swan_ast_parse(struct swan *s, const char *buf, size_t size)
{
    const char  *p = buf;
    const char  *pe = buf + size;
    const char  *eof = pe;
    CORK_ATTR_UNUSED const char  *ts;
    CORK_ATTR_UNUSED const char  *te;
    CORK_ATTR_UNUSED int  cs;
    CORK_ATTR_UNUSED int  act;

    size_t  curline = 0;
    const char  *xs = NULL;
    struct cork_buffer  scratch = CORK_BUFFER_INIT();
    char  hex_char;

    struct swan_static_string  *id = NULL;
    struct swan_static_string  *contents = NULL;
    struct swan_ast_call  *call = NULL;
    struct swan_ast_string  *ast_string = NULL;
    struct swan_ast  *ast = swan_ast_new(s);

    
#line 102 "libswanson/s0-parser.c"
	{
	cs = s0_start;
	}

#line 143 "libswanson/s0-parser.c.rl"
    
#line 109 "libswanson/s0-parser.c"
	{
	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	switch ( cs ) {
case 177:
	switch( (*p) ) {
		case 10: goto tr115;
		case 32: goto tr572;
		case 34: goto tr573;
		case 35: goto tr114;
		case 36: goto tr116;
		case 95: goto tr574;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr572;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr574;
	} else
		goto tr574;
	goto tr7;
case 0:
	goto _out;
case 1:
	switch( (*p) ) {
		case 34: goto tr1;
		case 92: goto tr2;
	}
	goto tr0;
case 2:
	switch( (*p) ) {
		case 34: goto tr4;
		case 92: goto tr5;
	}
	goto tr3;
case 3:
	switch( (*p) ) {
		case 10: goto tr8;
		case 32: goto tr6;
		case 35: goto tr9;
		case 44: goto tr10;
		case 46: goto tr11;
		case 61: goto tr12;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr6;
	goto tr7;
case 4:
	switch( (*p) ) {
		case 10: goto tr14;
		case 32: goto tr13;
		case 35: goto tr15;
		case 44: goto tr16;
		case 46: goto tr17;
		case 61: goto tr18;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr13;
	goto tr7;
case 5:
	if ( (*p) == 10 )
		goto tr14;
	goto tr15;
case 6:
	switch( (*p) ) {
		case 10: goto tr20;
		case 32: goto tr19;
		case 34: goto tr21;
		case 35: goto tr22;
		case 95: goto tr23;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr19;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr23;
	} else
		goto tr23;
	goto tr7;
case 7:
	switch( (*p) ) {
		case 10: goto tr25;
		case 32: goto tr24;
		case 34: goto tr26;
		case 35: goto tr27;
		case 95: goto tr28;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr24;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr28;
	} else
		goto tr28;
	goto tr7;
case 8:
	switch( (*p) ) {
		case 34: goto tr30;
		case 92: goto tr31;
	}
	goto tr29;
case 9:
	switch( (*p) ) {
		case 34: goto tr33;
		case 92: goto tr34;
	}
	goto tr32;
case 10:
	switch( (*p) ) {
		case 10: goto tr36;
		case 32: goto tr35;
		case 35: goto tr37;
		case 44: goto tr38;
		case 61: goto tr39;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr35;
	goto tr7;
case 11:
	switch( (*p) ) {
		case 10: goto tr41;
		case 32: goto tr40;
		case 35: goto tr42;
		case 44: goto tr16;
		case 61: goto tr18;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr40;
	goto tr7;
case 12:
	if ( (*p) == 10 )
		goto tr41;
	goto tr42;
case 13:
	switch( (*p) ) {
		case 10: goto tr44;
		case 32: goto tr43;
		case 34: goto tr45;
		case 35: goto tr46;
		case 95: goto tr47;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr43;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr47;
	} else
		goto tr47;
	goto tr7;
case 14:
	switch( (*p) ) {
		case 10: goto tr49;
		case 32: goto tr48;
		case 34: goto tr50;
		case 35: goto tr51;
		case 95: goto tr52;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr48;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr52;
	} else
		goto tr52;
	goto tr7;
case 15:
	switch( (*p) ) {
		case 34: goto tr54;
		case 92: goto tr55;
	}
	goto tr53;
case 16:
	switch( (*p) ) {
		case 34: goto tr57;
		case 92: goto tr58;
	}
	goto tr56;
case 17:
	switch( (*p) ) {
		case 10: goto tr60;
		case 32: goto tr59;
		case 35: goto tr61;
		case 46: goto tr62;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr59;
	goto tr7;
case 18:
	switch( (*p) ) {
		case 10: goto tr64;
		case 32: goto tr63;
		case 35: goto tr65;
		case 46: goto tr17;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr63;
	goto tr7;
case 19:
	if ( (*p) == 10 )
		goto tr64;
	goto tr65;
case 20:
	switch( (*p) ) {
		case 10: goto tr67;
		case 32: goto tr66;
		case 34: goto tr68;
		case 35: goto tr69;
		case 95: goto tr70;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr66;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr70;
	} else
		goto tr70;
	goto tr7;
case 21:
	switch( (*p) ) {
		case 10: goto tr72;
		case 32: goto tr71;
		case 34: goto tr73;
		case 35: goto tr74;
		case 95: goto tr75;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr71;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr75;
	} else
		goto tr75;
	goto tr7;
case 22:
	switch( (*p) ) {
		case 34: goto tr77;
		case 92: goto tr78;
	}
	goto tr76;
case 23:
	switch( (*p) ) {
		case 34: goto tr80;
		case 92: goto tr81;
	}
	goto tr79;
case 24:
	switch( (*p) ) {
		case 10: goto tr83;
		case 32: goto tr82;
		case 35: goto tr84;
		case 40: goto tr85;
		case 59: goto tr86;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr82;
	goto tr7;
case 25:
	switch( (*p) ) {
		case 10: goto tr88;
		case 32: goto tr87;
		case 35: goto tr89;
		case 40: goto tr90;
		case 59: goto tr91;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr87;
	goto tr7;
case 26:
	if ( (*p) == 10 )
		goto tr88;
	goto tr89;
case 27:
	switch( (*p) ) {
		case 10: goto tr92;
		case 32: goto tr90;
		case 34: goto tr93;
		case 35: goto tr94;
		case 41: goto tr95;
		case 95: goto tr96;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr90;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr96;
	} else
		goto tr96;
	goto tr7;
case 28:
	switch( (*p) ) {
		case 34: goto tr98;
		case 92: goto tr99;
	}
	goto tr97;
case 29:
	switch( (*p) ) {
		case 34: goto tr101;
		case 92: goto tr102;
	}
	goto tr100;
case 30:
	switch( (*p) ) {
		case 10: goto tr104;
		case 32: goto tr103;
		case 35: goto tr105;
		case 41: goto tr106;
		case 44: goto tr107;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr103;
	goto tr7;
case 31:
	switch( (*p) ) {
		case 10: goto tr109;
		case 32: goto tr108;
		case 35: goto tr110;
		case 41: goto tr95;
		case 44: goto tr111;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr108;
	goto tr7;
case 32:
	if ( (*p) == 10 )
		goto tr109;
	goto tr110;
case 33:
	switch( (*p) ) {
		case 10: goto tr112;
		case 32: goto tr95;
		case 35: goto tr113;
		case 59: goto tr91;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr95;
	goto tr7;
case 34:
	if ( (*p) == 10 )
		goto tr112;
	goto tr113;
case 178:
	switch( (*p) ) {
		case 10: goto tr576;
		case 32: goto tr575;
		case 34: goto tr577;
		case 35: goto tr578;
		case 36: goto tr579;
		case 95: goto tr580;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr575;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr580;
	} else
		goto tr580;
	goto tr7;
case 35:
	if ( (*p) == 10 )
		goto tr115;
	goto tr114;
case 36:
	switch( (*p) ) {
		case 10: goto tr117;
		case 32: goto tr116;
		case 34: goto tr118;
		case 35: goto tr119;
		case 95: goto tr120;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr116;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr120;
	} else
		goto tr120;
	goto tr7;
case 37:
	switch( (*p) ) {
		case 34: goto tr122;
		case 92: goto tr123;
	}
	goto tr121;
case 38:
	switch( (*p) ) {
		case 34: goto tr125;
		case 92: goto tr126;
	}
	goto tr124;
case 39:
	switch( (*p) ) {
		case 10: goto tr128;
		case 32: goto tr127;
		case 35: goto tr129;
		case 61: goto tr130;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr127;
	goto tr7;
case 40:
	switch( (*p) ) {
		case 10: goto tr132;
		case 32: goto tr131;
		case 35: goto tr133;
		case 61: goto tr134;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr131;
	goto tr7;
case 41:
	if ( (*p) == 10 )
		goto tr132;
	goto tr133;
case 42:
	switch( (*p) ) {
		case 10: goto tr135;
		case 32: goto tr134;
		case 34: goto tr136;
		case 35: goto tr137;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr134;
	goto tr7;
case 43:
	switch( (*p) ) {
		case 34: goto tr139;
		case 92: goto tr140;
	}
	goto tr138;
case 44:
	switch( (*p) ) {
		case 34: goto tr142;
		case 92: goto tr143;
	}
	goto tr141;
case 45:
	switch( (*p) ) {
		case 10: goto tr145;
		case 32: goto tr144;
		case 35: goto tr146;
		case 59: goto tr147;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr144;
	goto tr7;
case 46:
	if ( (*p) == 10 )
		goto tr145;
	goto tr146;
case 179:
	switch( (*p) ) {
		case 10: goto tr582;
		case 32: goto tr581;
		case 34: goto tr583;
		case 35: goto tr584;
		case 36: goto tr585;
		case 95: goto tr586;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr581;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr586;
	} else
		goto tr586;
	goto tr7;
case 47:
	switch( (*p) ) {
		case 10: goto tr149;
		case 32: goto tr148;
		case 35: goto tr150;
		case 44: goto tr151;
		case 46: goto tr152;
		case 61: goto tr154;
		case 95: goto tr153;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr148;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr153;
		} else if ( (*p) >= 65 )
			goto tr153;
	} else
		goto tr153;
	goto tr7;
case 48:
	switch( (*p) ) {
		case 34: goto tr155;
		case 39: goto tr156;
		case 92: goto tr157;
		case 97: goto tr158;
		case 98: goto tr159;
		case 102: goto tr160;
		case 110: goto tr161;
		case 114: goto tr162;
		case 116: goto tr163;
		case 118: goto tr164;
		case 120: goto tr165;
	}
	goto tr7;
case 49:
	switch( (*p) ) {
		case 34: goto tr167;
		case 92: goto tr168;
	}
	goto tr166;
case 50:
	switch( (*p) ) {
		case 34: goto tr170;
		case 92: goto tr171;
	}
	goto tr169;
case 51:
	switch( (*p) ) {
		case 34: goto tr173;
		case 92: goto tr174;
	}
	goto tr172;
case 52:
	switch( (*p) ) {
		case 34: goto tr176;
		case 92: goto tr177;
	}
	goto tr175;
case 53:
	switch( (*p) ) {
		case 34: goto tr179;
		case 92: goto tr180;
	}
	goto tr178;
case 54:
	switch( (*p) ) {
		case 34: goto tr182;
		case 92: goto tr183;
	}
	goto tr181;
case 55:
	switch( (*p) ) {
		case 34: goto tr185;
		case 92: goto tr186;
	}
	goto tr184;
case 56:
	switch( (*p) ) {
		case 34: goto tr188;
		case 92: goto tr189;
	}
	goto tr187;
case 57:
	switch( (*p) ) {
		case 34: goto tr191;
		case 92: goto tr192;
	}
	goto tr190;
case 58:
	switch( (*p) ) {
		case 34: goto tr194;
		case 92: goto tr195;
	}
	goto tr193;
case 59:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr196;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr196;
	} else
		goto tr196;
	goto tr7;
case 60:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr197;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr197;
	} else
		goto tr197;
	goto tr7;
case 61:
	switch( (*p) ) {
		case 34: goto tr199;
		case 92: goto tr200;
	}
	goto tr198;
case 62:
	if ( (*p) == 10 )
		goto tr135;
	goto tr137;
case 63:
	switch( (*p) ) {
		case 34: goto tr201;
		case 39: goto tr202;
		case 92: goto tr203;
		case 97: goto tr204;
		case 98: goto tr205;
		case 102: goto tr206;
		case 110: goto tr207;
		case 114: goto tr208;
		case 116: goto tr209;
		case 118: goto tr210;
		case 120: goto tr211;
	}
	goto tr7;
case 64:
	switch( (*p) ) {
		case 34: goto tr213;
		case 92: goto tr214;
	}
	goto tr212;
case 65:
	switch( (*p) ) {
		case 34: goto tr216;
		case 92: goto tr217;
	}
	goto tr215;
case 66:
	switch( (*p) ) {
		case 34: goto tr219;
		case 92: goto tr220;
	}
	goto tr218;
case 67:
	switch( (*p) ) {
		case 34: goto tr222;
		case 92: goto tr223;
	}
	goto tr221;
case 68:
	switch( (*p) ) {
		case 34: goto tr225;
		case 92: goto tr226;
	}
	goto tr224;
case 69:
	switch( (*p) ) {
		case 34: goto tr228;
		case 92: goto tr229;
	}
	goto tr227;
case 70:
	switch( (*p) ) {
		case 34: goto tr231;
		case 92: goto tr232;
	}
	goto tr230;
case 71:
	switch( (*p) ) {
		case 34: goto tr234;
		case 92: goto tr235;
	}
	goto tr233;
case 72:
	switch( (*p) ) {
		case 34: goto tr237;
		case 92: goto tr238;
	}
	goto tr236;
case 73:
	switch( (*p) ) {
		case 34: goto tr240;
		case 92: goto tr241;
	}
	goto tr239;
case 74:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr242;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr242;
	} else
		goto tr242;
	goto tr7;
case 75:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr243;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr243;
	} else
		goto tr243;
	goto tr7;
case 76:
	switch( (*p) ) {
		case 34: goto tr245;
		case 92: goto tr246;
	}
	goto tr244;
case 77:
	if ( (*p) == 10 )
		goto tr117;
	goto tr119;
case 78:
	switch( (*p) ) {
		case 10: goto tr248;
		case 32: goto tr247;
		case 35: goto tr249;
		case 61: goto tr251;
		case 95: goto tr250;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr247;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr250;
		} else if ( (*p) >= 65 )
			goto tr250;
	} else
		goto tr250;
	goto tr7;
case 79:
	switch( (*p) ) {
		case 10: goto tr252;
		case 32: goto tr111;
		case 34: goto tr253;
		case 35: goto tr254;
		case 95: goto tr255;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr111;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr255;
	} else
		goto tr255;
	goto tr7;
case 80:
	switch( (*p) ) {
		case 34: goto tr257;
		case 92: goto tr258;
	}
	goto tr256;
case 81:
	switch( (*p) ) {
		case 34: goto tr260;
		case 92: goto tr261;
	}
	goto tr259;
case 82:
	switch( (*p) ) {
		case 10: goto tr263;
		case 32: goto tr262;
		case 35: goto tr264;
		case 41: goto tr265;
		case 44: goto tr266;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr262;
	goto tr7;
case 83:
	switch( (*p) ) {
		case 34: goto tr267;
		case 39: goto tr268;
		case 92: goto tr269;
		case 97: goto tr270;
		case 98: goto tr271;
		case 102: goto tr272;
		case 110: goto tr273;
		case 114: goto tr274;
		case 116: goto tr275;
		case 118: goto tr276;
		case 120: goto tr277;
	}
	goto tr7;
case 84:
	switch( (*p) ) {
		case 34: goto tr279;
		case 92: goto tr280;
	}
	goto tr278;
case 85:
	switch( (*p) ) {
		case 34: goto tr282;
		case 92: goto tr283;
	}
	goto tr281;
case 86:
	switch( (*p) ) {
		case 34: goto tr285;
		case 92: goto tr286;
	}
	goto tr284;
case 87:
	switch( (*p) ) {
		case 34: goto tr288;
		case 92: goto tr289;
	}
	goto tr287;
case 88:
	switch( (*p) ) {
		case 34: goto tr291;
		case 92: goto tr292;
	}
	goto tr290;
case 89:
	switch( (*p) ) {
		case 34: goto tr294;
		case 92: goto tr295;
	}
	goto tr293;
case 90:
	switch( (*p) ) {
		case 34: goto tr297;
		case 92: goto tr298;
	}
	goto tr296;
case 91:
	switch( (*p) ) {
		case 34: goto tr300;
		case 92: goto tr301;
	}
	goto tr299;
case 92:
	switch( (*p) ) {
		case 34: goto tr303;
		case 92: goto tr304;
	}
	goto tr302;
case 93:
	switch( (*p) ) {
		case 34: goto tr306;
		case 92: goto tr307;
	}
	goto tr305;
case 94:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr308;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr308;
	} else
		goto tr308;
	goto tr7;
case 95:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr309;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr309;
	} else
		goto tr309;
	goto tr7;
case 96:
	switch( (*p) ) {
		case 34: goto tr311;
		case 92: goto tr312;
	}
	goto tr310;
case 97:
	if ( (*p) == 10 )
		goto tr252;
	goto tr254;
case 98:
	switch( (*p) ) {
		case 10: goto tr314;
		case 32: goto tr313;
		case 35: goto tr315;
		case 41: goto tr316;
		case 44: goto tr317;
		case 95: goto tr318;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr313;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr318;
		} else if ( (*p) >= 65 )
			goto tr318;
	} else
		goto tr318;
	goto tr7;
case 99:
	switch( (*p) ) {
		case 34: goto tr319;
		case 39: goto tr320;
		case 92: goto tr321;
		case 97: goto tr322;
		case 98: goto tr323;
		case 102: goto tr324;
		case 110: goto tr325;
		case 114: goto tr326;
		case 116: goto tr327;
		case 118: goto tr328;
		case 120: goto tr329;
	}
	goto tr7;
case 100:
	switch( (*p) ) {
		case 34: goto tr331;
		case 92: goto tr332;
	}
	goto tr330;
case 101:
	switch( (*p) ) {
		case 34: goto tr334;
		case 92: goto tr335;
	}
	goto tr333;
case 102:
	switch( (*p) ) {
		case 34: goto tr337;
		case 92: goto tr338;
	}
	goto tr336;
case 103:
	switch( (*p) ) {
		case 34: goto tr340;
		case 92: goto tr341;
	}
	goto tr339;
case 104:
	switch( (*p) ) {
		case 34: goto tr343;
		case 92: goto tr344;
	}
	goto tr342;
case 105:
	switch( (*p) ) {
		case 34: goto tr346;
		case 92: goto tr347;
	}
	goto tr345;
case 106:
	switch( (*p) ) {
		case 34: goto tr349;
		case 92: goto tr350;
	}
	goto tr348;
case 107:
	switch( (*p) ) {
		case 34: goto tr352;
		case 92: goto tr353;
	}
	goto tr351;
case 108:
	switch( (*p) ) {
		case 34: goto tr355;
		case 92: goto tr356;
	}
	goto tr354;
case 109:
	switch( (*p) ) {
		case 34: goto tr358;
		case 92: goto tr359;
	}
	goto tr357;
case 110:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr360;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr360;
	} else
		goto tr360;
	goto tr7;
case 111:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr361;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr361;
	} else
		goto tr361;
	goto tr7;
case 112:
	switch( (*p) ) {
		case 34: goto tr363;
		case 92: goto tr364;
	}
	goto tr362;
case 113:
	if ( (*p) == 10 )
		goto tr92;
	goto tr94;
case 114:
	switch( (*p) ) {
		case 10: goto tr366;
		case 32: goto tr365;
		case 35: goto tr367;
		case 41: goto tr368;
		case 44: goto tr369;
		case 95: goto tr370;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr365;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr370;
		} else if ( (*p) >= 65 )
			goto tr370;
	} else
		goto tr370;
	goto tr7;
case 115:
	switch( (*p) ) {
		case 34: goto tr371;
		case 39: goto tr372;
		case 92: goto tr373;
		case 97: goto tr374;
		case 98: goto tr375;
		case 102: goto tr376;
		case 110: goto tr377;
		case 114: goto tr378;
		case 116: goto tr379;
		case 118: goto tr380;
		case 120: goto tr381;
	}
	goto tr7;
case 116:
	switch( (*p) ) {
		case 34: goto tr383;
		case 92: goto tr384;
	}
	goto tr382;
case 117:
	switch( (*p) ) {
		case 34: goto tr386;
		case 92: goto tr387;
	}
	goto tr385;
case 118:
	switch( (*p) ) {
		case 34: goto tr389;
		case 92: goto tr390;
	}
	goto tr388;
case 119:
	switch( (*p) ) {
		case 34: goto tr392;
		case 92: goto tr393;
	}
	goto tr391;
case 120:
	switch( (*p) ) {
		case 34: goto tr395;
		case 92: goto tr396;
	}
	goto tr394;
case 121:
	switch( (*p) ) {
		case 34: goto tr398;
		case 92: goto tr399;
	}
	goto tr397;
case 122:
	switch( (*p) ) {
		case 34: goto tr401;
		case 92: goto tr402;
	}
	goto tr400;
case 123:
	switch( (*p) ) {
		case 34: goto tr404;
		case 92: goto tr405;
	}
	goto tr403;
case 124:
	switch( (*p) ) {
		case 34: goto tr407;
		case 92: goto tr408;
	}
	goto tr406;
case 125:
	switch( (*p) ) {
		case 34: goto tr410;
		case 92: goto tr411;
	}
	goto tr409;
case 126:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr412;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr412;
	} else
		goto tr412;
	goto tr7;
case 127:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr413;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr413;
	} else
		goto tr413;
	goto tr7;
case 128:
	switch( (*p) ) {
		case 34: goto tr415;
		case 92: goto tr416;
	}
	goto tr414;
case 129:
	if ( (*p) == 10 )
		goto tr72;
	goto tr74;
case 130:
	switch( (*p) ) {
		case 10: goto tr418;
		case 32: goto tr417;
		case 35: goto tr419;
		case 40: goto tr420;
		case 59: goto tr422;
		case 95: goto tr421;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr417;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr421;
		} else if ( (*p) >= 65 )
			goto tr421;
	} else
		goto tr421;
	goto tr7;
case 131:
	switch( (*p) ) {
		case 34: goto tr423;
		case 39: goto tr424;
		case 92: goto tr425;
		case 97: goto tr426;
		case 98: goto tr427;
		case 102: goto tr428;
		case 110: goto tr429;
		case 114: goto tr430;
		case 116: goto tr431;
		case 118: goto tr432;
		case 120: goto tr433;
	}
	goto tr7;
case 132:
	switch( (*p) ) {
		case 34: goto tr435;
		case 92: goto tr436;
	}
	goto tr434;
case 133:
	switch( (*p) ) {
		case 34: goto tr438;
		case 92: goto tr439;
	}
	goto tr437;
case 134:
	switch( (*p) ) {
		case 34: goto tr441;
		case 92: goto tr442;
	}
	goto tr440;
case 135:
	switch( (*p) ) {
		case 34: goto tr444;
		case 92: goto tr445;
	}
	goto tr443;
case 136:
	switch( (*p) ) {
		case 34: goto tr447;
		case 92: goto tr448;
	}
	goto tr446;
case 137:
	switch( (*p) ) {
		case 34: goto tr450;
		case 92: goto tr451;
	}
	goto tr449;
case 138:
	switch( (*p) ) {
		case 34: goto tr453;
		case 92: goto tr454;
	}
	goto tr452;
case 139:
	switch( (*p) ) {
		case 34: goto tr456;
		case 92: goto tr457;
	}
	goto tr455;
case 140:
	switch( (*p) ) {
		case 34: goto tr459;
		case 92: goto tr460;
	}
	goto tr458;
case 141:
	switch( (*p) ) {
		case 34: goto tr462;
		case 92: goto tr463;
	}
	goto tr461;
case 142:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr464;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr464;
	} else
		goto tr464;
	goto tr7;
case 143:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr465;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr465;
	} else
		goto tr465;
	goto tr7;
case 144:
	switch( (*p) ) {
		case 34: goto tr467;
		case 92: goto tr468;
	}
	goto tr466;
case 145:
	if ( (*p) == 10 )
		goto tr49;
	goto tr51;
case 146:
	switch( (*p) ) {
		case 10: goto tr470;
		case 32: goto tr469;
		case 35: goto tr471;
		case 46: goto tr472;
		case 95: goto tr473;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr469;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr473;
		} else if ( (*p) >= 65 )
			goto tr473;
	} else
		goto tr473;
	goto tr7;
case 147:
	switch( (*p) ) {
		case 34: goto tr474;
		case 39: goto tr475;
		case 92: goto tr476;
		case 97: goto tr477;
		case 98: goto tr478;
		case 102: goto tr479;
		case 110: goto tr480;
		case 114: goto tr481;
		case 116: goto tr482;
		case 118: goto tr483;
		case 120: goto tr484;
	}
	goto tr7;
case 148:
	switch( (*p) ) {
		case 34: goto tr486;
		case 92: goto tr487;
	}
	goto tr485;
case 149:
	switch( (*p) ) {
		case 34: goto tr489;
		case 92: goto tr490;
	}
	goto tr488;
case 150:
	switch( (*p) ) {
		case 34: goto tr492;
		case 92: goto tr493;
	}
	goto tr491;
case 151:
	switch( (*p) ) {
		case 34: goto tr495;
		case 92: goto tr496;
	}
	goto tr494;
case 152:
	switch( (*p) ) {
		case 34: goto tr498;
		case 92: goto tr499;
	}
	goto tr497;
case 153:
	switch( (*p) ) {
		case 34: goto tr501;
		case 92: goto tr502;
	}
	goto tr500;
case 154:
	switch( (*p) ) {
		case 34: goto tr504;
		case 92: goto tr505;
	}
	goto tr503;
case 155:
	switch( (*p) ) {
		case 34: goto tr507;
		case 92: goto tr508;
	}
	goto tr506;
case 156:
	switch( (*p) ) {
		case 34: goto tr510;
		case 92: goto tr511;
	}
	goto tr509;
case 157:
	switch( (*p) ) {
		case 34: goto tr513;
		case 92: goto tr514;
	}
	goto tr512;
case 158:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr515;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr515;
	} else
		goto tr515;
	goto tr7;
case 159:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr516;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr516;
	} else
		goto tr516;
	goto tr7;
case 160:
	switch( (*p) ) {
		case 34: goto tr518;
		case 92: goto tr519;
	}
	goto tr517;
case 161:
	if ( (*p) == 10 )
		goto tr25;
	goto tr27;
case 162:
	switch( (*p) ) {
		case 10: goto tr521;
		case 32: goto tr520;
		case 35: goto tr522;
		case 44: goto tr523;
		case 61: goto tr525;
		case 95: goto tr524;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr520;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr524;
		} else if ( (*p) >= 65 )
			goto tr524;
	} else
		goto tr524;
	goto tr7;
case 163:
	switch( (*p) ) {
		case 34: goto tr526;
		case 39: goto tr527;
		case 92: goto tr528;
		case 97: goto tr529;
		case 98: goto tr530;
		case 102: goto tr531;
		case 110: goto tr532;
		case 114: goto tr533;
		case 116: goto tr534;
		case 118: goto tr535;
		case 120: goto tr536;
	}
	goto tr7;
case 164:
	switch( (*p) ) {
		case 34: goto tr538;
		case 92: goto tr539;
	}
	goto tr537;
case 165:
	switch( (*p) ) {
		case 34: goto tr541;
		case 92: goto tr542;
	}
	goto tr540;
case 166:
	switch( (*p) ) {
		case 34: goto tr544;
		case 92: goto tr545;
	}
	goto tr543;
case 167:
	switch( (*p) ) {
		case 34: goto tr547;
		case 92: goto tr548;
	}
	goto tr546;
case 168:
	switch( (*p) ) {
		case 34: goto tr550;
		case 92: goto tr551;
	}
	goto tr549;
case 169:
	switch( (*p) ) {
		case 34: goto tr553;
		case 92: goto tr554;
	}
	goto tr552;
case 170:
	switch( (*p) ) {
		case 34: goto tr556;
		case 92: goto tr557;
	}
	goto tr555;
case 171:
	switch( (*p) ) {
		case 34: goto tr559;
		case 92: goto tr560;
	}
	goto tr558;
case 172:
	switch( (*p) ) {
		case 34: goto tr562;
		case 92: goto tr563;
	}
	goto tr561;
case 173:
	switch( (*p) ) {
		case 34: goto tr565;
		case 92: goto tr566;
	}
	goto tr564;
case 174:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr567;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr567;
	} else
		goto tr567;
	goto tr7;
case 175:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr568;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr568;
	} else
		goto tr568;
	goto tr7;
case 176:
	switch( (*p) ) {
		case 34: goto tr570;
		case 92: goto tr571;
	}
	goto tr569;
	}

	tr7: cs = 0; goto _again;
	tr573: cs = 1; goto _again;
	tr577: cs = 1; goto f60;
	tr583: cs = 1; goto f63;
	tr0: cs = 2; goto f0;
	tr3: cs = 2; goto f2;
	tr537: cs = 2; goto f26;
	tr540: cs = 2; goto f28;
	tr543: cs = 2; goto f30;
	tr546: cs = 2; goto f32;
	tr549: cs = 2; goto f34;
	tr552: cs = 2; goto f36;
	tr555: cs = 2; goto f38;
	tr558: cs = 2; goto f40;
	tr561: cs = 2; goto f42;
	tr564: cs = 2; goto f44;
	tr569: cs = 2; goto f48;
	tr1: cs = 3; goto f1;
	tr4: cs = 3; goto f3;
	tr538: cs = 3; goto f27;
	tr541: cs = 3; goto f29;
	tr544: cs = 3; goto f31;
	tr547: cs = 3; goto f33;
	tr550: cs = 3; goto f35;
	tr553: cs = 3; goto f37;
	tr556: cs = 3; goto f39;
	tr559: cs = 3; goto f41;
	tr562: cs = 3; goto f43;
	tr565: cs = 3; goto f45;
	tr570: cs = 3; goto f49;
	tr13: cs = 4; goto _again;
	tr6: cs = 4; goto f4;
	tr8: cs = 4; goto f5;
	tr14: cs = 4; goto f6;
	tr148: cs = 4; goto f23;
	tr149: cs = 4; goto f24;
	tr15: cs = 5; goto _again;
	tr9: cs = 5; goto f4;
	tr150: cs = 5; goto f23;
	tr16: cs = 6; goto _again;
	tr10: cs = 6; goto f4;
	tr38: cs = 6; goto f11;
	tr151: cs = 6; goto f23;
	tr523: cs = 6; goto f50;
	tr24: cs = 7; goto _again;
	tr25: cs = 7; goto f6;
	tr19: cs = 7; goto f7;
	tr20: cs = 7; goto f8;
	tr26: cs = 8; goto _again;
	tr21: cs = 8; goto f7;
	tr29: cs = 9; goto f0;
	tr32: cs = 9; goto f2;
	tr485: cs = 9; goto f26;
	tr488: cs = 9; goto f28;
	tr491: cs = 9; goto f30;
	tr494: cs = 9; goto f32;
	tr497: cs = 9; goto f34;
	tr500: cs = 9; goto f36;
	tr503: cs = 9; goto f38;
	tr506: cs = 9; goto f40;
	tr509: cs = 9; goto f42;
	tr512: cs = 9; goto f44;
	tr517: cs = 9; goto f48;
	tr30: cs = 10; goto f1;
	tr33: cs = 10; goto f3;
	tr486: cs = 10; goto f27;
	tr489: cs = 10; goto f29;
	tr492: cs = 10; goto f31;
	tr495: cs = 10; goto f33;
	tr498: cs = 10; goto f35;
	tr501: cs = 10; goto f37;
	tr504: cs = 10; goto f39;
	tr507: cs = 10; goto f41;
	tr510: cs = 10; goto f43;
	tr513: cs = 10; goto f45;
	tr518: cs = 10; goto f49;
	tr40: cs = 11; goto _again;
	tr41: cs = 11; goto f6;
	tr35: cs = 11; goto f11;
	tr36: cs = 11; goto f12;
	tr520: cs = 11; goto f50;
	tr521: cs = 11; goto f51;
	tr42: cs = 12; goto _again;
	tr37: cs = 12; goto f11;
	tr522: cs = 12; goto f50;
	tr18: cs = 13; goto _again;
	tr12: cs = 13; goto f4;
	tr39: cs = 13; goto f11;
	tr154: cs = 13; goto f23;
	tr525: cs = 13; goto f50;
	tr48: cs = 14; goto _again;
	tr49: cs = 14; goto f6;
	tr43: cs = 14; goto f13;
	tr44: cs = 14; goto f14;
	tr50: cs = 15; goto _again;
	tr45: cs = 15; goto f13;
	tr53: cs = 16; goto f0;
	tr56: cs = 16; goto f2;
	tr434: cs = 16; goto f26;
	tr437: cs = 16; goto f28;
	tr440: cs = 16; goto f30;
	tr443: cs = 16; goto f32;
	tr446: cs = 16; goto f34;
	tr449: cs = 16; goto f36;
	tr452: cs = 16; goto f38;
	tr455: cs = 16; goto f40;
	tr458: cs = 16; goto f42;
	tr461: cs = 16; goto f44;
	tr466: cs = 16; goto f48;
	tr54: cs = 17; goto f1;
	tr57: cs = 17; goto f3;
	tr435: cs = 17; goto f27;
	tr438: cs = 17; goto f29;
	tr441: cs = 17; goto f31;
	tr444: cs = 17; goto f33;
	tr447: cs = 17; goto f35;
	tr450: cs = 17; goto f37;
	tr453: cs = 17; goto f39;
	tr456: cs = 17; goto f41;
	tr459: cs = 17; goto f43;
	tr462: cs = 17; goto f45;
	tr467: cs = 17; goto f49;
	tr63: cs = 18; goto _again;
	tr64: cs = 18; goto f6;
	tr59: cs = 18; goto f11;
	tr60: cs = 18; goto f12;
	tr469: cs = 18; goto f50;
	tr470: cs = 18; goto f51;
	tr65: cs = 19; goto _again;
	tr61: cs = 19; goto f11;
	tr471: cs = 19; goto f50;
	tr17: cs = 20; goto _again;
	tr11: cs = 20; goto f4;
	tr62: cs = 20; goto f11;
	tr152: cs = 20; goto f23;
	tr472: cs = 20; goto f50;
	tr71: cs = 21; goto _again;
	tr72: cs = 21; goto f6;
	tr66: cs = 21; goto f16;
	tr67: cs = 21; goto f17;
	tr73: cs = 22; goto _again;
	tr68: cs = 22; goto f16;
	tr76: cs = 23; goto f0;
	tr79: cs = 23; goto f2;
	tr382: cs = 23; goto f26;
	tr385: cs = 23; goto f28;
	tr388: cs = 23; goto f30;
	tr391: cs = 23; goto f32;
	tr394: cs = 23; goto f34;
	tr397: cs = 23; goto f36;
	tr400: cs = 23; goto f38;
	tr403: cs = 23; goto f40;
	tr406: cs = 23; goto f42;
	tr409: cs = 23; goto f44;
	tr414: cs = 23; goto f48;
	tr77: cs = 24; goto f1;
	tr80: cs = 24; goto f3;
	tr383: cs = 24; goto f27;
	tr386: cs = 24; goto f29;
	tr389: cs = 24; goto f31;
	tr392: cs = 24; goto f33;
	tr395: cs = 24; goto f35;
	tr398: cs = 24; goto f37;
	tr401: cs = 24; goto f39;
	tr404: cs = 24; goto f41;
	tr407: cs = 24; goto f43;
	tr410: cs = 24; goto f45;
	tr415: cs = 24; goto f49;
	tr87: cs = 25; goto _again;
	tr88: cs = 25; goto f6;
	tr82: cs = 25; goto f19;
	tr83: cs = 25; goto f20;
	tr417: cs = 25; goto f58;
	tr418: cs = 25; goto f59;
	tr89: cs = 26; goto _again;
	tr84: cs = 26; goto f19;
	tr419: cs = 26; goto f58;
	tr90: cs = 27; goto _again;
	tr92: cs = 27; goto f6;
	tr85: cs = 27; goto f19;
	tr420: cs = 27; goto f58;
	tr93: cs = 28; goto _again;
	tr97: cs = 29; goto f0;
	tr100: cs = 29; goto f2;
	tr330: cs = 29; goto f26;
	tr333: cs = 29; goto f28;
	tr336: cs = 29; goto f30;
	tr339: cs = 29; goto f32;
	tr342: cs = 29; goto f34;
	tr345: cs = 29; goto f36;
	tr348: cs = 29; goto f38;
	tr351: cs = 29; goto f40;
	tr354: cs = 29; goto f42;
	tr357: cs = 29; goto f44;
	tr362: cs = 29; goto f48;
	tr98: cs = 30; goto f1;
	tr101: cs = 30; goto f3;
	tr331: cs = 30; goto f27;
	tr334: cs = 30; goto f29;
	tr337: cs = 30; goto f31;
	tr340: cs = 30; goto f33;
	tr343: cs = 30; goto f35;
	tr346: cs = 30; goto f37;
	tr349: cs = 30; goto f39;
	tr352: cs = 30; goto f41;
	tr355: cs = 30; goto f43;
	tr358: cs = 30; goto f45;
	tr363: cs = 30; goto f49;
	tr108: cs = 31; goto _again;
	tr109: cs = 31; goto f6;
	tr103: cs = 31; goto f21;
	tr104: cs = 31; goto f22;
	tr262: cs = 31; goto f52;
	tr263: cs = 31; goto f53;
	tr313: cs = 31; goto f54;
	tr314: cs = 31; goto f55;
	tr365: cs = 31; goto f56;
	tr366: cs = 31; goto f57;
	tr110: cs = 32; goto _again;
	tr105: cs = 32; goto f21;
	tr264: cs = 32; goto f52;
	tr315: cs = 32; goto f54;
	tr367: cs = 32; goto f56;
	tr95: cs = 33; goto _again;
	tr112: cs = 33; goto f6;
	tr106: cs = 33; goto f21;
	tr265: cs = 33; goto f52;
	tr316: cs = 33; goto f54;
	tr368: cs = 33; goto f56;
	tr113: cs = 34; goto _again;
	tr114: cs = 35; goto _again;
	tr578: cs = 35; goto f60;
	tr584: cs = 35; goto f63;
	tr116: cs = 36; goto _again;
	tr117: cs = 36; goto f6;
	tr579: cs = 36; goto f60;
	tr585: cs = 36; goto f63;
	tr118: cs = 37; goto _again;
	tr121: cs = 38; goto f0;
	tr124: cs = 38; goto f2;
	tr212: cs = 38; goto f26;
	tr215: cs = 38; goto f28;
	tr218: cs = 38; goto f30;
	tr221: cs = 38; goto f32;
	tr224: cs = 38; goto f34;
	tr227: cs = 38; goto f36;
	tr230: cs = 38; goto f38;
	tr233: cs = 38; goto f40;
	tr236: cs = 38; goto f42;
	tr239: cs = 38; goto f44;
	tr244: cs = 38; goto f48;
	tr122: cs = 39; goto f1;
	tr125: cs = 39; goto f3;
	tr213: cs = 39; goto f27;
	tr216: cs = 39; goto f29;
	tr219: cs = 39; goto f31;
	tr222: cs = 39; goto f33;
	tr225: cs = 39; goto f35;
	tr228: cs = 39; goto f37;
	tr231: cs = 39; goto f39;
	tr234: cs = 39; goto f41;
	tr237: cs = 39; goto f43;
	tr240: cs = 39; goto f45;
	tr245: cs = 39; goto f49;
	tr131: cs = 40; goto _again;
	tr132: cs = 40; goto f6;
	tr127: cs = 40; goto f11;
	tr128: cs = 40; goto f12;
	tr247: cs = 40; goto f50;
	tr248: cs = 40; goto f51;
	tr133: cs = 41; goto _again;
	tr129: cs = 41; goto f11;
	tr249: cs = 41; goto f50;
	tr134: cs = 42; goto _again;
	tr135: cs = 42; goto f6;
	tr130: cs = 42; goto f11;
	tr251: cs = 42; goto f50;
	tr136: cs = 43; goto _again;
	tr138: cs = 44; goto f0;
	tr141: cs = 44; goto f2;
	tr166: cs = 44; goto f26;
	tr169: cs = 44; goto f28;
	tr172: cs = 44; goto f30;
	tr175: cs = 44; goto f32;
	tr178: cs = 44; goto f34;
	tr181: cs = 44; goto f36;
	tr184: cs = 44; goto f38;
	tr187: cs = 44; goto f40;
	tr190: cs = 44; goto f42;
	tr193: cs = 44; goto f44;
	tr198: cs = 44; goto f48;
	tr144: cs = 45; goto _again;
	tr139: cs = 45; goto f1;
	tr142: cs = 45; goto f3;
	tr145: cs = 45; goto f6;
	tr167: cs = 45; goto f27;
	tr170: cs = 45; goto f29;
	tr173: cs = 45; goto f31;
	tr176: cs = 45; goto f33;
	tr179: cs = 45; goto f35;
	tr182: cs = 45; goto f37;
	tr185: cs = 45; goto f39;
	tr188: cs = 45; goto f41;
	tr191: cs = 45; goto f43;
	tr194: cs = 45; goto f45;
	tr199: cs = 45; goto f49;
	tr146: cs = 46; goto _again;
	tr153: cs = 47; goto _again;
	tr574: cs = 47; goto f10;
	tr580: cs = 47; goto f62;
	tr586: cs = 47; goto f65;
	tr140: cs = 48; goto f1;
	tr143: cs = 48; goto f3;
	tr168: cs = 48; goto f27;
	tr171: cs = 48; goto f29;
	tr174: cs = 48; goto f31;
	tr177: cs = 48; goto f33;
	tr180: cs = 48; goto f35;
	tr183: cs = 48; goto f37;
	tr186: cs = 48; goto f39;
	tr189: cs = 48; goto f41;
	tr192: cs = 48; goto f43;
	tr195: cs = 48; goto f45;
	tr200: cs = 48; goto f49;
	tr155: cs = 49; goto _again;
	tr156: cs = 50; goto _again;
	tr157: cs = 51; goto _again;
	tr158: cs = 52; goto _again;
	tr159: cs = 53; goto _again;
	tr160: cs = 54; goto _again;
	tr161: cs = 55; goto _again;
	tr162: cs = 56; goto _again;
	tr163: cs = 57; goto _again;
	tr164: cs = 58; goto _again;
	tr165: cs = 59; goto f25;
	tr196: cs = 60; goto f46;
	tr197: cs = 61; goto f47;
	tr137: cs = 62; goto _again;
	tr123: cs = 63; goto f1;
	tr126: cs = 63; goto f3;
	tr214: cs = 63; goto f27;
	tr217: cs = 63; goto f29;
	tr220: cs = 63; goto f31;
	tr223: cs = 63; goto f33;
	tr226: cs = 63; goto f35;
	tr229: cs = 63; goto f37;
	tr232: cs = 63; goto f39;
	tr235: cs = 63; goto f41;
	tr238: cs = 63; goto f43;
	tr241: cs = 63; goto f45;
	tr246: cs = 63; goto f49;
	tr201: cs = 64; goto _again;
	tr202: cs = 65; goto _again;
	tr203: cs = 66; goto _again;
	tr204: cs = 67; goto _again;
	tr205: cs = 68; goto _again;
	tr206: cs = 69; goto _again;
	tr207: cs = 70; goto _again;
	tr208: cs = 71; goto _again;
	tr209: cs = 72; goto _again;
	tr210: cs = 73; goto _again;
	tr211: cs = 74; goto f25;
	tr242: cs = 75; goto f46;
	tr243: cs = 76; goto f47;
	tr119: cs = 77; goto _again;
	tr250: cs = 78; goto _again;
	tr120: cs = 78; goto f10;
	tr111: cs = 79; goto _again;
	tr252: cs = 79; goto f6;
	tr107: cs = 79; goto f21;
	tr266: cs = 79; goto f52;
	tr317: cs = 79; goto f54;
	tr369: cs = 79; goto f56;
	tr253: cs = 80; goto _again;
	tr256: cs = 81; goto f0;
	tr259: cs = 81; goto f2;
	tr278: cs = 81; goto f26;
	tr281: cs = 81; goto f28;
	tr284: cs = 81; goto f30;
	tr287: cs = 81; goto f32;
	tr290: cs = 81; goto f34;
	tr293: cs = 81; goto f36;
	tr296: cs = 81; goto f38;
	tr299: cs = 81; goto f40;
	tr302: cs = 81; goto f42;
	tr305: cs = 81; goto f44;
	tr310: cs = 81; goto f48;
	tr257: cs = 82; goto f1;
	tr260: cs = 82; goto f3;
	tr279: cs = 82; goto f27;
	tr282: cs = 82; goto f29;
	tr285: cs = 82; goto f31;
	tr288: cs = 82; goto f33;
	tr291: cs = 82; goto f35;
	tr294: cs = 82; goto f37;
	tr297: cs = 82; goto f39;
	tr300: cs = 82; goto f41;
	tr303: cs = 82; goto f43;
	tr306: cs = 82; goto f45;
	tr311: cs = 82; goto f49;
	tr258: cs = 83; goto f1;
	tr261: cs = 83; goto f3;
	tr280: cs = 83; goto f27;
	tr283: cs = 83; goto f29;
	tr286: cs = 83; goto f31;
	tr289: cs = 83; goto f33;
	tr292: cs = 83; goto f35;
	tr295: cs = 83; goto f37;
	tr298: cs = 83; goto f39;
	tr301: cs = 83; goto f41;
	tr304: cs = 83; goto f43;
	tr307: cs = 83; goto f45;
	tr312: cs = 83; goto f49;
	tr267: cs = 84; goto _again;
	tr268: cs = 85; goto _again;
	tr269: cs = 86; goto _again;
	tr270: cs = 87; goto _again;
	tr271: cs = 88; goto _again;
	tr272: cs = 89; goto _again;
	tr273: cs = 90; goto _again;
	tr274: cs = 91; goto _again;
	tr275: cs = 92; goto _again;
	tr276: cs = 93; goto _again;
	tr277: cs = 94; goto f25;
	tr308: cs = 95; goto f46;
	tr309: cs = 96; goto f47;
	tr254: cs = 97; goto _again;
	tr318: cs = 98; goto _again;
	tr255: cs = 98; goto f10;
	tr99: cs = 99; goto f1;
	tr102: cs = 99; goto f3;
	tr332: cs = 99; goto f27;
	tr335: cs = 99; goto f29;
	tr338: cs = 99; goto f31;
	tr341: cs = 99; goto f33;
	tr344: cs = 99; goto f35;
	tr347: cs = 99; goto f37;
	tr350: cs = 99; goto f39;
	tr353: cs = 99; goto f41;
	tr356: cs = 99; goto f43;
	tr359: cs = 99; goto f45;
	tr364: cs = 99; goto f49;
	tr319: cs = 100; goto _again;
	tr320: cs = 101; goto _again;
	tr321: cs = 102; goto _again;
	tr322: cs = 103; goto _again;
	tr323: cs = 104; goto _again;
	tr324: cs = 105; goto _again;
	tr325: cs = 106; goto _again;
	tr326: cs = 107; goto _again;
	tr327: cs = 108; goto _again;
	tr328: cs = 109; goto _again;
	tr329: cs = 110; goto f25;
	tr360: cs = 111; goto f46;
	tr361: cs = 112; goto f47;
	tr94: cs = 113; goto _again;
	tr370: cs = 114; goto _again;
	tr96: cs = 114; goto f10;
	tr78: cs = 115; goto f1;
	tr81: cs = 115; goto f3;
	tr384: cs = 115; goto f27;
	tr387: cs = 115; goto f29;
	tr390: cs = 115; goto f31;
	tr393: cs = 115; goto f33;
	tr396: cs = 115; goto f35;
	tr399: cs = 115; goto f37;
	tr402: cs = 115; goto f39;
	tr405: cs = 115; goto f41;
	tr408: cs = 115; goto f43;
	tr411: cs = 115; goto f45;
	tr416: cs = 115; goto f49;
	tr371: cs = 116; goto _again;
	tr372: cs = 117; goto _again;
	tr373: cs = 118; goto _again;
	tr374: cs = 119; goto _again;
	tr375: cs = 120; goto _again;
	tr376: cs = 121; goto _again;
	tr377: cs = 122; goto _again;
	tr378: cs = 123; goto _again;
	tr379: cs = 124; goto _again;
	tr380: cs = 125; goto _again;
	tr381: cs = 126; goto f25;
	tr412: cs = 127; goto f46;
	tr413: cs = 128; goto f47;
	tr74: cs = 129; goto _again;
	tr69: cs = 129; goto f16;
	tr421: cs = 130; goto _again;
	tr75: cs = 130; goto f10;
	tr70: cs = 130; goto f18;
	tr55: cs = 131; goto f1;
	tr58: cs = 131; goto f3;
	tr436: cs = 131; goto f27;
	tr439: cs = 131; goto f29;
	tr442: cs = 131; goto f31;
	tr445: cs = 131; goto f33;
	tr448: cs = 131; goto f35;
	tr451: cs = 131; goto f37;
	tr454: cs = 131; goto f39;
	tr457: cs = 131; goto f41;
	tr460: cs = 131; goto f43;
	tr463: cs = 131; goto f45;
	tr468: cs = 131; goto f49;
	tr423: cs = 132; goto _again;
	tr424: cs = 133; goto _again;
	tr425: cs = 134; goto _again;
	tr426: cs = 135; goto _again;
	tr427: cs = 136; goto _again;
	tr428: cs = 137; goto _again;
	tr429: cs = 138; goto _again;
	tr430: cs = 139; goto _again;
	tr431: cs = 140; goto _again;
	tr432: cs = 141; goto _again;
	tr433: cs = 142; goto f25;
	tr464: cs = 143; goto f46;
	tr465: cs = 144; goto f47;
	tr51: cs = 145; goto _again;
	tr46: cs = 145; goto f13;
	tr473: cs = 146; goto _again;
	tr52: cs = 146; goto f10;
	tr47: cs = 146; goto f15;
	tr31: cs = 147; goto f1;
	tr34: cs = 147; goto f3;
	tr487: cs = 147; goto f27;
	tr490: cs = 147; goto f29;
	tr493: cs = 147; goto f31;
	tr496: cs = 147; goto f33;
	tr499: cs = 147; goto f35;
	tr502: cs = 147; goto f37;
	tr505: cs = 147; goto f39;
	tr508: cs = 147; goto f41;
	tr511: cs = 147; goto f43;
	tr514: cs = 147; goto f45;
	tr519: cs = 147; goto f49;
	tr474: cs = 148; goto _again;
	tr475: cs = 149; goto _again;
	tr476: cs = 150; goto _again;
	tr477: cs = 151; goto _again;
	tr478: cs = 152; goto _again;
	tr479: cs = 153; goto _again;
	tr480: cs = 154; goto _again;
	tr481: cs = 155; goto _again;
	tr482: cs = 156; goto _again;
	tr483: cs = 157; goto _again;
	tr484: cs = 158; goto f25;
	tr515: cs = 159; goto f46;
	tr516: cs = 160; goto f47;
	tr27: cs = 161; goto _again;
	tr22: cs = 161; goto f7;
	tr524: cs = 162; goto _again;
	tr23: cs = 162; goto f9;
	tr28: cs = 162; goto f10;
	tr2: cs = 163; goto f1;
	tr5: cs = 163; goto f3;
	tr539: cs = 163; goto f27;
	tr542: cs = 163; goto f29;
	tr545: cs = 163; goto f31;
	tr548: cs = 163; goto f33;
	tr551: cs = 163; goto f35;
	tr554: cs = 163; goto f37;
	tr557: cs = 163; goto f39;
	tr560: cs = 163; goto f41;
	tr563: cs = 163; goto f43;
	tr566: cs = 163; goto f45;
	tr571: cs = 163; goto f49;
	tr526: cs = 164; goto _again;
	tr527: cs = 165; goto _again;
	tr528: cs = 166; goto _again;
	tr529: cs = 167; goto _again;
	tr530: cs = 168; goto _again;
	tr531: cs = 169; goto _again;
	tr532: cs = 170; goto _again;
	tr533: cs = 171; goto _again;
	tr534: cs = 172; goto _again;
	tr535: cs = 173; goto _again;
	tr536: cs = 174; goto f25;
	tr567: cs = 175; goto f46;
	tr568: cs = 176; goto f47;
	tr572: cs = 177; goto _again;
	tr115: cs = 177; goto f6;
	tr575: cs = 177; goto f60;
	tr576: cs = 177; goto f61;
	tr581: cs = 177; goto f63;
	tr582: cs = 177; goto f64;
	tr91: cs = 178; goto _again;
	tr86: cs = 178; goto f19;
	tr422: cs = 178; goto f58;
	tr147: cs = 179; goto _again;

f10:
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f6:
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f31:
#line 52 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\\", 1); }
	goto _again;
f27:
#line 53 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\"", 1); }
	goto _again;
f29:
#line 54 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "'",  1); }
	goto _again;
f33:
#line 55 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\a",  1); }
	goto _again;
f35:
#line 56 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\b",  1); }
	goto _again;
f37:
#line 57 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\f",  1); }
	goto _again;
f39:
#line 58 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\n",  1); }
	goto _again;
f41:
#line 59 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\r",  1); }
	goto _again;
f43:
#line 60 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\t",  1); }
	goto _again;
f45:
#line 61 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\v",  1); }
	goto _again;
f47:
#line 63 "libswanson/s0-parser.c.rl"
	{ hex_char |= digit_values[(int) (*p)]; }
	goto _again;
f25:
#line 64 "libswanson/s0-parser.c.rl"
	{ hex_char = 0; }
	goto _again;
f49:
#line 66 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, &hex_char, 1); }
	goto _again;
f3:
#line 73 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, xs, (p-xs)); }
	goto _again;
f1:
#line 75 "libswanson/s0-parser.c.rl"
	{ cork_buffer_clear(&scratch); }
	goto _again;
f11:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
	goto _again;
f16:
#line 82 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_set_thing(s, call, id); }
	goto _again;
f13:
#line 84 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_add_result(s, call, id); }
	goto _again;
f7:
#line 87 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_add_result(s, call, id); }
	goto _again;
f60:
#line 105 "libswanson/s0-parser.c.rl"
	{ swan_ast_add_element(s, ast, &call->parent); }
	goto _again;
f63:
#line 108 "libswanson/s0-parser.c.rl"
	{
                 contents = swan_static_string_new(s, scratch.buf);
                 ast_string = swan_ast_string_new(s, id, contents);
                 swan_ast_add_element(s, ast, &ast_string->parent);
             }
	goto _again;
f50:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
	goto _again;
f30:
#line 52 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\\", 1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f26:
#line 53 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\"", 1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f28:
#line 54 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "'",  1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f32:
#line 55 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\a",  1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f34:
#line 56 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\b",  1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f36:
#line 57 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\f",  1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f38:
#line 58 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\n",  1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f40:
#line 59 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\r",  1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f42:
#line 60 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\t",  1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f44:
#line 61 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\v",  1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f46:
#line 63 "libswanson/s0-parser.c.rl"
	{ hex_char |= digit_values[(int) (*p)]; }
#line 65 "libswanson/s0-parser.c.rl"
	{ hex_char <<= 4; }
	goto _again;
f48:
#line 66 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, &hex_char, 1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f2:
#line 73 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, xs, (p-xs)); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f0:
#line 75 "libswanson/s0-parser.c.rl"
	{ cork_buffer_clear(&scratch); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f12:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f21:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 94 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_add_param(s, call, id); }
	goto _again;
f52:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 96 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_add_param(s, call, id); }
	goto _again;
f4:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 101 "libswanson/s0-parser.c.rl"
	{ call = swan_ast_call_new(s); }
	goto _again;
f19:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 103 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_set_method(s, call, id); }
	goto _again;
f18:
#line 82 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_set_thing(s, call, id); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f17:
#line 82 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_set_thing(s, call, id); }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f15:
#line 84 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_add_result(s, call, id); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f14:
#line 84 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_add_result(s, call, id); }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f9:
#line 87 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_add_result(s, call, id); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f8:
#line 87 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_add_result(s, call, id); }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f62:
#line 105 "libswanson/s0-parser.c.rl"
	{ swan_ast_add_element(s, ast, &call->parent); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f61:
#line 105 "libswanson/s0-parser.c.rl"
	{ swan_ast_add_element(s, ast, &call->parent); }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f65:
#line 108 "libswanson/s0-parser.c.rl"
	{
                 contents = swan_static_string_new(s, scratch.buf);
                 ast_string = swan_ast_string_new(s, id, contents);
                 swan_ast_add_element(s, ast, &ast_string->parent);
             }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f64:
#line 108 "libswanson/s0-parser.c.rl"
	{
                 contents = swan_static_string_new(s, scratch.buf);
                 ast_string = swan_ast_string_new(s, id, contents);
                 swan_ast_add_element(s, ast, &ast_string->parent);
             }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f51:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f56:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 94 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_add_param(s, call, id); }
	goto _again;
f54:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 96 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_add_param(s, call, id); }
	goto _again;
f23:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 101 "libswanson/s0-parser.c.rl"
	{ call = swan_ast_call_new(s); }
	goto _again;
f58:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 103 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_set_method(s, call, id); }
	goto _again;
f22:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 94 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_add_param(s, call, id); }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f53:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 96 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_add_param(s, call, id); }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f5:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 101 "libswanson/s0-parser.c.rl"
	{ call = swan_ast_call_new(s); }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f20:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 103 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_set_method(s, call, id); }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f57:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 94 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_add_param(s, call, id); }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f55:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 96 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_add_param(s, call, id); }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f24:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 101 "libswanson/s0-parser.c.rl"
	{ call = swan_ast_call_new(s); }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f59:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 103 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_set_method(s, call, id); }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	if ( p == eof )
	{
	switch ( _s0_eof_actions[cs] ) {
	case 61:
#line 105 "libswanson/s0-parser.c.rl"
	{ swan_ast_add_element(s, ast, &call->parent); }
	break;
	case 64:
#line 108 "libswanson/s0-parser.c.rl"
	{
                 contents = swan_static_string_new(s, scratch.buf);
                 ast_string = swan_ast_string_new(s, id, contents);
                 swan_ast_add_element(s, ast, &ast_string->parent);
             }
	break;
#line 2620 "libswanson/s0-parser.c"
	}
	}

	_out: {}
	}

#line 144 "libswanson/s0-parser.c.rl"

    if (cs < 
#line 2630 "libswanson/s0-parser.c"
177
#line 145 "libswanson/s0-parser.c.rl"
) {
        printf("error at line %zu\n", curline);
        cork_error_set
            (SWAN_METAMODEL_ERROR, SWAN_METAMODEL_PARSE_ERROR,
             "Parse error");
        return 0;
    }

    cork_buffer_done(&scratch);
    return ast;
}
