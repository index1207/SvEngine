using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace Sv
{
    public class Session
    {
        private Socket _socket;
        private byte[] _buffer = new byte[1024];
        int _isDisconnected;
        private SocketAsyncEventArgs _recvEvent = new SocketAsyncEventArgs();

        public Session(Socket socket)
        {
            _socket = socket;
            _isDisconnected = 0;
        }

        public void Run()
        { 
            _recvEvent.Completed += (object sender, SocketAsyncEventArgs recvEvent) =>
            {
                if (recvEvent.SocketError == SocketError.Success)
                {
                    OnReceive(new Span<byte>(_recvEvent.Buffer, 0, _recvEvent.Buffer.Length));
                    _socket.ReceiveAsync(_recvEvent);
                }
                else
                {
                    OnDisconnected();
                }
            };
            _recvEvent.SetBuffer(_buffer);

            _socket.ReceiveAsync(_recvEvent);
        }

        public void Send(Span<byte> data)
        {
            _socket.Send(data);
        }

        public void Disconnect()
        {
            if (Interlocked.Exchange(ref _isDisconnected, 1) == 0)
            {
                _socket.Disconnect(false);
                OnDisconnected();
            }
        }

        public virtual void OnConnected() { }
        public virtual void OnDisconnected() { }
        public virtual void OnReceive(Span<byte> data) { }
    }
}
