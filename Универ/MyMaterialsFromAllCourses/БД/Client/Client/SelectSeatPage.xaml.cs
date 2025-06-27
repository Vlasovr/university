using System.Collections.ObjectModel;
using Microsoft.Maui.Controls;
using CinemaClientService.ViewModels;

namespace CinemaClientService
{
    public partial class SelectSeatPage : ContentPage
    {
        public event EventHandler<SeatView> SeatSelected;

        private readonly ObservableCollection<SeatView> _seats;

        public SelectSeatPage(IEnumerable<SeatView> seats)
        {
            InitializeComponent();
            _seats = new ObservableCollection<SeatView>(seats);
            SeatList.ItemsSource = _seats;
        }

        private async void OnSeatSelected(object sender, SelectionChangedEventArgs e)
        {
            var selectedSeat = e.CurrentSelection.FirstOrDefault() as SeatView;
            if (selectedSeat != null)
            {
                SeatSelected?.Invoke(this, selectedSeat);
                await Navigation.PopAsync();
            }
        }
    }
}