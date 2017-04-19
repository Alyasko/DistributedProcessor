using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StatisticsProcessorBl.Interfaces
{
    public interface IStatisticsExporter : IFileExporter
    {
        IEnumerable<Iteration> InputIterations { get; set; }
        void Export();
    }
}
