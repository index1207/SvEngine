using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace DummyClient
{
    class Program
    {
        static Sv.Connector connector;
        static void Main(string[] args)
        {
            Thread.Sleep(1000);
            connector = new Sv.Connector();
            connector.OnConnect += (EndPoint ep) =>
            {
                Console.WriteLine("On Connected");
                connector.Send(Encoding.UTF8.GetBytes("HEllo"));
            };
            connector.OnDisconnect += () =>
            {
                Console.WriteLine("On Disconnected");
            };
            connector.OnReceive += (Sv.Message msg) =>
            {
                Console.WriteLine("On Receive");
            };
            connector.Connect(new IPEndPoint(IPAddress.Loopback, 9999));

            Console.ReadLine();
        }
    }
}
