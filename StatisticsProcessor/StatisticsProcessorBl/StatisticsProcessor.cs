using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using StatisticsProcessorBl.Interfaces;

namespace StatisticsProcessorBl
{
    public class StatisticsProcessor
    {
        public StatisticsProcessor(IStatisticsExporter exporter)
        {
            StatisticsExporter = exporter;
        }

        public void StartProcess()
        {
            List<Iteration> iterations = ParseIterations();

            StatisticsExporter.OutputFileName = OutputFileName;
            StatisticsExporter.InputIterations = iterations;

            StatisticsExporter.Export();
        }

        private List<Iteration> ParseIterations()
        {
            String[] lines = File.ReadLines(InputFileName).ToArray();

            List<Iteration> iterations = new List<Iteration>();
            Iteration iteration = null;

            foreach (String line in lines)
            {
                Match match = Regex.Match(line, @"iteration\s+(?<id>\d+)", RegexOptions.IgnoreCase);

                if (match.Success)
                {
                    iteration = new Iteration();
                    iteration.Number = Convert.ToInt32(match.Groups["id"].Value);

                    iterations.Add(iteration);
                }
                else
                {
                    match = Regex.Match(line, @"arr: (?<array>\d+) pr: (?<processes>\d+) th: (?<threads>\d+) t: (?<time>[\d\.]+)", RegexOptions.IgnoreCase);

                    if (match.Success == false)
                    {
                        throw new FormatException("Data point line incorrectly formatted.");
                    }

                    DataPoint dataPoint = new DataPoint();
                    dataPoint.ArraySize = Convert.ToInt32(match.Groups["array"].Value);
                    dataPoint.ProcessCount = Convert.ToInt32(match.Groups["processes"].Value);
                    dataPoint.ThreadsCount = Convert.ToInt32(match.Groups["threads"].Value);
                    dataPoint.Time = Convert.ToDouble(match.Groups["time"].Value);

                    iteration.DataPoints.Add(dataPoint);
                }
            }

            return iterations;
        }

        public String InputFileName { get; set; }
        public String OutputFileName { get; set; }
        public IStatisticsExporter StatisticsExporter { get; set; }
    }
}
