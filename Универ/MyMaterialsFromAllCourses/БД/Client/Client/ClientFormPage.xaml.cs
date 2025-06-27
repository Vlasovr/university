// ClientFormPage.xaml.cs
using System;
using System.Net.Http.Json;
using Microsoft.Maui.Controls;
using CinemaClientService.ViewModels; 

namespace CinemaClientService.Pages
{
    public partial class ClientFormPage : ContentPage
    {
        private readonly HttpClient _httpClient;
        private ClientView _client;

        public ClientFormPage(HttpClient httpClient, ClientView client)
        {
            InitializeComponent();
            _httpClient = httpClient;
            _client = client;

            if (_client != null)
            {
                FullNameEntry.Text = _client.FullName;
                EmailEntry.Text = _client.Email;
                OrderHistoryEditor.Text = _client.OrderHistory;
            }
        }

        private async void OnSaveClicked(object sender, EventArgs e)
        {
            try
            {
                var newClient = new ClientView
                {
                    ClientId = _client?.ClientId ?? 0,
                    FullName = FullNameEntry.Text,
                    Email = EmailEntry.Text,
                    OrderHistory = OrderHistoryEditor.Text
                };

                HttpResponseMessage response;
                if (_client == null)
                {
                    response = await _httpClient.PostAsJsonAsync("api/Clients", newClient);
                }
                else
                {
                    response = await _httpClient.PutAsJsonAsync($"api/Clients/{newClient.ClientId}", newClient);
                }

                if (!response.IsSuccessStatusCode)
                {
                    var msg = await response.Content.ReadAsStringAsync();
                    await DisplayAlert("Ошибка", msg, "OK");
                    return;
                }

                await DisplayAlert("Успешно", "Данные сохранены", "OK");
                await Navigation.PopAsync();
            }
            catch (Exception ex)
            {
                await DisplayAlert("Ошибка", ex.Message, "OK");
            }
        }

        private async void OnCancelClicked(object sender, EventArgs e)
        {
            await Navigation.PopAsync();
        }
    }
}