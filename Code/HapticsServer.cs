using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Net;
using System.Net.Sockets;

using HCIHaptics;

namespace HapticsServerNS
{
    public class ServicesContainer
    {
        private byte value = 128;

        public ServicesContainer()
        {
            //
        }

        public void KeyboardSender()
        {
            while (true)
            {
                if (Console.ReadKey(true).Key == ConsoleKey.N)
                {
                    value--;
                    Console.WriteLine(">>>>>>>>>>> " + value);
                }
                else if (Console.ReadKey(true).Key == ConsoleKey.M)
                {
                    value++;
                    Console.WriteLine(">>>>>>>>>>> " + value);
                }
                else if (Console.ReadKey(true).Key == ConsoleKey.V)
                {
                    for (int i = 0; i < 16; i++)
                    {
                        HCIHapticsBLEConnector.sendValueToCharacteristicVibrators(i, 0);
                    }
                    Console.WriteLine(">>>>>>>>>>> 0");
                }
                else if (Console.ReadKey(true).Key == ConsoleKey.O)
                {
                    for (int i = 0; i < 16; i++)
                    {
                        HCIHapticsBLEConnector.sendValueToCharacteristicVibrators(i, 180);
                    }
                    Console.WriteLine(">>>>>>>>>>> 180");
                }
                else if (Console.ReadKey(true).Key == ConsoleKey.B)
                {
                    Console.WriteLine(">>>>>>>>>>> ");
                    int count = 0;
                    while (count < 3)
                    {
                        HCIHapticsBLEConnector.sendValueToCharacteristicVibrators(0, 128); ;
                        Thread.Sleep(200);
                        HCIHapticsBLEConnector.sendValueToCharacteristicVibrators(0, 40); ;
                        Thread.Sleep(200);
                        count++;
                    }

                    HCIHapticsBLEConnector.sendValueToCharacteristicVibrators(0, 0);
                }
                else if (Console.ReadKey(true).Key == ConsoleKey.F1) { HCIHapticsBLEConnector.sendValueToCharacteristicVibrators(0, value); Console.WriteLine("0"); }
                else if (Console.ReadKey(true).Key == ConsoleKey.F2) { HCIHapticsBLEConnector.sendValueToCharacteristicVibrators(1, value); Console.WriteLine("1"); }
                else if (Console.ReadKey(true).Key == ConsoleKey.F3) { HCIHapticsBLEConnector.sendValueToCharacteristicVibrators(2, value); Console.WriteLine("2"); }
                else if (Console.ReadKey(true).Key == ConsoleKey.F4) { HCIHapticsBLEConnector.sendValueToCharacteristicVibrators(3, value); Console.WriteLine("3"); }
                else if (Console.ReadKey(true).Key == ConsoleKey.F5) { HCIHapticsBLEConnector.sendValueToCharacteristicVibrators(4, value); Console.WriteLine("4"); }
                else if (Console.ReadKey(true).Key == ConsoleKey.F6) { HCIHapticsBLEConnector.sendValueToCharacteristicVibrators(5, value); Console.WriteLine("5"); }
                else if (Console.ReadKey(true).Key == ConsoleKey.F7) { HCIHapticsBLEConnector.sendValueToCharacteristicVibrators(6, value); Console.WriteLine("6"); }
                else if (Console.ReadKey(true).Key == ConsoleKey.F8) { HCIHapticsBLEConnector.sendValueToCharacteristicVibrators(7, value); Console.WriteLine("7"); }
                else if (Console.ReadKey(true).Key == ConsoleKey.Q) { HCIHapticsBLEConnector.sendValueToCharacteristicVibrators(8, value); Console.WriteLine("8"); }
                else if (Console.ReadKey(true).Key == ConsoleKey.W) { HCIHapticsBLEConnector.sendValueToCharacteristicVibrators(9, value); Console.WriteLine("9"); }
                else if (Console.ReadKey(true).Key == ConsoleKey.E) { HCIHapticsBLEConnector.sendValueToCharacteristicVibrators(10, value); Console.WriteLine("10"); }
                else if (Console.ReadKey(true).Key == ConsoleKey.R) { HCIHapticsBLEConnector.sendValueToCharacteristicVibrators(11, value); Console.WriteLine("11"); }
                else if (Console.ReadKey(true).Key == ConsoleKey.A) { HCIHapticsBLEConnector.sendValueToCharacteristicVibrators(12, value); Console.WriteLine("12"); }
                else if (Console.ReadKey(true).Key == ConsoleKey.S) { HCIHapticsBLEConnector.sendValueToCharacteristicVibrators(13, value); Console.WriteLine("13"); }
                else if (Console.ReadKey(true).Key == ConsoleKey.D) { HCIHapticsBLEConnector.sendValueToCharacteristicVibrators(14, value); Console.WriteLine("14"); }
                else if (Console.ReadKey(true).Key == ConsoleKey.F) { HCIHapticsBLEConnector.sendValueToCharacteristicVibrators(15, value); Console.WriteLine("15"); }
            }
        }
    }

    class HapticsServer
    {
        static void Main(string[] args)
        {
            try
            {
                HCIHapticsBLEConnector.setup("HCI_HapGLeft");
                
                Console.WriteLine("Device Connected and Service Initialized!");

                ServicesContainer container = new ServicesContainer();
                (new Thread(() => container.KeyboardSender())).Start();
                
            }
            catch(Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }
    }
}
