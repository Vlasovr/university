using System.Collections.ObjectModel;
using Microsoft.Maui.Controls;
using CinemaClientService.ViewModels;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Net.Http.Json;

namespace CinemaClientService
{
    public partial class SessionMovieFormPage : ContentPage, INotifyPropertyChanged
    {
        private readonly HttpClient _httpClient;
        private readonly SessionMovieView? _editingSessionMovie;

        private ObservableCollection<SessionView> Sessions { get; set; }
        private ObservableCollection<MovieView> Movies { get; set; }

        private SessionView? _selectedSession;
        private MovieView? _selectedMovie;

        public string PageTitle => _editingSessionMovie == null ? "Добавить связь Сессия-Фильм" : "Редактировать связь Сессия-Фильм";

        public SessionMovieFormPage(HttpClient httpClient, SessionMovieView? sessionMovie)
        {
            InitializeComponent();
            _httpClient = httpClient;
            _editingSessionMovie = sessionMovie;

            Sessions = new ObservableCollection<SessionView>();
            Movies = new ObservableCollection<MovieView>();

            BindingContext = this;

            if (_editingSessionMovie != null)
            {
                // Режим редактирования
                SessionIdEntry.Text = _editingSessionMovie.SessionId.ToString();
                MovieIdEntry.Text = _editingSessionMovie.MovieId.ToString();
                SelectedSessionLabel.Text = _editingSessionMovie.SessionInfo;
                SelectedMovieLabel.Text = _editingSessionMovie.MovieInfo;

                _selectedSession = new SessionView { SessionId = _editingSessionMovie.SessionId };
                _selectedMovie = new MovieView { MovieId = _editingSessionMovie.MovieId };
            }
        }

        protected override async void OnAppearing()
        {
            base.OnAppearing();
            await LoadSessions();
            await LoadMovies();
        }

        private async Task LoadSessions()
        {
            try
            {
                var sessions = await _httpClient.GetFromJsonAsync<List<SessionView>>("api/Sessions");
                Sessions.Clear();
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
                await DisplayAlert("Ошибка", $"Не удалось загрузить сессии: {ex.Message}", "OK");
            }
        }

        private async Task LoadMovies()
        {
            try
            {
                var movies = await _httpClient.GetFromJsonAsync<List<MovieView>>("api/Movies");
                Movies.Clear();
                if (movies != null)
                {
                    foreach (var movie in movies)
                    {
                        Movies.Add(movie);
                    }
                }
            }
            catch (Exception ex)
            {
                await DisplayAlert("Ошибка", $"Не удалось загрузить фильмы: {ex.Message}", "OK");
            }
        }

        private async void OnSelectSessionClicked(object sender, EventArgs e)
        {
            // Открываем страницу выбора сессии
            var selectSessionPage = new SelectSessionPage(Sessions);
            selectSessionPage.SessionSelected += (s, session) =>
            {
                _selectedSession = session;
                // Отображаем более информативные данные о сессии
                SelectedSessionLabel.Text = $"{session.StartTime:dd.MM.yyyy HH:mm} - {session.Format}";
            };
            await Navigation.PushAsync(selectSessionPage);
        }

        private async void OnSelectMovieClicked(object sender, EventArgs e)
        {
            // Открываем страницу выбора фильма
            var selectMoviePage = new SelectMoviePage(Movies);
            selectMoviePage.MovieSelected += (s, movie) =>
            {
                _selectedMovie = movie;
                // Отображаем название фильма вместо ID
                SelectedMovieLabel.Text = movie.Name;
            };
            await Navigation.PushAsync(selectMoviePage);
        }

        private async void OnSaveClicked(object sender, EventArgs e)
        {
            // Валидация
            if (_selectedSession == null)
            {
                await DisplayAlert("Ошибка", "Пожалуйста, выберите сессию.", "OK");
                return;
            }

            if (_selectedMovie == null)
            {
                await DisplayAlert("Ошибка", "Пожалуйста, выберите фильм.", "OK");
                return;
            }

            var sessionMovie = new SessionMovieView
            {
                SessionId = _selectedSession.SessionId,
                MovieId = _selectedMovie.MovieId
            };

            try
            {
                if (_editingSessionMovie == null)
                {
                    // Добавление новой связи
                    var response = await _httpClient.PostAsJsonAsync("api/SessionMovies", sessionMovie);
                    if (response.IsSuccessStatusCode)
                    {
                        await DisplayAlert("Успех", "Связь успешно добавлена.", "OK");
                        await Navigation.PopAsync();
                    }
                    else
                    {
                        var content = await response.Content.ReadAsStringAsync();
                        await DisplayAlert("Ошибка", $"Не удалось добавить связь (код {response.StatusCode}): {content}", "OK");
                    }
                }
                else
                {
                    // Редактирование существующей связи (не рекомендуется для связующих таблиц с составным ключом)
                    await DisplayAlert("Информация", "Редактирование связей не поддерживается. Пожалуйста, удалите и создайте новую связь.", "OK");
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