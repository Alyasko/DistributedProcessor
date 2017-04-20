using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StatisticsProcessorBl
{
    public static class Utils
    {
        public static String FormatFloatingNumberSeparator(String input)
        {
            return input.Replace(",", CultureInfo.CurrentCulture.NumberFormat.NumberDecimalSeparator).Replace(".", CultureInfo.CurrentCulture.NumberFormat.NumberDecimalSeparator);
        }

        public static String ConvertIndexToExcel(int index)
        {
            String result = "";

            int d = index;
            int rem = 0;

            Stack<Char> stack = new Stack<Char>();

            int itCount = 0;

            do
            {
                rem = d % 26;
                char a = (char)('A' + rem);

                if (itCount > 0)
                {
                    a = (char) (a - 1);
                }

                stack.Push(a);

                d /= 26;


                itCount++;
            }
            while (d != 0);

            while (stack.Count != 0)
            {
                result += stack.Pop();
            }

            return result;
        }
    }
}
