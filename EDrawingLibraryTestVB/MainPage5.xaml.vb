Imports Windows.UI
Imports ModernComponents.System.Drawing
Imports ModernComponents.System.Drawing.Drawing2D

' The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

''' <summary>
''' An empty page that can be used on its own or navigated to within a Frame.
''' </summary>
Public NotInheritable Class MainPage5
    Inherits Page

    'Required for E Drawing Library to work.
    Dim g As Graphics
    Dim renderNeeded As Boolean = False

#Region "Initialization"
    Private Sub Page_Loaded(sender As Object, e As RoutedEventArgs)
        'Required for E Drawing Library to work.
        Dim DrawingBrush As New ImageBrush()
        g = New Graphics(DrawingBrush, Image1.RenderSize, True)

        AddHandler CompositionTarget.Rendering, AddressOf OnRendering

        'Trigger rendering when those events happen.
        Dim currentDisplayInformation As DisplayInformation = DisplayInformation.GetForCurrentView()
        AddHandler currentDisplayInformation.DpiChanged, AddressOf OnDpiChanged
        AddHandler currentDisplayInformation.OrientationChanged, AddressOf OnOrientationChanged
        AddHandler DisplayInformation.DisplayContentsInvalidated, AddressOf OnDisplayContentsInvalidated

        'Assign the brush's image source to an Image control
        Image1.Source = DrawingBrush.ImageSource
        renderNeeded = True

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
            Dim WhitePen As New Pen(Colors.White, 1)

            Dim rectangle As New Rect(60, 70, 250, 250)

            Dim small As New GraphicsPath()
            small.AddEllipse(105, 195, 105, 105)
            small.AddPie(115, 205, 85, 85, -90, 45)

            g.DrawEllipse(WhitePen, rectangle)
            g.DrawPie(WhitePen, 70, 80, 230, 230, -90, 45)

            g.DrawPath(New Pen(Colors.White, 2), small)
            'Required for displaying the drawn results
            g.Flush()

            renderNeeded = False
        End If
    End Sub
#End Region

    'Trigger rendering when those events happen.
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
