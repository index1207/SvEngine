import json
import argparse
from dataclasses import dataclass


@dataclass
class Format:
    fileFormat: str = None
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
    
    sv::Packet& operator>>(sv::Packet& pk, {5}) {{
        {6}
        return pk;
    }}

    sv::Packet& operator<<(sv::Packet& pk, const {5}) {{
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
    for packet in packetList:
        packetId = packet['id']
        packetIdList.append(packetId.upper())
        elements = packet['element']
        elementList = []

        for element in elements:
            elem = Element()
            elem.name = element['name']
            elem.type = getLangTypename(element['type'])

            elementList.append(elem)

        if args.lang == 'cpp':
            classList.append(readCppClass())
    if args.lang == 'cpp':
        output = cppFormat.file.format(
            args.namespace,
            ',\n'.join(str(f'\t\t{value} = {packetIdList.index(value)+1}') for value in packetIdList),
            '\n\t'.join(classList)
        )
        ext = 'hpp'

    outputFile = open(f'{args.output}/{args.lang}/Packet.{ext}', 'w')
    outputFile.write(output)