using System.Collections.ObjectModel;
using Microsoft.Maui.Controls;
using CinemaClientService.ViewModels;

namespace CinemaClientService
{
    public partial class SelectHallPage : ContentPage
    {
        public event EventHandler<HallView> HallSelected;

        private readonly ObservableCollection<HallView> _halls;

        public SelectHallPage(IEnumerable<HallView> halls)
        {
            InitializeComponent();
            _halls = new ObservableCollection<HallView>(halls);
            HallList.ItemsSource = _halls;
        }

        private async void OnHallSelected(object sender, SelectionChangedEventArgs e)
        {
            var selectedHall = e.CurrentSelection.FirstOrDefault() as HallView;
            if (selectedHall != null)
            {
                HallSelected?.Invoke(this, selectedHall);
                await Navigation.PopAsync();
            }
        }
    }
}