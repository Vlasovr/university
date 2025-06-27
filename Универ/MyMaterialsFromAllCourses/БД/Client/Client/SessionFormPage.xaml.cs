using System.Collections.ObjectModel;
using Microsoft.Maui.Controls;
using CinemaClientService.ViewModels;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Net.Http.Json;

namespace CinemaClientService
{
    public partial class SessionFormPage : ContentPage, INotifyPropertyChanged
    {
        private readonly HttpClient _httpClient;
        private readonly SessionView? _editingSession;

        private ObservableCollection<HallView> Halls { get; set; }

        private HallView? _selectedHall;

        public string PageTitle => _editingSession == null ? "Добавить сеанс" : "Редактировать сеанс";

        public SessionFormPage(HttpClient httpClient, SessionView? session)
        {
            InitializeComponent();
            _httpClient = httpClient;
            _editingSession = session;

            Halls = new ObservableCollection<HallView>();
            BindingContext = this;

            if (_editingSession != null)
            {
                // Режим редактирования
                SessionIdEntry.Text = _editingSession.SessionId.ToString();
                SelectedHallLabel.Text = _editingSession.HallId.ToString();
                FormatEntry.Text = _editingSession.Format;
                AvailableSeatsEntry.Text = _editingSession.AvailableSeats.ToString();

                // Разделение StartTime и EndTime на Date и Time
                StartDatePicker.Date = _editingSession.StartTime.Date;
                StartTimePicker.Time = _editingSession.StartTime.TimeOfDay;
                EndDatePicker.Date = _editingSession.EndTime.Date;
                EndTimePicker.Time = _editingSession.EndTime.TimeOfDay;

                _selectedHall = new HallView { HallId = _editingSession.HallId };
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

            if (!int.TryParse(AvailableSeatsEntry.Text, out int availableSeats) || availableSeats < 0)
            {
                await DisplayAlert("Ошибка", "Доступные места должны быть неотрицательным числом.", "OK");
                return;
            }

            if (string.IsNullOrWhiteSpace(FormatEntry.Text))
            {
                await DisplayAlert("Ошибка", "Пожалуйста, введите формат сеанса.", "OK");
                return;
            }

            if (StartDatePicker.Date + StartTimePicker.Time > EndDatePicker.Date + EndTimePicker.Time)
            {
                await DisplayAlert("Ошибка", "Время начала должно быть раньше времени окончания.", "OK");
                return;
            }

            // Конвертация DateTime в UTC
            var startDateTimeLocal = StartDatePicker.Date + StartTimePicker.Time;
            var endDateTimeLocal = EndDatePicker.Date + EndTimePicker.Time;

            var session = new SessionView
            {
                HallId = _selectedHall.HallId,
                Format = FormatEntry.Text,
                AvailableSeats = availableSeats,
                StartTime = startDateTimeLocal.ToUniversalTime(),
                EndTime = endDateTimeLocal.ToUniversalTime()
            };

            try
            {
                if (_editingSession == null)
                {
                    // Добавление нового сеанса
                    var response = await _httpClient.PostAsJsonAsync("api/Sessions", session);
                    if (response.IsSuccessStatusCode)
                    {
                        await DisplayAlert("Успех", "Сеанс успешно добавлен.", "OK");
                        await Navigation.PopAsync();
                    }
                    else
                    {
                        var content = await response.Content.ReadAsStringAsync();
                        await DisplayAlert("Ошибка", $"Не удалось добавить сеанс (код {response.StatusCode}): {content}", "OK");
                    }
                }
                else
                {
                    // Редактирование существующего сеанса
                    session.SessionId = _editingSession.SessionId;
                    var response = await _httpClient.PutAsJsonAsync($"api/Sessions/{session.SessionId}", session);
                    if (response.IsSuccessStatusCode)
                    {
                        await DisplayAlert("Успех", "Сеанс успешно обновлен.", "OK");
                        await Navigation.PopAsync();
                    }
                    else
                    {
                        var content = await response.Content.ReadAsStringAsync();
                        await DisplayAlert("Ошибка", $"Не удалось обновить сеанс (код {response.StatusCode}): {content}", "OK");
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