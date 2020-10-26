using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FillDownload
{
    public class FillReport
    {
        public TT_Fill fill;

        public FillReport(TT_Fill fill_)
        {
            fill = fill_;
        }


        virtual public string Price
        {
            get
            {
                return fill.Price;
            }
        }

        virtual public string Quantity
        {
            get
            {
                return fill.FillQty;
            }
        }

        public static List<FillReport> GetReports(TT_Fill fill, bool individual_fills)
        {
            List<FillReport> reports = new List<FillReport>();

            if(individual_fills && fill.HasFillsGroup)
            {
                int count = fill.FillsGroupCount;
                for(int i = 0; i < count; ++i)
                {
                    reports.Add(new IndividualFillReport(fill, i));
                }
            }
            else
            {
                reports.Add(new FillReport(fill));
            }

            return reports;
        }
    }

    public class IndividualFillReport : FillReport
    {
        private int index;

        public IndividualFillReport(TT_Fill fill_, int index_)
            :base(fill_)
        {
            index = index_;
        }

        public override string Price
        {
            get
            {
                return fill.GetIndividualPrice(index);
            }
        }

        public override string Quantity
        {
            get
            {
                return fill.GetIndividualQuantity(index);
            }
        }
    }
}
