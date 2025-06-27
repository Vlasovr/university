using System.Net.Http.Json;
using Microsoft.Maui.Controls;

namespace CinemaClientService
{
    public partial class EmployeeFormPage : ContentPage
    {
        private readonly HttpClient _httpClient;
        private readonly EmployeeView _employee;

        public EmployeeFormPage(HttpClient httpClient, EmployeeView employee)
        {
            InitializeComponent();
            _httpClient = httpClient;
            _employee = employee;

            if (_employee != null)
            {
                // Заполняем данные, если это редактирование
                FullNameEntry.Text = _employee.FullName;
                PhoneEntry.Text = _employee.Phone;
                PositionEntry.Text = _employee.Position;

                // Заполняем новые поля
                EmailEntry.Text = _employee.Email;
                AddressEntry.Text = _employee.Address;
            }
        }

        private async void OnSaveClicked(object sender, EventArgs e)
        {
            try
            {
                var employee = new EmployeeView
                {
                    EmployeeId = _employee?.EmployeeId ?? 0,
                    FullName = FullNameEntry.Text,
                    Phone = PhoneEntry.Text,
                    Position = PositionEntry.Text,

                    // Новые поля
                    Email = EmailEntry.Text,
                    Address = AddressEntry.Text
                };

                if (_employee == null)
                {
                    // Добавление
                    var response = await _httpClient.PostAsJsonAsync("api/Employees", employee);
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
                    await _httpClient.PutAsJsonAsync($"api/Employees/{employee.EmployeeId}", employee);
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