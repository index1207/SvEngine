using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace DummyClient
{
    class TestSession : Sv.Session
    {
        public TestSession(Socket sock) : base(sock)
        {
        }

        public override void OnConnected()
        {
            base.OnConnected();
            Console.WriteLine("Connected to server.");

            Send(Encoding.UTF8.GetBytes("HELLO"));
        }

        public override void OnDisconnected()
        {
            base.OnDisconnected();
            Console.WriteLine("Disconnected.");
        }

        public override void OnReceive(Span<byte> data)
        {
            base.OnReceive(data);
            Console.WriteLine(Encoding.UTF8.GetString(data));
        }
    }
    class Program
    {
        static void Main(string[] args)
        {
            Sv.Connector connector = new Sv.Connector();
            connector.SessionFactory += (sock) =>
            {
                return new TestSession(sock);
            };
            connector.Connect(new IPEndPoint(IPAddress.Loopback, 9999));

            Console.ReadLine();
        }
    }
}
