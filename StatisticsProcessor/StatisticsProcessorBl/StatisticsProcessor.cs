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
            CalculateAverage = true;
        }

        public void StartProcess()
        {
            List<Iteration> iterations = ParseIterations();

            StatisticsExporter.OutputFileName = OutputFileName;

            if (CalculateAverage)
            {
                Iteration avarageIteration = GetAvarageIteration(iterations);
                StatisticsExporter.InputIterations = new List<Iteration>() { avarageIteration };
            }
            else
            {
                StatisticsExporter.InputIterations = iterations;
            }
            
            StatisticsExporter.Export();
        }

        private Iteration GetAvarageIteration(List<Iteration> iterations)
        {
            Iteration result = new Iteration();
            result.Number = 0;

            if (iterations.Count > 0)
            {
                DataPoint[] avgPoints = new DataPoint[iterations[0].DataPoints.Count];

                int realIterationsCount = 0;

                foreach (Iteration iteration in iterations)
                {
                    if (iteration.DataPoints.Count == avgPoints.Length)
                    {
                        for (int i = 0; i < iteration.DataPoints.Count; i++)
                        {
                            if (avgPoints[i] == null)
                            {
                                avgPoints[i] = iteration.DataPoints[i].Clone();
                            }
                            else
                            {
                                avgPoints[i].Time += iteration.DataPoints[i].Time;
                            }
                        }

                        realIterationsCount++;
                    }
                }

                foreach (DataPoint dataPoint in avgPoints)
                {
                    dataPoint.Time /= realIterationsCount;
                }

                result.DataPoints = avgPoints.ToList();
            }

            return result;
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
                    dataPoint.Time = Convert.ToDouble(Utils.FormatFloatingNumberSeparator(match.Groups["time"].Value));

                    iteration.DataPoints.Add(dataPoint);
                }
            }

            return iterations;
        }

        public bool CalculateAverage { get; set; }

        public String InputFileName { get; set; }
        public String OutputFileName { get; set; }
        public IStatisticsExporter StatisticsExporter { get; set; }
    }
}
