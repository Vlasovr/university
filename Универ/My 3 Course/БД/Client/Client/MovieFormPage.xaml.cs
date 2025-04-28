using System.Net.Http.Json;
using Microsoft.Maui.Controls;

namespace CinemaClientService
{
    public partial class MovieFormPage : ContentPage
    {
        private readonly HttpClient _httpClient;
        private readonly MovieView _movie;

        public MovieFormPage(HttpClient httpClient, MovieView movie)
        {
            InitializeComponent();
            _httpClient = httpClient;
            _movie = movie;

            if (_movie != null)
            {
                // Режим редактирования
                MovieIdEntry.Text = _movie.MovieId.ToString();
                NameEntry.Text = _movie.Name;
                DurationEntry.Text = _movie.Duration.ToString();
                GenreEntry.Text = _movie.Genre;
            }
            else
            {
                // Режим добавления
                MovieIdEntry.IsVisible = false; // Если ID генерируется автоматически
            }
        }

        private async void OnSaveClicked(object sender, EventArgs e)
        {
            // Валидация ввода данных
            if (string.IsNullOrWhiteSpace(NameEntry.Text))
            {
                await DisplayAlert("Ошибка", "Название фильма не может быть пустым.", "OK");
                return;
            }

            if (!int.TryParse(DurationEntry.Text, out int duration) || duration <= 0)
            {
                await DisplayAlert("Ошибка", "Длительность должна быть положительным числом.", "OK");
                return;
            }

            if (string.IsNullOrWhiteSpace(GenreEntry.Text))
            {
                await DisplayAlert("Ошибка", "Жанр фильма не может быть пустым.", "OK");
                return;
            }

            try
            {
                var movie = new MovieView
                {
                    MovieId = _movie?.MovieId ?? 0, // Если ID генерируется на сервере, можно игнорировать это поле при добавлении
                    Name = NameEntry.Text,
                    Duration = duration,
                    Genre = GenreEntry.Text
                };

                if (_movie == null)
                {
                    // Добавление нового фильма
                    var response = await _httpClient.PostAsJsonAsync("api/Movies", movie);
                    if (response.IsSuccessStatusCode)
                    {
                        await DisplayAlert("Успешно", "Фильм добавлен", "OK");
                        await Navigation.PopAsync();
                    }
                    else
                    {
                        var content = await response.Content.ReadAsStringAsync();
                        await DisplayAlert("Ошибка", $"Не удалось добавить фильм (код {response.StatusCode}): {content}", "OK");
                    }
                }
                else
                {
                    // Редактирование существующего фильма
                    var response = await _httpClient.PutAsJsonAsync($"api/Movies/{movie.MovieId}", movie);
                    if (response.IsSuccessStatusCode)
                    {
                        await DisplayAlert("Успешно", "Фильм обновлен", "OK");
                        await Navigation.PopAsync();
                    }
                    else
                    {
                        var content = await response.Content.ReadAsStringAsync();
                        await DisplayAlert("Ошибка", $"Не удалось обновить фильм (код {response.StatusCode}): {content}", "OK");
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
    }
}