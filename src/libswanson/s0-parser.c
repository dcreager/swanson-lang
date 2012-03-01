
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


#line 108 "libswanson/s0-parser.c.rl"



#line 43 "/home/dcreager/git/swanson/src/libswanson/s0-parser.c"
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
	61
};

static const int s0_start = 135;
static const int s0_first_final = 135;
static const int s0_error = 0;

static const int s0_en_main = 135;


#line 111 "libswanson/s0-parser.c.rl"

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
    struct swan_ast_call  *call = NULL;
    struct swan_ast  *ast = swan_ast_new(s);

    
#line 95 "/home/dcreager/git/swanson/src/libswanson/s0-parser.c"
	{
	cs = s0_start;
	}

#line 133 "libswanson/s0-parser.c.rl"
    
#line 102 "/home/dcreager/git/swanson/src/libswanson/s0-parser.c"
	{
	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	switch ( cs ) {
case 135:
	switch( (*p) ) {
		case 10: goto tr115;
		case 32: goto tr443;
		case 34: goto tr444;
		case 35: goto tr114;
		case 95: goto tr445;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr443;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr445;
	} else
		goto tr445;
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
case 136:
	switch( (*p) ) {
		case 10: goto tr447;
		case 32: goto tr446;
		case 34: goto tr448;
		case 35: goto tr449;
		case 95: goto tr450;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr446;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr450;
	} else
		goto tr450;
	goto tr7;
case 35:
	if ( (*p) == 10 )
		goto tr115;
	goto tr114;
case 36:
	switch( (*p) ) {
		case 10: goto tr117;
		case 32: goto tr116;
		case 35: goto tr118;
		case 44: goto tr119;
		case 46: goto tr120;
		case 61: goto tr122;
		case 95: goto tr121;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr116;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr121;
		} else if ( (*p) >= 65 )
			goto tr121;
	} else
		goto tr121;
	goto tr7;
case 37:
	switch( (*p) ) {
		case 10: goto tr123;
		case 32: goto tr111;
		case 34: goto tr124;
		case 35: goto tr125;
		case 95: goto tr126;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr111;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr126;
	} else
		goto tr126;
	goto tr7;
case 38:
	switch( (*p) ) {
		case 34: goto tr128;
		case 92: goto tr129;
	}
	goto tr127;
case 39:
	switch( (*p) ) {
		case 34: goto tr131;
		case 92: goto tr132;
	}
	goto tr130;
case 40:
	switch( (*p) ) {
		case 10: goto tr134;
		case 32: goto tr133;
		case 35: goto tr135;
		case 41: goto tr136;
		case 44: goto tr137;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr133;
	goto tr7;
case 41:
	switch( (*p) ) {
		case 34: goto tr138;
		case 39: goto tr139;
		case 92: goto tr140;
		case 97: goto tr141;
		case 98: goto tr142;
		case 102: goto tr143;
		case 110: goto tr144;
		case 114: goto tr145;
		case 116: goto tr146;
		case 118: goto tr147;
		case 120: goto tr148;
	}
	goto tr7;
case 42:
	switch( (*p) ) {
		case 34: goto tr150;
		case 92: goto tr151;
	}
	goto tr149;
case 43:
	switch( (*p) ) {
		case 34: goto tr153;
		case 92: goto tr154;
	}
	goto tr152;
case 44:
	switch( (*p) ) {
		case 34: goto tr156;
		case 92: goto tr157;
	}
	goto tr155;
case 45:
	switch( (*p) ) {
		case 34: goto tr159;
		case 92: goto tr160;
	}
	goto tr158;
case 46:
	switch( (*p) ) {
		case 34: goto tr162;
		case 92: goto tr163;
	}
	goto tr161;
case 47:
	switch( (*p) ) {
		case 34: goto tr165;
		case 92: goto tr166;
	}
	goto tr164;
case 48:
	switch( (*p) ) {
		case 34: goto tr168;
		case 92: goto tr169;
	}
	goto tr167;
case 49:
	switch( (*p) ) {
		case 34: goto tr171;
		case 92: goto tr172;
	}
	goto tr170;
case 50:
	switch( (*p) ) {
		case 34: goto tr174;
		case 92: goto tr175;
	}
	goto tr173;
case 51:
	switch( (*p) ) {
		case 34: goto tr177;
		case 92: goto tr178;
	}
	goto tr176;
case 52:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr179;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr179;
	} else
		goto tr179;
	goto tr7;
case 53:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr180;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr180;
	} else
		goto tr180;
	goto tr7;
case 54:
	switch( (*p) ) {
		case 34: goto tr182;
		case 92: goto tr183;
	}
	goto tr181;
case 55:
	if ( (*p) == 10 )
		goto tr123;
	goto tr125;
case 56:
	switch( (*p) ) {
		case 10: goto tr185;
		case 32: goto tr184;
		case 35: goto tr186;
		case 41: goto tr187;
		case 44: goto tr188;
		case 95: goto tr189;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr184;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr189;
		} else if ( (*p) >= 65 )
			goto tr189;
	} else
		goto tr189;
	goto tr7;
case 57:
	switch( (*p) ) {
		case 34: goto tr190;
		case 39: goto tr191;
		case 92: goto tr192;
		case 97: goto tr193;
		case 98: goto tr194;
		case 102: goto tr195;
		case 110: goto tr196;
		case 114: goto tr197;
		case 116: goto tr198;
		case 118: goto tr199;
		case 120: goto tr200;
	}
	goto tr7;
case 58:
	switch( (*p) ) {
		case 34: goto tr202;
		case 92: goto tr203;
	}
	goto tr201;
case 59:
	switch( (*p) ) {
		case 34: goto tr205;
		case 92: goto tr206;
	}
	goto tr204;
case 60:
	switch( (*p) ) {
		case 34: goto tr208;
		case 92: goto tr209;
	}
	goto tr207;
case 61:
	switch( (*p) ) {
		case 34: goto tr211;
		case 92: goto tr212;
	}
	goto tr210;
case 62:
	switch( (*p) ) {
		case 34: goto tr214;
		case 92: goto tr215;
	}
	goto tr213;
case 63:
	switch( (*p) ) {
		case 34: goto tr217;
		case 92: goto tr218;
	}
	goto tr216;
case 64:
	switch( (*p) ) {
		case 34: goto tr220;
		case 92: goto tr221;
	}
	goto tr219;
case 65:
	switch( (*p) ) {
		case 34: goto tr223;
		case 92: goto tr224;
	}
	goto tr222;
case 66:
	switch( (*p) ) {
		case 34: goto tr226;
		case 92: goto tr227;
	}
	goto tr225;
case 67:
	switch( (*p) ) {
		case 34: goto tr229;
		case 92: goto tr230;
	}
	goto tr228;
case 68:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr231;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr231;
	} else
		goto tr231;
	goto tr7;
case 69:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr232;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr232;
	} else
		goto tr232;
	goto tr7;
case 70:
	switch( (*p) ) {
		case 34: goto tr234;
		case 92: goto tr235;
	}
	goto tr233;
case 71:
	if ( (*p) == 10 )
		goto tr92;
	goto tr94;
case 72:
	switch( (*p) ) {
		case 10: goto tr237;
		case 32: goto tr236;
		case 35: goto tr238;
		case 41: goto tr239;
		case 44: goto tr240;
		case 95: goto tr241;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr236;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr241;
		} else if ( (*p) >= 65 )
			goto tr241;
	} else
		goto tr241;
	goto tr7;
case 73:
	switch( (*p) ) {
		case 34: goto tr242;
		case 39: goto tr243;
		case 92: goto tr244;
		case 97: goto tr245;
		case 98: goto tr246;
		case 102: goto tr247;
		case 110: goto tr248;
		case 114: goto tr249;
		case 116: goto tr250;
		case 118: goto tr251;
		case 120: goto tr252;
	}
	goto tr7;
case 74:
	switch( (*p) ) {
		case 34: goto tr254;
		case 92: goto tr255;
	}
	goto tr253;
case 75:
	switch( (*p) ) {
		case 34: goto tr257;
		case 92: goto tr258;
	}
	goto tr256;
case 76:
	switch( (*p) ) {
		case 34: goto tr260;
		case 92: goto tr261;
	}
	goto tr259;
case 77:
	switch( (*p) ) {
		case 34: goto tr263;
		case 92: goto tr264;
	}
	goto tr262;
case 78:
	switch( (*p) ) {
		case 34: goto tr266;
		case 92: goto tr267;
	}
	goto tr265;
case 79:
	switch( (*p) ) {
		case 34: goto tr269;
		case 92: goto tr270;
	}
	goto tr268;
case 80:
	switch( (*p) ) {
		case 34: goto tr272;
		case 92: goto tr273;
	}
	goto tr271;
case 81:
	switch( (*p) ) {
		case 34: goto tr275;
		case 92: goto tr276;
	}
	goto tr274;
case 82:
	switch( (*p) ) {
		case 34: goto tr278;
		case 92: goto tr279;
	}
	goto tr277;
case 83:
	switch( (*p) ) {
		case 34: goto tr281;
		case 92: goto tr282;
	}
	goto tr280;
case 84:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr283;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr283;
	} else
		goto tr283;
	goto tr7;
case 85:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr284;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr284;
	} else
		goto tr284;
	goto tr7;
case 86:
	switch( (*p) ) {
		case 34: goto tr286;
		case 92: goto tr287;
	}
	goto tr285;
case 87:
	if ( (*p) == 10 )
		goto tr72;
	goto tr74;
case 88:
	switch( (*p) ) {
		case 10: goto tr289;
		case 32: goto tr288;
		case 35: goto tr290;
		case 40: goto tr291;
		case 59: goto tr293;
		case 95: goto tr292;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr288;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr292;
		} else if ( (*p) >= 65 )
			goto tr292;
	} else
		goto tr292;
	goto tr7;
case 89:
	switch( (*p) ) {
		case 34: goto tr294;
		case 39: goto tr295;
		case 92: goto tr296;
		case 97: goto tr297;
		case 98: goto tr298;
		case 102: goto tr299;
		case 110: goto tr300;
		case 114: goto tr301;
		case 116: goto tr302;
		case 118: goto tr303;
		case 120: goto tr304;
	}
	goto tr7;
case 90:
	switch( (*p) ) {
		case 34: goto tr306;
		case 92: goto tr307;
	}
	goto tr305;
case 91:
	switch( (*p) ) {
		case 34: goto tr309;
		case 92: goto tr310;
	}
	goto tr308;
case 92:
	switch( (*p) ) {
		case 34: goto tr312;
		case 92: goto tr313;
	}
	goto tr311;
case 93:
	switch( (*p) ) {
		case 34: goto tr315;
		case 92: goto tr316;
	}
	goto tr314;
case 94:
	switch( (*p) ) {
		case 34: goto tr318;
		case 92: goto tr319;
	}
	goto tr317;
case 95:
	switch( (*p) ) {
		case 34: goto tr321;
		case 92: goto tr322;
	}
	goto tr320;
case 96:
	switch( (*p) ) {
		case 34: goto tr324;
		case 92: goto tr325;
	}
	goto tr323;
case 97:
	switch( (*p) ) {
		case 34: goto tr327;
		case 92: goto tr328;
	}
	goto tr326;
case 98:
	switch( (*p) ) {
		case 34: goto tr330;
		case 92: goto tr331;
	}
	goto tr329;
case 99:
	switch( (*p) ) {
		case 34: goto tr333;
		case 92: goto tr334;
	}
	goto tr332;
case 100:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr335;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr335;
	} else
		goto tr335;
	goto tr7;
case 101:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr336;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr336;
	} else
		goto tr336;
	goto tr7;
case 102:
	switch( (*p) ) {
		case 34: goto tr338;
		case 92: goto tr339;
	}
	goto tr337;
case 103:
	if ( (*p) == 10 )
		goto tr49;
	goto tr51;
case 104:
	switch( (*p) ) {
		case 10: goto tr341;
		case 32: goto tr340;
		case 35: goto tr342;
		case 46: goto tr343;
		case 95: goto tr344;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr340;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr344;
		} else if ( (*p) >= 65 )
			goto tr344;
	} else
		goto tr344;
	goto tr7;
case 105:
	switch( (*p) ) {
		case 34: goto tr345;
		case 39: goto tr346;
		case 92: goto tr347;
		case 97: goto tr348;
		case 98: goto tr349;
		case 102: goto tr350;
		case 110: goto tr351;
		case 114: goto tr352;
		case 116: goto tr353;
		case 118: goto tr354;
		case 120: goto tr355;
	}
	goto tr7;
case 106:
	switch( (*p) ) {
		case 34: goto tr357;
		case 92: goto tr358;
	}
	goto tr356;
case 107:
	switch( (*p) ) {
		case 34: goto tr360;
		case 92: goto tr361;
	}
	goto tr359;
case 108:
	switch( (*p) ) {
		case 34: goto tr363;
		case 92: goto tr364;
	}
	goto tr362;
case 109:
	switch( (*p) ) {
		case 34: goto tr366;
		case 92: goto tr367;
	}
	goto tr365;
case 110:
	switch( (*p) ) {
		case 34: goto tr369;
		case 92: goto tr370;
	}
	goto tr368;
case 111:
	switch( (*p) ) {
		case 34: goto tr372;
		case 92: goto tr373;
	}
	goto tr371;
case 112:
	switch( (*p) ) {
		case 34: goto tr375;
		case 92: goto tr376;
	}
	goto tr374;
case 113:
	switch( (*p) ) {
		case 34: goto tr378;
		case 92: goto tr379;
	}
	goto tr377;
case 114:
	switch( (*p) ) {
		case 34: goto tr381;
		case 92: goto tr382;
	}
	goto tr380;
case 115:
	switch( (*p) ) {
		case 34: goto tr384;
		case 92: goto tr385;
	}
	goto tr383;
case 116:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr386;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr386;
	} else
		goto tr386;
	goto tr7;
case 117:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr387;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr387;
	} else
		goto tr387;
	goto tr7;
case 118:
	switch( (*p) ) {
		case 34: goto tr389;
		case 92: goto tr390;
	}
	goto tr388;
case 119:
	if ( (*p) == 10 )
		goto tr25;
	goto tr27;
case 120:
	switch( (*p) ) {
		case 10: goto tr392;
		case 32: goto tr391;
		case 35: goto tr393;
		case 44: goto tr394;
		case 61: goto tr396;
		case 95: goto tr395;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr391;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr395;
		} else if ( (*p) >= 65 )
			goto tr395;
	} else
		goto tr395;
	goto tr7;
case 121:
	switch( (*p) ) {
		case 34: goto tr397;
		case 39: goto tr398;
		case 92: goto tr399;
		case 97: goto tr400;
		case 98: goto tr401;
		case 102: goto tr402;
		case 110: goto tr403;
		case 114: goto tr404;
		case 116: goto tr405;
		case 118: goto tr406;
		case 120: goto tr407;
	}
	goto tr7;
case 122:
	switch( (*p) ) {
		case 34: goto tr409;
		case 92: goto tr410;
	}
	goto tr408;
case 123:
	switch( (*p) ) {
		case 34: goto tr412;
		case 92: goto tr413;
	}
	goto tr411;
case 124:
	switch( (*p) ) {
		case 34: goto tr415;
		case 92: goto tr416;
	}
	goto tr414;
case 125:
	switch( (*p) ) {
		case 34: goto tr418;
		case 92: goto tr419;
	}
	goto tr417;
case 126:
	switch( (*p) ) {
		case 34: goto tr421;
		case 92: goto tr422;
	}
	goto tr420;
case 127:
	switch( (*p) ) {
		case 34: goto tr424;
		case 92: goto tr425;
	}
	goto tr423;
case 128:
	switch( (*p) ) {
		case 34: goto tr427;
		case 92: goto tr428;
	}
	goto tr426;
case 129:
	switch( (*p) ) {
		case 34: goto tr430;
		case 92: goto tr431;
	}
	goto tr429;
case 130:
	switch( (*p) ) {
		case 34: goto tr433;
		case 92: goto tr434;
	}
	goto tr432;
case 131:
	switch( (*p) ) {
		case 34: goto tr436;
		case 92: goto tr437;
	}
	goto tr435;
case 132:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr438;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr438;
	} else
		goto tr438;
	goto tr7;
case 133:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr439;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr439;
	} else
		goto tr439;
	goto tr7;
case 134:
	switch( (*p) ) {
		case 34: goto tr441;
		case 92: goto tr442;
	}
	goto tr440;
	}

	tr7: cs = 0; goto _again;
	tr444: cs = 1; goto _again;
	tr448: cs = 1; goto f60;
	tr0: cs = 2; goto f0;
	tr3: cs = 2; goto f2;
	tr408: cs = 2; goto f28;
	tr411: cs = 2; goto f30;
	tr414: cs = 2; goto f32;
	tr417: cs = 2; goto f34;
	tr420: cs = 2; goto f36;
	tr423: cs = 2; goto f38;
	tr426: cs = 2; goto f40;
	tr429: cs = 2; goto f42;
	tr432: cs = 2; goto f44;
	tr435: cs = 2; goto f46;
	tr440: cs = 2; goto f50;
	tr1: cs = 3; goto f1;
	tr4: cs = 3; goto f3;
	tr409: cs = 3; goto f29;
	tr412: cs = 3; goto f31;
	tr415: cs = 3; goto f33;
	tr418: cs = 3; goto f35;
	tr421: cs = 3; goto f37;
	tr424: cs = 3; goto f39;
	tr427: cs = 3; goto f41;
	tr430: cs = 3; goto f43;
	tr433: cs = 3; goto f45;
	tr436: cs = 3; goto f47;
	tr441: cs = 3; goto f51;
	tr13: cs = 4; goto _again;
	tr6: cs = 4; goto f4;
	tr8: cs = 4; goto f5;
	tr14: cs = 4; goto f6;
	tr116: cs = 4; goto f23;
	tr117: cs = 4; goto f24;
	tr15: cs = 5; goto _again;
	tr9: cs = 5; goto f4;
	tr118: cs = 5; goto f23;
	tr16: cs = 6; goto _again;
	tr10: cs = 6; goto f4;
	tr38: cs = 6; goto f11;
	tr119: cs = 6; goto f23;
	tr394: cs = 6; goto f58;
	tr24: cs = 7; goto _again;
	tr25: cs = 7; goto f6;
	tr19: cs = 7; goto f7;
	tr20: cs = 7; goto f8;
	tr26: cs = 8; goto _again;
	tr21: cs = 8; goto f7;
	tr29: cs = 9; goto f0;
	tr32: cs = 9; goto f2;
	tr356: cs = 9; goto f28;
	tr359: cs = 9; goto f30;
	tr362: cs = 9; goto f32;
	tr365: cs = 9; goto f34;
	tr368: cs = 9; goto f36;
	tr371: cs = 9; goto f38;
	tr374: cs = 9; goto f40;
	tr377: cs = 9; goto f42;
	tr380: cs = 9; goto f44;
	tr383: cs = 9; goto f46;
	tr388: cs = 9; goto f50;
	tr30: cs = 10; goto f1;
	tr33: cs = 10; goto f3;
	tr357: cs = 10; goto f29;
	tr360: cs = 10; goto f31;
	tr363: cs = 10; goto f33;
	tr366: cs = 10; goto f35;
	tr369: cs = 10; goto f37;
	tr372: cs = 10; goto f39;
	tr375: cs = 10; goto f41;
	tr378: cs = 10; goto f43;
	tr381: cs = 10; goto f45;
	tr384: cs = 10; goto f47;
	tr389: cs = 10; goto f51;
	tr40: cs = 11; goto _again;
	tr41: cs = 11; goto f6;
	tr35: cs = 11; goto f11;
	tr36: cs = 11; goto f12;
	tr391: cs = 11; goto f58;
	tr392: cs = 11; goto f59;
	tr42: cs = 12; goto _again;
	tr37: cs = 12; goto f11;
	tr393: cs = 12; goto f58;
	tr18: cs = 13; goto _again;
	tr12: cs = 13; goto f4;
	tr39: cs = 13; goto f11;
	tr122: cs = 13; goto f23;
	tr396: cs = 13; goto f58;
	tr48: cs = 14; goto _again;
	tr49: cs = 14; goto f6;
	tr43: cs = 14; goto f13;
	tr44: cs = 14; goto f14;
	tr50: cs = 15; goto _again;
	tr45: cs = 15; goto f13;
	tr53: cs = 16; goto f0;
	tr56: cs = 16; goto f2;
	tr305: cs = 16; goto f28;
	tr308: cs = 16; goto f30;
	tr311: cs = 16; goto f32;
	tr314: cs = 16; goto f34;
	tr317: cs = 16; goto f36;
	tr320: cs = 16; goto f38;
	tr323: cs = 16; goto f40;
	tr326: cs = 16; goto f42;
	tr329: cs = 16; goto f44;
	tr332: cs = 16; goto f46;
	tr337: cs = 16; goto f50;
	tr54: cs = 17; goto f1;
	tr57: cs = 17; goto f3;
	tr306: cs = 17; goto f29;
	tr309: cs = 17; goto f31;
	tr312: cs = 17; goto f33;
	tr315: cs = 17; goto f35;
	tr318: cs = 17; goto f37;
	tr321: cs = 17; goto f39;
	tr324: cs = 17; goto f41;
	tr327: cs = 17; goto f43;
	tr330: cs = 17; goto f45;
	tr333: cs = 17; goto f47;
	tr338: cs = 17; goto f51;
	tr63: cs = 18; goto _again;
	tr64: cs = 18; goto f6;
	tr59: cs = 18; goto f11;
	tr60: cs = 18; goto f12;
	tr340: cs = 18; goto f58;
	tr341: cs = 18; goto f59;
	tr65: cs = 19; goto _again;
	tr61: cs = 19; goto f11;
	tr342: cs = 19; goto f58;
	tr17: cs = 20; goto _again;
	tr11: cs = 20; goto f4;
	tr62: cs = 20; goto f11;
	tr120: cs = 20; goto f23;
	tr343: cs = 20; goto f58;
	tr71: cs = 21; goto _again;
	tr72: cs = 21; goto f6;
	tr66: cs = 21; goto f16;
	tr67: cs = 21; goto f17;
	tr73: cs = 22; goto _again;
	tr68: cs = 22; goto f16;
	tr76: cs = 23; goto f0;
	tr79: cs = 23; goto f2;
	tr253: cs = 23; goto f28;
	tr256: cs = 23; goto f30;
	tr259: cs = 23; goto f32;
	tr262: cs = 23; goto f34;
	tr265: cs = 23; goto f36;
	tr268: cs = 23; goto f38;
	tr271: cs = 23; goto f40;
	tr274: cs = 23; goto f42;
	tr277: cs = 23; goto f44;
	tr280: cs = 23; goto f46;
	tr285: cs = 23; goto f50;
	tr77: cs = 24; goto f1;
	tr80: cs = 24; goto f3;
	tr254: cs = 24; goto f29;
	tr257: cs = 24; goto f31;
	tr260: cs = 24; goto f33;
	tr263: cs = 24; goto f35;
	tr266: cs = 24; goto f37;
	tr269: cs = 24; goto f39;
	tr272: cs = 24; goto f41;
	tr275: cs = 24; goto f43;
	tr278: cs = 24; goto f45;
	tr281: cs = 24; goto f47;
	tr286: cs = 24; goto f51;
	tr87: cs = 25; goto _again;
	tr88: cs = 25; goto f6;
	tr82: cs = 25; goto f19;
	tr83: cs = 25; goto f20;
	tr288: cs = 25; goto f56;
	tr289: cs = 25; goto f57;
	tr89: cs = 26; goto _again;
	tr84: cs = 26; goto f19;
	tr290: cs = 26; goto f56;
	tr90: cs = 27; goto _again;
	tr92: cs = 27; goto f6;
	tr85: cs = 27; goto f19;
	tr291: cs = 27; goto f56;
	tr93: cs = 28; goto _again;
	tr97: cs = 29; goto f0;
	tr100: cs = 29; goto f2;
	tr201: cs = 29; goto f28;
	tr204: cs = 29; goto f30;
	tr207: cs = 29; goto f32;
	tr210: cs = 29; goto f34;
	tr213: cs = 29; goto f36;
	tr216: cs = 29; goto f38;
	tr219: cs = 29; goto f40;
	tr222: cs = 29; goto f42;
	tr225: cs = 29; goto f44;
	tr228: cs = 29; goto f46;
	tr233: cs = 29; goto f50;
	tr98: cs = 30; goto f1;
	tr101: cs = 30; goto f3;
	tr202: cs = 30; goto f29;
	tr205: cs = 30; goto f31;
	tr208: cs = 30; goto f33;
	tr211: cs = 30; goto f35;
	tr214: cs = 30; goto f37;
	tr217: cs = 30; goto f39;
	tr220: cs = 30; goto f41;
	tr223: cs = 30; goto f43;
	tr226: cs = 30; goto f45;
	tr229: cs = 30; goto f47;
	tr234: cs = 30; goto f51;
	tr108: cs = 31; goto _again;
	tr109: cs = 31; goto f6;
	tr103: cs = 31; goto f21;
	tr104: cs = 31; goto f22;
	tr133: cs = 31; goto f25;
	tr134: cs = 31; goto f26;
	tr184: cs = 31; goto f52;
	tr185: cs = 31; goto f53;
	tr236: cs = 31; goto f54;
	tr237: cs = 31; goto f55;
	tr110: cs = 32; goto _again;
	tr105: cs = 32; goto f21;
	tr135: cs = 32; goto f25;
	tr186: cs = 32; goto f52;
	tr238: cs = 32; goto f54;
	tr95: cs = 33; goto _again;
	tr112: cs = 33; goto f6;
	tr106: cs = 33; goto f21;
	tr136: cs = 33; goto f25;
	tr187: cs = 33; goto f52;
	tr239: cs = 33; goto f54;
	tr113: cs = 34; goto _again;
	tr114: cs = 35; goto _again;
	tr449: cs = 35; goto f60;
	tr121: cs = 36; goto _again;
	tr445: cs = 36; goto f10;
	tr450: cs = 36; goto f62;
	tr111: cs = 37; goto _again;
	tr123: cs = 37; goto f6;
	tr107: cs = 37; goto f21;
	tr137: cs = 37; goto f25;
	tr188: cs = 37; goto f52;
	tr240: cs = 37; goto f54;
	tr124: cs = 38; goto _again;
	tr127: cs = 39; goto f0;
	tr130: cs = 39; goto f2;
	tr149: cs = 39; goto f28;
	tr152: cs = 39; goto f30;
	tr155: cs = 39; goto f32;
	tr158: cs = 39; goto f34;
	tr161: cs = 39; goto f36;
	tr164: cs = 39; goto f38;
	tr167: cs = 39; goto f40;
	tr170: cs = 39; goto f42;
	tr173: cs = 39; goto f44;
	tr176: cs = 39; goto f46;
	tr181: cs = 39; goto f50;
	tr128: cs = 40; goto f1;
	tr131: cs = 40; goto f3;
	tr150: cs = 40; goto f29;
	tr153: cs = 40; goto f31;
	tr156: cs = 40; goto f33;
	tr159: cs = 40; goto f35;
	tr162: cs = 40; goto f37;
	tr165: cs = 40; goto f39;
	tr168: cs = 40; goto f41;
	tr171: cs = 40; goto f43;
	tr174: cs = 40; goto f45;
	tr177: cs = 40; goto f47;
	tr182: cs = 40; goto f51;
	tr129: cs = 41; goto f1;
	tr132: cs = 41; goto f3;
	tr151: cs = 41; goto f29;
	tr154: cs = 41; goto f31;
	tr157: cs = 41; goto f33;
	tr160: cs = 41; goto f35;
	tr163: cs = 41; goto f37;
	tr166: cs = 41; goto f39;
	tr169: cs = 41; goto f41;
	tr172: cs = 41; goto f43;
	tr175: cs = 41; goto f45;
	tr178: cs = 41; goto f47;
	tr183: cs = 41; goto f51;
	tr138: cs = 42; goto _again;
	tr139: cs = 43; goto _again;
	tr140: cs = 44; goto _again;
	tr141: cs = 45; goto _again;
	tr142: cs = 46; goto _again;
	tr143: cs = 47; goto _again;
	tr144: cs = 48; goto _again;
	tr145: cs = 49; goto _again;
	tr146: cs = 50; goto _again;
	tr147: cs = 51; goto _again;
	tr148: cs = 52; goto f27;
	tr179: cs = 53; goto f48;
	tr180: cs = 54; goto f49;
	tr125: cs = 55; goto _again;
	tr189: cs = 56; goto _again;
	tr126: cs = 56; goto f10;
	tr99: cs = 57; goto f1;
	tr102: cs = 57; goto f3;
	tr203: cs = 57; goto f29;
	tr206: cs = 57; goto f31;
	tr209: cs = 57; goto f33;
	tr212: cs = 57; goto f35;
	tr215: cs = 57; goto f37;
	tr218: cs = 57; goto f39;
	tr221: cs = 57; goto f41;
	tr224: cs = 57; goto f43;
	tr227: cs = 57; goto f45;
	tr230: cs = 57; goto f47;
	tr235: cs = 57; goto f51;
	tr190: cs = 58; goto _again;
	tr191: cs = 59; goto _again;
	tr192: cs = 60; goto _again;
	tr193: cs = 61; goto _again;
	tr194: cs = 62; goto _again;
	tr195: cs = 63; goto _again;
	tr196: cs = 64; goto _again;
	tr197: cs = 65; goto _again;
	tr198: cs = 66; goto _again;
	tr199: cs = 67; goto _again;
	tr200: cs = 68; goto f27;
	tr231: cs = 69; goto f48;
	tr232: cs = 70; goto f49;
	tr94: cs = 71; goto _again;
	tr241: cs = 72; goto _again;
	tr96: cs = 72; goto f10;
	tr78: cs = 73; goto f1;
	tr81: cs = 73; goto f3;
	tr255: cs = 73; goto f29;
	tr258: cs = 73; goto f31;
	tr261: cs = 73; goto f33;
	tr264: cs = 73; goto f35;
	tr267: cs = 73; goto f37;
	tr270: cs = 73; goto f39;
	tr273: cs = 73; goto f41;
	tr276: cs = 73; goto f43;
	tr279: cs = 73; goto f45;
	tr282: cs = 73; goto f47;
	tr287: cs = 73; goto f51;
	tr242: cs = 74; goto _again;
	tr243: cs = 75; goto _again;
	tr244: cs = 76; goto _again;
	tr245: cs = 77; goto _again;
	tr246: cs = 78; goto _again;
	tr247: cs = 79; goto _again;
	tr248: cs = 80; goto _again;
	tr249: cs = 81; goto _again;
	tr250: cs = 82; goto _again;
	tr251: cs = 83; goto _again;
	tr252: cs = 84; goto f27;
	tr283: cs = 85; goto f48;
	tr284: cs = 86; goto f49;
	tr74: cs = 87; goto _again;
	tr69: cs = 87; goto f16;
	tr292: cs = 88; goto _again;
	tr75: cs = 88; goto f10;
	tr70: cs = 88; goto f18;
	tr55: cs = 89; goto f1;
	tr58: cs = 89; goto f3;
	tr307: cs = 89; goto f29;
	tr310: cs = 89; goto f31;
	tr313: cs = 89; goto f33;
	tr316: cs = 89; goto f35;
	tr319: cs = 89; goto f37;
	tr322: cs = 89; goto f39;
	tr325: cs = 89; goto f41;
	tr328: cs = 89; goto f43;
	tr331: cs = 89; goto f45;
	tr334: cs = 89; goto f47;
	tr339: cs = 89; goto f51;
	tr294: cs = 90; goto _again;
	tr295: cs = 91; goto _again;
	tr296: cs = 92; goto _again;
	tr297: cs = 93; goto _again;
	tr298: cs = 94; goto _again;
	tr299: cs = 95; goto _again;
	tr300: cs = 96; goto _again;
	tr301: cs = 97; goto _again;
	tr302: cs = 98; goto _again;
	tr303: cs = 99; goto _again;
	tr304: cs = 100; goto f27;
	tr335: cs = 101; goto f48;
	tr336: cs = 102; goto f49;
	tr51: cs = 103; goto _again;
	tr46: cs = 103; goto f13;
	tr344: cs = 104; goto _again;
	tr52: cs = 104; goto f10;
	tr47: cs = 104; goto f15;
	tr31: cs = 105; goto f1;
	tr34: cs = 105; goto f3;
	tr358: cs = 105; goto f29;
	tr361: cs = 105; goto f31;
	tr364: cs = 105; goto f33;
	tr367: cs = 105; goto f35;
	tr370: cs = 105; goto f37;
	tr373: cs = 105; goto f39;
	tr376: cs = 105; goto f41;
	tr379: cs = 105; goto f43;
	tr382: cs = 105; goto f45;
	tr385: cs = 105; goto f47;
	tr390: cs = 105; goto f51;
	tr345: cs = 106; goto _again;
	tr346: cs = 107; goto _again;
	tr347: cs = 108; goto _again;
	tr348: cs = 109; goto _again;
	tr349: cs = 110; goto _again;
	tr350: cs = 111; goto _again;
	tr351: cs = 112; goto _again;
	tr352: cs = 113; goto _again;
	tr353: cs = 114; goto _again;
	tr354: cs = 115; goto _again;
	tr355: cs = 116; goto f27;
	tr386: cs = 117; goto f48;
	tr387: cs = 118; goto f49;
	tr27: cs = 119; goto _again;
	tr22: cs = 119; goto f7;
	tr395: cs = 120; goto _again;
	tr23: cs = 120; goto f9;
	tr28: cs = 120; goto f10;
	tr2: cs = 121; goto f1;
	tr5: cs = 121; goto f3;
	tr410: cs = 121; goto f29;
	tr413: cs = 121; goto f31;
	tr416: cs = 121; goto f33;
	tr419: cs = 121; goto f35;
	tr422: cs = 121; goto f37;
	tr425: cs = 121; goto f39;
	tr428: cs = 121; goto f41;
	tr431: cs = 121; goto f43;
	tr434: cs = 121; goto f45;
	tr437: cs = 121; goto f47;
	tr442: cs = 121; goto f51;
	tr397: cs = 122; goto _again;
	tr398: cs = 123; goto _again;
	tr399: cs = 124; goto _again;
	tr400: cs = 125; goto _again;
	tr401: cs = 126; goto _again;
	tr402: cs = 127; goto _again;
	tr403: cs = 128; goto _again;
	tr404: cs = 129; goto _again;
	tr405: cs = 130; goto _again;
	tr406: cs = 131; goto _again;
	tr407: cs = 132; goto f27;
	tr438: cs = 133; goto f48;
	tr439: cs = 134; goto f49;
	tr443: cs = 135; goto _again;
	tr115: cs = 135; goto f6;
	tr446: cs = 135; goto f60;
	tr447: cs = 135; goto f61;
	tr91: cs = 136; goto _again;
	tr86: cs = 136; goto f19;
	tr293: cs = 136; goto f56;

f10:
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f6:
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f33:
#line 52 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\\", 1); }
	goto _again;
f29:
#line 53 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\"", 1); }
	goto _again;
f31:
#line 54 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "'",  1); }
	goto _again;
f35:
#line 55 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\a",  1); }
	goto _again;
f37:
#line 56 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\b",  1); }
	goto _again;
f39:
#line 57 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\f",  1); }
	goto _again;
f41:
#line 58 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\n",  1); }
	goto _again;
f43:
#line 59 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\r",  1); }
	goto _again;
f45:
#line 60 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\t",  1); }
	goto _again;
f47:
#line 61 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\v",  1); }
	goto _again;
f49:
#line 63 "libswanson/s0-parser.c.rl"
	{ hex_char |= digit_values[(int) (*p)]; }
	goto _again;
f27:
#line 64 "libswanson/s0-parser.c.rl"
	{ hex_char = 0; }
	goto _again;
f51:
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
	{ swan_ast_add_call(s, ast, call); }
	goto _again;
f58:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
	goto _again;
f32:
#line 52 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\\", 1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f28:
#line 53 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\"", 1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f30:
#line 54 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "'",  1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f34:
#line 55 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\a",  1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f36:
#line 56 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\b",  1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f38:
#line 57 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\f",  1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f40:
#line 58 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\n",  1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f42:
#line 59 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\r",  1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f44:
#line 60 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\t",  1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f46:
#line 61 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\v",  1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f48:
#line 63 "libswanson/s0-parser.c.rl"
	{ hex_char |= digit_values[(int) (*p)]; }
#line 65 "libswanson/s0-parser.c.rl"
	{ hex_char <<= 4; }
	goto _again;
f50:
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
f25:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 96 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_add_param(s, call, id); }
	goto _again;
f4:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 100 "libswanson/s0-parser.c.rl"
	{ call = swan_ast_call_new(s); }
	goto _again;
f19:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 102 "libswanson/s0-parser.c.rl"
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
	{ swan_ast_add_call(s, ast, call); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f61:
#line 105 "libswanson/s0-parser.c.rl"
	{ swan_ast_add_call(s, ast, call); }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f59:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f54:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 94 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_add_param(s, call, id); }
	goto _again;
f52:
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
#line 100 "libswanson/s0-parser.c.rl"
	{ call = swan_ast_call_new(s); }
	goto _again;
f56:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 102 "libswanson/s0-parser.c.rl"
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
f26:
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
#line 100 "libswanson/s0-parser.c.rl"
	{ call = swan_ast_call_new(s); }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f20:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 102 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_set_method(s, call, id); }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f55:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 94 "libswanson/s0-parser.c.rl"
	{ swan_ast_call_add_param(s, call, id); }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f53:
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
#line 100 "libswanson/s0-parser.c.rl"
	{ call = swan_ast_call_new(s); }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f57:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 102 "libswanson/s0-parser.c.rl"
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
	{ swan_ast_add_call(s, ast, call); }
	break;
#line 2102 "/home/dcreager/git/swanson/src/libswanson/s0-parser.c"
	}
	}

	_out: {}
	}

#line 134 "libswanson/s0-parser.c.rl"

    if (cs < 
#line 2112 "/home/dcreager/git/swanson/src/libswanson/s0-parser.c"
135
#line 135 "libswanson/s0-parser.c.rl"
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
