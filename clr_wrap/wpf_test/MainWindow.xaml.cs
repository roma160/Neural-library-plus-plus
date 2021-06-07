using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

using NNL;

namespace wpf_test
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private WSimpleNetwork network;
        private StreamTrainingData testingData;
        private Random r;

        public MainWindow()
        {
            InitializeComponent();
            r = new Random();
            network = new WSimpleNetwork(@"..\..\..\..\..\testing\tests\mnist\network_backup.b", true);
            testingData = new StreamTrainingData(@"..\..\..\..\..\testing\tests\mnist\testing_data.b", true);
        }

        private void random_test_button_Click(object sender, RoutedEventArgs e)
        {
            int testIndex = r.Next(1000);
            int right_ans = -1;
            double[] right_ans_data = testingData.GetOutputData(testIndex);
            for(int i = 0; i < 10; i++)
                if (right_ans_data[i] == 1.0)
                {
                    right_ans = i;
                    break;
                }

            test_index_text.Text = $"{testIndex}";
            test_answer_text.Text = $"{right_ans}";

            network_results.Items.Clear();
            double[] network_ans = network.GetNetworkOutput(testingData, testIndex);
            for (int i = 0; i < 10; i++)
                network_results.Items.Add($"{i}) {network_ans[i]:0.000}");
        }
    }
}
