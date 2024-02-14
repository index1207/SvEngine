#pragma once

#include <memory>
#include <mutex>
#include <thread>
#include <functional>
#include <string>
#include <format>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <span>

#include <ppl.h>
#include <concurrent_vector.h>
#include <concurrent_queue.h>
#include <concurrent_priority_queue.h>
#include <concurrent_unordered_map.h>
#include <concurrent_unordered_set.h>

#include <net/netcpp.hpp>

#include <core/Server.hpp>
#include <core/Session.hpp>
#include <core/Packet.hpp>
#include <core/Engine.hpp>
#include <util/Console.hpp>
#include <util/Action.hpp>
#include <util/Types.hpp>
#include <util/TLSStorage.hpp>

extern class ThreadManager* GThreadManager;
extern class GlobalQueue* GGlobalQueue;