using System.Collections.ObjectModel;
using Microsoft.Maui.Controls;
using CinemaClientService.ViewModels;

namespace CinemaClientService
{
    public partial class SelectSessionPage : ContentPage
    {
        public event EventHandler<SessionView> SessionSelected;

        private readonly ObservableCollection<SessionView> _sessions;

        public SelectSessionPage(IEnumerable<SessionView> sessions)
        {
            InitializeComponent();
            _sessions = new ObservableCollection<SessionView>(sessions);
            SessionList.ItemsSource = _sessions;
        }

        private async void OnSessionSelected(object sender, SelectionChangedEventArgs e)
        {
            var selectedSession = e.CurrentSelection.FirstOrDefault() as SessionView;
            if (selectedSession != null)
            {
                SessionSelected?.Invoke(this, selectedSession);
                await Navigation.PopAsync();
            }
        }
    }
}