using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StatisticsProcessorBl
{
    public class Iteration
    {
        public Iteration()
        {
            DataPoints = new List<DataPoint>();
        }

        public override string ToString()
        {
            return $"Iteration {Number}";
        }

        public Int32 Number { get; set; }
        public IList<DataPoint> DataPoints { get; set; }
    }
}
