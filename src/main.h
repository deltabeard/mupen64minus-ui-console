/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus - main.h                                                  *
 *   Mupen64Plus homepage: https://mupen64plus.org/                        *
 *   Copyright (C) 2009 Richard42                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __MAIN_H__
#define __MAIN_H__

#if defined(__GNUC__)
#define ATTR_FMT(fmtpos, attrpos) __attribute__ ((format (printf, fmtpos, attrpos)))
#else
#define ATTR_FMT(fmtpos, attrpos)
#endif

#ifndef ENABLE_DEBUGGER
# define ENABLE_DEBUGGER 0
#endif

extern void DebugMessage(int level, const char *message, ...) ATTR_FMT(2,3);
extern void DebugCallback(void *Context, int level, const char *message);

extern int  g_Verbose;

#endif /* __MAIN_H__ */

