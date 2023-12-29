using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace DummyClient
{
    class Program
    {
        static void Main(string[] args)
        {
            var sock = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            sock.Connect(new IPEndPoint(IPAddress.Loopback, 9999));

            string data = "HELLO";
            while(true)
            {
                sock.Send(Encoding.UTF8.GetBytes(data));
                Thread.Sleep(10);
            }
        }
    }
}
