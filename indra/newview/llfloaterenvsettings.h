/** 
 * @file llfloaterskysettings.h
 * @brief LLFloaterEnvSettings class definition
 *
 * $LicenseInfo:firstyear=2007&license=viewergpl$
 * 
 * Copyright (c) 2007-2010, Linden Research, Inc.
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

/*
 * Simple menu for adjusting the atmospheric settings of the world
 */

#ifndef LL_LLFLOATERENVSETTINGS_H
#define LL_LLFLOATERENVSETTINGS_H

#include "llfloater.h"

struct WaterColorControl;
struct WaterExpFloatControl;

/// Menuing system for all of windlight's functionality
class LLFloaterEnvSettings : public LLFloater
{
public:

	LLFloaterEnvSettings(const LLSD& key);
	/*virtual*/ ~LLFloaterEnvSettings();
	/*virtual*/	BOOL	postBuild();	
	/// initialize all the callbacks for the menu
	void initCallbacks(void);

	/// handle if time of day is changed
	void onChangeDayTime(LLUICtrl* ctrl);

	/// handle if cloud coverage is changed
	void onChangeCloudCoverage(LLUICtrl* ctrl);

	/// handle change in water fog density
	void onChangeWaterFogDensity(LLUICtrl* ctrl, WaterExpFloatControl* expFloatControl);

	/// handle change in water fog color
	void onChangeWaterColor(LLUICtrl* ctrl, WaterColorControl* colorControl);

	/// open the advanced sky settings menu
	void onOpenAdvancedSky();

	/// open the advanced water settings menu
	void onOpenAdvancedWater();

	/// sync time with the server
	void onUseEstateTime();

	//// menu management

	/// sync up sliders with parameters
	void syncMenu();

	/// convert the present time to a digital clock time
	std::string timeToString(F32 curTime);

private:
};


#endif
