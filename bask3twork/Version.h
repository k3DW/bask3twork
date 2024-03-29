#pragma once
#include <wx/string.h>

struct Version
{
	static constexpr int major = 0;
	static constexpr int minor = 10;
	static constexpr int patch = 0;

	static const inline wxString string = (patch == 0)
		? wxString::Format("%d.%d", major, minor)
		: wxString::Format("%d.%d.%d", major, minor, patch);
};
