/** 
 * @file llbutton.h
 * @brief Header for buttons
 *
 * $LicenseInfo:firstyear=2001&license=viewergpl$
 * 
 * Copyright (c) 2001-2010, Linden Research, Inc.
 * 
 * Second Life Viewer Source Code
 * The source code in this file ("Source Code") is provided by Linden Lab
 * to you under the terms of the GNU General Public License, version 2.0
 * ("GPL"), unless you have obtained a separate licensing agreement
 * ("Other License"), formally executed by you and Linden Lab.  Terms of
 * the GPL can be found in doc/GPL-license.txt in this distribution, or
 * online at http://secondlife.com/developers/opensource/gplv2
 * 
 * There are special exceptions to the terms and conditions of the GPL as
 * it is applied to this Source Code. View the full text of the exception
 * in the file doc/FLOSS-exception.txt in this software distribution, or
 * online at
 * http://secondlife.com/developers/opensource/flossexception
 * 
 * By copying, modifying or distributing this software, you acknowledge
 * that you have read and understood your obligations described above,
 * and agree to abide by those obligations.
 * 
 * ALL LINDEN LAB SOURCE CODE IS PROVIDED "AS IS." LINDEN LAB MAKES NO
 * WARRANTIES, EXPRESS, IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY,
 * COMPLETENESS OR PERFORMANCE.
 * $/LicenseInfo$
 * 
 */

#ifndef LL_LLMENUBUTTON_H
#define LL_LLMENUBUTTON_H

#include "llbutton.h"

class LLMenuGL;

class LLMenuButton
: public LLButton
{
public:
	struct Params 
	:	public LLInitParam::Block<Params, LLButton::Params>
	{
		// filename for it's toggleable menu
		Optional<std::string>	menu_filename;
	
		Params();
	};	
	
	void toggleMenu();
	/*virtual*/ void draw();
	/*virtual*/ BOOL handleMouseDown(S32 x, S32 y, MASK mask);
	/*virtual*/ BOOL handleKeyHere(KEY key, MASK mask );
	void hideMenu();
	LLMenuGL* getMenu() { return mMenu; }

protected:
	friend class LLUICtrlFactory;
	LLMenuButton(const Params&);

private:
	LLMenuGL*	mMenu;
	bool mMenuVisibleLastFrame;
};


#endif  // LL_LLMENUBUTTON_H
