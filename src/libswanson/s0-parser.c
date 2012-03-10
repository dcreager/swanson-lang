
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


#line 121 "libswanson/s0-parser.c.rl"



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
	0, 0, 0, 64, 67
};

static const int s0_start = 178;
static const int s0_first_final = 178;
static const int s0_error = 0;

static const int s0_en_main = 178;


#line 124 "libswanson/s0-parser.c.rl"

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

#line 148 "libswanson/s0-parser.c.rl"
    
#line 109 "libswanson/s0-parser.c"
	{
	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	switch ( cs ) {
case 178:
	switch( (*p) ) {
		case 10: goto tr118;
		case 32: goto tr582;
		case 34: goto tr583;
		case 35: goto tr117;
		case 36: goto tr119;
		case 95: goto tr584;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr582;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr584;
	} else
		goto tr584;
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
		case 58: goto tr12;
		case 61: goto tr13;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr6;
	goto tr7;
case 4:
	switch( (*p) ) {
		case 10: goto tr15;
		case 32: goto tr14;
		case 35: goto tr16;
		case 44: goto tr17;
		case 46: goto tr18;
		case 58: goto tr19;
		case 61: goto tr20;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr14;
	goto tr7;
case 5:
	if ( (*p) == 10 )
		goto tr15;
	goto tr16;
case 6:
	switch( (*p) ) {
		case 10: goto tr22;
		case 32: goto tr21;
		case 34: goto tr23;
		case 35: goto tr24;
		case 95: goto tr25;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr21;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr25;
	} else
		goto tr25;
	goto tr7;
case 7:
	switch( (*p) ) {
		case 10: goto tr27;
		case 32: goto tr26;
		case 34: goto tr28;
		case 35: goto tr29;
		case 95: goto tr30;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr26;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr30;
	} else
		goto tr30;
	goto tr7;
case 8:
	switch( (*p) ) {
		case 34: goto tr32;
		case 92: goto tr33;
	}
	goto tr31;
case 9:
	switch( (*p) ) {
		case 34: goto tr35;
		case 92: goto tr36;
	}
	goto tr34;
case 10:
	switch( (*p) ) {
		case 10: goto tr38;
		case 32: goto tr37;
		case 35: goto tr39;
		case 44: goto tr40;
		case 61: goto tr41;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr37;
	goto tr7;
case 11:
	switch( (*p) ) {
		case 10: goto tr43;
		case 32: goto tr42;
		case 35: goto tr44;
		case 44: goto tr17;
		case 61: goto tr20;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr42;
	goto tr7;
case 12:
	if ( (*p) == 10 )
		goto tr43;
	goto tr44;
case 13:
	switch( (*p) ) {
		case 10: goto tr46;
		case 32: goto tr45;
		case 34: goto tr47;
		case 35: goto tr48;
		case 95: goto tr49;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr45;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr49;
	} else
		goto tr49;
	goto tr7;
case 14:
	switch( (*p) ) {
		case 10: goto tr51;
		case 32: goto tr50;
		case 34: goto tr52;
		case 35: goto tr53;
		case 95: goto tr54;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr50;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr54;
	} else
		goto tr54;
	goto tr7;
case 15:
	switch( (*p) ) {
		case 34: goto tr56;
		case 92: goto tr57;
	}
	goto tr55;
case 16:
	switch( (*p) ) {
		case 34: goto tr59;
		case 92: goto tr60;
	}
	goto tr58;
case 17:
	switch( (*p) ) {
		case 10: goto tr62;
		case 32: goto tr61;
		case 35: goto tr63;
		case 46: goto tr64;
		case 58: goto tr65;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr61;
	goto tr7;
case 18:
	switch( (*p) ) {
		case 10: goto tr67;
		case 32: goto tr66;
		case 35: goto tr68;
		case 46: goto tr18;
		case 58: goto tr19;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr66;
	goto tr7;
case 19:
	if ( (*p) == 10 )
		goto tr67;
	goto tr68;
case 20:
	switch( (*p) ) {
		case 10: goto tr70;
		case 32: goto tr69;
		case 34: goto tr71;
		case 35: goto tr72;
		case 95: goto tr73;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr69;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr73;
	} else
		goto tr73;
	goto tr7;
case 21:
	switch( (*p) ) {
		case 10: goto tr75;
		case 32: goto tr74;
		case 34: goto tr76;
		case 35: goto tr77;
		case 95: goto tr78;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr74;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr78;
	} else
		goto tr78;
	goto tr7;
case 22:
	switch( (*p) ) {
		case 34: goto tr80;
		case 92: goto tr81;
	}
	goto tr79;
case 23:
	switch( (*p) ) {
		case 34: goto tr83;
		case 92: goto tr84;
	}
	goto tr82;
case 24:
	switch( (*p) ) {
		case 10: goto tr86;
		case 32: goto tr85;
		case 35: goto tr87;
		case 40: goto tr88;
		case 59: goto tr89;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr85;
	goto tr7;
case 25:
	switch( (*p) ) {
		case 10: goto tr91;
		case 32: goto tr90;
		case 35: goto tr92;
		case 40: goto tr93;
		case 59: goto tr94;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr90;
	goto tr7;
case 26:
	if ( (*p) == 10 )
		goto tr91;
	goto tr92;
case 27:
	switch( (*p) ) {
		case 10: goto tr95;
		case 32: goto tr93;
		case 34: goto tr96;
		case 35: goto tr97;
		case 41: goto tr98;
		case 95: goto tr99;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr93;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr99;
	} else
		goto tr99;
	goto tr7;
case 28:
	switch( (*p) ) {
		case 34: goto tr101;
		case 92: goto tr102;
	}
	goto tr100;
case 29:
	switch( (*p) ) {
		case 34: goto tr104;
		case 92: goto tr105;
	}
	goto tr103;
case 30:
	switch( (*p) ) {
		case 10: goto tr107;
		case 32: goto tr106;
		case 35: goto tr108;
		case 41: goto tr109;
		case 44: goto tr110;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr106;
	goto tr7;
case 31:
	switch( (*p) ) {
		case 10: goto tr112;
		case 32: goto tr111;
		case 35: goto tr113;
		case 41: goto tr98;
		case 44: goto tr114;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr111;
	goto tr7;
case 32:
	if ( (*p) == 10 )
		goto tr112;
	goto tr113;
case 33:
	switch( (*p) ) {
		case 10: goto tr115;
		case 32: goto tr98;
		case 35: goto tr116;
		case 59: goto tr94;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr98;
	goto tr7;
case 34:
	if ( (*p) == 10 )
		goto tr115;
	goto tr116;
case 179:
	switch( (*p) ) {
		case 10: goto tr586;
		case 32: goto tr585;
		case 34: goto tr587;
		case 35: goto tr588;
		case 36: goto tr589;
		case 95: goto tr590;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr585;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr590;
	} else
		goto tr590;
	goto tr7;
case 35:
	if ( (*p) == 10 )
		goto tr118;
	goto tr117;
case 36:
	switch( (*p) ) {
		case 10: goto tr120;
		case 32: goto tr119;
		case 34: goto tr121;
		case 35: goto tr122;
		case 95: goto tr123;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr119;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr123;
	} else
		goto tr123;
	goto tr7;
case 37:
	switch( (*p) ) {
		case 34: goto tr125;
		case 92: goto tr126;
	}
	goto tr124;
case 38:
	switch( (*p) ) {
		case 34: goto tr128;
		case 92: goto tr129;
	}
	goto tr127;
case 39:
	switch( (*p) ) {
		case 10: goto tr131;
		case 32: goto tr130;
		case 35: goto tr132;
		case 61: goto tr133;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr130;
	goto tr7;
case 40:
	switch( (*p) ) {
		case 10: goto tr135;
		case 32: goto tr134;
		case 35: goto tr136;
		case 61: goto tr137;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr134;
	goto tr7;
case 41:
	if ( (*p) == 10 )
		goto tr135;
	goto tr136;
case 42:
	switch( (*p) ) {
		case 10: goto tr138;
		case 32: goto tr137;
		case 34: goto tr139;
		case 35: goto tr140;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr137;
	goto tr7;
case 43:
	switch( (*p) ) {
		case 34: goto tr142;
		case 92: goto tr143;
	}
	goto tr141;
case 44:
	switch( (*p) ) {
		case 34: goto tr145;
		case 92: goto tr146;
	}
	goto tr144;
case 45:
	switch( (*p) ) {
		case 10: goto tr148;
		case 32: goto tr147;
		case 35: goto tr149;
		case 59: goto tr150;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr147;
	goto tr7;
case 46:
	if ( (*p) == 10 )
		goto tr148;
	goto tr149;
case 180:
	switch( (*p) ) {
		case 10: goto tr592;
		case 32: goto tr591;
		case 34: goto tr593;
		case 35: goto tr594;
		case 36: goto tr595;
		case 95: goto tr596;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr591;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr596;
	} else
		goto tr596;
	goto tr7;
case 47:
	switch( (*p) ) {
		case 10: goto tr152;
		case 32: goto tr151;
		case 35: goto tr153;
		case 44: goto tr154;
		case 46: goto tr155;
		case 58: goto tr157;
		case 61: goto tr158;
		case 95: goto tr156;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr151;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr156;
		} else if ( (*p) >= 65 )
			goto tr156;
	} else
		goto tr156;
	goto tr7;
case 48:
	switch( (*p) ) {
		case 10: goto tr160;
		case 32: goto tr159;
		case 34: goto tr161;
		case 35: goto tr162;
		case 95: goto tr163;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr159;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr163;
	} else
		goto tr163;
	goto tr7;
case 49:
	if ( (*p) == 10 )
		goto tr75;
	goto tr77;
case 50:
	switch( (*p) ) {
		case 10: goto tr165;
		case 32: goto tr164;
		case 35: goto tr166;
		case 40: goto tr167;
		case 59: goto tr169;
		case 95: goto tr168;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr164;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr168;
		} else if ( (*p) >= 65 )
			goto tr168;
	} else
		goto tr168;
	goto tr7;
case 51:
	switch( (*p) ) {
		case 34: goto tr170;
		case 39: goto tr171;
		case 92: goto tr172;
		case 97: goto tr173;
		case 98: goto tr174;
		case 102: goto tr175;
		case 110: goto tr176;
		case 114: goto tr177;
		case 116: goto tr178;
		case 118: goto tr179;
		case 120: goto tr180;
	}
	goto tr7;
case 52:
	switch( (*p) ) {
		case 34: goto tr182;
		case 92: goto tr183;
	}
	goto tr181;
case 53:
	switch( (*p) ) {
		case 34: goto tr185;
		case 92: goto tr186;
	}
	goto tr184;
case 54:
	switch( (*p) ) {
		case 34: goto tr188;
		case 92: goto tr189;
	}
	goto tr187;
case 55:
	switch( (*p) ) {
		case 34: goto tr191;
		case 92: goto tr192;
	}
	goto tr190;
case 56:
	switch( (*p) ) {
		case 34: goto tr194;
		case 92: goto tr195;
	}
	goto tr193;
case 57:
	switch( (*p) ) {
		case 34: goto tr197;
		case 92: goto tr198;
	}
	goto tr196;
case 58:
	switch( (*p) ) {
		case 34: goto tr200;
		case 92: goto tr201;
	}
	goto tr199;
case 59:
	switch( (*p) ) {
		case 34: goto tr203;
		case 92: goto tr204;
	}
	goto tr202;
case 60:
	switch( (*p) ) {
		case 34: goto tr206;
		case 92: goto tr207;
	}
	goto tr205;
case 61:
	switch( (*p) ) {
		case 34: goto tr209;
		case 92: goto tr210;
	}
	goto tr208;
case 62:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr211;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr211;
	} else
		goto tr211;
	goto tr7;
case 63:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr212;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr212;
	} else
		goto tr212;
	goto tr7;
case 64:
	switch( (*p) ) {
		case 34: goto tr214;
		case 92: goto tr215;
	}
	goto tr213;
case 65:
	if ( (*p) == 10 )
		goto tr138;
	goto tr140;
case 66:
	switch( (*p) ) {
		case 34: goto tr216;
		case 39: goto tr217;
		case 92: goto tr218;
		case 97: goto tr219;
		case 98: goto tr220;
		case 102: goto tr221;
		case 110: goto tr222;
		case 114: goto tr223;
		case 116: goto tr224;
		case 118: goto tr225;
		case 120: goto tr226;
	}
	goto tr7;
case 67:
	switch( (*p) ) {
		case 34: goto tr228;
		case 92: goto tr229;
	}
	goto tr227;
case 68:
	switch( (*p) ) {
		case 34: goto tr231;
		case 92: goto tr232;
	}
	goto tr230;
case 69:
	switch( (*p) ) {
		case 34: goto tr234;
		case 92: goto tr235;
	}
	goto tr233;
case 70:
	switch( (*p) ) {
		case 34: goto tr237;
		case 92: goto tr238;
	}
	goto tr236;
case 71:
	switch( (*p) ) {
		case 34: goto tr240;
		case 92: goto tr241;
	}
	goto tr239;
case 72:
	switch( (*p) ) {
		case 34: goto tr243;
		case 92: goto tr244;
	}
	goto tr242;
case 73:
	switch( (*p) ) {
		case 34: goto tr246;
		case 92: goto tr247;
	}
	goto tr245;
case 74:
	switch( (*p) ) {
		case 34: goto tr249;
		case 92: goto tr250;
	}
	goto tr248;
case 75:
	switch( (*p) ) {
		case 34: goto tr252;
		case 92: goto tr253;
	}
	goto tr251;
case 76:
	switch( (*p) ) {
		case 34: goto tr255;
		case 92: goto tr256;
	}
	goto tr254;
case 77:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr257;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr257;
	} else
		goto tr257;
	goto tr7;
case 78:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr258;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr258;
	} else
		goto tr258;
	goto tr7;
case 79:
	switch( (*p) ) {
		case 34: goto tr260;
		case 92: goto tr261;
	}
	goto tr259;
case 80:
	if ( (*p) == 10 )
		goto tr120;
	goto tr122;
case 81:
	switch( (*p) ) {
		case 10: goto tr263;
		case 32: goto tr262;
		case 35: goto tr264;
		case 61: goto tr266;
		case 95: goto tr265;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr262;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr265;
		} else if ( (*p) >= 65 )
			goto tr265;
	} else
		goto tr265;
	goto tr7;
case 82:
	switch( (*p) ) {
		case 10: goto tr267;
		case 32: goto tr114;
		case 34: goto tr268;
		case 35: goto tr269;
		case 95: goto tr270;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr114;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr270;
	} else
		goto tr270;
	goto tr7;
case 83:
	switch( (*p) ) {
		case 34: goto tr272;
		case 92: goto tr273;
	}
	goto tr271;
case 84:
	switch( (*p) ) {
		case 34: goto tr275;
		case 92: goto tr276;
	}
	goto tr274;
case 85:
	switch( (*p) ) {
		case 10: goto tr278;
		case 32: goto tr277;
		case 35: goto tr279;
		case 41: goto tr280;
		case 44: goto tr281;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr277;
	goto tr7;
case 86:
	switch( (*p) ) {
		case 34: goto tr282;
		case 39: goto tr283;
		case 92: goto tr284;
		case 97: goto tr285;
		case 98: goto tr286;
		case 102: goto tr287;
		case 110: goto tr288;
		case 114: goto tr289;
		case 116: goto tr290;
		case 118: goto tr291;
		case 120: goto tr292;
	}
	goto tr7;
case 87:
	switch( (*p) ) {
		case 34: goto tr294;
		case 92: goto tr295;
	}
	goto tr293;
case 88:
	switch( (*p) ) {
		case 34: goto tr297;
		case 92: goto tr298;
	}
	goto tr296;
case 89:
	switch( (*p) ) {
		case 34: goto tr300;
		case 92: goto tr301;
	}
	goto tr299;
case 90:
	switch( (*p) ) {
		case 34: goto tr303;
		case 92: goto tr304;
	}
	goto tr302;
case 91:
	switch( (*p) ) {
		case 34: goto tr306;
		case 92: goto tr307;
	}
	goto tr305;
case 92:
	switch( (*p) ) {
		case 34: goto tr309;
		case 92: goto tr310;
	}
	goto tr308;
case 93:
	switch( (*p) ) {
		case 34: goto tr312;
		case 92: goto tr313;
	}
	goto tr311;
case 94:
	switch( (*p) ) {
		case 34: goto tr315;
		case 92: goto tr316;
	}
	goto tr314;
case 95:
	switch( (*p) ) {
		case 34: goto tr318;
		case 92: goto tr319;
	}
	goto tr317;
case 96:
	switch( (*p) ) {
		case 34: goto tr321;
		case 92: goto tr322;
	}
	goto tr320;
case 97:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr323;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr323;
	} else
		goto tr323;
	goto tr7;
case 98:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr324;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr324;
	} else
		goto tr324;
	goto tr7;
case 99:
	switch( (*p) ) {
		case 34: goto tr326;
		case 92: goto tr327;
	}
	goto tr325;
case 100:
	if ( (*p) == 10 )
		goto tr267;
	goto tr269;
case 101:
	switch( (*p) ) {
		case 10: goto tr329;
		case 32: goto tr328;
		case 35: goto tr330;
		case 41: goto tr331;
		case 44: goto tr332;
		case 95: goto tr333;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr328;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr333;
		} else if ( (*p) >= 65 )
			goto tr333;
	} else
		goto tr333;
	goto tr7;
case 102:
	switch( (*p) ) {
		case 34: goto tr334;
		case 39: goto tr335;
		case 92: goto tr336;
		case 97: goto tr337;
		case 98: goto tr338;
		case 102: goto tr339;
		case 110: goto tr340;
		case 114: goto tr341;
		case 116: goto tr342;
		case 118: goto tr343;
		case 120: goto tr344;
	}
	goto tr7;
case 103:
	switch( (*p) ) {
		case 34: goto tr346;
		case 92: goto tr347;
	}
	goto tr345;
case 104:
	switch( (*p) ) {
		case 34: goto tr349;
		case 92: goto tr350;
	}
	goto tr348;
case 105:
	switch( (*p) ) {
		case 34: goto tr352;
		case 92: goto tr353;
	}
	goto tr351;
case 106:
	switch( (*p) ) {
		case 34: goto tr355;
		case 92: goto tr356;
	}
	goto tr354;
case 107:
	switch( (*p) ) {
		case 34: goto tr358;
		case 92: goto tr359;
	}
	goto tr357;
case 108:
	switch( (*p) ) {
		case 34: goto tr361;
		case 92: goto tr362;
	}
	goto tr360;
case 109:
	switch( (*p) ) {
		case 34: goto tr364;
		case 92: goto tr365;
	}
	goto tr363;
case 110:
	switch( (*p) ) {
		case 34: goto tr367;
		case 92: goto tr368;
	}
	goto tr366;
case 111:
	switch( (*p) ) {
		case 34: goto tr370;
		case 92: goto tr371;
	}
	goto tr369;
case 112:
	switch( (*p) ) {
		case 34: goto tr373;
		case 92: goto tr374;
	}
	goto tr372;
case 113:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr375;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr375;
	} else
		goto tr375;
	goto tr7;
case 114:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr376;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr376;
	} else
		goto tr376;
	goto tr7;
case 115:
	switch( (*p) ) {
		case 34: goto tr378;
		case 92: goto tr379;
	}
	goto tr377;
case 116:
	if ( (*p) == 10 )
		goto tr95;
	goto tr97;
case 117:
	switch( (*p) ) {
		case 10: goto tr381;
		case 32: goto tr380;
		case 35: goto tr382;
		case 41: goto tr383;
		case 44: goto tr384;
		case 95: goto tr385;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr380;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr385;
		} else if ( (*p) >= 65 )
			goto tr385;
	} else
		goto tr385;
	goto tr7;
case 118:
	switch( (*p) ) {
		case 34: goto tr386;
		case 39: goto tr387;
		case 92: goto tr388;
		case 97: goto tr389;
		case 98: goto tr390;
		case 102: goto tr391;
		case 110: goto tr392;
		case 114: goto tr393;
		case 116: goto tr394;
		case 118: goto tr395;
		case 120: goto tr396;
	}
	goto tr7;
case 119:
	switch( (*p) ) {
		case 34: goto tr398;
		case 92: goto tr399;
	}
	goto tr397;
case 120:
	switch( (*p) ) {
		case 34: goto tr401;
		case 92: goto tr402;
	}
	goto tr400;
case 121:
	switch( (*p) ) {
		case 34: goto tr404;
		case 92: goto tr405;
	}
	goto tr403;
case 122:
	switch( (*p) ) {
		case 34: goto tr407;
		case 92: goto tr408;
	}
	goto tr406;
case 123:
	switch( (*p) ) {
		case 34: goto tr410;
		case 92: goto tr411;
	}
	goto tr409;
case 124:
	switch( (*p) ) {
		case 34: goto tr413;
		case 92: goto tr414;
	}
	goto tr412;
case 125:
	switch( (*p) ) {
		case 34: goto tr416;
		case 92: goto tr417;
	}
	goto tr415;
case 126:
	switch( (*p) ) {
		case 34: goto tr419;
		case 92: goto tr420;
	}
	goto tr418;
case 127:
	switch( (*p) ) {
		case 34: goto tr422;
		case 92: goto tr423;
	}
	goto tr421;
case 128:
	switch( (*p) ) {
		case 34: goto tr425;
		case 92: goto tr426;
	}
	goto tr424;
case 129:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr427;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr427;
	} else
		goto tr427;
	goto tr7;
case 130:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr428;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr428;
	} else
		goto tr428;
	goto tr7;
case 131:
	switch( (*p) ) {
		case 34: goto tr430;
		case 92: goto tr431;
	}
	goto tr429;
case 132:
	switch( (*p) ) {
		case 34: goto tr432;
		case 39: goto tr433;
		case 92: goto tr434;
		case 97: goto tr435;
		case 98: goto tr436;
		case 102: goto tr437;
		case 110: goto tr438;
		case 114: goto tr439;
		case 116: goto tr440;
		case 118: goto tr441;
		case 120: goto tr442;
	}
	goto tr7;
case 133:
	switch( (*p) ) {
		case 34: goto tr444;
		case 92: goto tr445;
	}
	goto tr443;
case 134:
	switch( (*p) ) {
		case 34: goto tr447;
		case 92: goto tr448;
	}
	goto tr446;
case 135:
	switch( (*p) ) {
		case 34: goto tr450;
		case 92: goto tr451;
	}
	goto tr449;
case 136:
	switch( (*p) ) {
		case 34: goto tr453;
		case 92: goto tr454;
	}
	goto tr452;
case 137:
	switch( (*p) ) {
		case 34: goto tr456;
		case 92: goto tr457;
	}
	goto tr455;
case 138:
	switch( (*p) ) {
		case 34: goto tr459;
		case 92: goto tr460;
	}
	goto tr458;
case 139:
	switch( (*p) ) {
		case 34: goto tr462;
		case 92: goto tr463;
	}
	goto tr461;
case 140:
	switch( (*p) ) {
		case 34: goto tr465;
		case 92: goto tr466;
	}
	goto tr464;
case 141:
	switch( (*p) ) {
		case 34: goto tr468;
		case 92: goto tr469;
	}
	goto tr467;
case 142:
	switch( (*p) ) {
		case 34: goto tr471;
		case 92: goto tr472;
	}
	goto tr470;
case 143:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr473;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr473;
	} else
		goto tr473;
	goto tr7;
case 144:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr474;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr474;
	} else
		goto tr474;
	goto tr7;
case 145:
	switch( (*p) ) {
		case 34: goto tr476;
		case 92: goto tr477;
	}
	goto tr475;
case 146:
	if ( (*p) == 10 )
		goto tr51;
	goto tr53;
case 147:
	switch( (*p) ) {
		case 10: goto tr479;
		case 32: goto tr478;
		case 35: goto tr480;
		case 46: goto tr481;
		case 58: goto tr483;
		case 95: goto tr482;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr478;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr482;
		} else if ( (*p) >= 65 )
			goto tr482;
	} else
		goto tr482;
	goto tr7;
case 148:
	switch( (*p) ) {
		case 34: goto tr484;
		case 39: goto tr485;
		case 92: goto tr486;
		case 97: goto tr487;
		case 98: goto tr488;
		case 102: goto tr489;
		case 110: goto tr490;
		case 114: goto tr491;
		case 116: goto tr492;
		case 118: goto tr493;
		case 120: goto tr494;
	}
	goto tr7;
case 149:
	switch( (*p) ) {
		case 34: goto tr496;
		case 92: goto tr497;
	}
	goto tr495;
case 150:
	switch( (*p) ) {
		case 34: goto tr499;
		case 92: goto tr500;
	}
	goto tr498;
case 151:
	switch( (*p) ) {
		case 34: goto tr502;
		case 92: goto tr503;
	}
	goto tr501;
case 152:
	switch( (*p) ) {
		case 34: goto tr505;
		case 92: goto tr506;
	}
	goto tr504;
case 153:
	switch( (*p) ) {
		case 34: goto tr508;
		case 92: goto tr509;
	}
	goto tr507;
case 154:
	switch( (*p) ) {
		case 34: goto tr511;
		case 92: goto tr512;
	}
	goto tr510;
case 155:
	switch( (*p) ) {
		case 34: goto tr514;
		case 92: goto tr515;
	}
	goto tr513;
case 156:
	switch( (*p) ) {
		case 34: goto tr517;
		case 92: goto tr518;
	}
	goto tr516;
case 157:
	switch( (*p) ) {
		case 34: goto tr520;
		case 92: goto tr521;
	}
	goto tr519;
case 158:
	switch( (*p) ) {
		case 34: goto tr523;
		case 92: goto tr524;
	}
	goto tr522;
case 159:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr525;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr525;
	} else
		goto tr525;
	goto tr7;
case 160:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr526;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr526;
	} else
		goto tr526;
	goto tr7;
case 161:
	switch( (*p) ) {
		case 34: goto tr528;
		case 92: goto tr529;
	}
	goto tr527;
case 162:
	if ( (*p) == 10 )
		goto tr27;
	goto tr29;
case 163:
	switch( (*p) ) {
		case 10: goto tr531;
		case 32: goto tr530;
		case 35: goto tr532;
		case 44: goto tr533;
		case 61: goto tr535;
		case 95: goto tr534;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr530;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else if ( (*p) >= 65 )
			goto tr534;
	} else
		goto tr534;
	goto tr7;
case 164:
	switch( (*p) ) {
		case 34: goto tr536;
		case 39: goto tr537;
		case 92: goto tr538;
		case 97: goto tr539;
		case 98: goto tr540;
		case 102: goto tr541;
		case 110: goto tr542;
		case 114: goto tr543;
		case 116: goto tr544;
		case 118: goto tr545;
		case 120: goto tr546;
	}
	goto tr7;
case 165:
	switch( (*p) ) {
		case 34: goto tr548;
		case 92: goto tr549;
	}
	goto tr547;
case 166:
	switch( (*p) ) {
		case 34: goto tr551;
		case 92: goto tr552;
	}
	goto tr550;
case 167:
	switch( (*p) ) {
		case 34: goto tr554;
		case 92: goto tr555;
	}
	goto tr553;
case 168:
	switch( (*p) ) {
		case 34: goto tr557;
		case 92: goto tr558;
	}
	goto tr556;
case 169:
	switch( (*p) ) {
		case 34: goto tr560;
		case 92: goto tr561;
	}
	goto tr559;
case 170:
	switch( (*p) ) {
		case 34: goto tr563;
		case 92: goto tr564;
	}
	goto tr562;
case 171:
	switch( (*p) ) {
		case 34: goto tr566;
		case 92: goto tr567;
	}
	goto tr565;
case 172:
	switch( (*p) ) {
		case 34: goto tr569;
		case 92: goto tr570;
	}
	goto tr568;
case 173:
	switch( (*p) ) {
		case 34: goto tr572;
		case 92: goto tr573;
	}
	goto tr571;
case 174:
	switch( (*p) ) {
		case 34: goto tr575;
		case 92: goto tr576;
	}
	goto tr574;
case 175:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr577;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr577;
	} else
		goto tr577;
	goto tr7;
case 176:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr578;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr578;
	} else
		goto tr578;
	goto tr7;
case 177:
	switch( (*p) ) {
		case 34: goto tr580;
		case 92: goto tr581;
	}
	goto tr579;
	}

	tr7: cs = 0; goto _again;
	tr583: cs = 1; goto _again;
	tr587: cs = 1; goto f63;
	tr593: cs = 1; goto f66;
	tr0: cs = 2; goto f0;
	tr3: cs = 2; goto f2;
	tr547: cs = 2; goto f31;
	tr550: cs = 2; goto f33;
	tr553: cs = 2; goto f35;
	tr556: cs = 2; goto f37;
	tr559: cs = 2; goto f39;
	tr562: cs = 2; goto f41;
	tr565: cs = 2; goto f43;
	tr568: cs = 2; goto f45;
	tr571: cs = 2; goto f47;
	tr574: cs = 2; goto f49;
	tr579: cs = 2; goto f53;
	tr1: cs = 3; goto f1;
	tr4: cs = 3; goto f3;
	tr548: cs = 3; goto f32;
	tr551: cs = 3; goto f34;
	tr554: cs = 3; goto f36;
	tr557: cs = 3; goto f38;
	tr560: cs = 3; goto f40;
	tr563: cs = 3; goto f42;
	tr566: cs = 3; goto f44;
	tr569: cs = 3; goto f46;
	tr572: cs = 3; goto f48;
	tr575: cs = 3; goto f50;
	tr580: cs = 3; goto f54;
	tr14: cs = 4; goto _again;
	tr6: cs = 4; goto f4;
	tr8: cs = 4; goto f5;
	tr15: cs = 4; goto f6;
	tr151: cs = 4; goto f23;
	tr152: cs = 4; goto f24;
	tr16: cs = 5; goto _again;
	tr9: cs = 5; goto f4;
	tr153: cs = 5; goto f23;
	tr17: cs = 6; goto _again;
	tr10: cs = 6; goto f4;
	tr40: cs = 6; goto f11;
	tr154: cs = 6; goto f23;
	tr533: cs = 6; goto f55;
	tr26: cs = 7; goto _again;
	tr27: cs = 7; goto f6;
	tr21: cs = 7; goto f7;
	tr22: cs = 7; goto f8;
	tr28: cs = 8; goto _again;
	tr23: cs = 8; goto f7;
	tr31: cs = 9; goto f0;
	tr34: cs = 9; goto f2;
	tr495: cs = 9; goto f31;
	tr498: cs = 9; goto f33;
	tr501: cs = 9; goto f35;
	tr504: cs = 9; goto f37;
	tr507: cs = 9; goto f39;
	tr510: cs = 9; goto f41;
	tr513: cs = 9; goto f43;
	tr516: cs = 9; goto f45;
	tr519: cs = 9; goto f47;
	tr522: cs = 9; goto f49;
	tr527: cs = 9; goto f53;
	tr32: cs = 10; goto f1;
	tr35: cs = 10; goto f3;
	tr496: cs = 10; goto f32;
	tr499: cs = 10; goto f34;
	tr502: cs = 10; goto f36;
	tr505: cs = 10; goto f38;
	tr508: cs = 10; goto f40;
	tr511: cs = 10; goto f42;
	tr514: cs = 10; goto f44;
	tr517: cs = 10; goto f46;
	tr520: cs = 10; goto f48;
	tr523: cs = 10; goto f50;
	tr528: cs = 10; goto f54;
	tr42: cs = 11; goto _again;
	tr43: cs = 11; goto f6;
	tr37: cs = 11; goto f11;
	tr38: cs = 11; goto f12;
	tr530: cs = 11; goto f55;
	tr531: cs = 11; goto f56;
	tr44: cs = 12; goto _again;
	tr39: cs = 12; goto f11;
	tr532: cs = 12; goto f55;
	tr20: cs = 13; goto _again;
	tr13: cs = 13; goto f4;
	tr41: cs = 13; goto f11;
	tr158: cs = 13; goto f23;
	tr535: cs = 13; goto f55;
	tr50: cs = 14; goto _again;
	tr51: cs = 14; goto f6;
	tr45: cs = 14; goto f13;
	tr46: cs = 14; goto f14;
	tr52: cs = 15; goto _again;
	tr47: cs = 15; goto f13;
	tr55: cs = 16; goto f0;
	tr58: cs = 16; goto f2;
	tr443: cs = 16; goto f31;
	tr446: cs = 16; goto f33;
	tr449: cs = 16; goto f35;
	tr452: cs = 16; goto f37;
	tr455: cs = 16; goto f39;
	tr458: cs = 16; goto f41;
	tr461: cs = 16; goto f43;
	tr464: cs = 16; goto f45;
	tr467: cs = 16; goto f47;
	tr470: cs = 16; goto f49;
	tr475: cs = 16; goto f53;
	tr56: cs = 17; goto f1;
	tr59: cs = 17; goto f3;
	tr444: cs = 17; goto f32;
	tr447: cs = 17; goto f34;
	tr450: cs = 17; goto f36;
	tr453: cs = 17; goto f38;
	tr456: cs = 17; goto f40;
	tr459: cs = 17; goto f42;
	tr462: cs = 17; goto f44;
	tr465: cs = 17; goto f46;
	tr468: cs = 17; goto f48;
	tr471: cs = 17; goto f50;
	tr476: cs = 17; goto f54;
	tr66: cs = 18; goto _again;
	tr67: cs = 18; goto f6;
	tr61: cs = 18; goto f11;
	tr62: cs = 18; goto f12;
	tr478: cs = 18; goto f55;
	tr479: cs = 18; goto f56;
	tr68: cs = 19; goto _again;
	tr63: cs = 19; goto f11;
	tr480: cs = 19; goto f55;
	tr18: cs = 20; goto _again;
	tr11: cs = 20; goto f4;
	tr64: cs = 20; goto f11;
	tr155: cs = 20; goto f23;
	tr481: cs = 20; goto f55;
	tr74: cs = 21; goto _again;
	tr75: cs = 21; goto f6;
	tr69: cs = 21; goto f16;
	tr70: cs = 21; goto f17;
	tr159: cs = 21; goto f25;
	tr160: cs = 21; goto f26;
	tr76: cs = 22; goto _again;
	tr71: cs = 22; goto f16;
	tr161: cs = 22; goto f25;
	tr79: cs = 23; goto f0;
	tr82: cs = 23; goto f2;
	tr397: cs = 23; goto f31;
	tr400: cs = 23; goto f33;
	tr403: cs = 23; goto f35;
	tr406: cs = 23; goto f37;
	tr409: cs = 23; goto f39;
	tr412: cs = 23; goto f41;
	tr415: cs = 23; goto f43;
	tr418: cs = 23; goto f45;
	tr421: cs = 23; goto f47;
	tr424: cs = 23; goto f49;
	tr429: cs = 23; goto f53;
	tr80: cs = 24; goto f1;
	tr83: cs = 24; goto f3;
	tr398: cs = 24; goto f32;
	tr401: cs = 24; goto f34;
	tr404: cs = 24; goto f36;
	tr407: cs = 24; goto f38;
	tr410: cs = 24; goto f40;
	tr413: cs = 24; goto f42;
	tr416: cs = 24; goto f44;
	tr419: cs = 24; goto f46;
	tr422: cs = 24; goto f48;
	tr425: cs = 24; goto f50;
	tr430: cs = 24; goto f54;
	tr90: cs = 25; goto _again;
	tr91: cs = 25; goto f6;
	tr85: cs = 25; goto f19;
	tr86: cs = 25; goto f20;
	tr164: cs = 25; goto f28;
	tr165: cs = 25; goto f29;
	tr92: cs = 26; goto _again;
	tr87: cs = 26; goto f19;
	tr166: cs = 26; goto f28;
	tr93: cs = 27; goto _again;
	tr95: cs = 27; goto f6;
	tr88: cs = 27; goto f19;
	tr167: cs = 27; goto f28;
	tr96: cs = 28; goto _again;
	tr100: cs = 29; goto f0;
	tr103: cs = 29; goto f2;
	tr345: cs = 29; goto f31;
	tr348: cs = 29; goto f33;
	tr351: cs = 29; goto f35;
	tr354: cs = 29; goto f37;
	tr357: cs = 29; goto f39;
	tr360: cs = 29; goto f41;
	tr363: cs = 29; goto f43;
	tr366: cs = 29; goto f45;
	tr369: cs = 29; goto f47;
	tr372: cs = 29; goto f49;
	tr377: cs = 29; goto f53;
	tr101: cs = 30; goto f1;
	tr104: cs = 30; goto f3;
	tr346: cs = 30; goto f32;
	tr349: cs = 30; goto f34;
	tr352: cs = 30; goto f36;
	tr355: cs = 30; goto f38;
	tr358: cs = 30; goto f40;
	tr361: cs = 30; goto f42;
	tr364: cs = 30; goto f44;
	tr367: cs = 30; goto f46;
	tr370: cs = 30; goto f48;
	tr373: cs = 30; goto f50;
	tr378: cs = 30; goto f54;
	tr111: cs = 31; goto _again;
	tr112: cs = 31; goto f6;
	tr106: cs = 31; goto f21;
	tr107: cs = 31; goto f22;
	tr277: cs = 31; goto f57;
	tr278: cs = 31; goto f58;
	tr328: cs = 31; goto f59;
	tr329: cs = 31; goto f60;
	tr380: cs = 31; goto f61;
	tr381: cs = 31; goto f62;
	tr113: cs = 32; goto _again;
	tr108: cs = 32; goto f21;
	tr279: cs = 32; goto f57;
	tr330: cs = 32; goto f59;
	tr382: cs = 32; goto f61;
	tr98: cs = 33; goto _again;
	tr115: cs = 33; goto f6;
	tr109: cs = 33; goto f21;
	tr280: cs = 33; goto f57;
	tr331: cs = 33; goto f59;
	tr383: cs = 33; goto f61;
	tr116: cs = 34; goto _again;
	tr117: cs = 35; goto _again;
	tr588: cs = 35; goto f63;
	tr594: cs = 35; goto f66;
	tr119: cs = 36; goto _again;
	tr120: cs = 36; goto f6;
	tr589: cs = 36; goto f63;
	tr595: cs = 36; goto f66;
	tr121: cs = 37; goto _again;
	tr124: cs = 38; goto f0;
	tr127: cs = 38; goto f2;
	tr227: cs = 38; goto f31;
	tr230: cs = 38; goto f33;
	tr233: cs = 38; goto f35;
	tr236: cs = 38; goto f37;
	tr239: cs = 38; goto f39;
	tr242: cs = 38; goto f41;
	tr245: cs = 38; goto f43;
	tr248: cs = 38; goto f45;
	tr251: cs = 38; goto f47;
	tr254: cs = 38; goto f49;
	tr259: cs = 38; goto f53;
	tr125: cs = 39; goto f1;
	tr128: cs = 39; goto f3;
	tr228: cs = 39; goto f32;
	tr231: cs = 39; goto f34;
	tr234: cs = 39; goto f36;
	tr237: cs = 39; goto f38;
	tr240: cs = 39; goto f40;
	tr243: cs = 39; goto f42;
	tr246: cs = 39; goto f44;
	tr249: cs = 39; goto f46;
	tr252: cs = 39; goto f48;
	tr255: cs = 39; goto f50;
	tr260: cs = 39; goto f54;
	tr134: cs = 40; goto _again;
	tr135: cs = 40; goto f6;
	tr130: cs = 40; goto f11;
	tr131: cs = 40; goto f12;
	tr262: cs = 40; goto f55;
	tr263: cs = 40; goto f56;
	tr136: cs = 41; goto _again;
	tr132: cs = 41; goto f11;
	tr264: cs = 41; goto f55;
	tr137: cs = 42; goto _again;
	tr138: cs = 42; goto f6;
	tr133: cs = 42; goto f11;
	tr266: cs = 42; goto f55;
	tr139: cs = 43; goto _again;
	tr141: cs = 44; goto f0;
	tr144: cs = 44; goto f2;
	tr181: cs = 44; goto f31;
	tr184: cs = 44; goto f33;
	tr187: cs = 44; goto f35;
	tr190: cs = 44; goto f37;
	tr193: cs = 44; goto f39;
	tr196: cs = 44; goto f41;
	tr199: cs = 44; goto f43;
	tr202: cs = 44; goto f45;
	tr205: cs = 44; goto f47;
	tr208: cs = 44; goto f49;
	tr213: cs = 44; goto f53;
	tr147: cs = 45; goto _again;
	tr142: cs = 45; goto f1;
	tr145: cs = 45; goto f3;
	tr148: cs = 45; goto f6;
	tr182: cs = 45; goto f32;
	tr185: cs = 45; goto f34;
	tr188: cs = 45; goto f36;
	tr191: cs = 45; goto f38;
	tr194: cs = 45; goto f40;
	tr197: cs = 45; goto f42;
	tr200: cs = 45; goto f44;
	tr203: cs = 45; goto f46;
	tr206: cs = 45; goto f48;
	tr209: cs = 45; goto f50;
	tr214: cs = 45; goto f54;
	tr149: cs = 46; goto _again;
	tr156: cs = 47; goto _again;
	tr584: cs = 47; goto f10;
	tr590: cs = 47; goto f65;
	tr596: cs = 47; goto f68;
	tr19: cs = 48; goto _again;
	tr12: cs = 48; goto f4;
	tr65: cs = 48; goto f11;
	tr157: cs = 48; goto f23;
	tr483: cs = 48; goto f55;
	tr77: cs = 49; goto _again;
	tr72: cs = 49; goto f16;
	tr162: cs = 49; goto f25;
	tr168: cs = 50; goto _again;
	tr78: cs = 50; goto f10;
	tr73: cs = 50; goto f18;
	tr163: cs = 50; goto f27;
	tr143: cs = 51; goto f1;
	tr146: cs = 51; goto f3;
	tr183: cs = 51; goto f32;
	tr186: cs = 51; goto f34;
	tr189: cs = 51; goto f36;
	tr192: cs = 51; goto f38;
	tr195: cs = 51; goto f40;
	tr198: cs = 51; goto f42;
	tr201: cs = 51; goto f44;
	tr204: cs = 51; goto f46;
	tr207: cs = 51; goto f48;
	tr210: cs = 51; goto f50;
	tr215: cs = 51; goto f54;
	tr170: cs = 52; goto _again;
	tr171: cs = 53; goto _again;
	tr172: cs = 54; goto _again;
	tr173: cs = 55; goto _again;
	tr174: cs = 56; goto _again;
	tr175: cs = 57; goto _again;
	tr176: cs = 58; goto _again;
	tr177: cs = 59; goto _again;
	tr178: cs = 60; goto _again;
	tr179: cs = 61; goto _again;
	tr180: cs = 62; goto f30;
	tr211: cs = 63; goto f51;
	tr212: cs = 64; goto f52;
	tr140: cs = 65; goto _again;
	tr126: cs = 66; goto f1;
	tr129: cs = 66; goto f3;
	tr229: cs = 66; goto f32;
	tr232: cs = 66; goto f34;
	tr235: cs = 66; goto f36;
	tr238: cs = 66; goto f38;
	tr241: cs = 66; goto f40;
	tr244: cs = 66; goto f42;
	tr247: cs = 66; goto f44;
	tr250: cs = 66; goto f46;
	tr253: cs = 66; goto f48;
	tr256: cs = 66; goto f50;
	tr261: cs = 66; goto f54;
	tr216: cs = 67; goto _again;
	tr217: cs = 68; goto _again;
	tr218: cs = 69; goto _again;
	tr219: cs = 70; goto _again;
	tr220: cs = 71; goto _again;
	tr221: cs = 72; goto _again;
	tr222: cs = 73; goto _again;
	tr223: cs = 74; goto _again;
	tr224: cs = 75; goto _again;
	tr225: cs = 76; goto _again;
	tr226: cs = 77; goto f30;
	tr257: cs = 78; goto f51;
	tr258: cs = 79; goto f52;
	tr122: cs = 80; goto _again;
	tr265: cs = 81; goto _again;
	tr123: cs = 81; goto f10;
	tr114: cs = 82; goto _again;
	tr267: cs = 82; goto f6;
	tr110: cs = 82; goto f21;
	tr281: cs = 82; goto f57;
	tr332: cs = 82; goto f59;
	tr384: cs = 82; goto f61;
	tr268: cs = 83; goto _again;
	tr271: cs = 84; goto f0;
	tr274: cs = 84; goto f2;
	tr293: cs = 84; goto f31;
	tr296: cs = 84; goto f33;
	tr299: cs = 84; goto f35;
	tr302: cs = 84; goto f37;
	tr305: cs = 84; goto f39;
	tr308: cs = 84; goto f41;
	tr311: cs = 84; goto f43;
	tr314: cs = 84; goto f45;
	tr317: cs = 84; goto f47;
	tr320: cs = 84; goto f49;
	tr325: cs = 84; goto f53;
	tr272: cs = 85; goto f1;
	tr275: cs = 85; goto f3;
	tr294: cs = 85; goto f32;
	tr297: cs = 85; goto f34;
	tr300: cs = 85; goto f36;
	tr303: cs = 85; goto f38;
	tr306: cs = 85; goto f40;
	tr309: cs = 85; goto f42;
	tr312: cs = 85; goto f44;
	tr315: cs = 85; goto f46;
	tr318: cs = 85; goto f48;
	tr321: cs = 85; goto f50;
	tr326: cs = 85; goto f54;
	tr273: cs = 86; goto f1;
	tr276: cs = 86; goto f3;
	tr295: cs = 86; goto f32;
	tr298: cs = 86; goto f34;
	tr301: cs = 86; goto f36;
	tr304: cs = 86; goto f38;
	tr307: cs = 86; goto f40;
	tr310: cs = 86; goto f42;
	tr313: cs = 86; goto f44;
	tr316: cs = 86; goto f46;
	tr319: cs = 86; goto f48;
	tr322: cs = 86; goto f50;
	tr327: cs = 86; goto f54;
	tr282: cs = 87; goto _again;
	tr283: cs = 88; goto _again;
	tr284: cs = 89; goto _again;
	tr285: cs = 90; goto _again;
	tr286: cs = 91; goto _again;
	tr287: cs = 92; goto _again;
	tr288: cs = 93; goto _again;
	tr289: cs = 94; goto _again;
	tr290: cs = 95; goto _again;
	tr291: cs = 96; goto _again;
	tr292: cs = 97; goto f30;
	tr323: cs = 98; goto f51;
	tr324: cs = 99; goto f52;
	tr269: cs = 100; goto _again;
	tr333: cs = 101; goto _again;
	tr270: cs = 101; goto f10;
	tr102: cs = 102; goto f1;
	tr105: cs = 102; goto f3;
	tr347: cs = 102; goto f32;
	tr350: cs = 102; goto f34;
	tr353: cs = 102; goto f36;
	tr356: cs = 102; goto f38;
	tr359: cs = 102; goto f40;
	tr362: cs = 102; goto f42;
	tr365: cs = 102; goto f44;
	tr368: cs = 102; goto f46;
	tr371: cs = 102; goto f48;
	tr374: cs = 102; goto f50;
	tr379: cs = 102; goto f54;
	tr334: cs = 103; goto _again;
	tr335: cs = 104; goto _again;
	tr336: cs = 105; goto _again;
	tr337: cs = 106; goto _again;
	tr338: cs = 107; goto _again;
	tr339: cs = 108; goto _again;
	tr340: cs = 109; goto _again;
	tr341: cs = 110; goto _again;
	tr342: cs = 111; goto _again;
	tr343: cs = 112; goto _again;
	tr344: cs = 113; goto f30;
	tr375: cs = 114; goto f51;
	tr376: cs = 115; goto f52;
	tr97: cs = 116; goto _again;
	tr385: cs = 117; goto _again;
	tr99: cs = 117; goto f10;
	tr81: cs = 118; goto f1;
	tr84: cs = 118; goto f3;
	tr399: cs = 118; goto f32;
	tr402: cs = 118; goto f34;
	tr405: cs = 118; goto f36;
	tr408: cs = 118; goto f38;
	tr411: cs = 118; goto f40;
	tr414: cs = 118; goto f42;
	tr417: cs = 118; goto f44;
	tr420: cs = 118; goto f46;
	tr423: cs = 118; goto f48;
	tr426: cs = 118; goto f50;
	tr431: cs = 118; goto f54;
	tr386: cs = 119; goto _again;
	tr387: cs = 120; goto _again;
	tr388: cs = 121; goto _again;
	tr389: cs = 122; goto _again;
	tr390: cs = 123; goto _again;
	tr391: cs = 124; goto _again;
	tr392: cs = 125; goto _again;
	tr393: cs = 126; goto _again;
	tr394: cs = 127; goto _again;
	tr395: cs = 128; goto _again;
	tr396: cs = 129; goto f30;
	tr427: cs = 130; goto f51;
	tr428: cs = 131; goto f52;
	tr57: cs = 132; goto f1;
	tr60: cs = 132; goto f3;
	tr445: cs = 132; goto f32;
	tr448: cs = 132; goto f34;
	tr451: cs = 132; goto f36;
	tr454: cs = 132; goto f38;
	tr457: cs = 132; goto f40;
	tr460: cs = 132; goto f42;
	tr463: cs = 132; goto f44;
	tr466: cs = 132; goto f46;
	tr469: cs = 132; goto f48;
	tr472: cs = 132; goto f50;
	tr477: cs = 132; goto f54;
	tr432: cs = 133; goto _again;
	tr433: cs = 134; goto _again;
	tr434: cs = 135; goto _again;
	tr435: cs = 136; goto _again;
	tr436: cs = 137; goto _again;
	tr437: cs = 138; goto _again;
	tr438: cs = 139; goto _again;
	tr439: cs = 140; goto _again;
	tr440: cs = 141; goto _again;
	tr441: cs = 142; goto _again;
	tr442: cs = 143; goto f30;
	tr473: cs = 144; goto f51;
	tr474: cs = 145; goto f52;
	tr53: cs = 146; goto _again;
	tr48: cs = 146; goto f13;
	tr482: cs = 147; goto _again;
	tr54: cs = 147; goto f10;
	tr49: cs = 147; goto f15;
	tr33: cs = 148; goto f1;
	tr36: cs = 148; goto f3;
	tr497: cs = 148; goto f32;
	tr500: cs = 148; goto f34;
	tr503: cs = 148; goto f36;
	tr506: cs = 148; goto f38;
	tr509: cs = 148; goto f40;
	tr512: cs = 148; goto f42;
	tr515: cs = 148; goto f44;
	tr518: cs = 148; goto f46;
	tr521: cs = 148; goto f48;
	tr524: cs = 148; goto f50;
	tr529: cs = 148; goto f54;
	tr484: cs = 149; goto _again;
	tr485: cs = 150; goto _again;
	tr486: cs = 151; goto _again;
	tr487: cs = 152; goto _again;
	tr488: cs = 153; goto _again;
	tr489: cs = 154; goto _again;
	tr490: cs = 155; goto _again;
	tr491: cs = 156; goto _again;
	tr492: cs = 157; goto _again;
	tr493: cs = 158; goto _again;
	tr494: cs = 159; goto f30;
	tr525: cs = 160; goto f51;
	tr526: cs = 161; goto f52;
	tr29: cs = 162; goto _again;
	tr24: cs = 162; goto f7;
	tr534: cs = 163; goto _again;
	tr25: cs = 163; goto f9;
	tr30: cs = 163; goto f10;
	tr2: cs = 164; goto f1;
	tr5: cs = 164; goto f3;
	tr549: cs = 164; goto f32;
	tr552: cs = 164; goto f34;
	tr555: cs = 164; goto f36;
	tr558: cs = 164; goto f38;
	tr561: cs = 164; goto f40;
	tr564: cs = 164; goto f42;
	tr567: cs = 164; goto f44;
	tr570: cs = 164; goto f46;
	tr573: cs = 164; goto f48;
	tr576: cs = 164; goto f50;
	tr581: cs = 164; goto f54;
	tr536: cs = 165; goto _again;
	tr537: cs = 166; goto _again;
	tr538: cs = 167; goto _again;
	tr539: cs = 168; goto _again;
	tr540: cs = 169; goto _again;
	tr541: cs = 170; goto _again;
	tr542: cs = 171; goto _again;
	tr543: cs = 172; goto _again;
	tr544: cs = 173; goto _again;
	tr545: cs = 174; goto _again;
	tr546: cs = 175; goto f30;
	tr577: cs = 176; goto f51;
	tr578: cs = 177; goto f52;
	tr582: cs = 178; goto _again;
	tr118: cs = 178; goto f6;
	tr585: cs = 178; goto f63;
	tr586: cs = 178; goto f64;
	tr591: cs = 178; goto f66;
	tr592: cs = 178; goto f67;
	tr94: cs = 179; goto _again;
	tr89: cs = 179; goto f19;
	tr169: cs = 179; goto f28;
	tr150: cs = 180; goto _again;

f10:
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f6:
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f36:
#line 52 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\\", 1); }
	goto _again;
f32:
#line 53 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\"", 1); }
	goto _again;
f34:
#line 54 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "'",  1); }
	goto _again;
f38:
#line 55 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\a",  1); }
	goto _again;
f40:
#line 56 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\b",  1); }
	goto _again;
f42:
#line 57 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\f",  1); }
	goto _again;
f44:
#line 58 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\n",  1); }
	goto _again;
f46:
#line 59 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\r",  1); }
	goto _again;
f48:
#line 60 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\t",  1); }
	goto _again;
f50:
#line 61 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\v",  1); }
	goto _again;
f52:
#line 63 "libswanson/s0-parser.c.rl"
	{ hex_char |= digit_values[(int) (*p)]; }
	goto _again;
f30:
#line 64 "libswanson/s0-parser.c.rl"
	{ hex_char = 0; }
	goto _again;
f54:
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
f25:
#line 84 "libswanson/s0-parser.c.rl"
	{
                 swan_ast_call_set_thing(s, call, id);
                 swan_ast_call_add_param(s, call, cork_gc_incref(id));
             }
	goto _again;
f13:
#line 89 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_add_result(s, call, id); }
	goto _again;
f7:
#line 92 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_add_result(s, call, id); }
	goto _again;
f63:
#line 110 "libswanson/s0-parser.c.rl"
	{ swan_ast_add_element(s, ast, &call->parent); }
	goto _again;
f66:
#line 113 "libswanson/s0-parser.c.rl"
	{
                 contents = swan_static_string_new(s, scratch.buf);
                 ast_string = swan_ast_string_new(s, id, contents);
                 swan_ast_add_element(s, ast, &ast_string->parent);
             }
	goto _again;
f55:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
	goto _again;
f35:
#line 52 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\\", 1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f31:
#line 53 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\"", 1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f33:
#line 54 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "'",  1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f37:
#line 55 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\a",  1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f39:
#line 56 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\b",  1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f41:
#line 57 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\f",  1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f43:
#line 58 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\n",  1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f45:
#line 59 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\r",  1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f47:
#line 60 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\t",  1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f49:
#line 61 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\v",  1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f51:
#line 63 "libswanson/s0-parser.c.rl"
	{ hex_char |= digit_values[(int) (*p)]; }
#line 65 "libswanson/s0-parser.c.rl"
	{ hex_char <<= 4; }
	goto _again;
f53:
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
#line 99 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_add_param(s, call, id); }
	goto _again;
f57:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 101 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_add_param(s, call, id); }
	goto _again;
f4:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 106 "libswanson/s0-parser.c.rl"
	{ call = swan_ast_call_new(s); }
	goto _again;
f19:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 108 "libswanson/s0-parser.c.rl"
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
f27:
#line 84 "libswanson/s0-parser.c.rl"
	{
                 swan_ast_call_set_thing(s, call, id);
                 swan_ast_call_add_param(s, call, cork_gc_incref(id));
             }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f26:
#line 84 "libswanson/s0-parser.c.rl"
	{
                 swan_ast_call_set_thing(s, call, id);
                 swan_ast_call_add_param(s, call, cork_gc_incref(id));
             }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f15:
#line 89 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_add_result(s, call, id); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f14:
#line 89 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_add_result(s, call, id); }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f9:
#line 92 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_add_result(s, call, id); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f8:
#line 92 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_add_result(s, call, id); }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f65:
#line 110 "libswanson/s0-parser.c.rl"
	{ swan_ast_add_element(s, ast, &call->parent); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f64:
#line 110 "libswanson/s0-parser.c.rl"
	{ swan_ast_add_element(s, ast, &call->parent); }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f68:
#line 113 "libswanson/s0-parser.c.rl"
	{
                 contents = swan_static_string_new(s, scratch.buf);
                 ast_string = swan_ast_string_new(s, id, contents);
                 swan_ast_add_element(s, ast, &ast_string->parent);
             }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f67:
#line 113 "libswanson/s0-parser.c.rl"
	{
                 contents = swan_static_string_new(s, scratch.buf);
                 ast_string = swan_ast_string_new(s, id, contents);
                 swan_ast_add_element(s, ast, &ast_string->parent);
             }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f56:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f61:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 99 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_add_param(s, call, id); }
	goto _again;
f59:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 101 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_add_param(s, call, id); }
	goto _again;
f23:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 106 "libswanson/s0-parser.c.rl"
	{ call = swan_ast_call_new(s); }
	goto _again;
f28:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 108 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_set_method(s, call, id); }
	goto _again;
f22:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 99 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_add_param(s, call, id); }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f58:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 101 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_add_param(s, call, id); }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f5:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 106 "libswanson/s0-parser.c.rl"
	{ call = swan_ast_call_new(s); }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f20:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 108 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_set_method(s, call, id); }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f62:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 99 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_add_param(s, call, id); }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f60:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 101 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_add_param(s, call, id); }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f24:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 106 "libswanson/s0-parser.c.rl"
	{ call = swan_ast_call_new(s); }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f29:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 108 "libswanson/s0-parser.c.rl"
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
	case 64:
#line 110 "libswanson/s0-parser.c.rl"
	{ swan_ast_add_element(s, ast, &call->parent); }
	break;
	case 67:
#line 113 "libswanson/s0-parser.c.rl"
	{
                 contents = swan_static_string_new(s, scratch.buf);
                 ast_string = swan_ast_string_new(s, id, contents);
                 swan_ast_add_element(s, ast, &ast_string->parent);
             }
	break;
#line 2678 "libswanson/s0-parser.c"
	}
	}

	_out: {}
	}

#line 149 "libswanson/s0-parser.c.rl"

    if (cs < 
#line 2688 "libswanson/s0-parser.c"
178
#line 150 "libswanson/s0-parser.c.rl"
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
