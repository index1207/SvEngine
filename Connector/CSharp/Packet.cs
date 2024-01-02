using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace Sv
{
    class Packet
    {
        private ushort m_id;
        private ushort m_size;
        private ushort m_offset = 0;

        private byte[] m_buffer;
        public Packet(ushort id)
        {
            m_buffer = new byte[1024];

            BitConverter.TryWriteBytes(new Span<byte>(m_buffer, 0, sizeof(ushort)), id);
            m_offset += sizeof(ushort) + sizeof(ushort); // id + size
        }

        public void Finish()
        {
            BitConverter.TryWriteBytes(new Span<byte>(m_buffer, 2, sizeof(ushort)), m_size);
        }

        public void Append(byte data)
        {
            BitConverter.TryWriteBytes(new Span<byte>(m_buffer, m_offset, Marshal.SizeOf<byte>()), data);
            m_offset += sizeof(ushort);
        }

        public void Append(sbyte data)
        {
            BitConverter.TryWriteBytes(new Span<byte>(m_buffer, m_offset, Marshal.SizeOf<sbyte>()), data);
            m_offset += sizeof(sbyte);
        }

        public void Append(short data)
        {
            BitConverter.TryWriteBytes(new Span<byte>(m_buffer, m_offset, Marshal.SizeOf<short>()), data);
            m_offset += sizeof(short);
        }

        public void Append(ushort data)
        {
            BitConverter.TryWriteBytes(new Span<byte>(m_buffer, m_offset, Marshal.SizeOf<ushort>()), data);
            m_offset += sizeof(ushort);
        }

        public void Append(int data)
        {
            BitConverter.TryWriteBytes(new Span<byte>(m_buffer, m_offset, Marshal.SizeOf<int>()), data);
            m_offset += sizeof(int);
        }

        public void Append(uint data)
        {
            BitConverter.TryWriteBytes(new Span<byte>(m_buffer, m_offset, Marshal.SizeOf<uint>()), data);
            m_offset += sizeof(uint);
        }

        public void Append(long data)
        {
            BitConverter.TryWriteBytes(new Span<byte>(m_buffer, m_offset, Marshal.SizeOf<long>()), data);
            m_offset += sizeof(long);
        }

        public void Append(ulong data)
        {
            BitConverter.TryWriteBytes(new Span<byte>(m_buffer, m_offset, Marshal.SizeOf<ulong>()), data);
            m_offset += sizeof(ulong);
        }

        public void Append(float data)
        {
            BitConverter.TryWriteBytes(new Span<byte>(m_buffer, m_offset, Marshal.SizeOf<float>()), data);
            m_offset += sizeof(float);
        }

        public void Append(double data)
        {
            BitConverter.TryWriteBytes(new Span<byte>(m_buffer, m_offset, Marshal.SizeOf<double>()), data);
            m_offset += sizeof(double);
        }
    }
}
