using System;
using System.Collections.Generic;
using System.IO;
using System.Threading;
using System.Windows;
using System.Windows.Ink;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Drawing;
using NNL;
using Brush = System.Windows.Media.Brush;

namespace wpf_test
{
    public partial class MainWindow : Window
    {
        private const int pixel_num = 28;
        private const double brush_scale = 12;
        private const double scale_factor = 0.8;
        private bool useImageScaling;
        private const int dpi = 10;
        private double imageCanvasWidth;

        private WSimpleNetwork network;
        private EventWaitHandle waiter;
        private bool isCanvasChanged;
        private Thread workingThread;

        private TrainingData testTrainingData;

        private List<ListItem> result_progresses;
        private class ListItem
        {
            public int Number { get; set; }
            public int Value { get; set; }
        }

        public MainWindow()
        {
            InitializeComponent();

            result_progresses = new List<ListItem>();
            for(int i = 0; i < 10; i++)
                result_progresses.Add(new ListItem{Number = i, Value = 0});
            results_list.ItemsSource = result_progresses;

            testTrainingData = new TrainingData(@"..\..\..\..\..\testing\tests\mnist\testing_data.b", true);

            waiter = new EventWaitHandle(false, EventResetMode.AutoReset);
            workingThread = new Thread(WorkingThreadFunction);
        }

        void WorkingThreadFunction()
        {
            Brush rectangleBrush = new SolidColorBrush(Colors.White);
            Rect rectangleSize = Rect.Empty;
            Application.Current.Dispatcher.Invoke(() => 
                rectangleSize = new Rect(image_canvas.RenderSize));
            RenderTargetBitmap renderer = new RenderTargetBitmap(pixel_num, pixel_num, dpi, dpi, PixelFormats.Pbgra32);
            int stride = renderer.PixelWidth * renderer.Format.BitsPerPixel / 8;
            int height = renderer.PixelHeight;

            network = new WSimpleNetwork(@"..\..\..\..\..\testing\tests\mnist\network_backup.b", true);
            while (true)
            {
                if (!isCanvasChanged) waiter.WaitOne();
                if((workingThread.ThreadState & (ThreadState.SuspendRequested | ThreadState.AbortRequested | ThreadState.StopRequested)) != 0)
                    break;
                isCanvasChanged = false;

                StrokeCollection strokes = null;
                Application.Current.Dispatcher.Invoke(() =>
                {
                    strokes = image_canvas.Strokes.Clone();
                    if(image_canvas.DrawedStroke != null)
                        strokes.Add(image_canvas.DrawedStroke.Clone());
                });
                //image_canvas.Cli
                if(strokes.Count == 0) continue;
                if(useImageScaling){
                    Rect bounds = strokes.GetBounds();
                    double side = Math.Max(bounds.Width, bounds.Height);
                    double scale = imageCanvasWidth * scale_factor / side;
                    strokes.Transform(new Matrix(scale, 0, 0, scale,
                            -scale * (bounds.X - Math.Abs(side - bounds.Width)/2) + (1 - scale_factor) * imageCanvasWidth / 2,
                            -scale * (bounds.Y - Math.Abs(side - bounds.Height)/2) + (1 - scale_factor) * imageCanvasWidth / 2),
                        false);
                }
                DrawingVisual drawing = new DrawingVisual();
                using (DrawingContext context = drawing.RenderOpen())
                {
                    context.DrawRectangle(rectangleBrush, null, rectangleSize);
                    strokes.Draw(context);
                }
                renderer.Render(drawing);
                //SaveBitmapSource(renderer);

                byte[] rawPixels = new byte[stride * height];
                renderer.CopyPixels(Int32Rect.Empty, rawPixels, stride, 0);
                double[] inputData = new double[pixel_num * pixel_num];
                for(int i = 0; i < inputData.Length; i++)
                    inputData[i] = (768 - rawPixels[i * 4] - rawPixels[i * 4 + 1] - rawPixels[i * 4 + 2]) / 768.0;

                //for (int i = 0; i < inputData.Length; i++)
                //    inputData[i] = (768 - buffBytes[i * 4] - buffBytes[i * 4 + 1] - buffBytes[i * 4 + 2]) / 768.0;

                double[] outputData = network.GetNetworkOutput(inputData);
                Application.Current.Dispatcher.Invoke(() =>
                {
                    for (int i = 0; i < 10; i++)
                        result_progresses[i].Value = (int)(outputData[i] * 100);
                    results_list.Items.Refresh();

                    preview_image.Source = BitmapSource.Create(
                        pixel_num, pixel_num, dpi, dpi, PixelFormats.Pbgra32, null, rawPixels, stride);
                });
            }
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            DrawingAttributes attributes = image_canvas.DefaultDrawingAttributes;
            attributes.Width = image_canvas.ActualWidth / brush_scale;
            attributes.Height = image_canvas.ActualHeight / brush_scale;
            imageCanvasWidth = image_canvas.ActualWidth;

            useImageScaling = using_scaling_checkbox.IsChecked ?? false;

            image_canvas.OnStylusPointCollected += Image_canvas_OnStylusPointCollected;

            workingThread.Start();
        }

        private void clear_button_Click(object sender, RoutedEventArgs e)
        {
            image_canvas.Strokes.Clear();
            isCanvasChanged = true;
            waiter.Set();
        }

        private void Window_Closed(object sender, EventArgs e)
        {
            workingThread.Abort();
            waiter.Set();
        }

        private void test_btn_Click(object sender, RoutedEventArgs e)
        {
            //Brush rectangleBrush = new SolidColorBrush(Colors.White);
            //RenderTargetBitmap renderer = new RenderTargetBitmap(pixel_num, pixel_num, dpi, dpi, PixelFormats.Pbgra32);
            //renderer.Render(image_canvas);
            //SaveBitmapSource(renderer);

            //isCanvasChanged = true;
            //waiter.Set();

            //StrokeCollection strokes = null;
            //Application.Current.Dispatcher.Invoke(() =>
            //{
            //    strokes = image_canvas.Strokes.Clone();
            //    if (image_canvas.DrawedStroke != null)
            //        strokes.Add(image_canvas.DrawedStroke.Clone());
            //});
            //Rect bounds = strokes.GetBounds();
            //double side = Math.Max(bounds.Width, bounds.Height);
            //double scale = imageCanvasWidth * scale_factor / side;
            //strokes.Transform(new Matrix(scale, 0, 0, scale,
            //        -scale * (bounds.X - Math.Abs(bounds.Width - side) / 2),
            //        -scale * (bounds.Y - Math.Abs(bounds.Height - side) / 2)),
            //    false);
            //DrawingVisual drawing = new DrawingVisual();
            //using (DrawingContext context = drawing.RenderOpen())
            //{
            //    context.DrawRectangle(rectangleBrush, null, new Rect(image_canvas.RenderSize));
            //    strokes.Draw(context);
            //}
            //renderer.Render(drawing);
            //SaveBitmapSource(renderer);

            if (!int.TryParse(test_index_text.Text, out int test_index))
            {
                MessageBox.Show("Invalid test index text.");
                return;
            }
            DoTest(test_index);
        }

        private void DoTest(int test_index)
        {
            double[] testData = testTrainingData.GetInputData(test_index);
            double[] networkOutput = network.GetNetworkOutput(testData);
            for (int i = 0; i < 10; i++)
                result_progresses[i].Value = (int) (networkOutput[i] * 100);
            results_list.Items.Refresh();

            byte[] rawPixels = new byte[pixel_num * pixel_num * 4];
            for (int i = 0; i < testData.Length; i++)
            {
                rawPixels[i * 4] = rawPixels[i * 4 + 1] = rawPixels[i * 4 + 2] = (byte) (255 - testData[i] * 255);
                rawPixels[i * 4 + 3] = 255;
            }

            preview_image.Source = BitmapSource.Create(
                pixel_num, pixel_num, dpi, dpi, PixelFormats.Pbgra32, null, rawPixels, pixel_num * 4);
        }

        private void Image_canvas_OnStylusPointCollected()
        {
            isCanvasChanged = true;
            waiter.Set();
        }

        private void next_test_button_Click(object sender, RoutedEventArgs e)
        {
            if (!int.TryParse(test_index_text.Text, out int test_index))
            {
                MessageBox.Show("Invalid test index text.");
                return;
            }
            test_index_text.Text = $"{++test_index}";
            DoTest(test_index);
        }

        private void using_scaling_checkbox_Checked(object sender, RoutedEventArgs e)
        {
            useImageScaling = using_scaling_checkbox.IsChecked ?? false;
            isCanvasChanged = true;
            waiter.Set();
        }
    }
}
