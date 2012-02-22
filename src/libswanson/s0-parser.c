
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


#line 125 "libswanson/s0-parser.c.rl"



#line 43 "/Users/dcreager/git/swanson/src/libswanson/s0-parser.c"
static const short _s0_eof_actions[] = {
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
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 73, 0, 77
};

static const int s0_start = 1;
static const int s0_first_final = 205;
static const int s0_error = 0;

static const int s0_en_main = 1;


#line 128 "libswanson/s0-parser.c.rl"

struct swan_sllist *
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
    const char  *xs;
    struct cork_buffer  scratch = CORK_BUFFER_INIT();
    char  hex_char;

    struct swan_static_string  *thing;
    struct swan_static_string  *method;
    struct swan_static_string  *id;
    struct swan_sllist  *list_build;
    struct swan_sllist  *id_list;
    struct swan_sllist  *id_tail;
    struct swan_sllist  *param_list;
    struct swan_sllist  *result_list;
    struct swan_ast_call  *call = NULL;
    struct swan_sllist  *ast_list;
    struct swan_sllist  *ast_tail;

    
#line 111 "/Users/dcreager/git/swanson/src/libswanson/s0-parser.c"
	{
	cs = s0_start;
	}

#line 158 "libswanson/s0-parser.c.rl"
    
#line 118 "/Users/dcreager/git/swanson/src/libswanson/s0-parser.c"
	{
	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	switch ( cs ) {
case 1:
	switch( (*p) ) {
		case 10: goto tr2;
		case 32: goto tr0;
		case 34: goto tr3;
		case 35: goto tr4;
		case 61: goto tr5;
		case 95: goto tr6;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr0;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr6;
	} else
		goto tr6;
	goto tr1;
case 0:
	goto _out;
case 2:
	switch( (*p) ) {
		case 10: goto tr8;
		case 32: goto tr7;
		case 34: goto tr9;
		case 35: goto tr10;
		case 61: goto tr11;
		case 95: goto tr12;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr7;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr12;
	} else
		goto tr12;
	goto tr1;
case 3:
	switch( (*p) ) {
		case 34: goto tr14;
		case 92: goto tr15;
	}
	goto tr13;
case 4:
	switch( (*p) ) {
		case 34: goto tr17;
		case 92: goto tr18;
	}
	goto tr16;
case 5:
	switch( (*p) ) {
		case 10: goto tr20;
		case 32: goto tr19;
		case 35: goto tr21;
		case 44: goto tr22;
		case 46: goto tr23;
		case 61: goto tr24;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr19;
	goto tr1;
case 6:
	switch( (*p) ) {
		case 10: goto tr26;
		case 32: goto tr25;
		case 35: goto tr27;
		case 44: goto tr28;
		case 46: goto tr29;
		case 61: goto tr11;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr25;
	goto tr1;
case 7:
	if ( (*p) == 10 )
		goto tr26;
	goto tr27;
case 8:
	switch( (*p) ) {
		case 10: goto tr30;
		case 32: goto tr28;
		case 34: goto tr31;
		case 35: goto tr32;
		case 95: goto tr33;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr28;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr33;
	} else
		goto tr33;
	goto tr1;
case 9:
	switch( (*p) ) {
		case 34: goto tr35;
		case 92: goto tr36;
	}
	goto tr34;
case 10:
	switch( (*p) ) {
		case 34: goto tr38;
		case 92: goto tr39;
	}
	goto tr37;
case 11:
	switch( (*p) ) {
		case 10: goto tr41;
		case 32: goto tr40;
		case 35: goto tr42;
		case 44: goto tr43;
		case 61: goto tr44;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr40;
	goto tr1;
case 12:
	switch( (*p) ) {
		case 10: goto tr46;
		case 32: goto tr45;
		case 35: goto tr47;
		case 44: goto tr28;
		case 61: goto tr11;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr45;
	goto tr1;
case 13:
	if ( (*p) == 10 )
		goto tr46;
	goto tr47;
case 14:
	switch( (*p) ) {
		case 10: goto tr48;
		case 32: goto tr11;
		case 34: goto tr49;
		case 35: goto tr50;
		case 95: goto tr51;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr11;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr51;
	} else
		goto tr51;
	goto tr1;
case 15:
	switch( (*p) ) {
		case 34: goto tr53;
		case 92: goto tr54;
	}
	goto tr52;
case 16:
	switch( (*p) ) {
		case 34: goto tr56;
		case 92: goto tr57;
	}
	goto tr55;
case 17:
	switch( (*p) ) {
		case 10: goto tr59;
		case 32: goto tr58;
		case 35: goto tr60;
		case 46: goto tr61;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr58;
	goto tr1;
case 18:
	switch( (*p) ) {
		case 10: goto tr63;
		case 32: goto tr62;
		case 35: goto tr64;
		case 46: goto tr65;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr62;
	goto tr1;
case 19:
	if ( (*p) == 10 )
		goto tr63;
	goto tr64;
case 20:
	switch( (*p) ) {
		case 10: goto tr66;
		case 32: goto tr65;
		case 34: goto tr67;
		case 35: goto tr68;
		case 95: goto tr69;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr65;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr69;
	} else
		goto tr69;
	goto tr1;
case 21:
	switch( (*p) ) {
		case 34: goto tr71;
		case 92: goto tr72;
	}
	goto tr70;
case 22:
	switch( (*p) ) {
		case 34: goto tr74;
		case 92: goto tr75;
	}
	goto tr73;
case 23:
	switch( (*p) ) {
		case 10: goto tr77;
		case 32: goto tr76;
		case 35: goto tr78;
		case 40: goto tr79;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr76;
	goto tr1;
case 24:
	switch( (*p) ) {
		case 10: goto tr81;
		case 32: goto tr80;
		case 35: goto tr82;
		case 40: goto tr83;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr80;
	goto tr1;
case 25:
	if ( (*p) == 10 )
		goto tr81;
	goto tr82;
case 26:
	switch( (*p) ) {
		case 10: goto tr85;
		case 32: goto tr84;
		case 34: goto tr86;
		case 35: goto tr87;
		case 41: goto tr88;
		case 95: goto tr89;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr84;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr89;
	} else
		goto tr89;
	goto tr1;
case 27:
	switch( (*p) ) {
		case 10: goto tr85;
		case 32: goto tr84;
		case 34: goto tr86;
		case 35: goto tr87;
		case 95: goto tr89;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr84;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr89;
	} else
		goto tr89;
	goto tr1;
case 28:
	switch( (*p) ) {
		case 34: goto tr91;
		case 92: goto tr92;
	}
	goto tr90;
case 29:
	switch( (*p) ) {
		case 34: goto tr94;
		case 92: goto tr95;
	}
	goto tr93;
case 30:
	switch( (*p) ) {
		case 10: goto tr97;
		case 32: goto tr96;
		case 35: goto tr98;
		case 41: goto tr99;
		case 44: goto tr100;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr96;
	goto tr1;
case 31:
	switch( (*p) ) {
		case 10: goto tr102;
		case 32: goto tr101;
		case 35: goto tr103;
		case 44: goto tr104;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr101;
	goto tr1;
case 32:
	if ( (*p) == 10 )
		goto tr102;
	goto tr103;
case 33:
	switch( (*p) ) {
		case 10: goto tr105;
		case 32: goto tr104;
		case 34: goto tr106;
		case 35: goto tr107;
		case 95: goto tr108;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr104;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr108;
	} else
		goto tr108;
	goto tr1;
case 34:
	switch( (*p) ) {
		case 34: goto tr110;
		case 92: goto tr111;
	}
	goto tr109;
case 35:
	switch( (*p) ) {
		case 34: goto tr113;
		case 92: goto tr114;
	}
	goto tr112;
case 36:
	switch( (*p) ) {
		case 10: goto tr116;
		case 32: goto tr115;
		case 35: goto tr117;
		case 41: goto tr118;
		case 44: goto tr119;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr115;
	goto tr1;
case 37:
	switch( (*p) ) {
		case 10: goto tr121;
		case 32: goto tr120;
		case 35: goto tr122;
		case 59: goto tr123;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr120;
	goto tr1;
case 38:
	switch( (*p) ) {
		case 10: goto tr125;
		case 32: goto tr124;
		case 35: goto tr126;
		case 59: goto tr127;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr124;
	goto tr1;
case 39:
	if ( (*p) == 10 )
		goto tr125;
	goto tr126;
case 205:
	switch( (*p) ) {
		case 10: goto tr659;
		case 32: goto tr658;
		case 34: goto tr660;
		case 35: goto tr661;
		case 61: goto tr662;
		case 95: goto tr663;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr658;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr663;
	} else
		goto tr663;
	goto tr1;
case 206:
	switch( (*p) ) {
		case 10: goto tr129;
		case 32: goto tr664;
		case 34: goto tr9;
		case 35: goto tr128;
		case 61: goto tr11;
		case 95: goto tr12;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr664;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr12;
	} else
		goto tr12;
	goto tr1;
case 40:
	if ( (*p) == 10 )
		goto tr129;
	goto tr128;
case 41:
	switch( (*p) ) {
		case 10: goto tr131;
		case 32: goto tr130;
		case 35: goto tr132;
		case 44: goto tr133;
		case 46: goto tr134;
		case 61: goto tr136;
		case 95: goto tr135;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr130;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr135;
		} else if ( (*p) >= 65 )
			goto tr135;
	} else
		goto tr135;
	goto tr1;
case 42:
	switch( (*p) ) {
		case 10: goto tr137;
		case 32: goto tr29;
		case 34: goto tr138;
		case 35: goto tr139;
		case 95: goto tr140;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr29;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr140;
	} else
		goto tr140;
	goto tr1;
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
		case 40: goto tr150;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr147;
	goto tr1;
case 46:
	switch( (*p) ) {
		case 10: goto tr152;
		case 32: goto tr151;
		case 35: goto tr153;
		case 40: goto tr154;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr151;
	goto tr1;
case 47:
	if ( (*p) == 10 )
		goto tr152;
	goto tr153;
case 48:
	switch( (*p) ) {
		case 10: goto tr156;
		case 32: goto tr155;
		case 34: goto tr157;
		case 35: goto tr158;
		case 41: goto tr159;
		case 95: goto tr160;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr155;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr160;
	} else
		goto tr160;
	goto tr1;
case 49:
	switch( (*p) ) {
		case 10: goto tr156;
		case 32: goto tr155;
		case 34: goto tr157;
		case 35: goto tr158;
		case 95: goto tr160;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr155;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr160;
	} else
		goto tr160;
	goto tr1;
case 50:
	switch( (*p) ) {
		case 34: goto tr162;
		case 92: goto tr163;
	}
	goto tr161;
case 51:
	switch( (*p) ) {
		case 34: goto tr165;
		case 92: goto tr166;
	}
	goto tr164;
case 52:
	switch( (*p) ) {
		case 10: goto tr168;
		case 32: goto tr167;
		case 35: goto tr169;
		case 41: goto tr170;
		case 44: goto tr171;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr167;
	goto tr1;
case 53:
	switch( (*p) ) {
		case 10: goto tr173;
		case 32: goto tr172;
		case 35: goto tr174;
		case 44: goto tr175;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr172;
	goto tr1;
case 54:
	if ( (*p) == 10 )
		goto tr173;
	goto tr174;
case 55:
	switch( (*p) ) {
		case 10: goto tr176;
		case 32: goto tr175;
		case 34: goto tr177;
		case 35: goto tr178;
		case 95: goto tr179;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr175;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr179;
	} else
		goto tr179;
	goto tr1;
case 56:
	switch( (*p) ) {
		case 34: goto tr181;
		case 92: goto tr182;
	}
	goto tr180;
case 57:
	switch( (*p) ) {
		case 34: goto tr184;
		case 92: goto tr185;
	}
	goto tr183;
case 58:
	switch( (*p) ) {
		case 10: goto tr187;
		case 32: goto tr186;
		case 35: goto tr188;
		case 41: goto tr189;
		case 44: goto tr190;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr186;
	goto tr1;
case 59:
	switch( (*p) ) {
		case 10: goto tr192;
		case 32: goto tr191;
		case 35: goto tr193;
		case 59: goto tr194;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr191;
	goto tr1;
case 60:
	switch( (*p) ) {
		case 10: goto tr196;
		case 32: goto tr195;
		case 35: goto tr197;
		case 59: goto tr198;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr195;
	goto tr1;
case 61:
	if ( (*p) == 10 )
		goto tr196;
	goto tr197;
case 207:
	switch( (*p) ) {
		case 10: goto tr666;
		case 32: goto tr665;
		case 34: goto tr667;
		case 35: goto tr668;
		case 61: goto tr669;
		case 95: goto tr670;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr665;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr670;
	} else
		goto tr670;
	goto tr1;
case 62:
	switch( (*p) ) {
		case 34: goto tr199;
		case 39: goto tr200;
		case 92: goto tr201;
		case 97: goto tr202;
		case 98: goto tr203;
		case 102: goto tr204;
		case 110: goto tr205;
		case 114: goto tr206;
		case 116: goto tr207;
		case 118: goto tr208;
		case 120: goto tr209;
	}
	goto tr1;
case 63:
	switch( (*p) ) {
		case 34: goto tr211;
		case 92: goto tr212;
	}
	goto tr210;
case 64:
	switch( (*p) ) {
		case 34: goto tr214;
		case 92: goto tr215;
	}
	goto tr213;
case 65:
	switch( (*p) ) {
		case 34: goto tr217;
		case 92: goto tr218;
	}
	goto tr216;
case 66:
	switch( (*p) ) {
		case 34: goto tr220;
		case 92: goto tr221;
	}
	goto tr219;
case 67:
	switch( (*p) ) {
		case 34: goto tr223;
		case 92: goto tr224;
	}
	goto tr222;
case 68:
	switch( (*p) ) {
		case 34: goto tr226;
		case 92: goto tr227;
	}
	goto tr225;
case 69:
	switch( (*p) ) {
		case 34: goto tr229;
		case 92: goto tr230;
	}
	goto tr228;
case 70:
	switch( (*p) ) {
		case 34: goto tr232;
		case 92: goto tr233;
	}
	goto tr231;
case 71:
	switch( (*p) ) {
		case 34: goto tr235;
		case 92: goto tr236;
	}
	goto tr234;
case 72:
	switch( (*p) ) {
		case 34: goto tr238;
		case 92: goto tr239;
	}
	goto tr237;
case 73:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr240;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr240;
	} else
		goto tr240;
	goto tr1;
case 74:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr241;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr241;
	} else
		goto tr241;
	goto tr1;
case 75:
	switch( (*p) ) {
		case 34: goto tr243;
		case 92: goto tr244;
	}
	goto tr242;
case 76:
	if ( (*p) == 10 )
		goto tr176;
	goto tr178;
case 77:
	switch( (*p) ) {
		case 10: goto tr246;
		case 32: goto tr245;
		case 35: goto tr247;
		case 41: goto tr248;
		case 44: goto tr249;
		case 95: goto tr250;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr245;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr250;
		} else if ( (*p) >= 65 )
			goto tr250;
	} else
		goto tr250;
	goto tr1;
case 78:
	switch( (*p) ) {
		case 34: goto tr251;
		case 39: goto tr252;
		case 92: goto tr253;
		case 97: goto tr254;
		case 98: goto tr255;
		case 102: goto tr256;
		case 110: goto tr257;
		case 114: goto tr258;
		case 116: goto tr259;
		case 118: goto tr260;
		case 120: goto tr261;
	}
	goto tr1;
case 79:
	switch( (*p) ) {
		case 34: goto tr263;
		case 92: goto tr264;
	}
	goto tr262;
case 80:
	switch( (*p) ) {
		case 34: goto tr266;
		case 92: goto tr267;
	}
	goto tr265;
case 81:
	switch( (*p) ) {
		case 34: goto tr269;
		case 92: goto tr270;
	}
	goto tr268;
case 82:
	switch( (*p) ) {
		case 34: goto tr272;
		case 92: goto tr273;
	}
	goto tr271;
case 83:
	switch( (*p) ) {
		case 34: goto tr275;
		case 92: goto tr276;
	}
	goto tr274;
case 84:
	switch( (*p) ) {
		case 34: goto tr278;
		case 92: goto tr279;
	}
	goto tr277;
case 85:
	switch( (*p) ) {
		case 34: goto tr281;
		case 92: goto tr282;
	}
	goto tr280;
case 86:
	switch( (*p) ) {
		case 34: goto tr284;
		case 92: goto tr285;
	}
	goto tr283;
case 87:
	switch( (*p) ) {
		case 34: goto tr287;
		case 92: goto tr288;
	}
	goto tr286;
case 88:
	switch( (*p) ) {
		case 34: goto tr290;
		case 92: goto tr291;
	}
	goto tr289;
case 89:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr292;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr292;
	} else
		goto tr292;
	goto tr1;
case 90:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr293;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr293;
	} else
		goto tr293;
	goto tr1;
case 91:
	switch( (*p) ) {
		case 34: goto tr295;
		case 92: goto tr296;
	}
	goto tr294;
case 92:
	if ( (*p) == 10 )
		goto tr156;
	goto tr158;
case 93:
	switch( (*p) ) {
		case 10: goto tr298;
		case 32: goto tr297;
		case 35: goto tr299;
		case 41: goto tr300;
		case 44: goto tr301;
		case 95: goto tr302;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr297;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr302;
		} else if ( (*p) >= 65 )
			goto tr302;
	} else
		goto tr302;
	goto tr1;
case 94:
	switch( (*p) ) {
		case 34: goto tr303;
		case 39: goto tr304;
		case 92: goto tr305;
		case 97: goto tr306;
		case 98: goto tr307;
		case 102: goto tr308;
		case 110: goto tr309;
		case 114: goto tr310;
		case 116: goto tr311;
		case 118: goto tr312;
		case 120: goto tr313;
	}
	goto tr1;
case 95:
	switch( (*p) ) {
		case 34: goto tr315;
		case 92: goto tr316;
	}
	goto tr314;
case 96:
	switch( (*p) ) {
		case 34: goto tr318;
		case 92: goto tr319;
	}
	goto tr317;
case 97:
	switch( (*p) ) {
		case 34: goto tr321;
		case 92: goto tr322;
	}
	goto tr320;
case 98:
	switch( (*p) ) {
		case 34: goto tr324;
		case 92: goto tr325;
	}
	goto tr323;
case 99:
	switch( (*p) ) {
		case 34: goto tr327;
		case 92: goto tr328;
	}
	goto tr326;
case 100:
	switch( (*p) ) {
		case 34: goto tr330;
		case 92: goto tr331;
	}
	goto tr329;
case 101:
	switch( (*p) ) {
		case 34: goto tr333;
		case 92: goto tr334;
	}
	goto tr332;
case 102:
	switch( (*p) ) {
		case 34: goto tr336;
		case 92: goto tr337;
	}
	goto tr335;
case 103:
	switch( (*p) ) {
		case 34: goto tr339;
		case 92: goto tr340;
	}
	goto tr338;
case 104:
	switch( (*p) ) {
		case 34: goto tr342;
		case 92: goto tr343;
	}
	goto tr341;
case 105:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr344;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr344;
	} else
		goto tr344;
	goto tr1;
case 106:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr345;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr345;
	} else
		goto tr345;
	goto tr1;
case 107:
	switch( (*p) ) {
		case 34: goto tr347;
		case 92: goto tr348;
	}
	goto tr346;
case 108:
	if ( (*p) == 10 )
		goto tr137;
	goto tr139;
case 109:
	switch( (*p) ) {
		case 10: goto tr350;
		case 32: goto tr349;
		case 35: goto tr351;
		case 40: goto tr352;
		case 95: goto tr353;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr349;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr353;
		} else if ( (*p) >= 65 )
			goto tr353;
	} else
		goto tr353;
	goto tr1;
case 110:
	switch( (*p) ) {
		case 34: goto tr354;
		case 39: goto tr355;
		case 92: goto tr356;
		case 97: goto tr357;
		case 98: goto tr358;
		case 102: goto tr359;
		case 110: goto tr360;
		case 114: goto tr361;
		case 116: goto tr362;
		case 118: goto tr363;
		case 120: goto tr364;
	}
	goto tr1;
case 111:
	switch( (*p) ) {
		case 34: goto tr366;
		case 92: goto tr367;
	}
	goto tr365;
case 112:
	switch( (*p) ) {
		case 34: goto tr369;
		case 92: goto tr370;
	}
	goto tr368;
case 113:
	switch( (*p) ) {
		case 34: goto tr372;
		case 92: goto tr373;
	}
	goto tr371;
case 114:
	switch( (*p) ) {
		case 34: goto tr375;
		case 92: goto tr376;
	}
	goto tr374;
case 115:
	switch( (*p) ) {
		case 34: goto tr378;
		case 92: goto tr379;
	}
	goto tr377;
case 116:
	switch( (*p) ) {
		case 34: goto tr381;
		case 92: goto tr382;
	}
	goto tr380;
case 117:
	switch( (*p) ) {
		case 34: goto tr384;
		case 92: goto tr385;
	}
	goto tr383;
case 118:
	switch( (*p) ) {
		case 34: goto tr387;
		case 92: goto tr388;
	}
	goto tr386;
case 119:
	switch( (*p) ) {
		case 34: goto tr390;
		case 92: goto tr391;
	}
	goto tr389;
case 120:
	switch( (*p) ) {
		case 34: goto tr393;
		case 92: goto tr394;
	}
	goto tr392;
case 121:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr395;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr395;
	} else
		goto tr395;
	goto tr1;
case 122:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr396;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr396;
	} else
		goto tr396;
	goto tr1;
case 123:
	switch( (*p) ) {
		case 34: goto tr398;
		case 92: goto tr399;
	}
	goto tr397;
case 124:
	if ( (*p) == 10 )
		goto tr105;
	goto tr107;
case 125:
	switch( (*p) ) {
		case 10: goto tr401;
		case 32: goto tr400;
		case 35: goto tr402;
		case 41: goto tr403;
		case 44: goto tr404;
		case 95: goto tr405;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr400;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr405;
		} else if ( (*p) >= 65 )
			goto tr405;
	} else
		goto tr405;
	goto tr1;
case 126:
	switch( (*p) ) {
		case 34: goto tr406;
		case 39: goto tr407;
		case 92: goto tr408;
		case 97: goto tr409;
		case 98: goto tr410;
		case 102: goto tr411;
		case 110: goto tr412;
		case 114: goto tr413;
		case 116: goto tr414;
		case 118: goto tr415;
		case 120: goto tr416;
	}
	goto tr1;
case 127:
	switch( (*p) ) {
		case 34: goto tr418;
		case 92: goto tr419;
	}
	goto tr417;
case 128:
	switch( (*p) ) {
		case 34: goto tr421;
		case 92: goto tr422;
	}
	goto tr420;
case 129:
	switch( (*p) ) {
		case 34: goto tr424;
		case 92: goto tr425;
	}
	goto tr423;
case 130:
	switch( (*p) ) {
		case 34: goto tr427;
		case 92: goto tr428;
	}
	goto tr426;
case 131:
	switch( (*p) ) {
		case 34: goto tr430;
		case 92: goto tr431;
	}
	goto tr429;
case 132:
	switch( (*p) ) {
		case 34: goto tr433;
		case 92: goto tr434;
	}
	goto tr432;
case 133:
	switch( (*p) ) {
		case 34: goto tr436;
		case 92: goto tr437;
	}
	goto tr435;
case 134:
	switch( (*p) ) {
		case 34: goto tr439;
		case 92: goto tr440;
	}
	goto tr438;
case 135:
	switch( (*p) ) {
		case 34: goto tr442;
		case 92: goto tr443;
	}
	goto tr441;
case 136:
	switch( (*p) ) {
		case 34: goto tr445;
		case 92: goto tr446;
	}
	goto tr444;
case 137:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr447;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr447;
	} else
		goto tr447;
	goto tr1;
case 138:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr448;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr448;
	} else
		goto tr448;
	goto tr1;
case 139:
	switch( (*p) ) {
		case 34: goto tr450;
		case 92: goto tr451;
	}
	goto tr449;
case 140:
	if ( (*p) == 10 )
		goto tr85;
	goto tr87;
case 141:
	switch( (*p) ) {
		case 10: goto tr453;
		case 32: goto tr452;
		case 35: goto tr454;
		case 41: goto tr455;
		case 44: goto tr456;
		case 95: goto tr457;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr452;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr457;
		} else if ( (*p) >= 65 )
			goto tr457;
	} else
		goto tr457;
	goto tr1;
case 142:
	switch( (*p) ) {
		case 34: goto tr458;
		case 39: goto tr459;
		case 92: goto tr460;
		case 97: goto tr461;
		case 98: goto tr462;
		case 102: goto tr463;
		case 110: goto tr464;
		case 114: goto tr465;
		case 116: goto tr466;
		case 118: goto tr467;
		case 120: goto tr468;
	}
	goto tr1;
case 143:
	switch( (*p) ) {
		case 34: goto tr470;
		case 92: goto tr471;
	}
	goto tr469;
case 144:
	switch( (*p) ) {
		case 34: goto tr473;
		case 92: goto tr474;
	}
	goto tr472;
case 145:
	switch( (*p) ) {
		case 34: goto tr476;
		case 92: goto tr477;
	}
	goto tr475;
case 146:
	switch( (*p) ) {
		case 34: goto tr479;
		case 92: goto tr480;
	}
	goto tr478;
case 147:
	switch( (*p) ) {
		case 34: goto tr482;
		case 92: goto tr483;
	}
	goto tr481;
case 148:
	switch( (*p) ) {
		case 34: goto tr485;
		case 92: goto tr486;
	}
	goto tr484;
case 149:
	switch( (*p) ) {
		case 34: goto tr488;
		case 92: goto tr489;
	}
	goto tr487;
case 150:
	switch( (*p) ) {
		case 34: goto tr491;
		case 92: goto tr492;
	}
	goto tr490;
case 151:
	switch( (*p) ) {
		case 34: goto tr494;
		case 92: goto tr495;
	}
	goto tr493;
case 152:
	switch( (*p) ) {
		case 34: goto tr497;
		case 92: goto tr498;
	}
	goto tr496;
case 153:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr499;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr499;
	} else
		goto tr499;
	goto tr1;
case 154:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr500;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr500;
	} else
		goto tr500;
	goto tr1;
case 155:
	switch( (*p) ) {
		case 34: goto tr502;
		case 92: goto tr503;
	}
	goto tr501;
case 156:
	if ( (*p) == 10 )
		goto tr66;
	goto tr68;
case 157:
	switch( (*p) ) {
		case 10: goto tr505;
		case 32: goto tr504;
		case 35: goto tr506;
		case 40: goto tr507;
		case 95: goto tr508;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr504;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr508;
		} else if ( (*p) >= 65 )
			goto tr508;
	} else
		goto tr508;
	goto tr1;
case 158:
	switch( (*p) ) {
		case 34: goto tr509;
		case 39: goto tr510;
		case 92: goto tr511;
		case 97: goto tr512;
		case 98: goto tr513;
		case 102: goto tr514;
		case 110: goto tr515;
		case 114: goto tr516;
		case 116: goto tr517;
		case 118: goto tr518;
		case 120: goto tr519;
	}
	goto tr1;
case 159:
	switch( (*p) ) {
		case 34: goto tr521;
		case 92: goto tr522;
	}
	goto tr520;
case 160:
	switch( (*p) ) {
		case 34: goto tr524;
		case 92: goto tr525;
	}
	goto tr523;
case 161:
	switch( (*p) ) {
		case 34: goto tr527;
		case 92: goto tr528;
	}
	goto tr526;
case 162:
	switch( (*p) ) {
		case 34: goto tr530;
		case 92: goto tr531;
	}
	goto tr529;
case 163:
	switch( (*p) ) {
		case 34: goto tr533;
		case 92: goto tr534;
	}
	goto tr532;
case 164:
	switch( (*p) ) {
		case 34: goto tr536;
		case 92: goto tr537;
	}
	goto tr535;
case 165:
	switch( (*p) ) {
		case 34: goto tr539;
		case 92: goto tr540;
	}
	goto tr538;
case 166:
	switch( (*p) ) {
		case 34: goto tr542;
		case 92: goto tr543;
	}
	goto tr541;
case 167:
	switch( (*p) ) {
		case 34: goto tr545;
		case 92: goto tr546;
	}
	goto tr544;
case 168:
	switch( (*p) ) {
		case 34: goto tr548;
		case 92: goto tr549;
	}
	goto tr547;
case 169:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr550;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr550;
	} else
		goto tr550;
	goto tr1;
case 170:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr551;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr551;
	} else
		goto tr551;
	goto tr1;
case 171:
	switch( (*p) ) {
		case 34: goto tr553;
		case 92: goto tr554;
	}
	goto tr552;
case 172:
	if ( (*p) == 10 )
		goto tr48;
	goto tr50;
case 173:
	switch( (*p) ) {
		case 10: goto tr556;
		case 32: goto tr555;
		case 35: goto tr557;
		case 46: goto tr558;
		case 95: goto tr559;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr555;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr559;
		} else if ( (*p) >= 65 )
			goto tr559;
	} else
		goto tr559;
	goto tr1;
case 174:
	switch( (*p) ) {
		case 34: goto tr560;
		case 39: goto tr561;
		case 92: goto tr562;
		case 97: goto tr563;
		case 98: goto tr564;
		case 102: goto tr565;
		case 110: goto tr566;
		case 114: goto tr567;
		case 116: goto tr568;
		case 118: goto tr569;
		case 120: goto tr570;
	}
	goto tr1;
case 175:
	switch( (*p) ) {
		case 34: goto tr572;
		case 92: goto tr573;
	}
	goto tr571;
case 176:
	switch( (*p) ) {
		case 34: goto tr575;
		case 92: goto tr576;
	}
	goto tr574;
case 177:
	switch( (*p) ) {
		case 34: goto tr578;
		case 92: goto tr579;
	}
	goto tr577;
case 178:
	switch( (*p) ) {
		case 34: goto tr581;
		case 92: goto tr582;
	}
	goto tr580;
case 179:
	switch( (*p) ) {
		case 34: goto tr584;
		case 92: goto tr585;
	}
	goto tr583;
case 180:
	switch( (*p) ) {
		case 34: goto tr587;
		case 92: goto tr588;
	}
	goto tr586;
case 181:
	switch( (*p) ) {
		case 34: goto tr590;
		case 92: goto tr591;
	}
	goto tr589;
case 182:
	switch( (*p) ) {
		case 34: goto tr593;
		case 92: goto tr594;
	}
	goto tr592;
case 183:
	switch( (*p) ) {
		case 34: goto tr596;
		case 92: goto tr597;
	}
	goto tr595;
case 184:
	switch( (*p) ) {
		case 34: goto tr599;
		case 92: goto tr600;
	}
	goto tr598;
case 185:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr601;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr601;
	} else
		goto tr601;
	goto tr1;
case 186:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr602;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr602;
	} else
		goto tr602;
	goto tr1;
case 187:
	switch( (*p) ) {
		case 34: goto tr604;
		case 92: goto tr605;
	}
	goto tr603;
case 188:
	if ( (*p) == 10 )
		goto tr30;
	goto tr32;
case 189:
	switch( (*p) ) {
		case 10: goto tr607;
		case 32: goto tr606;
		case 35: goto tr608;
		case 44: goto tr609;
		case 61: goto tr611;
		case 95: goto tr610;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr606;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr610;
		} else if ( (*p) >= 65 )
			goto tr610;
	} else
		goto tr610;
	goto tr1;
case 190:
	switch( (*p) ) {
		case 34: goto tr612;
		case 39: goto tr613;
		case 92: goto tr614;
		case 97: goto tr615;
		case 98: goto tr616;
		case 102: goto tr617;
		case 110: goto tr618;
		case 114: goto tr619;
		case 116: goto tr620;
		case 118: goto tr621;
		case 120: goto tr622;
	}
	goto tr1;
case 191:
	switch( (*p) ) {
		case 34: goto tr624;
		case 92: goto tr625;
	}
	goto tr623;
case 192:
	switch( (*p) ) {
		case 34: goto tr627;
		case 92: goto tr628;
	}
	goto tr626;
case 193:
	switch( (*p) ) {
		case 34: goto tr630;
		case 92: goto tr631;
	}
	goto tr629;
case 194:
	switch( (*p) ) {
		case 34: goto tr633;
		case 92: goto tr634;
	}
	goto tr632;
case 195:
	switch( (*p) ) {
		case 34: goto tr636;
		case 92: goto tr637;
	}
	goto tr635;
case 196:
	switch( (*p) ) {
		case 34: goto tr639;
		case 92: goto tr640;
	}
	goto tr638;
case 197:
	switch( (*p) ) {
		case 34: goto tr642;
		case 92: goto tr643;
	}
	goto tr641;
case 198:
	switch( (*p) ) {
		case 34: goto tr645;
		case 92: goto tr646;
	}
	goto tr644;
case 199:
	switch( (*p) ) {
		case 34: goto tr648;
		case 92: goto tr649;
	}
	goto tr647;
case 200:
	switch( (*p) ) {
		case 34: goto tr651;
		case 92: goto tr652;
	}
	goto tr650;
case 201:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr653;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr653;
	} else
		goto tr653;
	goto tr1;
case 202:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr654;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr654;
	} else
		goto tr654;
	goto tr1;
case 203:
	switch( (*p) ) {
		case 34: goto tr656;
		case 92: goto tr657;
	}
	goto tr655;
case 204:
	if ( (*p) == 10 )
		goto tr8;
	goto tr10;
	}

	tr1: cs = 0; goto _again;
	tr7: cs = 2; goto _again;
	tr0: cs = 2; goto f0;
	tr2: cs = 2; goto f1;
	tr8: cs = 2; goto f4;
	tr9: cs = 3; goto _again;
	tr3: cs = 3; goto f2;
	tr660: cs = 3; goto f72;
	tr667: cs = 3; goto f76;
	tr13: cs = 4; goto f6;
	tr16: cs = 4; goto f8;
	tr623: cs = 4; goto f37;
	tr626: cs = 4; goto f39;
	tr629: cs = 4; goto f41;
	tr632: cs = 4; goto f43;
	tr635: cs = 4; goto f45;
	tr638: cs = 4; goto f47;
	tr641: cs = 4; goto f49;
	tr644: cs = 4; goto f51;
	tr647: cs = 4; goto f53;
	tr650: cs = 4; goto f55;
	tr655: cs = 4; goto f59;
	tr14: cs = 5; goto f7;
	tr17: cs = 5; goto f9;
	tr624: cs = 5; goto f38;
	tr627: cs = 5; goto f40;
	tr630: cs = 5; goto f42;
	tr633: cs = 5; goto f44;
	tr636: cs = 5; goto f46;
	tr639: cs = 5; goto f48;
	tr642: cs = 5; goto f50;
	tr645: cs = 5; goto f52;
	tr648: cs = 5; goto f54;
	tr651: cs = 5; goto f56;
	tr656: cs = 5; goto f60;
	tr25: cs = 6; goto _again;
	tr26: cs = 6; goto f4;
	tr19: cs = 6; goto f10;
	tr20: cs = 6; goto f11;
	tr130: cs = 6; goto f27;
	tr131: cs = 6; goto f28;
	tr27: cs = 7; goto _again;
	tr21: cs = 7; goto f10;
	tr132: cs = 7; goto f27;
	tr28: cs = 8; goto _again;
	tr30: cs = 8; goto f4;
	tr22: cs = 8; goto f12;
	tr43: cs = 8; goto f17;
	tr133: cs = 8; goto f29;
	tr609: cs = 8; goto f61;
	tr31: cs = 9; goto _again;
	tr34: cs = 10; goto f6;
	tr37: cs = 10; goto f8;
	tr571: cs = 10; goto f37;
	tr574: cs = 10; goto f39;
	tr577: cs = 10; goto f41;
	tr580: cs = 10; goto f43;
	tr583: cs = 10; goto f45;
	tr586: cs = 10; goto f47;
	tr589: cs = 10; goto f49;
	tr592: cs = 10; goto f51;
	tr595: cs = 10; goto f53;
	tr598: cs = 10; goto f55;
	tr603: cs = 10; goto f59;
	tr35: cs = 11; goto f7;
	tr38: cs = 11; goto f9;
	tr572: cs = 11; goto f38;
	tr575: cs = 11; goto f40;
	tr578: cs = 11; goto f42;
	tr581: cs = 11; goto f44;
	tr584: cs = 11; goto f46;
	tr587: cs = 11; goto f48;
	tr590: cs = 11; goto f50;
	tr593: cs = 11; goto f52;
	tr596: cs = 11; goto f54;
	tr599: cs = 11; goto f56;
	tr604: cs = 11; goto f60;
	tr45: cs = 12; goto _again;
	tr46: cs = 12; goto f4;
	tr40: cs = 12; goto f15;
	tr41: cs = 12; goto f16;
	tr606: cs = 12; goto f70;
	tr607: cs = 12; goto f71;
	tr47: cs = 13; goto _again;
	tr42: cs = 13; goto f15;
	tr608: cs = 13; goto f70;
	tr11: cs = 14; goto _again;
	tr5: cs = 14; goto f0;
	tr48: cs = 14; goto f4;
	tr24: cs = 14; goto f14;
	tr44: cs = 14; goto f15;
	tr136: cs = 14; goto f31;
	tr611: cs = 14; goto f70;
	tr662: cs = 14; goto f73;
	tr669: cs = 14; goto f77;
	tr49: cs = 15; goto _again;
	tr52: cs = 16; goto f6;
	tr55: cs = 16; goto f8;
	tr520: cs = 16; goto f37;
	tr523: cs = 16; goto f39;
	tr526: cs = 16; goto f41;
	tr529: cs = 16; goto f43;
	tr532: cs = 16; goto f45;
	tr535: cs = 16; goto f47;
	tr538: cs = 16; goto f49;
	tr541: cs = 16; goto f51;
	tr544: cs = 16; goto f53;
	tr547: cs = 16; goto f55;
	tr552: cs = 16; goto f59;
	tr53: cs = 17; goto f7;
	tr56: cs = 17; goto f9;
	tr521: cs = 17; goto f38;
	tr524: cs = 17; goto f40;
	tr527: cs = 17; goto f42;
	tr530: cs = 17; goto f44;
	tr533: cs = 17; goto f46;
	tr536: cs = 17; goto f48;
	tr539: cs = 17; goto f50;
	tr542: cs = 17; goto f52;
	tr545: cs = 17; goto f54;
	tr548: cs = 17; goto f56;
	tr553: cs = 17; goto f60;
	tr62: cs = 18; goto _again;
	tr63: cs = 18; goto f4;
	tr58: cs = 18; goto f18;
	tr59: cs = 18; goto f19;
	tr555: cs = 18; goto f68;
	tr556: cs = 18; goto f69;
	tr64: cs = 19; goto _again;
	tr60: cs = 19; goto f18;
	tr557: cs = 19; goto f68;
	tr65: cs = 20; goto _again;
	tr66: cs = 20; goto f4;
	tr61: cs = 20; goto f18;
	tr558: cs = 20; goto f68;
	tr67: cs = 21; goto _again;
	tr70: cs = 22; goto f6;
	tr73: cs = 22; goto f8;
	tr469: cs = 22; goto f37;
	tr472: cs = 22; goto f39;
	tr475: cs = 22; goto f41;
	tr478: cs = 22; goto f43;
	tr481: cs = 22; goto f45;
	tr484: cs = 22; goto f47;
	tr487: cs = 22; goto f49;
	tr490: cs = 22; goto f51;
	tr493: cs = 22; goto f53;
	tr496: cs = 22; goto f55;
	tr501: cs = 22; goto f59;
	tr71: cs = 23; goto f7;
	tr74: cs = 23; goto f9;
	tr470: cs = 23; goto f38;
	tr473: cs = 23; goto f40;
	tr476: cs = 23; goto f42;
	tr479: cs = 23; goto f44;
	tr482: cs = 23; goto f46;
	tr485: cs = 23; goto f48;
	tr488: cs = 23; goto f50;
	tr491: cs = 23; goto f52;
	tr494: cs = 23; goto f54;
	tr497: cs = 23; goto f56;
	tr502: cs = 23; goto f60;
	tr80: cs = 24; goto _again;
	tr81: cs = 24; goto f4;
	tr76: cs = 24; goto f20;
	tr77: cs = 24; goto f21;
	tr504: cs = 24; goto f66;
	tr505: cs = 24; goto f67;
	tr82: cs = 25; goto _again;
	tr78: cs = 25; goto f20;
	tr506: cs = 25; goto f66;
	tr83: cs = 26; goto _again;
	tr79: cs = 26; goto f20;
	tr507: cs = 26; goto f66;
	tr84: cs = 27; goto _again;
	tr85: cs = 27; goto f4;
	tr86: cs = 28; goto _again;
	tr90: cs = 29; goto f6;
	tr93: cs = 29; goto f8;
	tr417: cs = 29; goto f37;
	tr420: cs = 29; goto f39;
	tr423: cs = 29; goto f41;
	tr426: cs = 29; goto f43;
	tr429: cs = 29; goto f45;
	tr432: cs = 29; goto f47;
	tr435: cs = 29; goto f49;
	tr438: cs = 29; goto f51;
	tr441: cs = 29; goto f53;
	tr444: cs = 29; goto f55;
	tr449: cs = 29; goto f59;
	tr91: cs = 30; goto f7;
	tr94: cs = 30; goto f9;
	tr418: cs = 30; goto f38;
	tr421: cs = 30; goto f40;
	tr424: cs = 30; goto f42;
	tr427: cs = 30; goto f44;
	tr430: cs = 30; goto f46;
	tr433: cs = 30; goto f48;
	tr436: cs = 30; goto f50;
	tr439: cs = 30; goto f52;
	tr442: cs = 30; goto f54;
	tr445: cs = 30; goto f56;
	tr450: cs = 30; goto f60;
	tr101: cs = 31; goto _again;
	tr102: cs = 31; goto f4;
	tr96: cs = 31; goto f12;
	tr115: cs = 31; goto f17;
	tr97: cs = 31; goto f23;
	tr116: cs = 31; goto f24;
	tr452: cs = 31; goto f29;
	tr400: cs = 31; goto f61;
	tr401: cs = 31; goto f62;
	tr453: cs = 31; goto f63;
	tr103: cs = 32; goto _again;
	tr98: cs = 32; goto f12;
	tr117: cs = 32; goto f17;
	tr454: cs = 32; goto f29;
	tr402: cs = 32; goto f61;
	tr104: cs = 33; goto _again;
	tr105: cs = 33; goto f4;
	tr100: cs = 33; goto f12;
	tr119: cs = 33; goto f17;
	tr456: cs = 33; goto f29;
	tr404: cs = 33; goto f61;
	tr106: cs = 34; goto _again;
	tr109: cs = 35; goto f6;
	tr112: cs = 35; goto f8;
	tr365: cs = 35; goto f37;
	tr368: cs = 35; goto f39;
	tr371: cs = 35; goto f41;
	tr374: cs = 35; goto f43;
	tr377: cs = 35; goto f45;
	tr380: cs = 35; goto f47;
	tr383: cs = 35; goto f49;
	tr386: cs = 35; goto f51;
	tr389: cs = 35; goto f53;
	tr392: cs = 35; goto f55;
	tr397: cs = 35; goto f59;
	tr110: cs = 36; goto f7;
	tr113: cs = 36; goto f9;
	tr366: cs = 36; goto f38;
	tr369: cs = 36; goto f40;
	tr372: cs = 36; goto f42;
	tr375: cs = 36; goto f44;
	tr378: cs = 36; goto f46;
	tr381: cs = 36; goto f48;
	tr384: cs = 36; goto f50;
	tr387: cs = 36; goto f52;
	tr390: cs = 36; goto f54;
	tr393: cs = 36; goto f56;
	tr398: cs = 36; goto f60;
	tr99: cs = 37; goto f12;
	tr118: cs = 37; goto f17;
	tr88: cs = 37; goto f22;
	tr455: cs = 37; goto f29;
	tr403: cs = 37; goto f61;
	tr124: cs = 38; goto _again;
	tr125: cs = 38; goto f4;
	tr120: cs = 38; goto f25;
	tr121: cs = 38; goto f26;
	tr126: cs = 39; goto _again;
	tr122: cs = 39; goto f25;
	tr128: cs = 40; goto _again;
	tr661: cs = 40; goto f73;
	tr668: cs = 40; goto f77;
	tr135: cs = 41; goto _again;
	tr6: cs = 41; goto f3;
	tr12: cs = 41; goto f5;
	tr663: cs = 41; goto f75;
	tr670: cs = 41; goto f79;
	tr29: cs = 42; goto _again;
	tr137: cs = 42; goto f4;
	tr23: cs = 42; goto f13;
	tr134: cs = 42; goto f30;
	tr138: cs = 43; goto _again;
	tr141: cs = 44; goto f6;
	tr144: cs = 44; goto f8;
	tr314: cs = 44; goto f37;
	tr317: cs = 44; goto f39;
	tr320: cs = 44; goto f41;
	tr323: cs = 44; goto f43;
	tr326: cs = 44; goto f45;
	tr329: cs = 44; goto f47;
	tr332: cs = 44; goto f49;
	tr335: cs = 44; goto f51;
	tr338: cs = 44; goto f53;
	tr341: cs = 44; goto f55;
	tr346: cs = 44; goto f59;
	tr142: cs = 45; goto f7;
	tr145: cs = 45; goto f9;
	tr315: cs = 45; goto f38;
	tr318: cs = 45; goto f40;
	tr321: cs = 45; goto f42;
	tr324: cs = 45; goto f44;
	tr327: cs = 45; goto f46;
	tr330: cs = 45; goto f48;
	tr333: cs = 45; goto f50;
	tr336: cs = 45; goto f52;
	tr339: cs = 45; goto f54;
	tr342: cs = 45; goto f56;
	tr347: cs = 45; goto f60;
	tr151: cs = 46; goto _again;
	tr152: cs = 46; goto f4;
	tr147: cs = 46; goto f32;
	tr148: cs = 46; goto f33;
	tr349: cs = 46; goto f64;
	tr350: cs = 46; goto f65;
	tr153: cs = 47; goto _again;
	tr149: cs = 47; goto f32;
	tr351: cs = 47; goto f64;
	tr154: cs = 48; goto _again;
	tr150: cs = 48; goto f32;
	tr352: cs = 48; goto f64;
	tr155: cs = 49; goto _again;
	tr156: cs = 49; goto f4;
	tr157: cs = 50; goto _again;
	tr161: cs = 51; goto f6;
	tr164: cs = 51; goto f8;
	tr262: cs = 51; goto f37;
	tr265: cs = 51; goto f39;
	tr268: cs = 51; goto f41;
	tr271: cs = 51; goto f43;
	tr274: cs = 51; goto f45;
	tr277: cs = 51; goto f47;
	tr280: cs = 51; goto f49;
	tr283: cs = 51; goto f51;
	tr286: cs = 51; goto f53;
	tr289: cs = 51; goto f55;
	tr294: cs = 51; goto f59;
	tr162: cs = 52; goto f7;
	tr165: cs = 52; goto f9;
	tr263: cs = 52; goto f38;
	tr266: cs = 52; goto f40;
	tr269: cs = 52; goto f42;
	tr272: cs = 52; goto f44;
	tr275: cs = 52; goto f46;
	tr278: cs = 52; goto f48;
	tr281: cs = 52; goto f50;
	tr284: cs = 52; goto f52;
	tr287: cs = 52; goto f54;
	tr290: cs = 52; goto f56;
	tr295: cs = 52; goto f60;
	tr172: cs = 53; goto _again;
	tr173: cs = 53; goto f4;
	tr167: cs = 53; goto f12;
	tr186: cs = 53; goto f17;
	tr168: cs = 53; goto f23;
	tr187: cs = 53; goto f24;
	tr297: cs = 53; goto f29;
	tr245: cs = 53; goto f61;
	tr246: cs = 53; goto f62;
	tr298: cs = 53; goto f63;
	tr174: cs = 54; goto _again;
	tr169: cs = 54; goto f12;
	tr188: cs = 54; goto f17;
	tr299: cs = 54; goto f29;
	tr247: cs = 54; goto f61;
	tr175: cs = 55; goto _again;
	tr176: cs = 55; goto f4;
	tr171: cs = 55; goto f12;
	tr190: cs = 55; goto f17;
	tr301: cs = 55; goto f29;
	tr249: cs = 55; goto f61;
	tr177: cs = 56; goto _again;
	tr180: cs = 57; goto f6;
	tr183: cs = 57; goto f8;
	tr210: cs = 57; goto f37;
	tr213: cs = 57; goto f39;
	tr216: cs = 57; goto f41;
	tr219: cs = 57; goto f43;
	tr222: cs = 57; goto f45;
	tr225: cs = 57; goto f47;
	tr228: cs = 57; goto f49;
	tr231: cs = 57; goto f51;
	tr234: cs = 57; goto f53;
	tr237: cs = 57; goto f55;
	tr242: cs = 57; goto f59;
	tr181: cs = 58; goto f7;
	tr184: cs = 58; goto f9;
	tr211: cs = 58; goto f38;
	tr214: cs = 58; goto f40;
	tr217: cs = 58; goto f42;
	tr220: cs = 58; goto f44;
	tr223: cs = 58; goto f46;
	tr226: cs = 58; goto f48;
	tr229: cs = 58; goto f50;
	tr232: cs = 58; goto f52;
	tr235: cs = 58; goto f54;
	tr238: cs = 58; goto f56;
	tr243: cs = 58; goto f60;
	tr170: cs = 59; goto f12;
	tr189: cs = 59; goto f17;
	tr159: cs = 59; goto f22;
	tr300: cs = 59; goto f29;
	tr248: cs = 59; goto f61;
	tr195: cs = 60; goto _again;
	tr196: cs = 60; goto f4;
	tr191: cs = 60; goto f34;
	tr192: cs = 60; goto f35;
	tr197: cs = 61; goto _again;
	tr193: cs = 61; goto f34;
	tr182: cs = 62; goto f7;
	tr185: cs = 62; goto f9;
	tr212: cs = 62; goto f38;
	tr215: cs = 62; goto f40;
	tr218: cs = 62; goto f42;
	tr221: cs = 62; goto f44;
	tr224: cs = 62; goto f46;
	tr227: cs = 62; goto f48;
	tr230: cs = 62; goto f50;
	tr233: cs = 62; goto f52;
	tr236: cs = 62; goto f54;
	tr239: cs = 62; goto f56;
	tr244: cs = 62; goto f60;
	tr199: cs = 63; goto _again;
	tr200: cs = 64; goto _again;
	tr201: cs = 65; goto _again;
	tr202: cs = 66; goto _again;
	tr203: cs = 67; goto _again;
	tr204: cs = 68; goto _again;
	tr205: cs = 69; goto _again;
	tr206: cs = 70; goto _again;
	tr207: cs = 71; goto _again;
	tr208: cs = 72; goto _again;
	tr209: cs = 73; goto f36;
	tr240: cs = 74; goto f57;
	tr241: cs = 75; goto f58;
	tr178: cs = 76; goto _again;
	tr250: cs = 77; goto _again;
	tr179: cs = 77; goto f5;
	tr163: cs = 78; goto f7;
	tr166: cs = 78; goto f9;
	tr264: cs = 78; goto f38;
	tr267: cs = 78; goto f40;
	tr270: cs = 78; goto f42;
	tr273: cs = 78; goto f44;
	tr276: cs = 78; goto f46;
	tr279: cs = 78; goto f48;
	tr282: cs = 78; goto f50;
	tr285: cs = 78; goto f52;
	tr288: cs = 78; goto f54;
	tr291: cs = 78; goto f56;
	tr296: cs = 78; goto f60;
	tr251: cs = 79; goto _again;
	tr252: cs = 80; goto _again;
	tr253: cs = 81; goto _again;
	tr254: cs = 82; goto _again;
	tr255: cs = 83; goto _again;
	tr256: cs = 84; goto _again;
	tr257: cs = 85; goto _again;
	tr258: cs = 86; goto _again;
	tr259: cs = 87; goto _again;
	tr260: cs = 88; goto _again;
	tr261: cs = 89; goto f36;
	tr292: cs = 90; goto f57;
	tr293: cs = 91; goto f58;
	tr158: cs = 92; goto _again;
	tr302: cs = 93; goto _again;
	tr160: cs = 93; goto f5;
	tr143: cs = 94; goto f7;
	tr146: cs = 94; goto f9;
	tr316: cs = 94; goto f38;
	tr319: cs = 94; goto f40;
	tr322: cs = 94; goto f42;
	tr325: cs = 94; goto f44;
	tr328: cs = 94; goto f46;
	tr331: cs = 94; goto f48;
	tr334: cs = 94; goto f50;
	tr337: cs = 94; goto f52;
	tr340: cs = 94; goto f54;
	tr343: cs = 94; goto f56;
	tr348: cs = 94; goto f60;
	tr303: cs = 95; goto _again;
	tr304: cs = 96; goto _again;
	tr305: cs = 97; goto _again;
	tr306: cs = 98; goto _again;
	tr307: cs = 99; goto _again;
	tr308: cs = 100; goto _again;
	tr309: cs = 101; goto _again;
	tr310: cs = 102; goto _again;
	tr311: cs = 103; goto _again;
	tr312: cs = 104; goto _again;
	tr313: cs = 105; goto f36;
	tr344: cs = 106; goto f57;
	tr345: cs = 107; goto f58;
	tr139: cs = 108; goto _again;
	tr353: cs = 109; goto _again;
	tr140: cs = 109; goto f5;
	tr111: cs = 110; goto f7;
	tr114: cs = 110; goto f9;
	tr367: cs = 110; goto f38;
	tr370: cs = 110; goto f40;
	tr373: cs = 110; goto f42;
	tr376: cs = 110; goto f44;
	tr379: cs = 110; goto f46;
	tr382: cs = 110; goto f48;
	tr385: cs = 110; goto f50;
	tr388: cs = 110; goto f52;
	tr391: cs = 110; goto f54;
	tr394: cs = 110; goto f56;
	tr399: cs = 110; goto f60;
	tr354: cs = 111; goto _again;
	tr355: cs = 112; goto _again;
	tr356: cs = 113; goto _again;
	tr357: cs = 114; goto _again;
	tr358: cs = 115; goto _again;
	tr359: cs = 116; goto _again;
	tr360: cs = 117; goto _again;
	tr361: cs = 118; goto _again;
	tr362: cs = 119; goto _again;
	tr363: cs = 120; goto _again;
	tr364: cs = 121; goto f36;
	tr395: cs = 122; goto f57;
	tr396: cs = 123; goto f58;
	tr107: cs = 124; goto _again;
	tr405: cs = 125; goto _again;
	tr108: cs = 125; goto f5;
	tr92: cs = 126; goto f7;
	tr95: cs = 126; goto f9;
	tr419: cs = 126; goto f38;
	tr422: cs = 126; goto f40;
	tr425: cs = 126; goto f42;
	tr428: cs = 126; goto f44;
	tr431: cs = 126; goto f46;
	tr434: cs = 126; goto f48;
	tr437: cs = 126; goto f50;
	tr440: cs = 126; goto f52;
	tr443: cs = 126; goto f54;
	tr446: cs = 126; goto f56;
	tr451: cs = 126; goto f60;
	tr406: cs = 127; goto _again;
	tr407: cs = 128; goto _again;
	tr408: cs = 129; goto _again;
	tr409: cs = 130; goto _again;
	tr410: cs = 131; goto _again;
	tr411: cs = 132; goto _again;
	tr412: cs = 133; goto _again;
	tr413: cs = 134; goto _again;
	tr414: cs = 135; goto _again;
	tr415: cs = 136; goto _again;
	tr416: cs = 137; goto f36;
	tr447: cs = 138; goto f57;
	tr448: cs = 139; goto f58;
	tr87: cs = 140; goto _again;
	tr457: cs = 141; goto _again;
	tr89: cs = 141; goto f5;
	tr72: cs = 142; goto f7;
	tr75: cs = 142; goto f9;
	tr471: cs = 142; goto f38;
	tr474: cs = 142; goto f40;
	tr477: cs = 142; goto f42;
	tr480: cs = 142; goto f44;
	tr483: cs = 142; goto f46;
	tr486: cs = 142; goto f48;
	tr489: cs = 142; goto f50;
	tr492: cs = 142; goto f52;
	tr495: cs = 142; goto f54;
	tr498: cs = 142; goto f56;
	tr503: cs = 142; goto f60;
	tr458: cs = 143; goto _again;
	tr459: cs = 144; goto _again;
	tr460: cs = 145; goto _again;
	tr461: cs = 146; goto _again;
	tr462: cs = 147; goto _again;
	tr463: cs = 148; goto _again;
	tr464: cs = 149; goto _again;
	tr465: cs = 150; goto _again;
	tr466: cs = 151; goto _again;
	tr467: cs = 152; goto _again;
	tr468: cs = 153; goto f36;
	tr499: cs = 154; goto f57;
	tr500: cs = 155; goto f58;
	tr68: cs = 156; goto _again;
	tr508: cs = 157; goto _again;
	tr69: cs = 157; goto f5;
	tr54: cs = 158; goto f7;
	tr57: cs = 158; goto f9;
	tr522: cs = 158; goto f38;
	tr525: cs = 158; goto f40;
	tr528: cs = 158; goto f42;
	tr531: cs = 158; goto f44;
	tr534: cs = 158; goto f46;
	tr537: cs = 158; goto f48;
	tr540: cs = 158; goto f50;
	tr543: cs = 158; goto f52;
	tr546: cs = 158; goto f54;
	tr549: cs = 158; goto f56;
	tr554: cs = 158; goto f60;
	tr509: cs = 159; goto _again;
	tr510: cs = 160; goto _again;
	tr511: cs = 161; goto _again;
	tr512: cs = 162; goto _again;
	tr513: cs = 163; goto _again;
	tr514: cs = 164; goto _again;
	tr515: cs = 165; goto _again;
	tr516: cs = 166; goto _again;
	tr517: cs = 167; goto _again;
	tr518: cs = 168; goto _again;
	tr519: cs = 169; goto f36;
	tr550: cs = 170; goto f57;
	tr551: cs = 171; goto f58;
	tr50: cs = 172; goto _again;
	tr559: cs = 173; goto _again;
	tr51: cs = 173; goto f5;
	tr36: cs = 174; goto f7;
	tr39: cs = 174; goto f9;
	tr573: cs = 174; goto f38;
	tr576: cs = 174; goto f40;
	tr579: cs = 174; goto f42;
	tr582: cs = 174; goto f44;
	tr585: cs = 174; goto f46;
	tr588: cs = 174; goto f48;
	tr591: cs = 174; goto f50;
	tr594: cs = 174; goto f52;
	tr597: cs = 174; goto f54;
	tr600: cs = 174; goto f56;
	tr605: cs = 174; goto f60;
	tr560: cs = 175; goto _again;
	tr561: cs = 176; goto _again;
	tr562: cs = 177; goto _again;
	tr563: cs = 178; goto _again;
	tr564: cs = 179; goto _again;
	tr565: cs = 180; goto _again;
	tr566: cs = 181; goto _again;
	tr567: cs = 182; goto _again;
	tr568: cs = 183; goto _again;
	tr569: cs = 184; goto _again;
	tr570: cs = 185; goto f36;
	tr601: cs = 186; goto f57;
	tr602: cs = 187; goto f58;
	tr32: cs = 188; goto _again;
	tr610: cs = 189; goto _again;
	tr33: cs = 189; goto f5;
	tr15: cs = 190; goto f7;
	tr18: cs = 190; goto f9;
	tr625: cs = 190; goto f38;
	tr628: cs = 190; goto f40;
	tr631: cs = 190; goto f42;
	tr634: cs = 190; goto f44;
	tr637: cs = 190; goto f46;
	tr640: cs = 190; goto f48;
	tr643: cs = 190; goto f50;
	tr646: cs = 190; goto f52;
	tr649: cs = 190; goto f54;
	tr652: cs = 190; goto f56;
	tr657: cs = 190; goto f60;
	tr612: cs = 191; goto _again;
	tr613: cs = 192; goto _again;
	tr614: cs = 193; goto _again;
	tr615: cs = 194; goto _again;
	tr616: cs = 195; goto _again;
	tr617: cs = 196; goto _again;
	tr618: cs = 197; goto _again;
	tr619: cs = 198; goto _again;
	tr620: cs = 199; goto _again;
	tr621: cs = 200; goto _again;
	tr622: cs = 201; goto f36;
	tr653: cs = 202; goto f57;
	tr654: cs = 203; goto f58;
	tr10: cs = 204; goto _again;
	tr4: cs = 204; goto f0;
	tr127: cs = 205; goto _again;
	tr123: cs = 205; goto f25;
	tr664: cs = 206; goto _again;
	tr129: cs = 206; goto f4;
	tr658: cs = 206; goto f73;
	tr659: cs = 206; goto f74;
	tr665: cs = 206; goto f77;
	tr666: cs = 206; goto f78;
	tr198: cs = 207; goto _again;
	tr194: cs = 207; goto f34;

f5:
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f4:
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f42:
#line 52 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\\", 1); }
	goto _again;
f38:
#line 53 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\"", 1); }
	goto _again;
f40:
#line 54 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "'",  1); }
	goto _again;
f44:
#line 55 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\a",  1); }
	goto _again;
f46:
#line 56 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\b",  1); }
	goto _again;
f48:
#line 57 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\f",  1); }
	goto _again;
f50:
#line 58 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\n",  1); }
	goto _again;
f52:
#line 59 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\r",  1); }
	goto _again;
f54:
#line 60 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\t",  1); }
	goto _again;
f56:
#line 61 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\v",  1); }
	goto _again;
f58:
#line 63 "libswanson/s0-parser.c.rl"
	{ hex_char |= digit_values[(int) (*p)]; }
	goto _again;
f36:
#line 64 "libswanson/s0-parser.c.rl"
	{ hex_char = 0; }
	goto _again;
f60:
#line 66 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, &hex_char, 1); }
	goto _again;
f9:
#line 73 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, xs, (p-xs)); }
	goto _again;
f7:
#line 75 "libswanson/s0-parser.c.rl"
	{ cork_buffer_clear(&scratch); }
	goto _again;
f22:
#line 82 "libswanson/s0-parser.c.rl"
	{ id_list = NULL; }
	goto _again;
f25:
#line 98 "libswanson/s0-parser.c.rl"
	{ param_list = id_list; }
	goto _again;
f34:
#line 105 "libswanson/s0-parser.c.rl"
	{ param_list = id_list; }
	goto _again;
f2:
#line 124 "libswanson/s0-parser.c.rl"
	{ ast_tail = NULL; }
	goto _again;
f41:
#line 52 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\\", 1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f37:
#line 53 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\"", 1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f39:
#line 54 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "'",  1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f43:
#line 55 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\a",  1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f45:
#line 56 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\b",  1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f47:
#line 57 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\f",  1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f49:
#line 58 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\n",  1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f51:
#line 59 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\r",  1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f53:
#line 60 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\t",  1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f55:
#line 61 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, "\v",  1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f57:
#line 63 "libswanson/s0-parser.c.rl"
	{ hex_char |= digit_values[(int) (*p)]; }
#line 65 "libswanson/s0-parser.c.rl"
	{ hex_char <<= 4; }
	goto _again;
f59:
#line 66 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, &hex_char, 1); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f8:
#line 73 "libswanson/s0-parser.c.rl"
	{ cork_buffer_append(&scratch, xs, (p-xs)); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f6:
#line 75 "libswanson/s0-parser.c.rl"
	{ cork_buffer_clear(&scratch); }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f12:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 84 "libswanson/s0-parser.c.rl"
	{
              id_list = swan_sllist_new(s, &id->parent, NULL);
              id_tail = id_list;
          }
	goto _again;
f17:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 89 "libswanson/s0-parser.c.rl"
	{
              list_build = swan_sllist_new(s, &id->parent, NULL);
              id_tail->tail = list_build;
              id_tail = list_build;
          }
	goto _again;
f18:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 96 "libswanson/s0-parser.c.rl"
	{ thing = id; }
	goto _again;
f20:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 97 "libswanson/s0-parser.c.rl"
	{ method = id; }
	goto _again;
f13:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 103 "libswanson/s0-parser.c.rl"
	{ result_list = NULL; thing = id; }
	goto _again;
f32:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 104 "libswanson/s0-parser.c.rl"
	{ method = id; }
	goto _again;
f26:
#line 98 "libswanson/s0-parser.c.rl"
	{ param_list = id_list; }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f72:
#line 100 "libswanson/s0-parser.c.rl"
	{ call = swan_ast_call_new
                      (s, result_list, thing, method, param_list); }
#line 113 "libswanson/s0-parser.c.rl"
	{
                list_build = swan_sllist_new(s, &call->parent, ast_list);
                if (ast_tail == NULL) {
                    ast_list = list_build;
                } else {
                    ast_tail->tail = list_build;
                }
                ast_tail = list_build;
            }
	goto _again;
f35:
#line 105 "libswanson/s0-parser.c.rl"
	{ param_list = id_list; }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f76:
#line 107 "libswanson/s0-parser.c.rl"
	{ call = swan_ast_call_new
                   (s, result_list, thing, method, param_list); }
#line 113 "libswanson/s0-parser.c.rl"
	{
                list_build = swan_sllist_new(s, &call->parent, ast_list);
                if (ast_tail == NULL) {
                    ast_list = list_build;
                } else {
                    ast_tail->tail = list_build;
                }
                ast_tail = list_build;
            }
	goto _again;
f3:
#line 124 "libswanson/s0-parser.c.rl"
	{ ast_tail = NULL; }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f29:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 84 "libswanson/s0-parser.c.rl"
	{
              id_list = swan_sllist_new(s, &id->parent, NULL);
              id_tail = id_list;
          }
	goto _again;
f61:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 89 "libswanson/s0-parser.c.rl"
	{
              list_build = swan_sllist_new(s, &id->parent, NULL);
              id_tail->tail = list_build;
              id_tail = list_build;
          }
	goto _again;
f68:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 96 "libswanson/s0-parser.c.rl"
	{ thing = id; }
	goto _again;
f66:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 97 "libswanson/s0-parser.c.rl"
	{ method = id; }
	goto _again;
f30:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 103 "libswanson/s0-parser.c.rl"
	{ result_list = NULL; thing = id; }
	goto _again;
f64:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 104 "libswanson/s0-parser.c.rl"
	{ method = id; }
	goto _again;
f23:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 84 "libswanson/s0-parser.c.rl"
	{
              id_list = swan_sllist_new(s, &id->parent, NULL);
              id_tail = id_list;
          }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f14:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 84 "libswanson/s0-parser.c.rl"
	{
              id_list = swan_sllist_new(s, &id->parent, NULL);
              id_tail = id_list;
          }
#line 95 "libswanson/s0-parser.c.rl"
	{ result_list = id_list; }
	goto _again;
f24:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 89 "libswanson/s0-parser.c.rl"
	{
              list_build = swan_sllist_new(s, &id->parent, NULL);
              id_tail->tail = list_build;
              id_tail = list_build;
          }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f15:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 89 "libswanson/s0-parser.c.rl"
	{
              list_build = swan_sllist_new(s, &id->parent, NULL);
              id_tail->tail = list_build;
              id_tail = list_build;
          }
#line 95 "libswanson/s0-parser.c.rl"
	{ result_list = id_list; }
	goto _again;
f19:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 96 "libswanson/s0-parser.c.rl"
	{ thing = id; }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f21:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 97 "libswanson/s0-parser.c.rl"
	{ method = id; }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f33:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 104 "libswanson/s0-parser.c.rl"
	{ method = id; }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f75:
#line 100 "libswanson/s0-parser.c.rl"
	{ call = swan_ast_call_new
                      (s, result_list, thing, method, param_list); }
#line 113 "libswanson/s0-parser.c.rl"
	{
                list_build = swan_sllist_new(s, &call->parent, ast_list);
                if (ast_tail == NULL) {
                    ast_list = list_build;
                } else {
                    ast_tail->tail = list_build;
                }
                ast_tail = list_build;
            }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f79:
#line 107 "libswanson/s0-parser.c.rl"
	{ call = swan_ast_call_new
                   (s, result_list, thing, method, param_list); }
#line 113 "libswanson/s0-parser.c.rl"
	{
                list_build = swan_sllist_new(s, &call->parent, ast_list);
                if (ast_tail == NULL) {
                    ast_list = list_build;
                } else {
                    ast_tail->tail = list_build;
                }
                ast_tail = list_build;
            }
#line 41 "libswanson/s0-parser.c.rl"
	{ xs = p; }
	goto _again;
f0:
#line 124 "libswanson/s0-parser.c.rl"
	{ ast_tail = NULL; }
#line 82 "libswanson/s0-parser.c.rl"
	{ id_list = NULL; }
#line 95 "libswanson/s0-parser.c.rl"
	{ result_list = id_list; }
	goto _again;
f63:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 84 "libswanson/s0-parser.c.rl"
	{
              id_list = swan_sllist_new(s, &id->parent, NULL);
              id_tail = id_list;
          }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f31:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 84 "libswanson/s0-parser.c.rl"
	{
              id_list = swan_sllist_new(s, &id->parent, NULL);
              id_tail = id_list;
          }
#line 95 "libswanson/s0-parser.c.rl"
	{ result_list = id_list; }
	goto _again;
f62:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 89 "libswanson/s0-parser.c.rl"
	{
              list_build = swan_sllist_new(s, &id->parent, NULL);
              id_tail->tail = list_build;
              id_tail = list_build;
          }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f70:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 89 "libswanson/s0-parser.c.rl"
	{
              list_build = swan_sllist_new(s, &id->parent, NULL);
              id_tail->tail = list_build;
              id_tail = list_build;
          }
#line 95 "libswanson/s0-parser.c.rl"
	{ result_list = id_list; }
	goto _again;
f69:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 96 "libswanson/s0-parser.c.rl"
	{ thing = id; }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f67:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 97 "libswanson/s0-parser.c.rl"
	{ method = id; }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f65:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 104 "libswanson/s0-parser.c.rl"
	{ method = id; }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
	goto _again;
f16:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 89 "libswanson/s0-parser.c.rl"
	{
              list_build = swan_sllist_new(s, &id->parent, NULL);
              id_tail->tail = list_build;
              id_tail = list_build;
          }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
#line 95 "libswanson/s0-parser.c.rl"
	{ result_list = id_list; }
	goto _again;
f10:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 103 "libswanson/s0-parser.c.rl"
	{ result_list = NULL; thing = id; }
#line 84 "libswanson/s0-parser.c.rl"
	{
              id_list = swan_sllist_new(s, &id->parent, NULL);
              id_tail = id_list;
          }
#line 95 "libswanson/s0-parser.c.rl"
	{ result_list = id_list; }
	goto _again;
f73:
#line 100 "libswanson/s0-parser.c.rl"
	{ call = swan_ast_call_new
                      (s, result_list, thing, method, param_list); }
#line 113 "libswanson/s0-parser.c.rl"
	{
                list_build = swan_sllist_new(s, &call->parent, ast_list);
                if (ast_tail == NULL) {
                    ast_list = list_build;
                } else {
                    ast_tail->tail = list_build;
                }
                ast_tail = list_build;
            }
#line 82 "libswanson/s0-parser.c.rl"
	{ id_list = NULL; }
#line 95 "libswanson/s0-parser.c.rl"
	{ result_list = id_list; }
	goto _again;
f77:
#line 107 "libswanson/s0-parser.c.rl"
	{ call = swan_ast_call_new
                   (s, result_list, thing, method, param_list); }
#line 113 "libswanson/s0-parser.c.rl"
	{
                list_build = swan_sllist_new(s, &call->parent, ast_list);
                if (ast_tail == NULL) {
                    ast_list = list_build;
                } else {
                    ast_tail->tail = list_build;
                }
                ast_tail = list_build;
            }
#line 82 "libswanson/s0-parser.c.rl"
	{ id_list = NULL; }
#line 95 "libswanson/s0-parser.c.rl"
	{ result_list = id_list; }
	goto _again;
f1:
#line 124 "libswanson/s0-parser.c.rl"
	{ ast_tail = NULL; }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
#line 82 "libswanson/s0-parser.c.rl"
	{ id_list = NULL; }
#line 95 "libswanson/s0-parser.c.rl"
	{ result_list = id_list; }
	goto _again;
f71:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 89 "libswanson/s0-parser.c.rl"
	{
              list_build = swan_sllist_new(s, &id->parent, NULL);
              id_tail->tail = list_build;
              id_tail = list_build;
          }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
#line 95 "libswanson/s0-parser.c.rl"
	{ result_list = id_list; }
	goto _again;
f27:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 103 "libswanson/s0-parser.c.rl"
	{ result_list = NULL; thing = id; }
#line 84 "libswanson/s0-parser.c.rl"
	{
              id_list = swan_sllist_new(s, &id->parent, NULL);
              id_tail = id_list;
          }
#line 95 "libswanson/s0-parser.c.rl"
	{ result_list = id_list; }
	goto _again;
f11:
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 103 "libswanson/s0-parser.c.rl"
	{ result_list = NULL; thing = id; }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
#line 84 "libswanson/s0-parser.c.rl"
	{
              id_list = swan_sllist_new(s, &id->parent, NULL);
              id_tail = id_list;
          }
#line 95 "libswanson/s0-parser.c.rl"
	{ result_list = id_list; }
	goto _again;
f74:
#line 100 "libswanson/s0-parser.c.rl"
	{ call = swan_ast_call_new
                      (s, result_list, thing, method, param_list); }
#line 113 "libswanson/s0-parser.c.rl"
	{
                list_build = swan_sllist_new(s, &call->parent, ast_list);
                if (ast_tail == NULL) {
                    ast_list = list_build;
                } else {
                    ast_tail->tail = list_build;
                }
                ast_tail = list_build;
            }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
#line 82 "libswanson/s0-parser.c.rl"
	{ id_list = NULL; }
#line 95 "libswanson/s0-parser.c.rl"
	{ result_list = id_list; }
	goto _again;
f78:
#line 107 "libswanson/s0-parser.c.rl"
	{ call = swan_ast_call_new
                   (s, result_list, thing, method, param_list); }
#line 113 "libswanson/s0-parser.c.rl"
	{
                list_build = swan_sllist_new(s, &call->parent, ast_list);
                if (ast_tail == NULL) {
                    ast_list = list_build;
                } else {
                    ast_tail->tail = list_build;
                }
                ast_tail = list_build;
            }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
#line 82 "libswanson/s0-parser.c.rl"
	{ id_list = NULL; }
#line 95 "libswanson/s0-parser.c.rl"
	{ result_list = id_list; }
	goto _again;
f28:
#line 50 "libswanson/s0-parser.c.rl"
	{ cork_buffer_set(&scratch, xs, (p-xs)); }
#line 80 "libswanson/s0-parser.c.rl"
	{ id = swan_static_string_new(s, scratch.buf); }
#line 103 "libswanson/s0-parser.c.rl"
	{ result_list = NULL; thing = id; }
#line 44 "libswanson/s0-parser.c.rl"
	{curline += 1;}
#line 84 "libswanson/s0-parser.c.rl"
	{
              id_list = swan_sllist_new(s, &id->parent, NULL);
              id_tail = id_list;
          }
#line 95 "libswanson/s0-parser.c.rl"
	{ result_list = id_list; }
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
	case 73:
#line 100 "libswanson/s0-parser.c.rl"
	{ call = swan_ast_call_new
                      (s, result_list, thing, method, param_list); }
#line 113 "libswanson/s0-parser.c.rl"
	{
                list_build = swan_sllist_new(s, &call->parent, ast_list);
                if (ast_tail == NULL) {
                    ast_list = list_build;
                } else {
                    ast_tail->tail = list_build;
                }
                ast_tail = list_build;
            }
	break;
	case 77:
#line 107 "libswanson/s0-parser.c.rl"
	{ call = swan_ast_call_new
                   (s, result_list, thing, method, param_list); }
#line 113 "libswanson/s0-parser.c.rl"
	{
                list_build = swan_sllist_new(s, &call->parent, ast_list);
                if (ast_tail == NULL) {
                    ast_list = list_build;
                } else {
                    ast_tail->tail = list_build;
                }
                ast_tail = list_build;
            }
	break;
#line 3284 "/Users/dcreager/git/swanson/src/libswanson/s0-parser.c"
	}
	}

	_out: {}
	}

#line 159 "libswanson/s0-parser.c.rl"

    if (cs < 
#line 3294 "/Users/dcreager/git/swanson/src/libswanson/s0-parser.c"
205
#line 160 "libswanson/s0-parser.c.rl"
) {
        printf("error at line %zu\n", curline);
        cork_error_set
            (SWAN_METAMODEL_ERROR, SWAN_METAMODEL_PARSE_ERROR,
             "Parse error");
        return 0;
    }

    cork_buffer_done(&scratch);
    return ast_list;
}
