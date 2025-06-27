using System.Collections.ObjectModel;
using System.Linq;
using System.Net.Http.Json;
using Microsoft.Maui.Controls;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using CinemaClientService.ViewModels;

namespace CinemaClientService
{
    public partial class SessionMoviesPage : ContentPage, INotifyPropertyChanged
    {
        private readonly HttpClient _httpClient;
        public ObservableCollection<SessionMovieView> SessionMovies { get; set; }
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

        public SessionMovieView SelectedSessionMovie { get; set; }

        public SessionMoviesPage(HttpClient httpClient)
        {
            InitializeComponent();
            _httpClient = httpClient;
            SessionMovies = new ObservableCollection<SessionMovieView>();
            SessionMovieList.ItemsSource = SessionMovies;
            BindingContext = this;
        }

        protected override async void OnAppearing()
        {
            base.OnAppearing();
            await LoadSessionMovies();
        }

        private async Task LoadSessionMovies()
        {
            try
            {
                IsLoading = true;
                SessionMovies.Clear();

                var sessionMovies = await _httpClient.GetFromJsonAsync<List<SessionMovieView>>("api/SessionMovies");

                if (sessionMovies != null)
                {
                    foreach (var sm in sessionMovies)
                    {
                        SessionMovies.Add(sm);
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
                SessionMovies.Clear();
                await LoadSessionMovies();
            }
            else
            {
                // Попытка преобразовать запрос в целое число для поиска SessionId или MovieId
                bool isSessionId = int.TryParse(query, out int sessionId);
                bool isMovieId = int.TryParse(query, out int movieId);

                var filtered = SessionMovies.Where(sm =>
                                    (isSessionId && sm.SessionId.ToString().Contains(query)) ||
                                    (isMovieId && sm.MovieId.ToString().Contains(query)) ||
                                    sm.SessionInfo.ToLower().Contains(query) ||
                                    sm.MovieInfo.ToLower().Contains(query))
                                      .ToList();
                SessionMovieList.ItemsSource = filtered;
            }
        }

        private async void OnAddSessionMovieClicked(object sender, EventArgs e)
        {
            var form = new SessionMovieFormPage(_httpClient, null);
            await Navigation.PushAsync(form);
        }

        private async void OnDeleteSessionMovieClicked(object sender, EventArgs e)
        {
            if (SelectedSessionMovie != null)
            {
                bool confirm = await DisplayAlert("Удаление",
                    $"Вы уверены, что хотите удалить связь Session ID {SelectedSessionMovie.SessionId} и Movie ID {SelectedSessionMovie.MovieId}?",
                    "Да", "Нет");
                if (confirm)
                {
                    try
                    {
                        var response = await _httpClient.DeleteAsync($"api/SessionMovies/{SelectedSessionMovie.SessionId}/{SelectedSessionMovie.MovieId}");
                        if (response.IsSuccessStatusCode)
                        {
                            SessionMovies.Remove(SelectedSessionMovie);
                            SelectedSessionMovie = null;
                        }
                        else
                        {
                            var content = await response.Content.ReadAsStringAsync();
                            await DisplayAlert("Ошибка",
                                $"Не удалось удалить связь (код {response.StatusCode}): {content}",
                                "OK");
                        }
                    }
                    catch (Exception ex)
                    {
                        await DisplayAlert("Ошибка", $"Не удалось удалить связь: {ex.Message}", "OK");
                    }
                }
            }
            else
            {
                await DisplayAlert("Ошибка", "Выберите связь для удаления", "OK");
            }
        }

        private void OnSessionMovieSelected(object sender, SelectionChangedEventArgs e)
        {
            SelectedSessionMovie = e.CurrentSelection.FirstOrDefault() as SessionMovieView;
        }

        // Реализация интерфейса INotifyPropertyChanged
        public new event PropertyChangedEventHandler PropertyChanged;

        protected new void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}