/* See LICENSE file for copyright and license details. */

// XF86*
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int snap      = 1;       /* snap pixel */
static const int focusonwheel       = 0;
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=18" };
static const char dmenufont[]       = "monospace:size=18";
static const char col_gray1[]       = "#282828";
static const char col_gray2[]       = "#ffffff";
static const char col_gray3[]       = "#ebdbb2";
static const char col_gray4[]       = "#fbf1c7";
static const char col_cyan[]        = "#928374";

static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class       instance  title           tags mask  isfloating  isterminal   noswallow monitor */
	{ "Alacritty", NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ NULL,        NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 0; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[M]",      monocle }, /* first entry is default */
	{ "[]=",      tile },    /* tiling behaviour */
	{ "><>",      NULL },    /* no layout function means floating behavior */
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

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char *chromiumcmd[]  = { "chromium", NULL };
static const char *emacscmd[]  = { "emacs", NULL };

/* audio */
static const char *volumemute[]  = { "amixer", "set", "'Master'", "0%", NULL };
static const char *volumelower[]  = { "amixer", "set", "'Master'", "5%-", NULL };
static const char *volumeraise[]  = { "amixer", "set", "'Master'", "5%+", NULL };

/* raiseables */
static const Raiseable raiseables[] = {
	/* arg           class */
  { {.v = termcmd }, "Alacritty" },
  { {.v = chromiumcmd }, "Chromium" },
  { {.v = emacscmd }, "Emacs" },
};

static Key keys[] = {
	/* modifier                     key                       function        argument */
	{ MODKEY,                       XK_space,                 run_or_raise,   {.v = &raiseables[0]} },
	{ MODKEY,                       XK_d,                     spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_v,                     run_or_raise,   {.v = &raiseables[0] } },
	{ MODKEY,                       XK_c,                     run_or_raise,   {.v = &raiseables[1] } },
	{ MODKEY,                       XK_e,                     run_or_raise,   {.v = &raiseables[2] } },
	{ MODKEY,                       XK_b,                     togglebar,      {0} },
	{ MODKEY,                       XK_l,                     focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_h,                     focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_plus,                  incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_minus,                 incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_j,                     setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_k,                     setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_f,                     zoom,           {0} },
	{ MODKEY|ShiftMask,             XK_f,                     view,           {0} },
	{ MODKEY,                       XK_q,                     killclient,     {0} },
	{ MODKEY,                       XK_t,                     setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_y,                     setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_n,                     togglefullscr,  {0} },
	{ MODKEY|ShiftMask,             XK_y,                     setlayout,      {0} },
	{ MODKEY,                       XK_0,                     view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,                     tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,                 focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,                focusmon,       {.i = +1 } },
	{ MODKEY,                       XK_Left,                  tagmon,         {.i = -1 } },
	{ MODKEY,                       XK_Right,                 tagmon,         {.i = +1 } },
	{ 0,                            XF86XK_AudioMute,         spawn,          {.v = volumemute } },
	{ 0,                            XF86XK_AudioLowerVolume,  spawn,          {.v = volumelower } },
	{ 0,                            XF86XK_AudioRaiseVolume,  spawn,          {.v = volumeraise } },
	TAGKEYS(                        XK_1,                                     0)
	TAGKEYS(                        XK_2,                                     1)
	TAGKEYS(                        XK_3,                                     2)
	TAGKEYS(                        XK_4,                                     3)
	TAGKEYS(                        XK_5,                                     4)
	TAGKEYS(                        XK_6,                                     5)
	TAGKEYS(                        XK_7,                                     6)
	TAGKEYS(                        XK_8,                                     7)
	TAGKEYS(                        XK_9,                                     8)
	{ MODKEY|ShiftMask,             XK_q,                     quit,           {0} },
	{ MODKEY,                       XK_m,                     setlayout,      {.v = &layouts[0]} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

