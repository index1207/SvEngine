using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace Sv
{
    public enum FailCause
    {
        Connect
    }
    public class Connector
    {
        private Socket m_socket;
        public Action<EndPoint> OnConnect { get; set; } = (_) => { };
        public Action OnDisconnect { get; set; } = () => { };
        public Action<Message> OnReceive { get; set; } = (_) => { };
        public Action<FailCause> OnFail { get; set; } = (_) => { };

        int _isDisconnected;
        private SocketAsyncEventArgs _recvEvent;

        object _lock = new object();

        public Connector()
        {
            m_socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

            _recvEvent = new SocketAsyncEventArgs();
            _recvEvent.Completed += (object sender, SocketAsyncEventArgs args) =>
            {
                if(args.SocketError == SocketError.Success)
                {
                    Message msg = new Message();
                    msg.RawData = args.Buffer;
                    msg.Length = args.BytesTransferred;
                    OnReceive(msg);

                    m_socket.ReceiveAsync(args);
                }
                else
                {
                    OnDisconnect();
                }
            };
            _recvEvent.SetBuffer(new byte[1024], 0, 1024);

            OnConnect += (_) => m_socket.ReceiveAsync(_recvEvent);

            _isDisconnected = 0;
        }

        public void Connect(IPEndPoint endpoint)
        {
            SocketAsyncEventArgs args = new SocketAsyncEventArgs();
            args.RemoteEndPoint = endpoint;
            args.Completed += (object sender, SocketAsyncEventArgs connectArgs) =>
            {
                if(connectArgs.SocketError == SocketError.Success)
                {
                    OnConnect(connectArgs.RemoteEndPoint);
                }
                else
                {
                    OnFail(FailCause.Connect);
                }
            };
            m_socket.ConnectAsync(args);
        }

        public void Send(byte[] buffer)
        {
            lock (_lock)
            {
                m_socket.Send(buffer);
            }
        }

        public void Disconnect()
        {
            if (Interlocked.Exchange(ref _isDisconnected, 1) == 0)
            {
                m_socket.Disconnect(false);
            }
        }
    }
}
