/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 5;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 5;       /* vert inner gap between windows */
static const unsigned int gappoh    = 8;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int vertpad            = 5;       /* vertical padding of bar */
static const int sidepad            = 8;       /* horizontal padding of bar */
static const char *fonts[]          = { "DejaVu Mono:size=12", "Font awesome:size=12" };
static const char dmenufont[]       = "DejaVu Sans Mono:size=12";
//background color
static const char col_gray1[]       = "#1a1b26";
//inactive windowborder
static const char col_gray2[]       = "#1a1b26";
//font color
static const char col_gray3[]       = "#d2d5d3";
//current tag and current window font color
static const char col_gray4[]       = "#eaeaea";
//top bar second color and active window border color
static const char col_cyan[]        = "#7c008e"; 
static const unsigned int baralpha = 20;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

static const unsigned int alphas[][3]      = {
	/*               fg         bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

static const char *const autostart[] = {
	"dwmblocks", NULL,
	"clipmenud", NULL,	
	"unclutter", "-idle", "3", NULL,
	"xrdb", "~/.Xresources", NULL,
	"sh", "-c", "~/.fehbg", NULL,
	"picom", NULL,
	NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "", "", "", "", "", "", "", "", "" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance   title       tags mask  isfloating  iscentered   isterminal  noswallow isfakefullscreen monitor */
	{ "Pcmanfm",  NULL,      NULL,       1 << 1,       0, 		0,           0,     	0,		0,	-1 },
	{ NULL,       NULL,      "Firefox",  1 << 2,       0, 		0,           0,     	0,		0,	-1 },
	{ NULL,       NULL,      "Discord",  1 << 3,       0, 		0,           0,     	0,		0,	-1 },
	{ NULL,       NULL,      "Telegram", 0,     	   1, 		1,           0,     	0,		0,	-1 },
	{ NULL,       NULL,      "Steam",    1 << 4,       0, 		0,           0,     	0,		1,	-1 },
	{ "URxvt",    NULL,      NULL,       0,   	   0, 		0,           1,     	0,		0,	-1 },
	{ "Alacritty",NULL,      NULL,       0,   	   0, 		0,           1,     	0,		0,	-1 }, 
	{ NULL,       NULL,      "ncmpcpp",  0,   	   1, 		1,           0,     	0,		0,	-1 },
	{ NULL,       NULL,      "Flex",     0,   	   1, 		1,           0,     	0,		0,	-1 },
	{ NULL,       NULL,      "htop",     0,   	   1, 		1,           0,     	0,		0,	-1 },
	{ NULL,       NULL,      "gotop",    0,   	   1, 		1,           0,     	0,		0,	-1 },
	{ NULL,       NULL,      "nvim",     0,   	   1, 		1,           0,     	0,		0,	-1 },
	{ NULL,       NULL,      "alsamixer",0,   	   1, 		1,           0,     	0,		0,	-1 },
	{ NULL,       NULL,"Openbox Logout", 0,   	   1, 		1,           0,     	0,		0,	-1 },
	{ "Zathura",  NULL,	 NULL, 	     0,   	   0, 		0,           0,     	1,		0,	-1 },
	{ "Lxappearance", NULL,  NULL,	     0,   	   1, 		1,           0,     	0,		0,	-1 },
	{ NULL,     "sxiv",  	 NULL, 	     0,   	   1, 		1,           0,     	0,		0,	-1 },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
//set terminal
static const char *termcmd[]  = { "alacritty", NULL };
//volume controls
//static const char *upvol[]   = { "amixer", "-q", "set", "Master", "5%+", "unmute", NULL };
//static const char *downvol[] = { "amixer", "-q", "set", "Master", "5%-", "unmute", NULL };
//static const char *mutevol[] = { "amixer", "-q", "set", "Master", "toggle", NULL };
//media control
static const char *toggleaud[] = { "mpc", "toggle", NULL };
static const char *nextaud[]  = { "mpc", "next", NULL };
static const char *prevaud[]  = { "mpc", "prev", NULL };
//system control functions
static const char *fn1[] = { "configmenu", NULL };
static const char *fn2[] = { "sxiv", "-bt", "/home/pole/.config/wallpapers", NULL };
static const char *fn3[] = { "settingsmenu", NULL };
//launch apps
static const char *web[] = { "firefox", NULL };
static const char *fm[]  = { "pcmanfm", NULL };
static const char *dis[] = { "discord", NULL };
static const char *tlg[] = { "telegram-desktop", NULL };
static const char *scr[] = { "flameshot", "gui", NULL };
static const char *stm[] = { "steam", NULL };
static const char *std[] = { "dgl", NULL };
static const char *ncm[] = { "urxvt", "-e", "ncmpcpp", NULL };
static const char *pas[] = { "passmenu", NULL };
static const char *clp[] = { "clipmenu", NULL };
static const char *slc[] = { "slock", NULL };
static const char *obl[] = { "oblogout", NULL };
static const char *scl[] = { "superclipmenu", NULL };
static const char *top[] = { "urxvt", "-e", "htop", NULL };
static const char *skp[] = { "skippy-xd", NULL };
static const char *vwi[] = { "urxvt", "-e", "nvim", "/home/pole/vimwiki/index.wiki", NULL };
static const char *ani[] = { "watchanime", NULL };
static const char *trs[] = { "transmission-gtk", NULL };

#include "shiftview.c"
#include <X11/XF86keysym.h>

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_u,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,      rotatestack,    {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      rotatestack,    {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
//	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
//	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
//	{ MODKEY,                       XK_r,      setlayout,      {.v = &layouts[0]} },
//	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
//	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[2]} },
//	{ MODKEY,                       XK_y,      setlayout,      {.v = &layouts[3]} },
//	{ MODKEY|ShiftMask,             XK_y,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY|ControlMask,		XK_comma,  cyclelayout,    {.i = -1 } },
	{ MODKEY|ControlMask,           XK_period, cyclelayout,    {.i = +1 } },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY,             		XK_f,      togglefullscr,  {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
//	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
//	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
//	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
//	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
//	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
//	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,              		XK_equal,  incrgaps,       {.i = +1 } },
	{ MODKEY,    			XK_minus,  incrgaps,       {.i = -1 } },
//	{ MODKEY|Mod4Mask,              XK_i,      incrigaps,      {.i = +1 } },
//	{ MODKEY|Mod4Mask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } },
//	{ MODKEY|Mod4Mask,              XK_o,      incrogaps,      {.i = +1 } },
//	{ MODKEY|Mod4Mask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } },
//	{ MODKEY|Mod4Mask,              XK_6,      incrihgaps,     {.i = +1 } },
//	{ MODKEY|Mod4Mask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } },
//	{ MODKEY|Mod4Mask,              XK_7,      incrivgaps,     {.i = +1 } },
//	{ MODKEY|Mod4Mask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } },
//	{ MODKEY|Mod4Mask,              XK_8,      incrohgaps,     {.i = +1 } },
//	{ MODKEY|Mod4Mask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } },
//	{ MODKEY|Mod4Mask,              XK_9,      incrovgaps,     {.i = +1 } },
//	{ MODKEY|Mod4Mask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } },
	{ MODKEY|ControlMask,           XK_equal,  togglegaps,     {0} },
	{ MODKEY|ShiftMask,    		XK_equal,  defaultgaps,    {0} },	
        { MODKEY,                       XK_Down,   moveresize,     {.v = "0x 25y 0w 0h" } },
	{ MODKEY,                       XK_Up,     moveresize,     {.v = "0x -25y 0w 0h" } },
	{ MODKEY,                       XK_Right,  moveresize,     {.v = "25x 0y 0w 0h" } },
	{ MODKEY,                       XK_Left,   moveresize,     {.v = "-25x 0y 0w 0h" } },
	{ MODKEY|ShiftMask,             XK_Down,   moveresize,     {.v = "0x 0y 0w 25h" } },
	{ MODKEY|ShiftMask,             XK_Up,     moveresize,     {.v = "0x 0y 0w -25h" } },
	{ MODKEY|ShiftMask,             XK_Right,  moveresize,     {.v = "0x 0y 25w 0h" } },
	{ MODKEY|ShiftMask,             XK_Left,   moveresize,     {.v = "0x 0y -25w 0h" } },
	{ MODKEY|ControlMask,           XK_Up,     moveresizeedge, {.v = "t"} },
	{ MODKEY|ControlMask,           XK_Down,   moveresizeedge, {.v = "b"} },
	{ MODKEY|ControlMask,           XK_Left,   moveresizeedge, {.v = "l"} },
	{ MODKEY|ControlMask,           XK_Right,  moveresizeedge, {.v = "r"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Up,     moveresizeedge, {.v = "T"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Down,   moveresizeedge, {.v = "B"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Left,   moveresizeedge, {.v = "L"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Right,  moveresizeedge, {.v = "R"} },
	{ MODKEY,                       XK_period, shiftview,      {.i = +1 } },
        { MODKEY,                       XK_comma,  shiftview,      {.i = -1 } },
        
	{ MODKEY,                       XK_F1,     spawn,          {.v = fn1 } },
	{ MODKEY,                       XK_F2,     spawn,          {.v = fn2 } },
	{ MODKEY,                       XK_F3,     spawn,          {.v = fn3 } },

	{ MODKEY,                       XK_e,      spawn,          {.v = fm } },
        { MODKEY,                       XK_w,      spawn,          {.v = web } },
        { MODKEY,                       XK_d,      spawn,          {.v = dis } },
        { MODKEY|ShiftMask,             XK_d,      spawn,          {.v = tlg } },
        { 0,                            XK_Print,  spawn,          {.v = scr } },
        { MODKEY|ShiftMask,             XK_s,      spawn,          {.v = stm } },	
        { MODKEY,            		XK_s,      spawn,          {.v = std } },	
        { MODKEY,             		XK_m,      spawn,          {.v = ncm } },	
        { MODKEY|ShiftMask,             XK_p,      spawn,          {.v = pas } },	
        { MODKEY,             		XK_v,      spawn,          {.v = clp } },	
        { MODKEY|ShiftMask,             XK_v,      spawn,          {.v = scl } },	
        { MODKEY|ControlMask,           XK_l,      spawn,          {.v = slc } },	
        { MODKEY|ControlMask|ShiftMask, XK_x,      spawn,          {.v = obl } },	
        { 0|ShiftMask|ControlMask,      XK_Escape, spawn,          {.v = top } },	
        { MODKEY,             		XK_Tab,    spawn,          {.v = skp } },	
        { MODKEY,             		XK_n,      spawn,          {.v = vwi } },	
        { MODKEY|ShiftMask,             XK_a,      spawn,          {.v = ani } },	
        { MODKEY,             		XK_t,      spawn,          {.v = trs } },	
	
	{ 0, XF86XK_AudioRaiseVolume,              spawn,          SHCMD("amixer -q set Master 5%+ unmute ; kill -38 $(pidof dwmblocks)") },
        { 0, XF86XK_AudioLowerVolume,              spawn,          SHCMD("amixer -q set Master 5%- unmute ; kill -38 $(pidof dwmblocks)") },
        { 0, XF86XK_AudioMute,                     spawn,          SHCMD("amixer -q set Master toggle ; kill -38 $(pidof dwmblocks)") },	
	{ 0, XF86XK_AudioPlay,			   spawn,	   {.v = toggleaud } },
	{ 0, XF86XK_AudioNext,			   spawn,	   {.v = nextaud } },
	{ 0, XF86XK_AudioPrev,			   spawn,	   {.v = prevaud } },

	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
//	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_q,      quit,           {1} }, 
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
	{ ClkStatusText,        ShiftMask,      Button1,        sigstatusbar,   {.i = 4} },	
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

