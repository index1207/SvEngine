import itertools
import os
import json
import argparse
from re import I
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
#include <generated/Packet.gen.hpp>

#include <core/Packet.hpp>
#include <util/Types.hpp>

#include <vector>

/* Additional generated packet headers. */
{0}

namespace {1} {{
    {2}
}}
#pragma warning(pop)
'''

cppFormat.handler = '''#pragma once
#include <generated/Packet.gen.hpp>
#include <core/Packet.hpp>
{0}

namespace sv {{ class Session; }}

namespace {1}
{{
    class PacketHandler
	{{
	public:
		static void onReceivePacket(sv::Session* session, PacketId id, sv::Packet* packet)
        {{
	        switch (id)
	        {{
{2}
	        }}
        }}
	private:
{3}
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
csharpFormat.file = """using System;
using System.Collections.Generic;


/*
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

csharpFormat.classFormat = """class {0} : Sv.Packet
    {{
        {1}
        
        public {0}() : base((ushort)PacketId.{2}) {{}}
        
        protected override void Read()
        {{
            base.Read();
            {3}
        }}
        
        protected override void Write()
        {{
            base.Write();
            {4}
        }}
    }}
"""


@dataclass
class Element:
    type: str = None
    name: str = None

def readCpp(conditionList, handlerList):
    read_class = ' >> '.join(value.name for value in dataList)
    write_line = ' << '.join(value.name for value in dataList)
    if read_class == '' or write_line == '':
        raise Exception("Packet element can't be null.")

    read_class = '*this >> ' + read_class + ';'
    write_line = '*this << ' + write_line + ';'

    read_op = ' >> '.join(f'{stringcase.camelcase(messageName)}.{value.name}' for value in dataList)
    write_op = ' << '.join(f'{stringcase.camelcase(messageName)}.{value.name}' for value in dataList)
    read_op = f'pk >> {read_op};'
    write_op = f'pk << {write_op};'

    return cppFormat.classFormat.format(
        stringcase.pascalcase(messageName),
        stringcase.constcase(messageName),
        read_class,
        write_line,
        '\t'.join('{} {};\n\t'.format(value.type, value.name) for value in dataList),
        f'{stringcase.pascalcase(messageName)}& {stringcase.camelcase(messageName)}',
        read_op,
        write_op
    )

def readCsharp(conditionList, handlerList):
    readLine = ''
    writeLine = ''
    for elem in dataList:
        if elem.type.find('List') != -1:
            listTy = elem.type[elem.type.find("<")+1:elem.type.find(">")]
            readLine += f'ushort {elem.name}Len = BitConverter.ToUInt16(m_buffer, m_readOffset);\
            \n\t\t\tm_readOffset += sizeof(ushort);\
            \n\t\t\tfor (ushort i = 0; i < {elem.name}Len; ++i) {{\n\
                {elem.name}.Add(BitConverter.To{stringcase.pascalcase(listTy)}(m_buffer, m_readOffset));\n\
                m_readOffset += sizeof({listTy});\n\
            }}'
            # writeLine += f'BitConverter.TryWriteBytes(new Span<byte>(m_buffer, m_writeOffset, m_buffer.Length), {elem.name});\
            # \n\t\t\tm_writeOffset += sizeof({elem.type});\n'
        # elif elem.type == 'String':
            # readLine += f'{elem.name} = BitConverter.To{convType}(m_buffer, m_readOffset);\
            # \n\t\t\tm_readOffset += sizeof({elem.type});\n'
            # writeLine += f'BitConverter.TryWriteBytes(new Span<byte>(m_buffer, m_writeOffset, m_buffer.Length), {elem.name});\
            # \n\t\t\tm_writeOffset += sizeof({elem.type});\n'
        else:
            convType = stringcase.pascalcase(elem.type)
            readLine += f'{elem.name} = BitConverter.To{convType}(m_buffer, m_readOffset);\
            \n\t\t\tm_readOffset += sizeof({elem.type});\n'
            writeLine += f'BitConverter.TryWriteBytes(new Span<byte>(m_buffer, m_writeOffset, m_buffer.Length), {elem.name});\
            \n\t\t\tm_writeOffset += sizeof({elem.type});\n'

    return csharpFormat.classFormat.format(
        stringcase.pascalcase(messageName), # class name
        '\n\t\t'.join(f'private {value.type} {value.name};' for value in dataList), # element declaration
        stringcase.constcase(messageName), # packet id bind
        readLine, # data read
        writeLine # data write
    )


def getLangTypename(typename):
    if typename == 'string':
        if args.lang == 'cpp':
            return 'std::string'
    if typename.find('list') != -1:
        val = typename.split('|')
        if args.lang == 'cpp':
            return f'std::vector<{getLangTypename(val[1])}>'
        if args.lang == 'csharp':
            return f'List<{getLangTypename(val[1])}>'
    if args.lang == 'csharp':
        if typename == 'bool':
            typename = 'Boolean'
    else:
        typename = stringcase.pascalcase(typename)
    return typename




parser = argparse.ArgumentParser()
parser.add_argument('-l', '--language', default='cpp', action='store', dest='lang', help='generated file language')
parser.add_argument('-n', '--namespace', default='gen', action='store', dest='namespace', help='namespace name')
args = parser.parse_args()


ext = ''
outputHandler = ['','']

messageNameList = [[],[]]
conditionList = [[],[]]
handlerList = [[],[]]

defList = os.listdir("define/")
for filename in defList:
    classList= []
    with open("define/"+filename) as jsonFile:
        data = json.load(jsonFile)
        
        messageList = data['message']
        
        includeList = []
        if len(data) > 1:
            includeList = data['include']

        outputPacket = ''

        # defined message list
        for message in messageList:
            messageName = ''
            if "client" in messageList[messageList.index(message)]:
                messageName = message['client']
                messageNameList[0].append(stringcase.pascalcase(messageName))
            else:
               messageName = message['server']
               messageNameList[1].append(stringcase.pascalcase(messageName))
            
            data = message['data']
            dataList = []

            # message data
            for element in data:
                elem = Element()
                elem.name = element['name']
                elem.type = getLangTypename(element['type'])

                dataList.append(elem)

            # Add class list.
            if args.lang == 'cpp':
                classList.append(readCpp(conditionList, handlerList))
            elif args.lang == 'csharp':
                classList.append(readCsharp(conditionList, handlerList))

        # formatting data
        if args.lang == 'cpp':
            outputPacket = cppFormat.file.format(
                '\n'.join(f'#include "./{stringcase.pascalcase(inc.rstrip(".json"))}.gen.hpp"' for inc in includeList),
                args.namespace.lower(), #namespace
                '\n\t'.join(classList), #packet classes
                )
            ext = 'hpp'
        if args.lang == 'csharp':
            outputPacket = csharpFormat.file.format(
                stringcase.pascalcase(args.namespace),
                ',\n'.join(str(f'\t\t{stringcase.constcase(value)} = {messageNameList.index(value)+1}') for value in messageNameList),
                '\n\t'.join(classList)
            )
            ext = 'cs'

        # write packet & handler file
        genPacket = open(f'generated/{stringcase.pascalcase(filename.rstrip(".json"))}.gen.{ext}', 'w')
        genPacket.write(outputPacket)

if args.lang == 'cpp':
    for i in range(2):
        for classes in messageNameList[i]:
            handlerName = f'{stringcase.pascalcase(classes)}PacketHandler'
            handlerList[i].append(handlerName)
            conditionList[i].append(f'\t\t\tcase PacketId::{stringcase.constcase(classes)}:\n'
                                    + f'\t\t\t\t{handlerName}(session, static_cast<{stringcase.pascalcase(classes)}*>(packet));\n'
                                    + '\t\t\t\tbreak;'
            )
    for i in range(2):
        outputHandler[i] = cppFormat.handler.format(
            '\n'.join(f'#include <generated/{stringcase.pascalcase(value.rstrip(".json"))}.gen.hpp>' for value in defList),
            args.namespace,
            '\n'.join(str(value) for value in conditionList[i]),    #dispatches
            '\n'.join(str('\t\tstatic void '+value+f'(sv::Session* session, {stringcase.pascalcase(messageNameList[i][handlerList[i].index(value)])}* packet);') for value in handlerList[i]) #handlers
        )
    

types = open(f'generated/Packet.gen.{ext}', 'w')
if args.lang == 'cpp':
    allMessageList = list(itertools.chain(*messageNameList))
    types.write('#pragma once\n\n\
namespace {0} {{\n\
    enum class PacketId {{\n\
{1}\
    \n\t}};\n\
\n}}'.format(args.namespace, ',\n'.join(str(f'\t\t{stringcase.constcase(value)} = {allMessageList.index(value)+1}') for value in allMessageList)))

open(f'generated/ServerPacketHandler.gen.{ext}', 'w').write(outputHandler[0])
open(f'generated/ClientPacketHandler.gen.{ext}', 'w').write(outputHandler[1])