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
    public sealed partial class MainPage2 : Page
    {
        //Required for E Drawing Library to work.
        Graphics g;
        bool renderNeeded = false;
#region Initialization
        public MainPage2()
        {
            this.InitializeComponent();
            
            //Required for E Drawing Library to work.
            g = new Graphics(Window.Current.CoreWindow, DrawingCanvas);
            
            CompositionTarget.Rendering += OnRendering;
            renderNeeded = true;

            //Trigger rendering when those events happen.
            //Window.Current.CoreWindow.SizeChanged += OnWindowSizeChanged;
            //DisplayProperties.LogicalDpiChanged += OnLogicalDpiChanged;
            //DisplayProperties.OrientationChanged += OnOrientationChanged;
            //DisplayProperties.DisplayContentsInvalidated += OnDisplayContentsInvalidated;

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
            //if (renderNeeded)
            {
                //Clears the buffer, if not done, the buffer will use a random (uninitialized) color
                g.Clear(Colors.DarkGray);
                //Drawing operations
                SolidColorBrush WhiteBrush = new SolidColorBrush(Colors.White);
                Pen BrushPen = new Pen(WhiteBrush, 3);

                int[] xPoints = { 155, 167, 209, 173, 183, 155, 127, 137, 101, 143 };
                int[] yPoints = { 100, 136, 136, 154, 196, 172, 196, 154, 136, 136 };
                
                GraphicsPath head = new GraphicsPath();
                head.AddArc(115, 80, 40, 40, 160, 200);
                for (int i = 0; i <= 6; i += 4)
                {
                    head.AddBezier(xPoints[i], yPoints[i], xPoints[i + 1], yPoints[i + 1], xPoints[i + 2], yPoints[i + 2], xPoints[i + 3], yPoints[i + 3]);
                }
                head.AddLine(xPoints[xPoints.Length - 1], yPoints[yPoints.Length - 1], 135, 100);

                g.DrawPath(BrushPen, head);
                //Required for displaying the drawn results
                g.Flush();

                renderNeeded = false;
            }
        }
#endregion
        //Trigger rendering when those events happen.
        /*private void OnWindowSizeChanged(CoreWindow sender, WindowSizeChangedEventArgs args)
        {
	        renderNeeded = true;
        }

        private void OnLogicalDpiChanged(Object sender)
        {
	        renderNeeded = true;
        }

        private void OnOrientationChanged(Object sender)
        {
	        renderNeeded = true;
        }
        private void OnDisplayContentsInvalidated(Object sender)
        {
            renderNeeded = true;
        }*/

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
    }
}
