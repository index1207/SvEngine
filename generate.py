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

enum PacketId {{
{0}
}};

{1}
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
args = parser.parse_args()

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
            elem.type = element['type']
            if elem.type == 'string':
                if args.lang == 'cpp':
                    elem.type = 'std::string'
            if elem.type.find('list') != -1:
                if args.lang == 'cpp':
                    t = elem.type.split('|')
                    elem.type = f'std::vector<{t[1]}>'
            elementList.append(elem)

        if args.lang == 'cpp':
            classList.append(readCppClass())
    if args.lang == 'cpp':
        output = cppFormat.file.format(
            ',\n'.join(str('    ' + value) for value in packetIdList),
            ''.join(classList)
        )
        ext = 'hpp'

    outputFile = open(f'{args.output}/packet.{ext}', 'w')
    outputFile.write(output)
