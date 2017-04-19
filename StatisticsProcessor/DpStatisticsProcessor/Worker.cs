using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using StatisticsProcessorBl;

namespace DpStatisticsProcessor
{
    public class Worker
    {
        private readonly StatisticsProcessor _statisticsProcessor;

        public Worker()
        {
            _statisticsProcessor = new StatisticsProcessor();
        }

        public void Run()
        {
            _statisticsProcessor.InputFileName = GetFileNameFromArgs(FileNameType.InputFileName);
            _statisticsProcessor.OutputFileName = GetFileNameFromArgs(FileNameType.OutputFileName);
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
                    fileName = GetFileNameSafe(1);
                    break;
                case FileNameType.OutputFileName:
                    fileName = GetFileNameSafe(2);
                    break;
                default:
                    throw new ArgumentOutOfRangeException(nameof(fileNameType), fileNameType, null);
            }

            return fileName;
        }

        private String GetFileNameSafe(int argIndex)
        {
            if (argIndex < Arguments.Length)
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
                throw new IndexOutOfRangeException("Not element with such index in command line arguments."); 
            }
        }

        public string[] Arguments { get; set; }
    }
}
