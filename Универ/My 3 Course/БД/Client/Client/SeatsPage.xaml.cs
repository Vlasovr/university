using System.Collections.ObjectModel;
using System.Linq;
using System.Net.Http.Json;
using Microsoft.Maui.Controls;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using CinemaClientService.ViewModels;

namespace CinemaClientService
{
    public partial class SeatsPage : ContentPage, INotifyPropertyChanged
    {
        private readonly HttpClient _httpClient;
        public ObservableCollection<SeatView> Seats { get; set; }
        private bool isLoading;

        public bool IsLoading
        {
            get => isLoading;
            set
            {
                isLoading = value;
                OnPropertyChanged();
            }
        }

        public SeatView SelectedSeat { get; set; }

        public SeatsPage(HttpClient httpClient)
        {
            InitializeComponent();
            _httpClient = httpClient;
            Seats = new ObservableCollection<SeatView>();
            SeatList.ItemsSource = Seats;
            BindingContext = this;
        }

        protected override async void OnAppearing()
        {
            base.OnAppearing();
            await LoadSeats();
        }

        private async Task LoadSeats()
        {
            try
            {
                IsLoading = true;
                // Очищаем список, чтобы при повторной загрузке не было дублей
                Seats.Clear();

                // Загружаем базовый список мест (с HallId)
                var seats = await _httpClient.GetFromJsonAsync<List<SeatView>>("api/Seats");

                if (seats != null)
                {
                    // Получаем все залы заранее, чтобы уменьшить количество запросов
                    var halls = await _httpClient.GetFromJsonAsync<List<HallView>>("api/Halls");
                    var hallDictionary = halls?.ToDictionary(h => h.HallId, h => h.HallId.ToString()) ?? new Dictionary<int, string>();

                    foreach (var seat in seats)
                    {
                        // Поскольку Hall не имеет поля Name, используем HallId как идентификатор
                        seat.HallId = hallDictionary.ContainsKey(seat.HallId) ? seat.HallId : 0;
                        Seats.Add(seat);
                    }
                }
            }
            catch (Exception ex)
            {
                await DisplayAlert("Ошибка", $"Не удалось загрузить данные: {ex.Message}", "OK");
            }
            finally
            {
                IsLoading = false;
            }
        }

        private async void OnSearchBarTextChanged(object sender, TextChangedEventArgs e)
        {
            string query = e.NewTextValue.ToLower();
            if (string.IsNullOrEmpty(query))
            {
                Seats.Clear();
                await LoadSeats();
            }
            else
            {
                // Попытка преобразовать запрос в целое число для поиска HallId
                bool isHallId = int.TryParse(query, out int hallId);

                var filtered = Seats.Where(s => (isHallId && s.HallId.ToString().Contains(query)) ||
                                                 s.Row.ToLower().Contains(query)).ToList();
                SeatList.ItemsSource = filtered;
            }
        }

        private async void OnAddSeatClicked(object sender, EventArgs e)
        {
            var form = new SeatFormPage(_httpClient, null);
            await Navigation.PushAsync(form);
        }

        private async void OnEditSeatClicked(object sender, EventArgs e)
        {
            if (SelectedSeat != null)
            {
                var form = new SeatFormPage(_httpClient, SelectedSeat);
                await Navigation.PushAsync(form);
            }
            else
            {
                await DisplayAlert("Ошибка", "Выберите место для редактирования", "OK");
            }
        }

        private async void OnDeleteSeatClicked(object sender, EventArgs e)
        {
            if (SelectedSeat != null)
            {
                bool confirm = await DisplayAlert("Удаление",
                    $"Вы уверены, что хотите удалить место ID {SelectedSeat.SeatId}?",
                    "Да", "Нет");
                if (confirm)
                {
                    try
                    {
                        var response = await _httpClient.DeleteAsync($"api/Seats/{SelectedSeat.SeatId}");
                        if (response.IsSuccessStatusCode)
                        {
                            Seats.Remove(SelectedSeat);
                            SelectedSeat = null;
                        }
                        else
                        {
                            var content = await response.Content.ReadAsStringAsync();
                            await DisplayAlert("Ошибка",
                                $"Не удалось удалить место (код {response.StatusCode}): {content}",
                                "OK");
                        }
                    }
                    catch (Exception ex)
                    {
                        await DisplayAlert("Ошибка", $"Не удалось удалить место: {ex.Message}", "OK");
                    }
                }
            }
            else
            {
                await DisplayAlert("Ошибка", "Выберите место для удаления", "OK");
            }
        }

        private void OnSeatSelected(object sender, SelectionChangedEventArgs e)
        {
            SelectedSeat = e.CurrentSelection.FirstOrDefault() as SeatView;
        }

        // Реализация интерфейса INotifyPropertyChanged
        public new event PropertyChangedEventHandler PropertyChanged;

        protected new void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}