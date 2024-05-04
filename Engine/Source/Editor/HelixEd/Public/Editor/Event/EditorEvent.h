// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "Engine/Event/Event.h"


// ---------------------------------
//			Content
// ---------------------------------

class ContentItemDoubleClicked : public Event
{
public:
	ContentItemDoubleClicked( const TChar* pFilepath )
		: m_Filepath( pFilepath )
	{
	}
	virtual ~ContentItemDoubleClicked()
	{
	}

	const TChar* GetClickedFilename() const { return m_Filepath; }

	EVENT_CLASS_TYPE( ET_ContentItemDoubleClicked )
		EVENT_CLASS_CATEGORY( EC_Editor )

private:
	const TChar* m_Filepath;

};


// ---------------------------------
//			Editor Tabs
// ---------------------------------

class EditorTabOpened : public Event
{
public:
	EditorTabOpened()
	{
	}
	virtual ~EditorTabOpened()
	{
	}

	EVENT_CLASS_TYPE( ET_EditorTabOpened )
		EVENT_CLASS_CATEGORY( EC_Editor )
};

class EditorTabClosed : public Event
{
public:
	EditorTabClosed()
	{
	}
	virtual ~EditorTabClosed()
	{
	}

	EVENT_CLASS_TYPE( ET_EditorTabClosed )
		EVENT_CLASS_CATEGORY( EC_Editor )
};