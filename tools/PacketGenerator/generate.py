import itertools
import os
import json
import argparse
import shutil
import stringcase
from dataclasses import dataclass, field


@dataclass
class Format:
    file: str = None
    handler: str = None
    classFormat: str = None
    enumFormat: str = None


cppFormat = Format()
cppFormat.file = '''#pragma once
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
{0}
namespace gen {{
namespace {1} {{
    {2}
}}
}}
#pragma warning(pop)
'''

cppFormat.handler = '''#pragma once
#include "Packet.gen.hpp"
{0}
                         
#ifdef __UNREAL__
#include "winsock2.h"
#include "Network/Packet.h"

using Session = class FSession;

#define STATIC_POINTER_CAST(to, from) StaticCastSharedPtr<to>(from)

#else

template<typename T>
using TSharedPtr = std::shared_ptr<T>;
template<typename T>
using TFunction = std::function<T>;

#define STATIC_POINTER_CAST(to, from) std::static_pointer_cast<to>(from)

class Session;
#endif

#define BIND_HANDLER(pckname, buffer) std::bind(pckname##PacketHandler, std::placeholders::_1, STATIC_POINTER_CAST(pckname, Packet::ParseFrom<pckname>(buffer)));
namespace gen
{{
namespace {1}
{{
    class PacketHandler
	{{
    	using Handler = TFunction<bool(TSharedPtr<Session>)>;
	public:
		static Handler getHandler(std::span<char> buffer)
        {{
            PacketId id = PacketId::NONE;
			std::memcpy(&id, buffer.data(), sizeof(unsigned short));
			id = (PacketId)ntohs((u_short)id);
            
            switch (id)
            {{
            case PacketId::NONE:
                break;
{2}
            default:
                break;
            }}
            return nullptr;
        }}
        static bool handlePacket(TSharedPtr<Session> session, std::span<char> buffer)
        {{
            auto handler = getHandler(buffer);
            if (!handler || !session)
                return false;
            return handler(session);
        }}
{3}
	}};
}}
}}
'''

cppFormat.classFormat = '''class {0}
            : public Packet {{
    public:
        {0}() : Packet(static_cast<unsigned short>(PacketId::{1})) {{
        }}
        ~{0}() {{
    
        }}
    protected:
        virtual void Read() override
        {{
            Packet::Read();
            {2}
        }}
        virtual void Write() override
        {{
            {3}
            Finish();
        }}
    public:
        {4}
    }};
    
    inline Packet& operator>>(Packet& pk, {5}) {{
        {6}
        return pk;
    }}

    inline Packet& operator<<(Packet& pk, const {5}) {{
        {7}
        return pk;
    }}
'''

cppFormat.enumFormat = """enum {0} : uint16 {{
        {1}
    }};"""
                             
csharpFormat = Format()
csharpFormat.file = """using System;
using System.Collections.Generic;


/*
    TODO: Copy to your project.
*/

namespace {0}
{{
    public enum PacketId : ushort
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

def readCpp(readType):
    inElem = [[], []]
    for value in dataList:
        if value.type[0] == 'E':
            inElem[0].append(f'({value.name})')
            inElem[1].append(f'({stringcase.camelcase(messageName)}.{value.name})')
        else:
            inElem[0].append(value.name)
            inElem[1].append(f'{stringcase.camelcase(messageName)}.{value.name}')
            
    read_class = ' >> '.join(map(lambda x: 'reinterpret_cast<uint16&>'+x if x[0] == '(' else x, inElem[0]))
    write_line = ' << '.join(value for value in inElem[0])

    read_op = ' >> '.join(map(lambda x: 'reinterpret_cast<uint16&>'+x if x[0] == '(' else x, inElem[1]))
    write_op = ' << '.join(value for value in inElem[1])
    
    if read_class != '' or write_line != '':
        read_class = '*this >> ' + read_class + ';'
        write_line = '*this << ' + write_line + ';'
        read_op = f'pk >> {read_op};'
        write_op = f'pk << {write_op};'

    idname = stringcase.constcase(messageName)
    if readType == 'struct':
        idname = 'NONE'

    return cppFormat.classFormat.format(
        (messageName),
        idname,
        read_class,
        write_line,
        '\t'.join('{} {};\n\t'.format(value.type, value.name) for value in dataList),
        f'{(messageName)}& {stringcase.camelcase(messageName)}',
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
                {elem.name}.Add(BitConverter.To{(listTy)}(m_buffer, m_readOffset));\n\
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
            convType = (elem.type)
            readLine += f'{elem.name} = BitConverter.To{convType}(m_buffer, m_readOffset);\
            \n\t\t\tm_readOffset += sizeof({elem.type});\n'
            writeLine += f'BitConverter.TryWriteBytes(new Span<byte>(m_buffer, m_writeOffset, m_buffer.Length), {elem.name});\
            \n\t\t\tm_writeOffset += sizeof({elem.type});\n'

    return csharpFormat.classFormat.format(
        (messageName), # class name
        '\n\t\t'.join(f'private {value.type} {value.name};' for value in dataList), # element declaration
        stringcase.constcase(messageName), # packet id bind
        readLine, # data read
        writeLine # data write
    )


def getLangTypename(typename):
    if typename == 'string':
        if args.lang == 'cpp':
            return 'String'
    if typename.find('list') != -1:
        val = typename.split('|')
        if args.lang == 'cpp':
            return f'std::vector<{getLangTypename(val[1])}>'
        if args.lang == 'csharp':
            return f'List<{getLangTypename(val[1])}>'
    if args.lang == 'csharp':
        if typename == 'bool':
            typename = 'Boolean'

    return typename

def DeleteAllFiles(filePath):
    if os.path.exists(filePath):
        for file in os.scandir(filePath):
            os.remove(file.path)
        return 'Remove All File'
    else:
        return 'Directory Not Found'


parser = argparse.ArgumentParser()
parser.add_argument('-l', '--language', default='cpp', action='store', dest='lang', help='generated file language')
parser.add_argument('-p', '--path', default='./define', action='store', dest='path', help='define directory path')
parser.add_argument('-n', '--namespace', default='gen', action='store', dest='namespace', help='namespace name')
args = parser.parse_args()

if os.path.exists(f'generated/{args.namespace}') != True:
    os.mkdir(f'generated/{args.namespace}')

ext = ''
outputHandler = ['', '']

messageNameList = [[],[],[]] # Client, Server, Struct
bindList = [[],[]]
handlerList = [[],[]]

defList = os.listdir(args.path)
for filename in defList:
    with open(args.path + filename) as jsonFile:
        classList= []
        enumClassList= []
        data = json.load(jsonFile)
        
        messageList = []
        structList = []
        enumList = []
        includeList = []
        if 'message' in data:
            messageList = data['message']
        if 'struct' in data:
            structList = data['struct']
        if 'enum' in data:
            enumList = data['enum']
        if 'include' in data:
            includeList = data['include']

        outputFile = ''

        if len(enumList) > 0:
            for enum in enumList:
                if 'name' in enum:
                    enumClassList.append(cppFormat.enumFormat.format(enum['name'], ',\n\t\t'.join(enum['list'])))
        
        if len(structList) > 0:
            for struct in structList:
                messageName = struct['name']
                data = struct['data']
                dataList = []

                # message data
                for element in data:
                    elem = Element()
                    elem.name = element['name']
                    elem.type = getLangTypename(element['type'])

                    dataList.append(elem)

                # Add class list.
                if args.lang == 'cpp':
                    classList.append(readCpp('struct'))
                elif args.lang == 'csharp':
                    classList.append(readCsharp(bindList, handlerList))
        # defined message list
        if len(messageList) > 0:
            for message in messageList['client']:
                messageName = message['name']
                messageNameList[0].append(messageName)
                
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
                    classList.append(readCpp('message'))
                elif args.lang == 'csharp':
                    classList.append(readCsharp(bindList, handlerList))
            for message in messageList['server']:
                messageName = message['name']
                messageNameList[1].append(messageName)
                
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
                    classList.append(readCpp('message'))
                elif args.lang == 'csharp':
                    classList.append(readCsharp(bindList, handlerList))

        # formatting data
        if args.lang == 'cpp':
            outputFile = cppFormat.file.format(
                '\n'.join(f'#include "{(inc.rstrip(".json"))}.gen.hpp"' for inc in includeList),
                args.namespace.lower(), #namespace
                '\n\t'.join(enumClassList + classList), #packet classes
                )
            ext = 'hpp'
        if args.lang == 'csharp':
            outputFile = csharpFormat.file.format(
                (args.namespace),
                ',\n'.join(str(f'\t\t{stringcase.constcase(value)} = {messageNameList.index(value)+1}') for value in messageNameList),
                '\n\t'.join(classList)
            )
            ext = 'cs'

        # write packet & handler file
        genPacket = open(f'generated/{args.namespace}/{(filename.rstrip(".json"))}.gen.{ext}', 'w')
        genPacket.write(outputFile)

if args.lang == 'cpp':
    for i in range(2):
        for classes in messageNameList[i]:
            handlerName = f'{(classes)}PacketHandler'
            handlerList[i].append(handlerName)
    for i in range(2):
        outputHandler[i] = cppFormat.handler.format(
            '\n'.join(f'#include "generated/{args.namespace}/{(value.rstrip(".json"))}.gen.hpp"' for value in defList),
            args.namespace,
            '\n'.join(f'\t\t\tcase {stringcase.constcase(value)}:\n\t\t\t\treturn BIND_HANDLER({value}, buffer);' for value in messageNameList[i]),
            '\n'.join(str('\t\tstatic bool '+value+f'(TSharedPtr<Session> session, TSharedPtr<{(messageNameList[i][handlerList[i].index(value)])}> packet);') for value in handlerList[i]) #handlers
        )
    

types = open(f'generated/{args.namespace}/Packet.gen.{ext}', 'w')
if args.lang == 'cpp':
    allMessageList = list(itertools.chain(*messageNameList))
    types.write('#pragma once\n\n\
template<class T> inline T& unmove(T&& t) {{ return static_cast<T&>(t); }}\n\n\
namespace gen {{\n\
namespace {0} {{\n\
    enum PacketId : uint16 {{\n\
        NONE = 0,\n\
{1}\
    \n\t}};\n\
\n}}\n}}'.format(args.namespace, ',\n'.join(str(f'\t\t{stringcase.constcase(value)} = {allMessageList.index(value)+1}') for value in allMessageList)))

open(f'generated/{args.namespace}/ServerPacketHandler.gen.{ext}', 'w').write(outputHandler[0])
open(f'generated/{args.namespace}/ClientPacketHandler.gen.{ext}', 'w').write(outputHandler[1])