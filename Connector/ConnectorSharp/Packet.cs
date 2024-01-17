using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace Sv
{
    class Packet
    {
        private ushort m_id = 0;
        private ushort m_size = 0;

        protected ushort m_writeOffset = 0;
        protected ushort m_readOffset = 0;
        protected byte[] m_buffer;

        public ushort Id => m_id;
        public ushort Size => m_size;

        public Packet(ushort id)
        {
            m_buffer = new byte[1024];

            BitConverter.TryWriteBytes(new Span<byte>(m_buffer, 0, sizeof(ushort)), id);
            m_writeOffset += sizeof(ushort) + sizeof(ushort); // id + size
        }

        public void Finish()
        {
            BitConverter.TryWriteBytes(new Span<byte>(m_buffer, 2, sizeof(ushort)), m_writeOffset);
        }
            
        virtual protected void Write() { }
        virtual protected void Read()
        {
            m_id = BitConverter.ToUInt16(m_buffer, m_readOffset);
            m_readOffset += sizeof(ushort);
            m_size = BitConverter.ToUInt16(m_buffer, m_readOffset);
            m_readOffset += sizeof(ushort);
        }
    }
}
