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
        public void Export()
        {
            Excel.Application xlApp;
            Excel.Workbook xlWorkBook;
            Excel.Worksheet xlWorkSheet;
            object misValue = System.Reflection.Missing.Value;

            xlApp = new Excel.Application();
            xlWorkBook = xlApp.Workbooks.Add(misValue);
            xlWorkSheet = (Excel.Worksheet)xlWorkBook.Worksheets[1];

            //add data 
            xlWorkSheet.Cells[1, 1].Value = "";
            xlWorkSheet.Cells[1, 2].Value = "Student1";
            xlWorkSheet.Cells[1, 3].Value = "Student2";
            xlWorkSheet.Cells[1, 4].Value = "Student3";

            xlWorkSheet.Cells[2, 1].Value = "Term1";
            xlWorkSheet.Cells[2, 2].Value = "80";
            xlWorkSheet.Cells[2, 3].Value = "65";
            xlWorkSheet.Cells[2, 4].Value = "45";

            xlWorkSheet.Cells[3, 1].Value = "Term2";
            xlWorkSheet.Cells[3, 2].Value = "78";
            xlWorkSheet.Cells[3, 3].Value = "72";
            xlWorkSheet.Cells[3, 4].Value = "60";

            xlWorkSheet.Cells[4, 1].Value = "Term3";
            xlWorkSheet.Cells[4, 2].Value = "82";
            xlWorkSheet.Cells[4, 3].Value = "80";
            xlWorkSheet.Cells[4, 4].Value = "65";

            xlWorkSheet.Cells[5, 1].Value = "Term4";
            xlWorkSheet.Cells[5, 2].Value = "75";
            xlWorkSheet.Cells[5, 3].Value = "82";
            xlWorkSheet.Cells[5, 4].Value = "68";

            Excel.Range chartRange;

            Excel.ChartObjects xlCharts = (Excel.ChartObjects)xlWorkSheet.ChartObjects(Type.Missing);
            Excel.ChartObject myChart = (Excel.ChartObject)xlCharts.Add(10, 80, 300, 250);
            Excel.Chart chartPage = myChart.Chart;

            chartRange = xlWorkSheet.get_Range("A1", "d5");
            chartPage.SetSourceData(chartRange, misValue);
            chartPage.ChartType = XlChartType.xl3DColumnClustered;

            xlWorkBook.SaveAs(OutputFileName, XlFileFormat.xlWorkbookNormal, misValue, misValue, misValue, misValue, XlSaveAsAccessMode.xlExclusive, misValue, misValue, misValue, misValue, misValue);
            xlWorkBook.Close(true, misValue, misValue);
            xlApp.Quit();

            ReleaseObject(xlWorkSheet);
            ReleaseObject(xlWorkBook);
            ReleaseObject(xlApp);
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
