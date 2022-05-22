' The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

'Required for E Drawing Library to work.
Imports ModernComponents.System.Drawing

'Added namespaces
Imports ModernComponents.System.Drawing.Drawing2D
Imports Windows.UI
Imports Windows.UI.Text


''' <summary>
''' An empty page that can be used on its own or navigated to within a Frame.
''' </summary>
Public NotInheritable Class MainPage
    Inherits Page

    'Required for E Drawing Library to work.
    Dim g As Graphics
    Dim renderNeeded As Boolean = False

#Region "Initialization"
    Private Sub Page_Loaded(sender As Object, e As RoutedEventArgs)

        'Required for E Drawing Library to work.
        g = New Graphics(Window.Current.CoreWindow, DrawingCanvas)

        AddHandler CompositionTarget.Rendering, AddressOf OnRendering
        renderNeeded = True

        'Trigger rendering when those events happen.
        Dim currentDisplayInformation As DisplayInformation = DisplayInformation.GetForCurrentView()
        AddHandler currentDisplayInformation.DpiChanged, AddressOf OnDpiChanged
        AddHandler currentDisplayInformation.OrientationChanged, AddressOf OnOrientationChanged
        AddHandler DisplayInformation.DisplayContentsInvalidated, AddressOf OnDisplayContentsInvalidated
    End Sub
#End Region

#Region "Usage"
    'Required for E Drawing Library to work.
    Private Sub OnRendering(sender As Object, args As Object)
        'Render only when needed, for continuous rendering comment renderNeeded = false; 
        If renderNeeded Then
            'Clears the buffer, if not done, the buffer will use a random (uninitialized) color
            g.Clear(Colors.DarkGray)
            'Drawing operations
            Dim WhiteBrush As New SolidColorBrush(Colors.White)
            Dim BrushPen As New Pen(WhiteBrush, 3)
            Dim ColorPen As New Pen(Colors.White, 2)

            Dim textFont As New Font("Segoe UI", 30, FontStyle.Normal, FontWeights.Normal, FontStretch.Normal)
            g.DrawString("E Drawing Library", textFont, WhiteBrush, 50, 30)

            Dim CalibriFont As New FontFamily("Calibri")
            Dim textFont2 As New Font(CalibriFont, textFont.Size, textFont.Style, textFont.Weight, textFont.Stretch)
            g.DrawString("E Drawing Library", textFont2, WhiteBrush, 50, 130)

            Dim xPoints As Integer() = {55, 67, 109, 73, 83, 55, 27, 37, 1, 43}
            Dim yPoints As Integer() = {0, 36, 36, 54, 96, 72, 96, 54, 36, 36}
            Dim star As New GraphicsPath()

            For i As Integer = 0 To 8 Step 2
                star.AddLine(xPoints(i), yPoints(i), xPoints(i + 1), yPoints(i + 1))
            Next
            star.AddLine(xPoints(xPoints.Length - 1), yPoints(yPoints.Length - 1), xPoints(0), yPoints(0))

            g.DrawPath(BrushPen, star)
            g.DrawLine(ColorPen, 200, 0, 100, 100)

            BrushPen.Brush = New SolidColorBrush(Colors.WhiteSmoke)
            BrushPen.Width = 1

            ColorPen.Color = Colors.WhiteSmoke
            ColorPen.Width = 1

            g.DrawBezier(ColorPen, 200, 0, 200, 75, 225, 100, 300, 100)
            g.DrawArc(BrushPen, 250, 0, 100, 100, -90, 180)
            'Required for displaying the drawn results
            g.Flush()

            renderNeeded = False
        End If
    End Sub
#End Region


    'Trigger rendering when those events happen.
    Private Sub OnSwapChainPanelSizeChanged(sender As Object, e As SizeChangedEventArgs) Handles DrawingCanvas.SizeChanged
        renderNeeded = True
    End Sub

    Private Sub OnSwapChainPanelCompositionScaleChanged(sender As SwapChainPanel, args As Object) Handles DrawingCanvas.CompositionScaleChanged
        renderNeeded = True
    End Sub

    Private Sub OnDpiChanged(sender As DisplayInformation, args As Object)
        renderNeeded = True
    End Sub

    Private Sub OnOrientationChanged(sender As DisplayInformation, args As Object)
        renderNeeded = True
    End Sub

    Private Sub OnDisplayContentsInvalidated(sender As DisplayInformation, args As Object)
        renderNeeded = True
    End Sub

#Region "Save"
    Private Sub SaveButton_Click(sender As Object, e As RoutedEventArgs)
        g.ToFile()
    End Sub
#End Region

    Private Sub RenderButton_Click(sender As Object, e As RoutedEventArgs)
        renderNeeded = True
    End Sub
End Class