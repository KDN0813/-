#pragma once

class CopyDisable
{
public:
	CopyDisable() = default;
	virtual ~CopyDisable() = default;

	CopyDisable(const CopyDisable&) = delete;
	CopyDisable& operator=(const CopyDisable&) = delete;
private:
};