using System.Collections.ObjectModel;
using System.Net.Http.Json;
using Microsoft.Maui.Controls;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace CinemaClientService
{
    public partial class MoviesPage : ContentPage, INotifyPropertyChanged
    {
        private readonly HttpClient _httpClient;
        public ObservableCollection<MovieView> Movies { get; set; }
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

        public MovieView SelectedMovie { get; set; }

        public MoviesPage(HttpClient httpClient)
        {
            InitializeComponent();
            _httpClient = httpClient;
            Movies = new ObservableCollection<MovieView>();
            MovieList.ItemsSource = Movies;
            BindingContext = this;
        }

        protected override async void OnAppearing()
        {
            base.OnAppearing();
            await LoadMovies();
        }

        private async Task LoadMovies()
        {
            try
            {
                IsLoading = true;
                var movies = await _httpClient.GetFromJsonAsync<List<MovieView>>("api/Movies");
                Movies.Clear();
                foreach (var movie in movies)
                {
                    Movies.Add(movie);
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
                await LoadMovies();
            }
            else
            {
                var filtered = Movies.Where(m => m.Name.ToLower().Contains(query) ||
                                                m.Genre.ToLower().Contains(query)).ToList();
                MovieList.ItemsSource = filtered;
            }
        }

        private async void OnAddMovieClicked(object sender, EventArgs e)
        {
            var form = new MovieFormPage(_httpClient, null);
            await Navigation.PushAsync(form);
        }

        private async void OnEditMovieClicked(object sender, EventArgs e)
        {
            if (SelectedMovie != null)
            {
                var form = new MovieFormPage(_httpClient, SelectedMovie);
                await Navigation.PushAsync(form);
            }
            else
            {
                await DisplayAlert("Ошибка", "Выберите фильм для редактирования", "OK");
            }
        }

        private async void OnDeleteMovieClicked(object sender, EventArgs e)
        {
            if (SelectedMovie != null)
            {
                bool confirm = await DisplayAlert("Удаление", $"Вы уверены, что хотите удалить фильм \"{SelectedMovie.Name}\"?", "Да", "Нет");
                if (confirm)
                {
                    try
                    {
                        var response = await _httpClient.DeleteAsync($"api/Movies/{SelectedMovie.MovieId}");
                        if (response.IsSuccessStatusCode)
                        {
                            Movies.Remove(SelectedMovie);
                            SelectedMovie = null;
                        }
                        else
                        {
                            var content = await response.Content.ReadAsStringAsync();
                            await DisplayAlert("Ошибка", $"Не удалось удалить фильм (код {response.StatusCode}): {content}", "OK");
                        }
                    }
                    catch (Exception ex)
                    {
                        await DisplayAlert("Ошибка", $"Не удалось удалить фильм: {ex.Message}", "OK");
                    }
                }
            }
            else
            {
                await DisplayAlert("Ошибка", "Выберите фильм для удаления", "OK");
            }
        }

        private void OnMovieSelected(object sender, SelectionChangedEventArgs e)
        {
            SelectedMovie = e.CurrentSelection.FirstOrDefault() as MovieView;
        }

        // Реализация интерфейса INotifyPropertyChanged
        public new event PropertyChangedEventHandler PropertyChanged;

        protected new void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}