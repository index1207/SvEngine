using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

using Sv;

namespace DummyClient
{
    class Program
    {
        static void Main(string[] args)
        {
            var sock = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            sock.Connect(new IPEndPoint(IPAddress.Loopback, 9999));

            byte[] buffer = new byte[128];
            BitConverter.TryWriteBytes(new Span<byte>(buffer, 0, 2), (ushort)1);
            BitConverter.TryWriteBytes(new Span<byte>(buffer, 2, 4), (ushort)1);
            while (true)
            {
                sock.Send(new ArraySegment<byte>(buffer, 0, 4));
                Thread.Sleep(500);
            }
        }
    }
}
