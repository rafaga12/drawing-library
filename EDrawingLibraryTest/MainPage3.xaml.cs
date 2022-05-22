using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
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

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

namespace App1
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage3 : Page
    {
        //Required for E Drawing Library to work.
        Graphics g;
        bool renderNeeded = false;

#region Initialization
        public MainPage3()
        {
            this.InitializeComponent();
        }

        //We handle the loaded event of the page so we can have the rendered size of the controls
        private void Page_Loaded(object sender, RoutedEventArgs e)
        {
            //Required for E Drawing Library to work.
            ImageBrush DrawingBrush = new ImageBrush();
            g = new Graphics(DrawingBrush, Image1.RenderSize, true);
            
            CompositionTarget.Rendering += OnRendering;

            //Trigger rendering when those events happen.
            DisplayInformation currentDisplayInformation = DisplayInformation.GetForCurrentView();
            currentDisplayInformation.DpiChanged += OnDpiChanged;
            currentDisplayInformation.OrientationChanged += OnOrientationChanged;
            DisplayInformation.DisplayContentsInvalidated += OnDisplayContentsInvalidated;

            //Assign the brush's image source to an Image control
            Image1.Source = DrawingBrush.ImageSource;
            renderNeeded = true;
        }

#endregion

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
                Pen WhitePen = new Pen(Colors.White, 1);

                Point[] points = { new Point(70, 317), new Point(211, 290), new Point(264, 77), new Point(316, 290), new Point(457, 317)};
                Point[] points2 = { new Point(155, 100), new Point(167, 136), new Point(209, 136), new Point(173, 154), new Point(183, 196), new Point(155, 172), new Point(127, 196), new Point(137, 154), new Point(101, 136), new Point(143, 136) };
                Point[] points3 = { new Point(380, 100), new Point(434, 136), new Point(408, 196), new Point(352, 196), new Point(326, 136)};
                Rect rectangle = new Rect(60, 70, 410, 250);

                g.DrawCurve(WhitePen, points, 0.5f);
                g.DrawClosedCurve(WhitePen, points2, 0.5f);
                g.DrawPolygon(WhitePen, points3);
                g.DrawRectangle(WhitePen, rectangle);
                //Required for displaying the drawn results
                g.Flush();

                renderNeeded = false;
            }
        }
#endregion

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

        private void SaveButton_Click(object sender, RoutedEventArgs e)
        {
            g.ToFile();
        }

        private void RenderButton_Click(object sender, RoutedEventArgs e)
        {
            renderNeeded = true;
        }

        private void TensionSlider_ValueChanged(object sender, RangeBaseValueChangedEventArgs e)
        {
            renderNeeded = true;
        }
    }
}
