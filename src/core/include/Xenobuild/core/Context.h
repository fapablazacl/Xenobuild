
#pragma once 

#include <iostream>

#include "Package.h"
#include "Toolchain.h"

namespace Xenobuild {
	struct Context {
		Package package;
		Toolchain toolchain;
	};
}
