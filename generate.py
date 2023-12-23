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

using int8 = char;
using int16 = short;
using int32 = int;
using int64 = long long;
using uint8 = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;
using uint64 = unsigned long long;

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
'''


@dataclass
class Element:
    type: str = None
    name: str = None


def readCppClass():
    read_line = ' >> '.join(value.name for value in elementList) + ';'
    write_line = ' << '.join(value.name for value in elementList) + ';'
    if read_line != '':
        read_line = '*this >> ' + read_line
    if write_line != '':
        write_line = '*this << ' + write_line

    return cppFormat.classFormat.format(
        packetId.title(),
        packetId.upper(),
        read_line,
        write_line,
        '\n    '.join('{} {};'.format(value.type, value.name) for value in elementList),
    )


parser = argparse.ArgumentParser()
parser.add_argument('-o', '--output', default='packet.cpp', action='store', dest='output', help='output path')
parser.add_argument('-l', '--language', default='cpp', action='store', dest='lang', help='generated file language')
parser.add_argument('-n', '--namespace', default='gen', action='store', dest='namespace', help='namespace name')
args = parser.parse_args()


def getLangTypename(typename):
    if typename == 'string':
        if args.lang == 'cpp':
            return 'std::string'
    if typename.find('list') != -1:
        if args.lang == 'cpp':
            tmp = typename.split('|')
            return f'std::vector<{getLangTypename(tmp[1])}>'
    return typename


with open('packet.json') as jsonFile:
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
            ',\n'.join(str('    ' + value) for value in packetIdList),
            ''.join(classList)
        )
        ext = 'hpp'

    outputFile = open(f'{args.output}/packet.{ext}', 'w')
    outputFile.write(output)
