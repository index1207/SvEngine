#pragma once
#pragma warning(push)
#pragma warning(disable: 26495)
#pragma warning(disable: 4100)
#include "Packet.gen.hpp"

#ifdef __UNREAL__
#include "Network/Packet.h"
#elif __SERVER__
#include "Network/Packet.hpp"
#include "Util/Types.hpp"
#endif

#include <vector>

/* Additional headers. */

namespace gen {
namespace mmo {
    enum ERoomEvent : uint16 {
        CREATE,
		ENTER,
		LEAVE
    };
}
}
#pragma warning(pop)
