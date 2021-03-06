Imports Windows.UI
Imports ModernComponents.System.Drawing
Imports ModernComponents.System.Drawing.Drawing2D

' The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

''' <summary>
''' An empty page that can be used on its own or navigated to within a Frame.
''' </summary>
Public NotInheritable Class MainPage4
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

            Dim points As Point() = {New Point(70, 317), New Point(211, 290), New Point(264, 77), New Point(316, 290), New Point(457, 317)}
            Dim points2 As Point() = {New Point(155, 100), New Point(167, 136), New Point(209, 136), New Point(173, 154), New Point(183, 196), New Point(155, 172), New Point(127, 196), New Point(137, 154), New Point(101, 136), New Point(143, 136)}
            Dim points3 As Point() = {New Point(380, 100), New Point(434, 136), New Point(408, 196), New Point(352, 196), New Point(326, 136)}
            Dim rectangle As New Rect(70, 70, 390, 250)

            Dim curve As New GraphicsPath()
            curve.AddCurve(points, 0.6F)

            Dim seaStar As New GraphicsPath()
            seaStar.AddClosedCurve(points2, 0.6F)

            Dim pentagon As New GraphicsPath()
            pentagon.AddPolygon(points3)

            Dim frame As New GraphicsPath()
            frame.AddRectangle(rectangle)

            Dim picture As New GraphicsPath()
            picture.AddPath(frame, False)
            picture.AddPath(curve, False)
            picture.AddPath(seaStar, False)
            picture.AddPath(pentagon, False)

            g.DrawPath(WhitePen, picture)
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
