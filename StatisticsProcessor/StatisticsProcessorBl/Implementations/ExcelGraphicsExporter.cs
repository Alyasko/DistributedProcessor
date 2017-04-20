using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NetOffice.ExcelApi.Enums;
using StatisticsProcessorBl.Interfaces;
using Excel = NetOffice.ExcelApi;

namespace StatisticsProcessorBl.Implementations
{
    public class ExcelGraphicsExporter : IStatisticsExporter
    {
        private Excel.Application _xlApp;
        private Excel.Workbook _xlWorkBook;
        private Excel.Worksheet _xlWorkSheet;

        private readonly object _misValue;

        public ExcelGraphicsExporter()
        {
            _misValue = System.Reflection.Missing.Value;
        }

        public void Export()
        {
            _xlApp = new Excel.Application();
            _xlWorkBook = _xlApp.Workbooks.Add(_misValue);
            _xlWorkSheet = (Excel.Worksheet)_xlWorkBook.Worksheets[1];

            int iterCol = 1;

            foreach (Iteration iteration in InputIterations)
            {
                PrintIterationCells(iteration, 1, iterCol);

                iterCol += 11;
            }

            _xlWorkBook.SaveAs(OutputFileName, XlFileFormat.xlWorkbookNormal, _misValue, _misValue, _misValue, _misValue, XlSaveAsAccessMode.xlExclusive, _misValue, _misValue, _misValue, _misValue, _misValue);
            _xlWorkBook.Close(true, _misValue, _misValue);
            _xlApp.Quit();

            ReleaseObject(_xlWorkSheet);
            ReleaseObject(_xlWorkBook);
            ReleaseObject(_xlApp);
        }

        private void PrintIterationChart(Iteration iteration, int row, int col)
        {
            Excel.Range chartRange;

            Excel.ChartObjects xlCharts = (Excel.ChartObjects)_xlWorkSheet.ChartObjects();
            Excel.ChartObject myChart = xlCharts.Add(100 + 45 * col, row * 15, 380, 250);
            Excel.Chart chartPage = myChart.Chart;

            String range = Utils.ConvertIndexToExcel(col);

            chartRange =  _xlWorkSheet.Range($"{range}{row + 1}", $"{range}{row + 1 + 16}");
            chartPage.SetSourceData(chartRange);
            chartPage.ChartType = XlChartType.xlLineMarkersStacked;
        }

        private void PrintIterationCells(Iteration iteration, int row, int col)
        {
            int processCount = 0;
            int iterRow = row;
            int iterCol = col;

            int dataRow = 0;

            foreach (DataPoint dataPoint in iteration.DataPoints)
            {
                if (processCount != dataPoint.ProcessCount)
                {
                    if (processCount != 0)
                    {
                        iterRow += 19;
                    }

                    _xlWorkSheet.Cells[iterRow, iterCol].Value = "Processes";
                    _xlWorkSheet.Cells[iterRow, iterCol + 1].Value = dataPoint.ProcessCount;
                    _xlWorkSheet.Cells[iterRow + 1, iterCol].Value = "Threads";
                    _xlWorkSheet.Cells[iterRow + 1, iterCol + 1].Value = "Time";

                    dataRow = 2;

                    processCount = dataPoint.ProcessCount;

                    PrintIterationChart(iteration, iterRow, iterCol);
                }

                _xlWorkSheet.Cells[iterRow + dataRow, iterCol].Value = dataPoint.ThreadsCount;
                _xlWorkSheet.Cells[iterRow + dataRow, iterCol + 1].Value = dataPoint.Time;

                dataRow++;
            }
        }

        private void PrintCellsHeader(Iteration iteration)
        {
            
        }



        private void ReleaseObject(object obj)
        {
            try
            {
                System.Runtime.InteropServices.Marshal.ReleaseComObject(obj);
                obj = null;
            }
            catch (Exception ex)
            {
                obj = null;
            }
            finally
            {
                GC.Collect();
            }
        }

        public string OutputFileName { get; set; }

        public IEnumerable<Iteration> InputIterations { get; set; }
    }
}
