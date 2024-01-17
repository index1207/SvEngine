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
        private ushort m_writeOffset = 0;
        private ushort m_readOffset = 0;

        private byte[] m_buffer;

        public ushort Id => m_id;
        public ushort Size => m_size;

        public Packet(ushort id)
        {
            m_buffer = new byte[1024];

            if (BitConverter.TryWriteBytes(new Span<byte>(m_buffer, 0, sizeof(ushort)), id))
                Console.WriteLine("SUCCESS");
            m_writeOffset += sizeof(ushort) + sizeof(ushort); // id + size
        }

        public void Finish()
        {
            BitConverter.TryWriteBytes(new Span<byte>(m_buffer, 2, sizeof(ushort)), m_writeOffset);
        }
            
        virtual protected void Write() { }
        virtual protected void Read()
        {
            m_id = GetUint16();
            m_size = GetUint16();
        }

        #region Append
        public void Append(byte data)
        {
            BitConverter.TryWriteBytes(new Span<byte>(m_buffer, m_writeOffset, Marshal.SizeOf<byte>()), data);
            m_writeOffset += sizeof(ushort);
        }

        public void Append(sbyte data)
        {
            BitConverter.TryWriteBytes(new Span<byte>(m_buffer, m_writeOffset, Marshal.SizeOf<sbyte>()), data);
            m_writeOffset += sizeof(sbyte);
        }

        public void Append(short data)
        {
            BitConverter.TryWriteBytes(new Span<byte>(m_buffer, m_writeOffset, Marshal.SizeOf<short>()), data);
            m_writeOffset += sizeof(short);
        }

        public void Append(ushort data)
        {
            BitConverter.TryWriteBytes(new Span<byte>(m_buffer, m_writeOffset, Marshal.SizeOf<ushort>()), data);
            m_writeOffset += sizeof(ushort);
        }

        public void Append(int data)
        {
            BitConverter.TryWriteBytes(new Span<byte>(m_buffer, m_writeOffset, Marshal.SizeOf<int>()), data);
            m_writeOffset += sizeof(int);
        }

        public void Append(uint data)
        {
            BitConverter.TryWriteBytes(new Span<byte>(m_buffer, m_writeOffset, Marshal.SizeOf<uint>()), data);
            m_writeOffset += sizeof(uint);
        }

        public void Append(long data)
        {
            BitConverter.TryWriteBytes(new Span<byte>(m_buffer, m_writeOffset, Marshal.SizeOf<long>()), data);
            m_writeOffset += sizeof(long);
        }

        public void Append(ulong data)
        {
            BitConverter.TryWriteBytes(new Span<byte>(m_buffer, m_writeOffset, Marshal.SizeOf<ulong>()), data);
            m_writeOffset += sizeof(ulong);
        }

        public void Append(float data)
        {
            BitConverter.TryWriteBytes(new Span<byte>(m_buffer, m_writeOffset, Marshal.SizeOf<float>()), data);
            m_writeOffset += sizeof(float);
        }

        public void Append(double data)
        {
            BitConverter.TryWriteBytes(new Span<byte>(m_buffer, m_writeOffset, Marshal.SizeOf<double>()), data);
            m_writeOffset += sizeof(double);
        }
        #endregion

        #region Get data

        public byte GetUint8()
        {
            return m_buffer[m_readOffset++];
        }

        public sbyte GetInt8()
        {
            return (sbyte)m_buffer[m_readOffset++];
        }

        public ushort GetUint16()
        {
            ushort data = BitConverter.ToUInt16(new Span<byte>(m_buffer, m_readOffset, sizeof(ushort)));
            m_readOffset += sizeof(ushort);
            return data;
        }

        public short Getint16()
        {
            short data = BitConverter.ToInt16(new Span<byte>(m_buffer, m_readOffset, sizeof(short)));
            m_readOffset += sizeof(short);
            return data;
        }

        public uint GetUint32()
        {
            uint data = BitConverter.ToUInt16(new Span<byte>(m_buffer, m_readOffset, sizeof(uint)));
            m_readOffset += sizeof(uint);
            return data;
        }

        public int GetInt32()
        {
            int data = BitConverter.ToUInt16(new Span<byte>(m_buffer, m_readOffset, sizeof(int)));
            m_readOffset += sizeof(int);
            return data;
        }

        public ulong GetUint64()
        {
            ulong data = BitConverter.ToUInt16(new Span<byte>(m_buffer, m_readOffset, sizeof(ulong)));
            m_readOffset += sizeof(ulong);
            return data;
        }

        public long GetInt64()
        {
            long data = BitConverter.ToUInt16(new Span<byte>(m_buffer, m_readOffset, sizeof(long)));
            m_readOffset += sizeof(long);
            return data;
        }
        #endregion
    }
}
