#include <X11/XF86keysym.h>

static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, norm_bar, col_gray2 },
	[SchemeSel]  = { col_gray4, active_tag, active_window_border  },
};


static const Rule rules[] = {
	/* xprop(1):
	 *	WM_NAME(STRING) = title
	 */
	/* class      	instance    	title       tags mask     isfloating   monitor */
	{ "Alacritty",	"Alacritty",	NULL,	1 << 1,		0,	-1 },
	{ "firefox",  	"Navigator",	NULL,	1 << 2,		0,	-1 },
	{ "Code",  	"code",		NULL,	1 << 3,		0,	-1 },
	{ "obsidian",	"obsidian",	NULL,	1 << 4,		0,	-1 },
	{ "discord",    "discord",	NULL,	1 << 5,		0,           -1 },
	{ "Plexamp",  	"plexamp",	NULL,	1 << 6,		1,           -1 }, // not working
	{ "steamwebhelper",  	"steam",	NULL,		1 << 7,       0,           -1 },
	//{ "Gimp",    	NULL,		NULL,       	0,            1,           -1 },
};



#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *code[]     = { "code", NULL };
static const char *firefox[]  = { "firefox", NULL };
static const char *rofi[]     = { "rofi", "-show", "drun", "-show-icons", NULL };
static const char *obi[]      = { "md.obsidian.Obsidian", NULL };
static const char *termcmd[]  = { "alacritty", NULL };
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
	{ MODKEY,			XK_Return,			spawn,		{.v = termcmd } }, // spawn terminal
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
