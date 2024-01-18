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
		LOGIN_RES = 4
    }

    class EnterGameReq : Sv.Packet
    {
        private int32 playerId;
        
        public EnterGameReq() : base((ushort)PacketId.ENTER_GAME_REQ) {}
        
        protected override void Read()
        {
            base.Read();
            playerId = BitConverter.ToInt32(m_buffer, m_readOffset);            
			m_readOffset += sizeof(int32);

        }
        
        protected override void Write()
        {
            base.Write();
            BitConverter.TryWriteBytes(new Span<byte>(m_buffer, m_writeOffset, m_buffer.Length), playerId);            
			m_writeOffset += sizeof(int32);

        }
    }

	class EnterGameRes : Sv.Packet
    {
        private List<int32> playerList;
        
        public EnterGameRes() : base((ushort)PacketId.ENTER_GAME_RES) {}
        
        protected override void Read()
        {
            base.Read();
            ushort playerListLen = BitConverter.ToUInt16(m_buffer, m_readOffset);            
			m_readOffset += sizeof(ushort);            
			for (ushort i = 0; i < playerListLen; ++i) {
                playerList.Add(BitConverter.ToInt32(m_buffer, m_readOffset));
                m_readOffset += sizeof(int32);
            }
        }
        
        protected override void Write()
        {
            base.Write();
            
        }
    }

	class LoginReq : Sv.Packet
    {
        private string usrId;
		private string usrPwd;
        
        public LoginReq() : base((ushort)PacketId.LOGIN_REQ) {}
        
        protected override void Read()
        {
            base.Read();
            usrId = BitConverter.ToString(m_buffer, m_readOffset);            
			m_readOffset += sizeof(string);
usrPwd = BitConverter.ToString(m_buffer, m_readOffset);            
			m_readOffset += sizeof(string);

        }
        
        protected override void Write()
        {
            base.Write();
            BitConverter.TryWriteBytes(new Span<byte>(m_buffer, m_writeOffset, m_buffer.Length), usrId);            
			m_writeOffset += sizeof(string);
BitConverter.TryWriteBytes(new Span<byte>(m_buffer, m_writeOffset, m_buffer.Length), usrPwd);            
			m_writeOffset += sizeof(string);

        }
    }

	class LoginRes : Sv.Packet
    {
        private Boolean isSuccess;
        
        public LoginRes() : base((ushort)PacketId.LOGIN_RES) {}
        
        protected override void Read()
        {
            base.Read();
            isSuccess = BitConverter.ToBoolean(m_buffer, m_readOffset);            
			m_readOffset += sizeof(Boolean);

        }
        
        protected override void Write()
        {
            base.Write();
            BitConverter.TryWriteBytes(new Span<byte>(m_buffer, m_writeOffset, m_buffer.Length), isSuccess);            
			m_writeOffset += sizeof(Boolean);

        }
    }

}
