using System.Collections.ObjectModel;
using Microsoft.Maui.Controls;
using CinemaClientService.ViewModels;
using System.ComponentModel;
using System.Net.Http.Json;
using System.Runtime.CompilerServices;

namespace CinemaClientService
{
    public partial class SeatFormPage : ContentPage, INotifyPropertyChanged
    {
        private readonly HttpClient _httpClient;
        private readonly SeatView? _editingSeat;

        private ObservableCollection<HallView> Halls { get; set; }

        private HallView? _selectedHall;

        public string PageTitle => _editingSeat == null ? "Добавить место" : "Редактировать место";

        public SeatFormPage(HttpClient httpClient, SeatView? seat)
        {
            InitializeComponent();
            _httpClient = httpClient;
            _editingSeat = seat;

            Halls = new ObservableCollection<HallView>();
            BindingContext = this;

            if (_editingSeat != null)
            {
                // Режим редактирования
                SeatIdEntry.Text = _editingSeat.SeatId.ToString();
                RowEntry.Text = _editingSeat.Row;
                IsElectronicSwitch.IsToggled = _editingSeat.IsElectronic;
                SelectedHallLabel.Text = _editingSeat.HallId.ToString();
                _selectedHall = new HallView { HallId = _editingSeat.HallId };
            }
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
                var halls = await _httpClient.GetFromJsonAsync<List<HallView>>("api/Halls");
                Halls.Clear();
                if (halls != null)
                {
                    foreach (var hall in halls)
                    {
                        Halls.Add(hall);
                    }
                }
            }
            catch (Exception ex)
            {
                await DisplayAlert("Ошибка", $"Не удалось загрузить залы: {ex.Message}", "OK");
            }
        }

        private async void OnSelectHallClicked(object sender, EventArgs e)
        {
            // Открываем страницу выбора зала
            var selectHallPage = new SelectHallPage(Halls);
            selectHallPage.HallSelected += (s, hall) =>
            {
                _selectedHall = hall;
                SelectedHallLabel.Text = hall.HallId.ToString();
            };
            await Navigation.PushAsync(selectHallPage);
        }

        private async void OnSaveClicked(object sender, EventArgs e)
        {
            // Валидация
            if (_selectedHall == null)
            {
                await DisplayAlert("Ошибка", "Пожалуйста, выберите зал.", "OK");
                return;
            }

            if (string.IsNullOrWhiteSpace(RowEntry.Text))
            {
                await DisplayAlert("Ошибка", "Пожалуйста, введите ряд.", "OK");
                return;
            }

            var seat = new SeatView
            {
                Row = RowEntry.Text,
                IsElectronic = IsElectronicSwitch.IsToggled,
                HallId = _selectedHall.HallId
            };

            try
            {
                if (_editingSeat == null)
                {
                    // Добавление нового места
                    var response = await _httpClient.PostAsJsonAsync("api/Seats", seat);
                    if (response.IsSuccessStatusCode)
                    {
                        await DisplayAlert("Успех", "Место успешно добавлено.", "OK");
                        await Navigation.PopAsync();
                    }
                    else
                    {
                        var content = await response.Content.ReadAsStringAsync();
                        await DisplayAlert("Ошибка", $"Не удалось добавить место (код {response.StatusCode}): {content}", "OK");
                    }
                }
                else
                {
                    // Редактирование существующего места
                    seat.SeatId = _editingSeat.SeatId;
                    var response = await _httpClient.PutAsJsonAsync($"api/Seats/{seat.SeatId}", seat);
                    if (response.IsSuccessStatusCode)
                    {
                        await DisplayAlert("Успех", "Место успешно обновлено.", "OK");
                        await Navigation.PopAsync();
                    }
                    else
                    {
                        var content = await response.Content.ReadAsStringAsync();
                        await DisplayAlert("Ошибка", $"Не удалось обновить место (код {response.StatusCode}): {content}", "OK");
                    }
                }
            }
            catch (Exception ex)
            {
                await DisplayAlert("Ошибка", $"Произошла ошибка: {ex.Message}", "OK");
            }
        }

        private async void OnCancelClicked(object sender, EventArgs e)
        {
            await Navigation.PopAsync();
        }

        // Реализация интерфейса INotifyPropertyChanged
        public new event PropertyChangedEventHandler PropertyChanged;

        protected new void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}