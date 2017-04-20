using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DpStatisticsProcessor
{
    public class InfoPrinter
    {
        public void PrintHelp()
        {
            Console.WriteLine("Usage: <executable> <input text file> <absolute path to output .xls file> [calculate average?]");
        }
    }
}
