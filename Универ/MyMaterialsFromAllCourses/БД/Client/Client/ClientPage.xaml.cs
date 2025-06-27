// ClientsPage.xaml.cs
using System;
using System.Collections.ObjectModel;
using System.Net.Http.Json;
using Microsoft.Maui.Controls;
using CinemaClientService.ViewModels; 

namespace CinemaClientService.Pages
{
    
    public partial class ClientsPage : ContentPage
    {
        private readonly HttpClient _httpClient;
        public ObservableCollection<ClientView> Clients { get; set; }
        
        public ClientsPage(HttpClient httpClient)
        {
            InitializeComponent();
            _httpClient = httpClient;
            Clients = new ObservableCollection<ClientView>();
            ClientsCollection.ItemsSource = Clients; // ClientsCollection из XAML
        }

        protected override async void OnAppearing()
        {
            base.OnAppearing();
            await LoadClients();
        }

        private async Task LoadClients()
        {
            try
            {
                var list = await _httpClient.GetFromJsonAsync<List<ClientView>>("api/Clients");
                Clients.Clear();
                if (list != null)
                {
                    foreach (var c in list)
                        Clients.Add(c);
                }
            }
            catch (Exception ex)
            {
                await DisplayAlert("Ошибка", ex.Message, "ОК");
            }
        }

        private async void OnAddClicked(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new ClientFormPage(_httpClient, null));
        }

        private async void OnEditClicked(object sender, EventArgs e)
        {
            if (ClientsCollection.SelectedItem is ClientView selected)
            {
                await Navigation.PushAsync(new ClientFormPage(_httpClient, selected));
            }
            else
            {
                await DisplayAlert("Ошибка", "Выберите клиента для редактирования", "ОК");
            }
        }

        private async void OnDeleteClicked(object sender, EventArgs e)
        {
            if (ClientsCollection.SelectedItem is ClientView selected)
            {
                bool confirm = await DisplayAlert("Удаление", 
                    $"Удалить клиента {selected.FullName}?", "Да", "Нет");
                if (!confirm) return;

                try
                {
                    var response = await _httpClient.DeleteAsync($"api/Clients/{selected.ClientId}");
                    if (!response.IsSuccessStatusCode)
                    {
                        var msg = await response.Content.ReadAsStringAsync();
                        await DisplayAlert("Ошибка", msg, "OK");
                        return;
                    }
                    Clients.Remove(selected);
                }
                catch (Exception ex)
                {
                    await DisplayAlert("Ошибка", ex.Message, "ОК");
                }
            }
        }

        private void OnSelectionChanged(object sender, SelectionChangedEventArgs e)
        {
        }

        private void OnSearchTextChanged(object sender, TextChangedEventArgs e)
        {
            // Реализуйте фильтрацию списка клиентов по введённому тексту
            string query = e.NewTextValue?.ToLower() ?? string.Empty;
            var filtered = query == string.Empty
                ? Clients
                : new ObservableCollection<ClientView>(Clients.Where(c => c.FullName.ToLower().Contains(query)));
            ClientsCollection.ItemsSource = filtered;
        }
    }
}