using System.Collections.ObjectModel;
using System.Linq;
using System.Net.Http.Json;
using Microsoft.Maui.Controls;
using CinemaClientService.ViewModels;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace CinemaClientService
{
    public partial class ReviewFormPage : ContentPage, INotifyPropertyChanged
    {
        private readonly HttpClient _httpClient;
        private readonly ReviewView? _review;

        private ObservableCollection<MovieView> Movies { get; set; }
        private ObservableCollection<ClientView> Clients { get; set; }

        private MovieView? _selectedMovie;
        private ClientView? _selectedClient;

        public ReviewFormPage(HttpClient httpClient, ReviewView? review)
        {
            InitializeComponent();
            _httpClient = httpClient;
            _review = review;

            Movies = new ObservableCollection<MovieView>();
            Clients = new ObservableCollection<ClientView>();

            LoadMoviesAndClients();

            if (_review != null)
            {
                // Режим редактирования
                ReviewIdEntry.Text = _review.ReviewId.ToString();
                RatingEntry.Text = _review.Rating.ToString();
                CommentEditor.Text = _review.Comment;
                SourceEntry.Text = _review.Source;

                // Установка выбранных фильма и клиента после загрузки
                _selectedMovie = Movies.FirstOrDefault(m => m.MovieId == _review.MovieId);
                SelectedMovieLabel.Text = _selectedMovie?.Name ?? "Не выбрано";

                _selectedClient = Clients.FirstOrDefault(c => c.ClientId == _review.ClientId);
                SelectedClientLabel.Text = _selectedClient?.FullName ?? "Не выбрано";
            }
            else
            {
                // Режим добавления
                ReviewIdEntry.IsVisible = false; // Если ID генерируется автоматически
            }
        }

        private async void LoadMoviesAndClients()
        {
            try
            {
                // Загружаем фильмы
                var movies = await _httpClient.GetFromJsonAsync<List<MovieView>>("api/Movies");
                Movies.Clear();
                if (movies != null)
                {
                    foreach (var movie in movies)
                    {
                        System.Diagnostics.Debug.WriteLine($"MovieId={movie.MovieId}, Name='{movie.Name}'");
                        Movies.Add(movie);
                    }
                }

                // Загружаем клиентов
                var clients = await _httpClient.GetFromJsonAsync<List<ClientView>>("api/Clients");
                Clients.Clear();
                if (clients != null)
                {
                    foreach (var client in clients)
                    {
                        Clients.Add(client);
                    }
                }

                // Отображение количества загруженных элементов
                await DisplayAlert("Info", $"Загружено {Movies.Count} фильмов и {Clients.Count} клиентов.", "OK");
            }
            catch (Exception ex)
            {
                await DisplayAlert("Ошибка", $"Не удалось загрузить данные: {ex.Message}", "OK");
            }
        }

        private async void OnSelectMovieClicked(object sender, EventArgs e)
        {
            // Открываем страницу выбора фильма
            var selectMoviePage = new SelectMoviePage(Movies);
            selectMoviePage.MovieSelected += (s, movie) =>
            {
                _selectedMovie = movie;
                SelectedMovieLabel.Text = movie.Name;
            };
            await Navigation.PushAsync(selectMoviePage);
        }

        private async void OnSelectClientClicked(object sender, EventArgs e)
        {
            // Открываем страницу выбора клиента
            var selectClientPage = new SelectClientPage(Clients);
            selectClientPage.ClientSelected += (s, client) =>
            {
                _selectedClient = client;
                SelectedClientLabel.Text = client.FullName;
            };
            await Navigation.PushAsync(selectClientPage);
        }

        private async void OnSaveClicked(object sender, EventArgs e)
        {
            // Валидация ввода данных
            if (_selectedMovie == null)
            {
                await DisplayAlert("Ошибка", "Выберите фильм.", "OK");
                return;
            }

            if (_selectedClient == null)
            {
                await DisplayAlert("Ошибка", "Выберите клиента.", "OK");
                return;
            }

            if (!int.TryParse(RatingEntry.Text, out int rating) || rating < 0)
            {
                await DisplayAlert("Ошибка", "Рейтинг должен быть неотрицательным числом.", "OK");
                return;
            }

            if (string.IsNullOrWhiteSpace(CommentEditor.Text))
            {
                await DisplayAlert("Ошибка", "Комментарий не может быть пустым.", "OK");
                return;
            }

            if (string.IsNullOrWhiteSpace(SourceEntry.Text))
            {
                await DisplayAlert("Ошибка", "Источник не может быть пустым.", "OK");
                return;
            }

            try
            {
                var review = new ReviewView
                {
                    ReviewId = _review?.ReviewId ?? 0, // Если ID генерируется на сервере, можно игнорировать это поле при добавлении
                    Rating = rating,
                    Comment = CommentEditor.Text,
                    Source = SourceEntry.Text,
                    MovieId = _selectedMovie.MovieId,
                    ClientId = _selectedClient.ClientId,
                    MovieName = _selectedMovie.Name,
                    ClientName = _selectedClient.FullName
                };

                if (_review == null)
                {
                    // Добавление нового отзыва
                    var response = await _httpClient.PostAsJsonAsync("api/Reviews", review);
                    if (response.IsSuccessStatusCode)
                    {
                        await DisplayAlert("Успешно", "Отзыв добавлен", "OK");
                        await Navigation.PopAsync();
                    }
                    else
                    {
                        var content = await response.Content.ReadAsStringAsync();
                        await DisplayAlert("Ошибка", $"Не удалось добавить отзыв (код {response.StatusCode}): {content}", "OK");
                    }
                }
                else
                {
                    // Редактирование существующего отзыва
                    var response = await _httpClient.PutAsJsonAsync($"api/Reviews/{review.ReviewId}", review);
                    if (response.IsSuccessStatusCode)
                    {
                        await DisplayAlert("Успешно", "Отзыв обновлен", "OK");
                        await Navigation.PopAsync();
                    }
                    else
                    {
                        var content = await response.Content.ReadAsStringAsync();
                        await DisplayAlert("Ошибка", $"Не удалось обновить отзыв (код {response.StatusCode}): {content}", "OK");
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
        public new event PropertyChangedEventHandler? PropertyChanged;

        protected new void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}