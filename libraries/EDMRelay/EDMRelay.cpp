#include "EDMRelay.h"

inline void EDMRelay::toggleState() {
	if (isOpen())
		close();
	else
		open();
}
