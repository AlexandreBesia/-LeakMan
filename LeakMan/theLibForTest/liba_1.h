#pragma once

namespace hearc
{
	namespace liba
	{
		class LibA_1
		{
			public:
			
			LibA_1() = default;
			virtual ~LibA_1() = default;
			LibA_1(const LibA_1 &) = delete;
			LibA_1 &operator=(const LibA_1 &) = delete;

			int Add(int a, int b);
		};
	}
}
