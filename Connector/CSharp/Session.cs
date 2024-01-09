using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace Sv
{
    public class Session
    {
        private Socket _socket;
        private byte[] _buffer = new byte[1024];
        private SocketAsyncEventArgs _recvEvent = new SocketAsyncEventArgs();

        public void Run(Socket sock)
        {
            _socket = sock;
 
            _recvEvent.Completed += OnRecvCompleted;
            _recvEvent.SetBuffer(_buffer);

            _socket.ReceiveAsync(_recvEvent);
        }

        public void Send(Span<byte> data)
        {
            _socket.Send(data);
        }

        public virtual void OnConnected() { }
        public virtual void OnDisconnected() { }
        public virtual void OnReceive(Span<byte> data) { }

        private void OnRecvCompleted(object sender, SocketAsyncEventArgs recvEvent)
        {
            if(recvEvent.SocketError == SocketError.Success)
            {
                OnReceive(new Span<byte>(_recvEvent.Buffer, 0, _recvEvent.Count));
            }
            _socket.ReceiveAsync(_recvEvent);
        }
    }
}
