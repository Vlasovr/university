using System.Net.Http.Json;
using Microsoft.Maui.Controls;

namespace CinemaClientService
{
    public partial class HallFormPage : ContentPage
    {
        private readonly HttpClient _httpClient;
        private readonly HallView _hall;

        public HallFormPage(HttpClient httpClient, HallView hall)
        {
            InitializeComponent();
            _httpClient = httpClient;
            _hall = hall;

            if (_hall != null)
            {
                // Заполняем данные, если это редактирование
                RatingEntry.Text = _hall.Rating.ToString();
                CapacityEntry.Text = _hall.Capacity.ToString();
                CleaningScheduleEntry.Text = _hall.CleaningSchedule;
            }
        }

        private async void OnSaveClicked(object sender, EventArgs e)
        {
            try
            {
                var hall = new HallView
                {
                    HallId = _hall?.HallId ?? 0,
                    Rating = int.TryParse(RatingEntry.Text, out int rating) ? rating : 0,
                    Capacity = int.TryParse(CapacityEntry.Text, out int capacity) ? capacity : 0,
                    CleaningSchedule = CleaningScheduleEntry.Text
                };

                if (_hall == null)
                {
                    // Добавление
                    var response = await _httpClient.PostAsJsonAsync("api/Halls", hall);
                    if (!response.IsSuccessStatusCode)
                    {
                        var content = await response.Content.ReadAsStringAsync();
                        await DisplayAlert("Ошибка", $"Не удалось сохранить (код {response.StatusCode}): {content}", "OK");
                        return;
                    }
                }
                else
                {
                    // Редактирование
                    await _httpClient.PutAsJsonAsync($"api/Halls/{hall.HallId}", hall);
                }

                await DisplayAlert("Успешно", "Данные сохранены", "ОК");
                await Navigation.PopAsync();
            }
            catch (Exception ex)
            {
                await DisplayAlert("Ошибка", $"Не удалось сохранить: {ex.Message}", "ОК");
            }
        }

        private async void OnCancelClicked(object sender, EventArgs e)
        {
            await Navigation.PopAsync();
        }
    }
}