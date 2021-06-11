using System.Windows;
using System.Windows.Controls;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Input.StylusPlugIns;

namespace wpf_test.Custom_elements
{
    public class CustomInkCanvas : InkCanvas
    {
        public Stroke DrawedStroke { get; protected set; }

        public delegate void StylusPointCollected();
        public event StylusPointCollected OnStylusPointCollected;


        static CustomInkCanvas()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(CustomInkCanvas), new FrameworkPropertyMetadata(typeof(CustomInkCanvas)));
        }

        public CustomInkCanvas()
        {
            DynamicRenderer = new CustomDynamicRenderer(this);
        }

        private class CustomDynamicRenderer : DynamicRenderer
        {
            private readonly CustomInkCanvas inkCanvas;

            public CustomDynamicRenderer(CustomInkCanvas inkCanvas)
            {
                this.inkCanvas = inkCanvas;
            }

            protected override void OnStylusDown(RawStylusInput rawStylusInput)
            {
                base.OnStylusDown(rawStylusInput);
                inkCanvas.DrawedStroke = new Stroke(rawStylusInput.GetStylusPoints(), DrawingAttributes);
                inkCanvas.OnStylusPointCollected();
            }

            protected override void OnStylusUp(RawStylusInput rawStylusInput)
            {
                base.OnStylusUp(rawStylusInput);
                inkCanvas.DrawedStroke = null;
                inkCanvas.OnStylusPointCollected();
            }

            protected override void OnStylusMove(RawStylusInput rawStylusInput)
            {
                base.OnStylusMove(rawStylusInput);
                inkCanvas.DrawedStroke.StylusPoints.Add(rawStylusInput.GetStylusPoints());
                inkCanvas.OnStylusPointCollected();
            }
        }
    }
}
