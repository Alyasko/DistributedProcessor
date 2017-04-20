using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StatisticsProcessorBl
{
    public class DataPoint
    {
        public Int32 ArraySize { get; set; }
        public Int32 ProcessCount { get; set; }
        public Int32 ThreadsCount { get; set; }
        public Double Time { get; set; }

        public DataPoint Clone()
        {
            return new DataPoint()
            {
                ArraySize = ArraySize,
                ProcessCount = ProcessCount,
                ThreadsCount = ThreadsCount,
                Time = Time
            };
        }

        public override string ToString()
        {
            return $"Arr: {ArraySize}, Pr: {ProcessCount}, Th: {ThreadsCount}, T: {Time}";
        }
    }
}
