Imports ModernComponents.System.Drawing
Imports ModernComponents.System.Drawing.Drawing2D
Imports Windows.UI

' The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

''' <summary>
''' An empty page that can be used on its own or navigated to within a Frame.
''' </summary>
Public NotInheritable Class MainPage2
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

            Dim xPoints As Integer() = {155, 167, 209, 173, 183, 155, 127, 137, 101, 143}
            Dim yPoints As Integer() = {100, 136, 136, 154, 196, 172, 196, 154, 136, 136}

            Dim head As New GraphicsPath()
            head.AddArc(115, 80, 40, 40, 160, 200)

            For i As Integer = 0 To 6 Step 4
                head.AddBezier(xPoints(i), yPoints(i), xPoints(i + 1), yPoints(i + 1), xPoints(i + 2), yPoints(i + 2), xPoints(i + 3), yPoints(i + 3))
            Next
            head.AddLine(xPoints(xPoints.Length - 1), yPoints(yPoints.Length - 1), 135, 100)

            g.DrawPath(BrushPen, head)
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
End Class
