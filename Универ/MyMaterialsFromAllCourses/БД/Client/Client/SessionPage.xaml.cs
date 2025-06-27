using System.Collections.ObjectModel;
using System.Linq;
using System.Net.Http.Json;
using Microsoft.Maui.Controls;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using CinemaClientService.ViewModels;

namespace CinemaClientService
{
    public partial class SessionsPage : ContentPage, INotifyPropertyChanged
    {
        private readonly HttpClient _httpClient;
        public ObservableCollection<SessionView> Sessions { get; set; }
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

        public SessionView SelectedSession { get; set; }

        public SessionsPage(HttpClient httpClient)
        {
            InitializeComponent();
            _httpClient = httpClient;
            Sessions = new ObservableCollection<SessionView>();
            SessionList.ItemsSource = Sessions;
            BindingContext = this;
        }

        protected override async void OnAppearing()
        {
            base.OnAppearing();
            await LoadSessions();
        }

        private async Task LoadSessions()
        {
            try
            {
                IsLoading = true;
                // Очищаем список, чтобы при повторной загрузке не было дублей
                Sessions.Clear();

                // Загружаем базовый список сеансов (с HallId)
                var sessions = await _httpClient.GetFromJsonAsync<List<SessionView>>("api/Sessions");

                if (sessions != null)
                {
                    foreach (var session in sessions)
                    {
                        Sessions.Add(session);
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
                Sessions.Clear();
                await LoadSessions();
            }
            else
            {
                // Попытка преобразовать запрос в целое число для поиска HallId или AvailableSeats
                bool isHallId = int.TryParse(query, out int hallId);
                bool isAvailableSeats = int.TryParse(query, out int availableSeats);

                var filtered = Sessions.Where(s => 
                                    (isHallId && s.HallId.ToString().Contains(query)) ||
                                    (isAvailableSeats && s.AvailableSeats.ToString().Contains(query)) ||
                                    s.Format.ToLower().Contains(query))
                                      .ToList();
                SessionList.ItemsSource = filtered;
            }
        }

        private async void OnAddSessionClicked(object sender, EventArgs e)
        {
            var form = new SessionFormPage(_httpClient, null);
            await Navigation.PushAsync(form);
        }

        private async void OnEditSessionClicked(object sender, EventArgs e)
        {
            if (SelectedSession != null)
            {
                var form = new SessionFormPage(_httpClient, SelectedSession);
                await Navigation.PushAsync(form);
            }
            else
            {
                await DisplayAlert("Ошибка", "Выберите сеанс для редактирования", "OK");
            }
        }

        private async void OnDeleteSessionClicked(object sender, EventArgs e)
        {
            if (SelectedSession != null)
            {
                bool confirm = await DisplayAlert("Удаление",
                    $"Вы уверены, что хотите удалить сеанс ID {SelectedSession.SessionId}?",
                    "Да", "Нет");
                if (confirm)
                {
                    try
                    {
                        var response = await _httpClient.DeleteAsync($"api/Sessions/{SelectedSession.SessionId}");
                        if (response.IsSuccessStatusCode)
                        {
                            Sessions.Remove(SelectedSession);
                            SelectedSession = null;
                        }
                        else
                        {
                            var content = await response.Content.ReadAsStringAsync();
                            await DisplayAlert("Ошибка",
                                $"Не удалось удалить сеанс (код {response.StatusCode}): {content}",
                                "OK");
                        }
                    }
                    catch (Exception ex)
                    {
                        await DisplayAlert("Ошибка", $"Не удалось удалить сеанс: {ex.Message}", "OK");
                    }
                }
            }
            else
            {
                await DisplayAlert("Ошибка", "Выберите сеанс для удаления", "OK");
            }
        }

        private void OnSessionSelected(object sender, SelectionChangedEventArgs e)
        {
            SelectedSession = e.CurrentSelection.FirstOrDefault() as SessionView;
        }

        // Реализация интерфейса INotifyPropertyChanged
        public new event PropertyChangedEventHandler PropertyChanged;

        protected new void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}