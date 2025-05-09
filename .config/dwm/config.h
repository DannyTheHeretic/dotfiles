/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
/* appearance */
static const unsigned int borderpx  = 5;        /* border pixel of windows */
static const unsigned int gappx     = 10;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=20", "SymbolsNerdFont:weight=bold:size=20" };
static const char dmenufont[]	= "monospace:size=10";
static const char col_gray1[]	= "#222222";
static const char col_gray2[]	= "#444444";
static const char col_gray3[]	= "#bbbbbb";
static const char col_gray4[]	= "#eeeeee";
static const char col_cyan[]	= "#005577";
static const char col_black[]	= "#000000";
static const char col_white[]	= "#ffffff";

static const char active_tag[]        = "#434043"; // current tag and background of upper bar
static const char active_window_border[]        = "#070000"; // border of active window
static const char norm_bar[]        = "#434043"; // inactive tags, selection mode, systray & dwmblocks

static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, norm_bar, col_gray2 },
	[SchemeSel]  = { col_gray4, active_tag, active_window_border  },
};

/* tagging */
static const char *tags[] = { "", "", "", "", "" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_NAME(STRING) = title
	 */
	/*  class              instance	      title			      tag     float monitor */
  	{ "Alacritty",	      "Alacritty",	"Alacritty",		1 << 1,		0,	-1 },
  	{ "firefox",  	      "Navigator",	NULL,			      1 << 2,		0,	-1 },
	  { "Code",  	          "code",		    NULL,			      1 << 3,		0,	-1 },
	  { "obsidian",       	"obsidian", 	NULL,			      1 << 0,		0,	-1 },
	  { "discord",          "discord",  	NULL,			      1 << 0,		0,	-1 },
	  { "Plexamp",  	      "plexamp",  	NULL,			      1 << 0,		1,	-1 },
	  { "steamwebhelper",  	"steam",      NULL,			      1 << 5,		0,	-1 },
    { "Thunar",            NULL,        NULL,           1 << 0,   0,  -1 },
    { "Blueman-manager",   NULL,        NULL,           1 << 0,   0,  -1 },
    { "pavucontrol",       NULL,        NULL,           1 << 0,   0,  -1 },
    { "LibreWolf",         NULL,        NULL,           1 << 2,   0,  -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
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
static const char *code[]     = { "code", NULL };
static const char *firefox[]  = { "firefox", NULL };
static const char *rofi[]     = { "rofi", "-show", "drun", "-show-icons", NULL };
static const char *obi[]      = { "md.obsidian.Obsidian", NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char *files[]    = { "thunar", NULL };
static const char *brightness[2][3] = {
	{"brightnessctl", "set", "5%+"},
	{"brightnessctl", "set", "5%-"}
};

static const char *volume[3][4] = { 
	{"pactl", "set-sink-volume", "@DEFAULT_SINK@", "+10%"},
	{"pactl", "set-sink-volume", "@DEFAULT_SINK@", "-10%"},
	{"pactl", "set-sink-mute", "@DEFAULT_SINK@", "toggle"} 
};

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,			XK_o,				spawn,		{.v = code } }, // spawn code
	{ MODKEY,			XK_f,				spawn,		{.v = firefox } }, // spawn firefox
	{ MODKEY,			XK_p,				spawn,		{.v = rofi } }, // spawn rofi
	{ MODKEY,			XK_n,				spawn,		{.v = obi } },
	{ MODKEY,			XK_Return,	spawn,		{.v = termcmd } }, // spawn terminal
  { MODKEY,     XK_e,       spawn,    {.v = files} },
	{ MODKEY,			XK_b,				togglebar,	{0} },
	{ MODKEY,			XK_j,				focusstack,	{.i = +1 } },
	{ MODKEY,			XK_k,				focusstack,	{.i = -1 } },
	{ MODKEY,			XK_i,				incnmaster,	{.i = +1 } },
	{ MODKEY,			XK_d,				incnmaster,	{.i = -1 } },
	{ MODKEY,			XK_h,				setmfact,	{.f = -0.05} },
	{ MODKEY,			XK_l,				setmfact,	{.f = +0.05} },
	{ MODKEY|ShiftMask,		XK_Return,			zoom,		{0} },
	{ MODKEY,			XK_q,				view,		{0} },
	{ MODKEY|ShiftMask,		XK_c,				killclient,	{0} },
	{ MODKEY,			XK_t,				setlayout,	{.v = &layouts[0]} },
	{0,				XF86XK_MonBrightnessUp,		spawn,		{.v=brightness[0]} },
	{0,				XF86XK_MonBrightnessDown,	spawn,		{.v=brightness[1]} },
	{0,				XF86XK_AudioRaiseVolume,	spawn,		{.v=volume[0]}},
	{0,				XF86XK_AudioLowerVolume,	spawn,		{.v=volume[1]}},
	{0,				XF86XK_AudioMute,		spawn,		{.v=volume[2]}},
	// { MODKEY,			XK_f,				setlayout,	{.v = &layouts[1]} },
	{ MODKEY,			XK_m,				setlayout,	{.v = &layouts[2]} },
	{ MODKEY,			XK_space,			setlayout,	{0} },
	{ MODKEY|ShiftMask,		XK_space,			togglefloating,	{0} },
	{ MODKEY,			XK_0,				view,		{.ui = ~0 } },
	{ MODKEY|ShiftMask,		XK_0,				tag,		{.ui = ~0 } },
	{ MODKEY,			XK_comma,			focusmon,	{.i = -1 } },
	{ MODKEY,			XK_period,			focusmon,	{.i = +1 } },
	{ MODKEY|ShiftMask,		XK_comma,			tagmon,		{.i = -1 } },
	{ MODKEY|ShiftMask,		XK_period,			tagmon,		{.i = +1 } },
	{ MODKEY,			XK_minus,			setgaps,	{.i = -1 } },
	{ MODKEY,			XK_equal,			setgaps,	{.i = +1 } },
	{ MODKEY|ShiftMask,		XK_equal,			setgaps,	{.i = 0  } },
	{ Mod1Mask,			XK_Tab,				rotatetags,	{.i = 1} },
	{ Mod1Mask|ShiftMask,		XK_Tab,				rotatetags,	{.i = -1} },
	TAGKEYS(                        XK_1,				0)
	TAGKEYS(                        XK_2,				1)
	TAGKEYS(                        XK_3,				2)
	TAGKEYS(                        XK_4,				3)
	TAGKEYS(                        XK_5,				4)
	TAGKEYS(                        XK_6,				5)
	TAGKEYS(                        XK_7,				6)
	TAGKEYS(                        XK_8,				7)
	TAGKEYS(                        XK_9,				8)
	{ MODKEY|ShiftMask,		XK_q,				quit,		{0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
