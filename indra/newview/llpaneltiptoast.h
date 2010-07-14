/**
 * @file llpaneltiptoast.h
 * @brief Represents a base class of tip toast panels.
 *
 * $LicenseInfo:firstyear=2010&license=viewergpl$
 * 
 * Copyright (c) 2010, Linden Research, Inc.
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


#include "lltoastpanel.h"

#ifndef LL_PANELTOASTTIP_H
#define LL_PANELTOASTTIP_H

/**
 * Base class for tip toast panels.
 *
 * Tip toast panels are required to have text message box named as 'message'.
 */
class LLPanelTipToast : public LLToastPanel
{
	LOG_CLASS(LLPanelTipToast);
public:
	LLPanelTipToast(const LLNotificationPtr& notification): LLToastPanel(notification) {}
	virtual BOOL postBuild();
private:
	void onMessageTextClick();
	void onPanelClick(S32 x, S32 y, MASK mask);

	LLUICtrl* mMessageText;
};

#endif /* LL_PANELTOASTTIP_H */
