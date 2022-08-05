#pragma once
#include "wx/string.h"

struct Version
{
	static constexpr int major = 0;
	static constexpr int minor = 6;
	static constexpr int patch = 1;

	static const inline wxString string = (patch == 0)
		? wxString::Format("%d.%d", major, minor)
		: wxString::Format("%d.%d.%d", major, minor, patch);
};
