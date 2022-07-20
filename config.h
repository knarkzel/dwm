/* See LICENSE file for copyright and license details. */

// XF86*
#include <X11/XF86keysym.h>

/* solarized colors http://ethanschoonover.com/solarized */
static const char s_base03[]        = "#002b36";
static const char s_base02[]        = "#073642";
static const char s_base01[]        = "#586e75";
static const char s_base00[]        = "#605a52";
static const char s_base0[]         = "#839496";
static const char s_base1[]         = "#93a1a1";
static const char s_base2[]         = "#e4ddd2";
static const char s_base3[]         = "#f7f3ee";

/* appearance */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int snap      = 1;       /* snap pixel */
static const int focusonwheel       = 0;
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 0;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=18" };
static const char dmenufont[]       = "monospace:size=18";

static const char *colors[][3]      = {
	/*               fg        bg       border   */
	[SchemeNorm] = { s_base00, s_base3, s_base02 },
	[SchemeSel]  = { s_base00, s_base2, s_base02 },
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
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
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
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", s_base3, "-nf", s_base00, "-sb", s_base2, "-sf", s_base00, NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char *firefoxcmd[]  = { "firefox", NULL };
static const char *emacscmd[]  = { "emacs", NULL };

/* audio */
static const char *volumemute[]  = { "amixer", "set", "'Master'", "0%", NULL };
static const char *volumelower[]  = { "amixer", "set", "'Master'", "5%-", NULL };
static const char *volumeraise[]  = { "amixer", "set", "'Master'", "5%+", NULL };

/* print */
#define Print 0x0000ff61
static const char *printdesktop[]  = { "scrot", "-s", NULL };

/* raiseables */
static const Raiseable raiseables[] = {
	/* arg           class */
  { {.v = termcmd }, "Alacritty" },
  { {.v = firefoxcmd }, "firefox" },
  { {.v = emacscmd }, "Emacs" },
};

static Key keys[] = {
	/* modifier                     key                       function        argument */
	//{ MODKEY,                     XK_space,                 run_or_raise,   {.v = &raiseables[0]} },
	{ MODKEY,                       XK_s,                     spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_v,                     run_or_raise,   {.v = &raiseables[0] } },
	{ MODKEY|ShiftMask,             XK_v,                     spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_c,                     run_or_raise,   {.v = &raiseables[1] } },
	{ MODKEY|ShiftMask,             XK_c,                     spawn,          {.v = firefoxcmd } },
	{ MODKEY,                       XK_f,                     run_or_raise,   {.v = &raiseables[2] } },
	{ MODKEY|ShiftMask,             XK_f,                     spawn,          {.v = emacscmd } },
	{ MODKEY,                       XK_b,                     togglebar,      {0} },
	{ MODKEY,                       XK_plus,                  incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_minus,                 incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_u,                     setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_e,                     setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_t,                     cycle,          {0} },
	{ MODKEY,                       XK_q,                     killclient,     {0} },
	{ MODKEY,                       XK_g,                     setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_j,                     setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_k,                     togglefullscr,  {0} },
	{ MODKEY|ShiftMask,             XK_k,                     setlayout,      {0} },
	{ MODKEY,                       XK_0,                     view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,                     tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_n,                     focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_i,                     focusmon,       {.i = +1 } },
	{ MODKEY,                       XK_Left,                  tagmon,         {.i = -1 } },
	{ MODKEY,                       XK_Right,                 tagmon,         {.i = +1 } },
	{ 0,                            XF86XK_AudioMute,         spawn,          {.v = volumemute } },
	{ 0,                            XF86XK_AudioLowerVolume,  spawn,          {.v = volumelower } },
	{ 0,                            XF86XK_AudioRaiseVolume,  spawn,          {.v = volumeraise } },
	{ 0,                            Print,                    spawn,          SHCMD("~/.scripts/print") },
	{ 0|ShiftMask,                  Print,                    spawn,          {.v = printdesktop } },
	{ MODKEY,                       XK_y,                     spawn,          SHCMD("~/.scripts/launch") },
	TAGKEYS(                        XK_1,                                     0)
	TAGKEYS(                        XK_2,                                     1)
	TAGKEYS(                        XK_3,                                     2)
	TAGKEYS(                        XK_4,                                     3)
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

