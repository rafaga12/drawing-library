using ModernComponents.System.Drawing;
using ModernComponents.System.Drawing.Drawing2D;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Graphics.Display;
using Windows.UI;
using Windows.UI.Text;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

namespace App1
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage5 : Page
    {
        //Required for E Drawing Library to work.
        Graphics g;
        bool renderNeeded = false;
        public MainPage5()
        {
            this.InitializeComponent();
        }
#region Initialization
        //We handle the loaded event of the page so we can have the rendered size of the controls
        private void Page_Loaded(object sender, RoutedEventArgs e)
        {
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

                Rect rectangle = new Rect(60, 70, 250, 250);

                GraphicsPath small = new GraphicsPath();
                small.AddEllipse(105, 195, 105, 105);
                small.AddPie(115, 205, 85, 85, -90, 45);

                g.DrawEllipse(WhitePen, rectangle);
                g.DrawPie(WhitePen, 70, 80, 230, 230, -90, 45);

                g.DrawPath(new Pen(Colors.White, 2), small);
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
