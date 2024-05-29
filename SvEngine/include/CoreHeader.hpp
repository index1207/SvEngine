#pragma once

#include <string>
#include <format>

#include "Util/Types.hpp"

#include <net/netcpp.hpp>

#include <sql.h>
#include <sqlext.h>

#include "GameFramework/Vector.hpp"

#include "Util/Action.hpp"
#include "Util/Console.hpp"
#include "Thread/TLSStorage.hpp"
#include "Network/Packet.hpp"
#include "Subsystem/Engine.hpp"

#include "Memory/Arena.hpp"
#include "Memory/ObjectPool.hpp"

using namespace net;