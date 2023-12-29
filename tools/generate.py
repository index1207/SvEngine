import json
import argparse
from dataclasses import dataclass


@dataclass
class Format:
    file: str = None
    handler: str = None
    classFormat: str = None


cppFormat = Format()
cppFormat.file = '''#pragma once
#include <core/Packet.hpp>

#include <vector>

using Int8 = char;
using Int16 = short;
using Int32 = int;
using Int64 = long long;
using Uint8 = unsigned char;
using Uint16 = unsigned short;
using Uint32 = unsigned int;
using Uint64 = unsigned long long;

namespace {0} {{
    enum PacketId {{
{1}
    }};

    {2}
}}
'''

cppFormat.handler = '''#pragma once
#include <core/Packet.hpp>

#include "DefinedPacket.hpp"

namespace {0}
{{
    class PacketHandler
	{{
	public:
		static void onReceivePacket(PacketId id, sv::Packet* packet)
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
        {0}() : sv::Packet(PacketId::{1}) {{
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


@dataclass
class Element:
    type: str = None
    name: str = None


def readCppClass():
    read_class = ' >> '.join(value.name for value in elementList)
    write_line = ' << '.join(value.name for value in elementList)
    if read_class == '' or write_line == '':
        raise Exception("Packet element can't be null.")

    read_class = '*this >> ' + read_class + ';'
    write_line = '*this << ' + write_line + ';'

    read_op = ' >> '.join(f'{packetId}.{value.name}' for value in elementList)
    write_op = ' << '.join(f'{packetId}.{value.name}' for value in elementList)
    read_op = f'pk >> {read_op};'
    write_op = f'pk << {write_op};'

    return cppFormat.classFormat.format(
        packetId.title(),
        packetId.upper(),
        read_class,
        write_line,
        '\t'.join('{} {};\n\t'.format(value.type, value.name) for value in elementList),
        f'{packetId.title()}& {packetId.lower()}',
        read_op,
        write_op
    )


parser = argparse.ArgumentParser()
parser.add_argument('-o', '--output', default='generated', action='store', dest='output', help='output path')
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
    return typename.title()


with open('PacketDefine.json') as jsonFile:
    data = json.load(jsonFile)

    packetList = data['packet']
    packetIdList = []
    classList = []

    output = ''
    ext = ''

    # defined packet list
    for packet in packetList:
        packetId = packet['id']
        packetIdList.append(packetId.upper())
        elements = packet['element']
        elementList = []

        # packet element
        for element in elements:
            elem = Element()
            elem.name = element['name']
            elem.type = getLangTypename(element['type'])

            elementList.append(elem)

        # Add class list.
        if args.lang == 'cpp':
            classList.append(readCppClass())

        conditionList = []
        handlerList = []
        for classes in packetIdList:
            handlerName = f'{classes.title()}PacketHandler'
            handlerList.append(handlerName)
            conditionList.append(f'case PacketId::{classes.upper()}:\n'
                                    + f'\t\t\t\tauto {classes.lower()} = static_cast<{classes.title()}*>(packet);\n'
                                    + f'\t\t\t\t{handlerName}({classes.lower()});\n'
                                    + '\t\t\t\tbreak;'
            )

    # generate data
    if args.lang == 'cpp':
        output = cppFormat.file.format(
            args.namespace, #namespace
            ',\n'.join(str(f'\t\t{value} = {packetIdList.index(value)+1}') for value in packetIdList), # packet enum
            '\n\t'.join(classList), #packet classes
            )
        ext = 'hpp'
        handler = open(f'{args.output}/{args.lang}/PacketHandler.{ext}', 'w')
        handler.write(cppFormat.handler.format(
            args.namespace,
            '\n'.join(str(value) for value in conditionList), #dispatches
            '\n'.join(str('static void '+value+f'({packetIdList[handlerList.index(value)].title()}* packet);') for value in handlerList) #handlers
            )
        )

    # write file
    outputFile = open(f'{args.output}/{args.lang}/Packet.{ext}', 'w')
    outputFile.write(output)