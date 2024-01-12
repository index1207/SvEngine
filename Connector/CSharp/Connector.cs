using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace Sv
{
    public class Connector
    {
        private Socket m_socket;
        public Func<Socket, Session> SessionFactory { get; set; }

        public Connector()
        {
            m_socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        }

        public void Connect(IPEndPoint endpoint)
        {
            SocketAsyncEventArgs args = new SocketAsyncEventArgs();
            args.RemoteEndPoint = endpoint;
            args.Completed += (object sender, SocketAsyncEventArgs connectArgs) =>
            {
                if(connectArgs.SocketError == SocketError.Success)
                {
                    Session session = SessionFactory(m_socket);
                    session.OnConnected();
                    session.Run();
                }
            };
            m_socket.ConnectAsync(args);
        }
    }
}
