using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
//Required for E Drawing Library to work.
using ModernComponents.System.Drawing;
//Required for triggering rendering when certain events happen.
using Windows.UI.Core;
using Windows.Graphics.Display;
//Added namespaces
using Windows.UI;
using Windows.UI.Text;
using ModernComponents.System.Drawing.Drawing2D;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

namespace App1
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        //Required for E Drawing Library to work.
        Graphics g;
        bool renderNeeded = false;
#region Initialization
        public MainPage()
        {
            this.InitializeComponent();
            
            //Required for E Drawing Library to work.
            g = new Graphics(Window.Current.CoreWindow, DrawingCanvas);
            
            CompositionTarget.Rendering += OnRendering;
            renderNeeded = true;

            //Trigger rendering when those events happen.
            DrawingCanvas.SizeChanged += OnSwapChainPanelSizeChanged;
            DrawingCanvas.CompositionScaleChanged += OnSwapChainPanelCompositionScaleChanged;

            DisplayInformation currentDisplayInformation = DisplayInformation.GetForCurrentView();
            currentDisplayInformation.DpiChanged += OnDpiChanged;
            currentDisplayInformation.OrientationChanged += OnOrientationChanged;
            DisplayInformation.DisplayContentsInvalidated += OnDisplayContentsInvalidated;
            
        }

#endregion

        /// <summary>
        /// Invoked when this page is about to be displayed in a Frame.
        /// </summary>
        /// <param name="e">Event data that describes how this page was reached.  The Parameter
        /// property is typically used to configure the page.</param>
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
        }
#region Usage
        //Required for E Drawing Library to work.
        private void OnRendering(Object sender, Object args)
        {
            //Render only when needed, for continuous rendering comment renderNeeded = false;
            if (renderNeeded)
            {
                //Clears the buffer, if not done, the buffer will use a random (uninitialized) color
                g.Clear(Colors.DarkGray);
                //Drawing operations
                SolidColorBrush WhiteBrush = new SolidColorBrush(Colors.White);
                Pen BrushPen = new Pen(WhiteBrush, 3);
                Pen ColorPen = new Pen(Colors.White, 2);

                Font textFont = new Font("Segoe UI", 30, FontStyle.Normal, FontWeights.Normal, FontStretch.Normal);
                g.DrawString("E Drawing Library", textFont, WhiteBrush, 50, 30);

                FontFamily CalibriFont = new FontFamily("Calibri");
                Font textFont2 = new Font(CalibriFont, textFont.Size, textFont.Style, textFont.Weight, textFont.Stretch);
                g.DrawString("E Drawing Library", textFont2, WhiteBrush, 50, 130);

                int[] xPoints = { 55, 67, 109, 73, 83, 55, 27, 37, 1, 43 };
                int[] yPoints = { 0, 36, 36, 54, 96, 72, 96, 54, 36, 36 };
                GraphicsPath star = new GraphicsPath();
                for (int i = 0; i <= 8; i += 2)
                {
                    star.AddLine(xPoints[i], yPoints[i], xPoints[i + 1], yPoints[i + 1]);
                }
                star.AddLine(xPoints[xPoints.Length - 1], yPoints[yPoints.Length - 1], xPoints[0], yPoints[0]);

                g.DrawPath(BrushPen, star);
                g.DrawLine(ColorPen, 200, 0, 100, 100);

                BrushPen.Brush = new SolidColorBrush(Colors.WhiteSmoke);
                BrushPen.Width = 1;

                ColorPen.Color = Colors.WhiteSmoke;
                ColorPen.Width = 1;

                g.DrawBezier(ColorPen, 200, 0, 200, 75, 225, 100, 300, 100);
                g.DrawArc(BrushPen, 250, 0, 100, 100, -90, 180);
                //Required for displaying the drawn results
                g.Flush();

                renderNeeded = false;
            }
        }
#endregion
        //Trigger rendering when those events happen.
        void OnSwapChainPanelSizeChanged(object sender, SizeChangedEventArgs e)
        {
            renderNeeded = true;
        }

        void OnSwapChainPanelCompositionScaleChanged(SwapChainPanel sender, object args)
        {
            renderNeeded = true;
        }

        void OnDpiChanged(DisplayInformation sender, object args)
        {
            renderNeeded = true;
        }
        
        void OnOrientationChanged(DisplayInformation sender, object args)
        {
            renderNeeded = true;
        }

        void OnDisplayContentsInvalidated(DisplayInformation sender, object args)
        {
            renderNeeded = true;
        }
#region Save
        private void SaveButton_Click(object sender, RoutedEventArgs e)
        {
            g.ToFile();
        }
#endregion

        private void RenderButton_Click(object sender, RoutedEventArgs e)
        {
            renderNeeded = true;
        }
    }
}
