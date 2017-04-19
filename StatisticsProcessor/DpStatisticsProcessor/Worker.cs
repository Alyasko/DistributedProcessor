using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using StatisticsProcessorBl;
using StatisticsProcessorBl.Implementations;

namespace DpStatisticsProcessor
{
    public class Worker
    {
        private readonly StatisticsProcessor _statisticsProcessor;

        public Worker()
        {
            _statisticsProcessor = new StatisticsProcessor(new ExcelGraphicsExporter());
        }

        public void Run()
        {
            _statisticsProcessor.InputFileName = GetFileNameFromArgs(FileNameType.InputFileName);
            _statisticsProcessor.OutputFileName = GetFileNameFromArgs(FileNameType.OutputFileName);

            _statisticsProcessor.StartProcess();
        }

        private String GetFileNameFromArgs(FileNameType fileNameType)
        {
            String fileName = "";

            if (Arguments.Length == 1)
            {
                throw new ArgumentException("Insufficient number of command line arguments.");
            }

            switch (fileNameType)
            {
                case FileNameType.InputFileName:
                    fileName = GetFileNameSafe(0);
                    break;
                case FileNameType.OutputFileName:
                    fileName = GetFileNameSafe(1, true);
                    break;
                default:
                    throw new ArgumentOutOfRangeException(nameof(fileNameType), fileNameType, null);
            }

            return fileName;
        }

        private String GetFileNameSafe(int argIndex, bool bypassCheck = false)
        {
            if (argIndex < Arguments.Length)
            {
                if (bypassCheck == false)
                {
                    if (File.Exists(Arguments[argIndex]))
                    {
                        return Arguments[argIndex];
                    }
                    else
                    {
                        throw new FileNotFoundException("File does not exist.");
                    }
                }
                else
                {
                    return Arguments[argIndex];
                }
            }
            else
            {
                throw new IndexOutOfRangeException("No element with such index in command line arguments."); 
            }
        }

        public string[] Arguments { get; set; }
    }
}
