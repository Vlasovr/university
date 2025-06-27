using System.Collections.ObjectModel;
using System.Net.Http.Json;
using Microsoft.Maui.Controls;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace CinemaClientService
{
    public partial class HallsPage : ContentPage, INotifyPropertyChanged
    {
        private readonly HttpClient _httpClient;
        public ObservableCollection<HallView> Halls { get; set; }
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

        public HallView SelectedHall { get; set; }

        public HallsPage(HttpClient httpClient)
        {
            InitializeComponent();
            _httpClient = httpClient;
            Halls = new ObservableCollection<HallView>();
            HallList.ItemsSource = Halls;
            BindingContext = this;
        }

        protected override async void OnAppearing()
        {
            base.OnAppearing();
            await LoadHalls();
        }

        private async Task LoadHalls()
        {
            try
            {
                IsLoading = true;
                var halls = await _httpClient.GetFromJsonAsync<List<HallView>>("api/Halls");
                Halls.Clear();
                foreach (var hall in halls)
                {
                    Halls.Add(hall);
                }
            }
            catch (Exception ex)
            {
                await DisplayAlert("Ошибка", $"Не удалось загрузить данные: {ex.Message}", "ОК");
            }
            finally
            {
                IsLoading = false;
            }
        }

        private async void OnSearchBarTextChanged(object sender, TextChangedEventArgs e)
        {
            string query = e.NewTextValue;
            if (string.IsNullOrEmpty(query))
            {
                await LoadHalls();
            }
            else
            {
                var filtered = Halls.Where(h => h.HallId.ToString().Contains(query) ||
                                                 h.CleaningSchedule.Contains(query, StringComparison.OrdinalIgnoreCase))
                                   .ToList();
                HallList.ItemsSource = filtered;
            }
        }

        private async void OnAddHallClicked(object sender, EventArgs e)
        {
            var form = new HallFormPage(_httpClient, null);
            await Navigation.PushAsync(form);
        }

        private async void OnEditHallClicked(object sender, EventArgs e)
        {
            if (SelectedHall != null)
            {
                var form = new HallFormPage(_httpClient, SelectedHall);
                await Navigation.PushAsync(form);
            }
            else
            {
                await DisplayAlert("Ошибка", "Выберите зал для редактирования", "ОК");
            }
        }

        private async void OnDeleteHallClicked(object sender, EventArgs e)
        {
            if (SelectedHall != null)
            {
                bool confirm = await DisplayAlert("Удаление", $"Вы уверены, что хотите удалить зал {SelectedHall.HallId}?", "Да", "Нет");
                if (confirm)
                {
                    try
                    {
                        var response = await _httpClient.DeleteAsync($"api/Halls/{SelectedHall.HallId}");
                        if (response.IsSuccessStatusCode)
                        {
                            Halls.Remove(SelectedHall);
                            SelectedHall = null;
                        }
                        else
                        {
                            var content = await response.Content.ReadAsStringAsync();
                            await DisplayAlert("Ошибка", $"Не удалось удалить зал (код {response.StatusCode}): {content}", "ОК");
                        }
                    }
                    catch (Exception ex)
                    {
                        await DisplayAlert("Ошибка", $"Не удалось удалить зал: {ex.Message}", "ОК");
                    }
                }
            }
            else
            {
                await DisplayAlert("Ошибка", "Выберите зал для удаления", "ОК");
            }
        }

        private void OnHallSelected(object sender, SelectionChangedEventArgs e)
        {
            SelectedHall = e.CurrentSelection.FirstOrDefault() as HallView;
        }

        // Реализация интерфейса INotifyPropertyChanged
        public new event PropertyChangedEventHandler PropertyChanged;

        protected new void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}