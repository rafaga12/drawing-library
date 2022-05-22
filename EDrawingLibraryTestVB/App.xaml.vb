' The Blank Application template is documented at http://go.microsoft.com/fwlink/?LinkId=234227

''' <summary>
''' Provides application-specific behavior to supplement the default Application class.
''' </summary>
NotInheritable Class App
    Inherits Application

    ''' <summary>
    ''' Invoked when the application is launched normally by the end user.  Other entry points
    ''' will be used when the application is launched to open a specific file, to display
    ''' search results, and so forth.
    ''' </summary>
    ''' <param name="e">Details about the launch request and process.</param>
    Protected Overrides Sub OnLaunched(e As Windows.ApplicationModel.Activation.LaunchActivatedEventArgs)
#If DEBUG Then
        ' Show graphics profiling information while debugging.
        If System.Diagnostics.Debugger.IsAttached Then
            ' Display the current frame rate counters
            Me.DebugSettings.EnableFrameRateCounter = True
        End If
#End If

        'Initialization code for DirectX
        Dim mainPage As New MainPage()
        'Dim mainPage As New MainPage2()
        'Dim mainPage As New MainPage3()
        'Dim mainPage As New MainPage4()
        'Dim mainPage As New MainPage5()

        If e.PreviousExecutionState = ApplicationExecutionState.Terminated Then
            'Loads saved state (if applicable) 
            'mainPage.LoadInternalState(ApplicationData.Current.LocalSettings.Values)
        End If


        'Place the page in the current window and ensure that it is active.
        Window.Current.Content = mainPage
        Window.Current.Activate()

    End Sub

    ''' <summary>
    ''' Invoked when Navigation to a certain page fails
    ''' </summary>
    ''' <param name="sender">The Frame which failed navigation</param>
    ''' <param name="e">Details about the navigation failure</param>
    Private Sub OnNavigationFailed(sender As Object, e As NavigationFailedEventArgs)
        Throw New Exception("Failed to load Page " + e.SourcePageType.FullName)
    End Sub

    ''' <summary>
    ''' Invoked when application execution is being suspended.  Application state is saved
    ''' without knowing whether the application will be terminated or resumed with the contents
    ''' of memory still intact.
    ''' </summary>
    ''' <param name="sender">The source of the suspend request.</param>
    ''' <param name="e">Details about the suspend request.</param>
    Private Sub OnSuspending(sender As Object, e As SuspendingEventArgs) Handles Me.Suspending
        Dim deferral As SuspendingDeferral = e.SuspendingOperation.GetDeferral()
        ' TODO: Save application state and stop any background activity
        deferral.Complete()
    End Sub

End Class
