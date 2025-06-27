using System.Collections.ObjectModel;
using System.Net.Http.Json;
using Microsoft.Maui.Controls;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using CinemaClientService.ViewModels;

namespace CinemaClientService
{
    public partial class ReviewsPage : ContentPage, INotifyPropertyChanged
    {
        private readonly HttpClient _httpClient;
        public ObservableCollection<ReviewView> Reviews { get; set; }
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

        public ReviewView SelectedReview { get; set; }

        public ReviewsPage(HttpClient httpClient)
        {
            InitializeComponent();
            _httpClient = httpClient;
            Reviews = new ObservableCollection<ReviewView>();
            ReviewList.ItemsSource = Reviews;
            BindingContext = this;
        }

        protected override async void OnAppearing()
        {
            base.OnAppearing();
            await LoadReviews();
        }

        private async Task LoadReviews()
        {
            try
            {
                IsLoading = true;
                // Очищаем список, чтобы при повторной загрузке не было дублей
                Reviews.Clear();

                // Загружаем базовый список отзывов (с MovieId и ClientId)
                var reviews = await _httpClient.GetFromJsonAsync<List<ReviewView>>("api/Reviews");

                // Для каждого отзыва дополнительно получаем название фильма и имя клиента
                foreach (var review in reviews)
                {
                    // Получаем название фильма
                    var movieResponse = await _httpClient.GetAsync($"api/Movies/{review.MovieId}");
                    if (movieResponse.IsSuccessStatusCode)
                    {
                        var movie = await movieResponse.Content.ReadFromJsonAsync<MovieView>();
                        review.MovieName = movie?.Name ?? "Неизвестно";
                    }
                    else
                    {
                        review.MovieName = "Неизвестно";
                    }

                    // Получаем имя клиента
                    var clientResponse = await _httpClient.GetAsync($"api/Clients/{review.ClientId}");
                    if (clientResponse.IsSuccessStatusCode)
                    {
                        var client = await clientResponse.Content.ReadFromJsonAsync<ClientView>();
                        review.ClientName = client?.FullName ?? "Неизвестно";
                    }
                    else
                    {
                        review.ClientName = "Неизвестно";
                    }

                    Reviews.Add(review);
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
                Reviews.Clear();
                await LoadReviews();
            }
            else
            {
                var filtered = Reviews.Where(r => r.MovieName.ToLower().Contains(query) ||
                                                  r.ClientName.ToLower().Contains(query)).ToList();
                ReviewList.ItemsSource = filtered;
            }
        }

        private async void OnAddReviewClicked(object sender, EventArgs e)
        {
            var form = new ReviewFormPage(_httpClient, null);
            await Navigation.PushAsync(form);
        }

        private async void OnEditReviewClicked(object sender, EventArgs e)
        {
            if (SelectedReview != null)
            {
                var form = new ReviewFormPage(_httpClient, SelectedReview);
                await Navigation.PushAsync(form);
            }
            else
            {
                await DisplayAlert("Ошибка", "Выберите отзыв для редактирования", "OK");
            }
        }

        private async void OnDeleteReviewClicked(object sender, EventArgs e)
        {
            if (SelectedReview != null)
            {
                bool confirm = await DisplayAlert("Удаление", 
                    $"Вы уверены, что хотите удалить отзыв ID {SelectedReview.ReviewId}?", 
                    "Да", "Нет");
                if (confirm)
                {
                    try
                    {
                        var response = await _httpClient.DeleteAsync($"api/Reviews/{SelectedReview.ReviewId}");
                        if (response.IsSuccessStatusCode)
                        {
                            Reviews.Remove(SelectedReview);
                            SelectedReview = null;
                        }
                        else
                        {
                            var content = await response.Content.ReadAsStringAsync();
                            await DisplayAlert("Ошибка", 
                                $"Не удалось удалить отзыв (код {response.StatusCode}): {content}", 
                                "OK");
                        }
                    }
                    catch (Exception ex)
                    {
                        await DisplayAlert("Ошибка", $"Не удалось удалить отзыв: {ex.Message}", "OK");
                    }
                }
            }
            else
            {
                await DisplayAlert("Ошибка", "Выберите отзыв для удаления", "OK");
            }
        }

        private void OnReviewSelected(object sender, SelectionChangedEventArgs e)
        {
            SelectedReview = e.CurrentSelection.FirstOrDefault() as ReviewView;
        }

        // Реализация интерфейса INotifyPropertyChanged
        public new event PropertyChangedEventHandler PropertyChanged;

        protected new void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}