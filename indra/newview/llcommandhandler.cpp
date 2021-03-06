/**
 * @file llcommandhandler.cpp
 * @brief Central registry for text-driven "commands", most of
 * which manipulate user interface.  For example, the command
 * "agent (uuid) about" will open the UI for an avatar's profile.
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
#include "llviewerprecompiledheaders.h"

#include "llcommandhandler.h"
#include "llnotificationsutil.h"
#include "llcommanddispatcherlistener.h"
#include "stringize.h"

// system includes
#include <boost/tokenizer.hpp>

#define THROTTLE_PERIOD    15    // required secs between throttled commands

static LLCommandDispatcherListener sCommandDispatcherListener;

//---------------------------------------------------------------------------
// Underlying registry for command handlers, not directly accessible.
//---------------------------------------------------------------------------
struct LLCommandHandlerInfo
{
	LLCommandHandler::EUntrustedAccess mUntrustedBrowserAccess;
	LLCommandHandler* mHandler;	// safe, all of these are static objects
};

class LLCommandHandlerRegistry
{
public:
	static LLCommandHandlerRegistry& instance();
	void add(const char* cmd,
			 LLCommandHandler::EUntrustedAccess untrusted_access,
			 LLCommandHandler* handler);
	bool dispatch(const std::string& cmd,
				  const LLSD& params,
				  const LLSD& query_map,
				  LLMediaCtrl* web,
				  bool trusted_browser);

private:
	friend LLSD LLCommandDispatcher::enumerate();
	std::map<std::string, LLCommandHandlerInfo> mMap;
};

// static 
LLCommandHandlerRegistry& LLCommandHandlerRegistry::instance()
{
	// Force this to be initialized on first call, because we're going
	// to be adding items to the std::map before main() and we can't
	// rely on a global being initialized in the right order.
	static LLCommandHandlerRegistry instance;
	return instance;
}

void LLCommandHandlerRegistry::add(const char* cmd,
								   LLCommandHandler::EUntrustedAccess untrusted_access,
								   LLCommandHandler* handler)
{
	LLCommandHandlerInfo info;
	info.mUntrustedBrowserAccess = untrusted_access;
	info.mHandler = handler;

	mMap[cmd] = info;
}

bool LLCommandHandlerRegistry::dispatch(const std::string& cmd,
										const LLSD& params,
										const LLSD& query_map,
										LLMediaCtrl* web,
										bool trusted_browser)
{
	static bool slurl_blocked = false;
	static bool slurl_throttled = false;
	static F64 last_throttle_time = 0.0;
	F64 cur_time = 0.0;
	std::map<std::string, LLCommandHandlerInfo>::iterator it = mMap.find(cmd);
	if (it == mMap.end()) return false;
	const LLCommandHandlerInfo& info = it->second;
	if (!trusted_browser)
	{
		switch (info.mUntrustedBrowserAccess)
		{
		case LLCommandHandler::UNTRUSTED_ALLOW:
			// fall through and let the command be handled
			break;

		case LLCommandHandler::UNTRUSTED_BLOCK:
			// block request from external browser, but report as
			// "handled" because it was well formatted.
			LL_WARNS_ONCE("SLURL") << "Blocked SLURL command from untrusted browser" << LL_ENDL;
			if (! slurl_blocked)
			{
				LLNotificationsUtil::add("BlockedSLURL");
				slurl_blocked = true;
			}
			return true;

		case LLCommandHandler::UNTRUSTED_THROTTLE:
			cur_time = LLTimer::getElapsedSeconds();
			if (cur_time < last_throttle_time + THROTTLE_PERIOD)
			{
				// block request from external browser if it happened
				// within THROTTLE_PERIOD secs of the last command
				LL_WARNS_ONCE("SLURL") << "Throttled SLURL command from untrusted browser" << LL_ENDL;
				if (! slurl_throttled)
				{
					LLNotificationsUtil::add("ThrottledSLURL");
					slurl_throttled = true;
				}
				return true;
			}
			last_throttle_time = cur_time;
			break;
		}
	}
	if (!info.mHandler) return false;
	return info.mHandler->handle(params, query_map, web);
}

//---------------------------------------------------------------------------
// Automatic registration of commands, runs before main()
//---------------------------------------------------------------------------

LLCommandHandler::LLCommandHandler(const char* cmd,
								   EUntrustedAccess untrusted_access)
{
	LLCommandHandlerRegistry::instance().add(cmd, untrusted_access, this);
}

LLCommandHandler::~LLCommandHandler()
{
	// Don't care about unregistering these, all the handlers
	// should be static objects.
}

//---------------------------------------------------------------------------
// Public interface
//---------------------------------------------------------------------------

// static
bool LLCommandDispatcher::dispatch(const std::string& cmd,
								   const LLSD& params,
								   const LLSD& query_map,
								   LLMediaCtrl* web,
								   bool trusted_browser)
{
	return LLCommandHandlerRegistry::instance().dispatch(
		cmd, params, query_map, web, trusted_browser);
}

static std::string lookup(LLCommandHandler::EUntrustedAccess value);

LLSD LLCommandDispatcher::enumerate()
{
	LLSD response;
	LLCommandHandlerRegistry& registry(LLCommandHandlerRegistry::instance());
	for (std::map<std::string, LLCommandHandlerInfo>::const_iterator chi(registry.mMap.begin()),
																	 chend(registry.mMap.end());
		 chi != chend; ++chi)
	{
		LLSD info;
		info["untrusted"] = chi->second.mUntrustedBrowserAccess;
		info["untrusted_str"] = lookup(chi->second.mUntrustedBrowserAccess);
		response[chi->first] = info;
	}
	return response;
}

/*------------------------------ lookup stuff ------------------------------*/
struct symbol_info
{
	const char* name;
	LLCommandHandler::EUntrustedAccess value;
};

#define ent(SYMBOL)										\
	{													\
		#SYMBOL + 28, /* skip "LLCommandHandler::UNTRUSTED_" prefix */	\
		SYMBOL											\
	}

symbol_info symbols[] =
{
	ent(LLCommandHandler::UNTRUSTED_ALLOW),		  // allow commands from untrusted browsers
	ent(LLCommandHandler::UNTRUSTED_BLOCK),		  // ignore commands from untrusted browsers
	ent(LLCommandHandler::UNTRUSTED_THROTTLE)	  // allow untrusted, but only a few per min.
};

#undef ent

static std::string lookup(LLCommandHandler::EUntrustedAccess value)
{
	for (symbol_info *sii(symbols), *siend(symbols + (sizeof(symbols)/sizeof(symbols[0])));
		 sii != siend; ++sii)
	{
		if (sii->value == value)
		{
			return sii->name;
		}
	}
	return STRINGIZE("UNTRUSTED_" << value);
}
