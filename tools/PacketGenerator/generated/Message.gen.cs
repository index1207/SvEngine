using System;
using System.Collections.Generic;


/*
    TODO: Copy to your project.
*/

namespace Gen
{
    public enum PacketId
    {
		ENTER_GAME_REQ = 1,
		ENTER_GAME_RES = 2,
		LOGIN_REQ = 3,
		LOGIN_RES = 4,
		MESSAGE = 5,
		SEND_MESSAGE_REQ = 6
    }

    class Message : Sv.Packet
    {
        private string data;
        
        public Message() : base((ushort)PacketId.MESSAGE) {}
        
        protected override void Read()
        {
            base.Read();
            data = BitConverter.ToString(m_buffer, m_readOffset);            
			m_readOffset += sizeof(string);

        }
        
        protected override void Write()
        {
            base.Write();
            BitConverter.TryWriteBytes(new Span<byte>(m_buffer, m_writeOffset, m_buffer.Length), data);            
			m_writeOffset += sizeof(string);

        }
    }

	class SendMessageReq : Sv.Packet
    {
        private List<message> msg;
        
        public SendMessageReq() : base((ushort)PacketId.SEND_MESSAGE_REQ) {}
        
        protected override void Read()
        {
            base.Read();
            ushort msgLen = BitConverter.ToUInt16(m_buffer, m_readOffset);            
			m_readOffset += sizeof(ushort);            
			for (ushort i = 0; i < msgLen; ++i) {
                msg.Add(BitConverter.ToMessage(m_buffer, m_readOffset));
                m_readOffset += sizeof(message);
            }
        }
        
        protected override void Write()
        {
            base.Write();
            
        }
    }

}
