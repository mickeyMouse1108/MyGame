#pragma once

namespace Quasi::Graphics {
	struct TriIndices {
		u32 i, j, k;

		[[nodiscard]] TriIndices add(u32 x) const { return { i + x, j + x, k + x }; }
		TriIndices& incr(u32 x) { i += x; j += x; k += x; return *this; }

		TriIndices  operator+ (std::convertible_to<u32> auto x) const { return add((u32)x); }
		TriIndices& operator+=(std::convertible_to<u32> auto x) { return incr((u32)x); }
	};
}