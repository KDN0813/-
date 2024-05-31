#pragma once

class MoveDisable
{
public:
	MoveDisable() = default;
	virtual ~MoveDisable() = default;

	MoveDisable(MoveDisable&&) noexcept = delete;
	MoveDisable& operator=(MoveDisable&&) noexcept = delete;
private:
};