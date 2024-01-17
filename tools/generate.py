import os
import json
import argparse
import stringcase
from dataclasses import dataclass


@dataclass
class Format:
    file: str = None
    handler: str = None
    classFormat: str = None


cppFormat = Format()
cppFormat.file = '''#pragma once
#pragma warning(push)
#pragma warning(disable: 26495)
#include <core/Packet.hpp>

#include <vector>

using int8 = char;
using int16 = short;
using int32 = int;
using int64 = long long;
using uint8 = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;
using uint64 = unsigned long long;

namespace {0} {{
    enum class PacketId {{
{1}
    }};

    {2}
}}
#pragma warning(pop)
'''

cppFormat.handler = '''#pragma once
#include <core/Packet.hpp>
#include <generated/Packet.hpp>

namespace sv {{ class Session; }}

namespace {0}
{{
    class PacketHandler
	{{
	public:
		static void onReceivePacket(sv::Session* session, PacketId id, sv::Packet* packet)
        {{
	        switch (id)
	        {{
{1}
	        }}
        }}
	private:
{2}
	}};
}}
'''

cppFormat.classFormat = '''class {0}
            : public sv::Packet {{
    public:
        {0}() : sv::Packet(static_cast<unsigned short>(PacketId::{1})) {{
        }}
        ~{0}() {{
    
        }}
    public:
        void read() override
        {{
            Packet::read();
            {2}
        }}
        void write() override
        {{
            {3}
            finish();
        }}
    public:
        {4}
    }};
    
    inline sv::Packet& operator>>(sv::Packet& pk, {5}) {{
        {6}
        return pk;
    }}

    inline sv::Packet& operator<<(sv::Packet& pk, const {5}) {{
        {7}
        return pk;
    }}
'''

csharpFormat = Format()
csharpFormat.file = """/*
    TODO: Copy to your project.
*/

namespace {0}
{{
    public enum PacketId
    {{
{1}
    }}

    {2}
}}
"""

csharpFormat.classFormat = """class {0} : Sv.Packet {{

    }}
"""


@dataclass
class Element:
    type: str = None
    name: str = None


def camelcase(s):
    camelcase = stringcase.camelcase(s)
    return camelcase[0].upper() + camelcase[1:]


def readCppClass(conditionList, handlerList):
    read_class = ' >> '.join(value.name for value in elementList)
    write_line = ' << '.join(value.name for value in elementList)
    if read_class == '' or write_line == '':
        raise Exception("Packet element can't be null.")

    read_class = '*this >> ' + read_class + ';'
    write_line = '*this << ' + write_line + ';'

    read_op = ' >> '.join(f'{stringcase.camelcase(packetId)}.{value.name}' for value in elementList)
    write_op = ' << '.join(f'{stringcase.camelcase(packetId)}.{value.name}' for value in elementList)
    read_op = f'pk >> {read_op};'
    write_op = f'pk << {write_op};'

    for classes in packetIdList:
        handlerName = f'{camelcase(classes)}PacketHandler'
        handlerList.append(handlerName)
        conditionList.append(f'\t\t\tcase PacketId::{classes.upper()}:\n'
                                + f'\t\t\t\t{handlerName}(session, static_cast<{camelcase(classes)}*>(packet));\n'
                                + '\t\t\t\tbreak;'
        )

    return cppFormat.classFormat.format(
        camelcase(packetId),
        packetId.upper(),
        read_class,
        write_line,
        '\t'.join('{} {};\n\t'.format(value.type, value.name) for value in elementList),
        f'{camelcase(packetId)}& {stringcase.camelcase(packetId)}',
        read_op,
        write_op
    )

def readCsharpClass(conditionList, handlerList):
    return csharpFormat.classFormat.format(
        camelcase(packetId)
    )


parser = argparse.ArgumentParser()
parser.add_argument('-l', '--language', default='cpp', action='store', dest='lang', help='generated file language')
parser.add_argument('-n', '--namespace', default='gen', action='store', dest='namespace', help='namespace name')
args = parser.parse_args()


def getLangTypename(typename):
    if typename == 'int':
        if args.lang == 'cpp':
            return 'Int32'
    if typename == 'string':
        if args.lang == 'cpp':
            return 'std::string'
    if typename.find('list') != -1:
        if args.lang == 'cpp':
            tmp = typename.split('|')
            return f'std::vector<{getLangTypename(tmp[1])}>'
    return typename


with open('define.json') as jsonFile:
    data = json.load(jsonFile)

    packetList = data['packet']
    packetIdList = []
    classList = []

    outputPacket = ''
    outputHandler = ''
    ext = ''

    # defined packet list
    for packet in packetList:
        packetId = packet['id']
        packetIdList.append(packetId)
        elements = packet['element']
        elementList = []

        # packet element
        for element in elements:
            elem = Element()
            elem.name = element['name']
            elem.type = getLangTypename(element['type'])

            elementList.append(elem)

        conditionList = []
        handlerList = []

        # Add class list.
        if args.lang == 'cpp':
            classList.append(readCppClass(conditionList, handlerList))
        elif args.lang == 'csharp':
            classList.append(readCsharpClass(conditionList, handlerList))

    # formatting data
    if args.lang == 'cpp':
        outputPacket = cppFormat.file.format(
            args.namespace.lower(), #namespace
            ',\n'.join(str(f'\t\t{value.upper()} = {packetIdList.index(value)+1}') for value in packetIdList), # packet enum
            '\n\t'.join(classList), #packet classes
            )
        outputHandler = cppFormat.handler.format(
            args.namespace,
            '\n'.join(str(value) for value in conditionList), #dispatches
            '\n'.join(str('\t\tstatic void '+value+f'(sv::Session* session, {camelcase(packetIdList[handlerList.index(value)])}* packet);') for value in handlerList) #handlers
            )
        ext = 'hpp'
    if args.lang == 'csharp':
        outputPacket = csharpFormat.file.format(
            camelcase(args.namespace),
            ',\n'.join(str(f'\t\t{value.upper()} = {packetIdList.index(value)+1}') for value in packetIdList),
            '\n\t'.join(classList)
        )
        ext = 'cs'

    # write packet & handler file
    genPacket = open(f'generated/GenPacket.{ext}', 'w')
    genPacket.write(outputPacket)
    genHandler = open(f'generated/PacketHandler.{ext}', 'w')
    genHandler.write(outputHandler)
