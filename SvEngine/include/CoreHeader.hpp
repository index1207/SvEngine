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
#include <sstream>
#include <algorithm>

#include <ppl.h>
#include <concurrent_vector.h>
#include <concurrent_queue.h>
#include <concurrent_priority_queue.h>
#include <concurrent_unordered_map.h>
#include <concurrent_unordered_set.h>

#include <net/netcpp.hpp>

#include "sqltypes.h"
#include "sql.h"
#include "sqlext.h"

#include "Util/Types.hpp"
#include "Util/Console.hpp"
#include "Thread/TLSStorage.hpp"

extern class ThreadManager* GThreadManager;
extern class GlobalQueue* GGlobalQueue;
extern class DbConnectionPool* GDbConnectionPool;

using namespace net;