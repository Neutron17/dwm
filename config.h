/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>
#include <stdlib.h>

#define SESSION_FILE "~/.dwm-session"

/* alt-tab configuration */
static const unsigned int tabModKey 		= 0x40;	/* if this key is hold the alt-tab functionality stays acitve. This key must be the same as key that is used to active functin altTabStart `*/
static const unsigned int tabCycleKey 		= 0x17;	/* if this key is hit the alt-tab program moves one position forward in clients stack. This key must be the same as key that is used to active functin altTabStart */
static const unsigned int tabPosY 			= 1;	/* tab position on Y axis, 0 = bottom, 1 = center, 2 = top */
static const unsigned int tabPosX 			= 1;	/* tab position on X axis, 0 = left, 1 = center, 2 = right */
static const unsigned int maxWTab 			= 600;	/* tab menu width */
static const unsigned int maxHTab 			= 200;	/* tab menu height */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
#define ICONSIZE 16   /* icon size */
#define ICONSPACING 5 /* space between icon and title */
static const char *fonts[]          = { "monospace:size=12"/*, "fontawesome:size=10"*/ };
static const char dmenufont[]       = "monospace:size=12";

// background color
static const char col_gray1[]       = "#222222";
// inactive window border color
static const char col_gray2[]       = "#444444";
// font color
static const char col_gray3[]       = "#bbbbbb";
// current tag & curent window font color
static const char col_gray4[]       = "#000000";//"#ffa700";
// bar second color and active window border color
static const char col_cyan[]        = "#ffa700";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[9] = { "1","2","3","4","5","6","7","8","9" };
//static const char *tags[] = { "" , "", "", "", "", "", "", "", "", "" };
static const char *defaulttagapps[9] = { "st", "firefox", "firefox", "thunar", NULL, NULL, NULL, NULL, NULL };

/*
static const char *upvol[]   = { "/usr/bin/pactl", "set-sink-volume", "0", "+5%",     NULL };
static const char *downvol[] = { "/usr/bin/pactl", "set-sink-volume", "0", "-5%",     NULL };
static const char *mutevol[] = { "/usr/bin/pactl", "set-sink-mute",   "0", "toggle",  NULL };
*/
/*static const char *upvol[]	= { "/usr/bin/amixer", "-D pulse", "sset", "Master", "5%+", NULL };
static const char *downvol[]	= { "/usr/bin/amixer", "-D pulse", "sset", "Master", "5%-", NULL };
static const char *mutevol[]	= { "/usr/bin/amixer", "-D", "pulse", "sset", "Master", "0%", NULL };*/
static const char upvol[]	= "/usr/bin/amixer -D pulse sset Master 5%+";
static const char downvol[]	= "/usr/bin/amixer -D pulse sset Master 5%-";
static const char mutevol[]	= "/usr/bin/amixer -D pulse sset Master 0%";

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating, CenterThisWindow   monitor */
	//{ "Gimp",     NULL,       NULL,       0,            0,     1,      -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,     1,      -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#include "fibonacci.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",      dwindle },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
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
static const char *termcmd[]  = { "st", NULL };
static const char *shutdowncmd[] = { "prompt", "\"Are you sure to shut down?\"", "\"sudo poweroff -h now\"", NULL };

#include "shiftview.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_w,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_r,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,             XK_r,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_e,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY,                       XK_z,      setlayout,      {.v = &layouts[6]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ Mod1Mask,             		XK_Tab,    altTabStart,	   {0} },

	{ MODKEY|ShiftMask,		XK_n,	spawn,		SHCMD("setxkbmap us && xdotool type $(grep -v '^#' ~/.local/share/snippets | dmenu -i -l 50 | cut -d' ' -f1) && setxkbmap hu") },
	{ ControlMask,			XK_1,	spawn,		SHCMD("setxkbmap hu") },
	{ ControlMask,			XK_2,	spawn,		SHCMD("setxkbmap gr") },
	{ ControlMask,			XK_3,	spawn,		SHCMD("setxkbmap us") },
	{ ControlMask,			XK_4,	spawn,		SHCMD("setxkbmap de") },

	{ MODKEY|ShiftMask,		XK_b,	    spawn,	  	{.v=(const char *[]){ "bookmark", NULL } } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY|ControlMask, 		XK_q,      quit,           {1} },
	{ MODKEY|ControlMask|ShiftMask, XK_q,	   spawn,	  {.v=shutdowncmd} },
	{ MODKEY,			XK_n,		shiftview, {.i = +1} },
	{ MODKEY,			XK_n,		shiftview, {.i = -1} },
	{ MODKEY,			XK_s,		spawndefault, {0} },

	{ 0,                       XF86XK_AudioLowerVolume, spawn, {.v = downvol } },
	{ 0,                       XF86XK_AudioMute, spawn, {.v = mutevol } },
	{ 0,                       XF86XK_AudioRaiseVolume, spawn, {.v = upvol   } },
	{ MODKEY,                       XK_F9,  spawn, SHCMD(mutevol) },
	{ MODKEY,                       XK_F10, spawn, SHCMD(downvol) },
	{ MODKEY,                       XK_F11, spawn, SHCMD(upvol) },
	{ MODKEY,			XK_F1,		spawn,		{.v = (const char*[]){ "mounter", NULL } } },
	{ MODKEY,			XK_F2,		spawn,		{.v = (const char*[]){ "unmounter", NULL } } },
	{ 0,				XF86XK_MonBrightnessUp,		spawn,	SHCMD("light -A 10") },
	{ 0,				XF86XK_MonBrightnessDown,	spawn,	SHCMD("light -U 10") },

	/*{ MODKEY|ControlMask|ShiftMask,	XK_h,		togglehorizontalmax,	NULL },
	{ MODKEY|ControlMask|ShiftMask,	XK_l,		togglehorizontalmax,	NULL },
	{ MODKEY|ControlMask|ShiftMask,	XK_j,		toggleverticalmax,	NULL },
	{ MODKEY|ControlMask|ShiftMask,	XK_k,		togglevertixalmax,	NULL },
	{ MODKEY|ControlMask|ShiftMask, XK_m,		togglemaximize,		{0} },*/

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
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

